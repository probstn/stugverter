pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtGraphs

ApplicationWindow {
    id: root

    width: 1620
    height: 960
    visible: true
    title: "SIC Studio - Inverter Operations Console"
    color: "#e8edf3"

    property var inverterClientObj: inverterClient
    property var dictionaryModelObj: dictionaryModel
    property var plotControllerObj: plotController

    property var lineSeriesByAddress: ({})
    property var colorByAddress: ({})
    property var seriesPalette: ["#00a6a6", "#f95738", "#3867d6", "#ff9f1c", "#1a936f", "#6c5ce7", "#c0392b", "#2f4858", "#2d6a4f", "#ec9a29"]
    property bool clearingSeries: false
    property string searchText: ""
    property bool showPlotOnly: false
    property bool xViewManual: false
    property bool yViewManual: false
    property bool xFollowEnabled: false
    property real xFollowSpan: 10
    property bool livePaused: false
    property bool pausedRefreshPending: false
    property bool pausedViewRefreshDirty: false
    property real latestSampleTime: 0
    property real xViewMin: 0
    property real xViewMax: 10
    property real yViewMin: -1
    property real yViewMax: 1
    readonly property real minXSpan: Math.max(0.000001, root.plotControllerObj.minSampleSpacing * 0.5)
    readonly property real minYSpan: 0.0002

    readonly property color panelColor: "#ffffff"
    readonly property color panelBorder: "#ccd6e0"
    readonly property color panelHeader: "#f3f6fa"
    readonly property color pageAccent: "#134074"
    readonly property color textPrimary: "#17212b"
    readonly property color textMuted: "#607080"
    readonly property color liveGreen: "#2d9d78"

    function containsText(base, query) {
        if (!query || query.length === 0) {
            return true
        }
        return base.toLowerCase().indexOf(query.toLowerCase()) >= 0
    }

    function rowMatchesFilter(address, name, unit, typeName, accessName, plotEnabled) {
        if (root.showPlotOnly && !plotEnabled) {
            return false
        }
        if (!root.searchText || root.searchText.length === 0) {
            return true
        }
        const query = root.searchText.trim()
        if (query.length === 0) {
            return true
        }
        const addressHex = "0x" + address.toString(16).padStart(4, "0")
        const bucket = name + " " + unit + " " + typeName + " " + accessName + " " + addressHex
        return containsText(bucket, query)
    }

    function colorForAddress(address) {
        if (colorByAddress[address]) {
            return colorByAddress[address]
        }
        const c = seriesPalette[Math.abs(address) % seriesPalette.length]
        colorByAddress[address] = c
        return c
    }

    function clearGraphSeries() {
        if (root.clearingSeries) {
            return
        }

        root.clearingSeries = true
        const oldMap = root.lineSeriesByAddress
        root.lineSeriesByAddress = ({})
        const keys = Object.keys(oldMap)
        for (let i = 0; i < keys.length; ++i) {
            const key = keys[i]
            const series = oldMap[key]
            if (series) {
                mainTrendView.removeSeries(series)
            }
        }
        root.clearingSeries = false
    }

    function removeSeriesForAddress(address) {
        const series = root.lineSeriesByAddress[address]
        if (!series) {
            return
        }
        mainTrendView.removeSeries(series)
        delete root.lineSeriesByAddress[address]
    }

    function currentXSpan() {
        return Math.max(root.minXSpan, root.xViewMax - root.xViewMin)
    }

    function formatSampleRate(rate) {
        const safeRate = Math.max(0, Number(rate))
        if (safeRate >= 1000000) {
            return (safeRate / 1000000).toFixed(2) + " MS/s"
        }
        if (safeRate >= 1000) {
            return (safeRate / 1000).toFixed(1) + " kS/s"
        }
        return safeRate.toFixed(0) + " S/s"
    }

    function formatNumberCompact(value) {
        const v = Math.max(0, Number(value))
        if (v >= 1000000) {
            return (v / 1000000).toFixed(2) + "M"
        }
        if (v >= 1000) {
            return (v / 1000).toFixed(1) + "k"
        }
        return v.toFixed(0)
    }

    function inputRateColor() {
        const rate = root.plotControllerObj.inputSamplesPerSecond
        if (rate >= 95000) {
            return "#0b8f52"
        }
        if (rate >= 70000) {
            return "#b57600"
        }
        return "#b73a3a"
    }

    function clampXViewToBuffer() {
        const minSpan = root.minXSpan
        let span = Math.max(minSpan, root.xViewMax - root.xViewMin)
        const bufferMin = root.plotControllerObj.bufferMinX
        const bufferMax = root.plotControllerObj.bufferMaxX

        if (bufferMax > bufferMin) {
            const bufferSpan = Math.max(minSpan, bufferMax - bufferMin)
            if (span >= bufferSpan) {
                root.xViewMin = bufferMin
                root.xViewMax = bufferMax
                return
            }

            if (root.xViewMin < bufferMin) {
                root.xViewMin = bufferMin
                root.xViewMax = root.xViewMin + span
            }
            if (root.xViewMax > bufferMax) {
                root.xViewMax = bufferMax
                root.xViewMin = root.xViewMax - span
            }

            root.xViewMin = Math.max(bufferMin, root.xViewMin)
            root.xViewMax = Math.min(bufferMax, root.xViewMax)
            if (root.xViewMax <= root.xViewMin) {
                root.xViewMin = bufferMin
                root.xViewMax = Math.min(bufferMax, bufferMin + minSpan)
            }
            return
        }

        if (root.xViewMax <= root.xViewMin) {
            root.xViewMax = root.xViewMin + minSpan
        }
    }

    function schedulePausedViewRefresh() {
        root.pausedViewRefreshDirty = true
        if (!pausedViewRefreshTimer.running) {
            pausedViewRefreshTimer.start()
        }
    }

    function updateRenderResolutionFromView() {
        root.clampXViewToBuffer()
        const span = currentXSpan()
        root.plotControllerObj.renderWindowSeconds = span
        if (root.livePaused) {
            root.schedulePausedViewRefresh()
        } else {
            root.plotControllerObj.clearViewFrame()
            root.plotControllerObj.windowSeconds = span
            root.pausedRefreshPending = false
        }
    }

    function pauseLiveStream() {
        if (!root.inverterClientObj.connected || !root.inverterClientObj.streamActive) {
            return
        }

        root.inverterClientObj.stopActiveStream()
        root.livePaused = true
        root.xFollowEnabled = false
        root.xViewManual = true
        root.yViewManual = true
        root.updateRenderResolutionFromView()
    }

    function playLiveStream() {
        if (!root.inverterClientObj.connected) {
            return
        }

        const selected = root.dictionaryModelObj.selectedPlotAddresses()
        if (!selected || selected.length === 0) {
            return
        }

        root.plotControllerObj.clear()
        root.clearGraphSeries()
        root.plotControllerObj.clearViewFrame()

        root.livePaused = false
        root.pausedRefreshPending = false
        root.xFollowEnabled = true
        root.xViewManual = true
        root.yViewManual = false
        root.latestSampleTime = 0

        root.xFollowSpan = Math.max(root.minXSpan, root.xFollowSpan)
        root.xViewMin = 0
        root.xViewMax = root.xFollowSpan
        root.updateRenderResolutionFromView()

        root.inverterClientObj.startStream(streamIdField.value, streamDividerField.value, selected)
    }

    function resetTrendInteractionState() {
        root.livePaused = false
        root.pausedRefreshPending = false
        root.xFollowEnabled = false
        root.xViewManual = false
        root.yViewManual = false
        root.xViewMin = root.plotControllerObj.xMin
        root.xViewMax = root.plotControllerObj.xMax
        root.yViewMin = root.plotControllerObj.yMin
        root.yViewMax = root.plotControllerObj.yMax
        root.latestSampleTime = 0
        root.xFollowSpan = root.currentXSpan()
        root.updateRenderResolutionFromView()
    }

    Connections {
        target: root.inverterClientObj

        function onDictionaryReloaded() {
            root.plotControllerObj.clear()
            root.clearGraphSeries()
            root.resetTrendInteractionState()
        }

        function onConnectedChanged() {
            if (!root.inverterClientObj.connected) {
                root.plotControllerObj.clear()
                root.clearGraphSeries()
                root.resetTrendInteractionState()
            }
        }

        function onStreamActiveChanged() {
            root.livePaused = !root.inverterClientObj.streamActive
            if (!root.livePaused) {
                root.pausedRefreshPending = false
                root.plotControllerObj.clearViewFrame()
            }
        }
    }

    Connections {
        target: root.plotControllerObj

        function onSeriesFrame(address, name, points) {
            if (root.clearingSeries) {
                return
            }

            let series = root.lineSeriesByAddress[address]

            if (points.length === 0) {
                if (series) {
                    series.replace([])
                }
                return
            }

            root.latestSampleTime = Math.max(root.latestSampleTime, Number(points[points.length - 1].x))

            if (!series) {
                series = lineSeriesComponent.createObject(mainTrendView, {
                    "name": name + " [0x" + address.toString(16) + "]",
                    "color": root.colorForAddress(address)
                })
                mainTrendView.addSeries(series)
                root.lineSeriesByAddress[address] = series
            }

            if (!root.livePaused || root.pausedRefreshPending) {
                series.replace(points)
            }
        }

        function onCleared() {
            root.clearGraphSeries()
        }

        function onFrameCompleted() {
            if (root.pausedRefreshPending) {
                root.pausedRefreshPending = false
            }
        }
    }

    Timer {
        id: pausedViewRefreshTimer
        interval: 8
        repeat: false
        onTriggered: {
            if (!root.livePaused || !root.pausedViewRefreshDirty) {
                return
            }
            root.pausedViewRefreshDirty = false
            root.pausedRefreshPending = true
            root.plotControllerObj.requestViewFrame(root.xViewMin, root.xViewMax)
        }
    }

    Component.onCompleted: {
        root.plotControllerObj.windowSeconds = 40
        root.plotControllerObj.renderWindowSeconds = 40
        root.plotControllerObj.retentionSeconds = 180
        root.plotControllerObj.maxPointsPerSeries = 5000
        root.plotControllerObj.displayPointBudget = Math.max(800, Math.floor(mainTrendView.width * 2.2))
        root.plotControllerObj.syncTargetFpsToPrimaryScreen()
        root.plotControllerObj.renderingEnabled = true
        root.xViewMin = root.plotControllerObj.xMin
        root.xViewMax = root.plotControllerObj.xMax
        root.yViewMin = root.plotControllerObj.yMin
        root.yViewMax = root.plotControllerObj.yMax
        root.xFollowSpan = currentXSpan()
        root.updateRenderResolutionFromView()
    }

    Connections {
        target: root.plotControllerObj

        function onAxesChanged() {
            root.latestSampleTime = root.plotControllerObj.bufferMaxX
            if (root.xFollowEnabled) {
                root.xViewMax = root.latestSampleTime
                root.xViewMin = Math.max(root.plotControllerObj.bufferMinX, root.xViewMax - root.xFollowSpan)
                root.xViewManual = true
            } else if (!root.xViewManual) {
                root.xViewMin = root.plotControllerObj.xMin
                root.xViewMax = root.plotControllerObj.xMax
            }
            root.clampXViewToBuffer()
            if (!root.yViewManual && !root.livePaused) {
                root.yViewMin = root.plotControllerObj.yMin
                root.yViewMax = root.plotControllerObj.yMax
            }
        }
    }

    Component {
        id: lineSeriesComponent

        LineSeries {
            width: 2
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#f4f8fc" }
            GradientStop { position: 0.45; color: "#e9f0f8" }
            GradientStop { position: 1.0; color: "#e6edf6" }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 86
                radius: 10
                color: root.pageAccent

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 10

                    ColumnLayout {
                        Layout.preferredWidth: 280
                        spacing: 2

                        Label {
                            text: "Inverter Control Link"
                            color: "#d6e3f2"
                            font.pixelSize: 12
                        }

                        Label {
                            text: root.inverterClientObj.connected ? "ONLINE" : "OFFLINE"
                            color: root.inverterClientObj.connected ? "#9be8c7" : "#ffd2cf"
                            font.pixelSize: 24
                            font.bold: true
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 1
                        color: "#33608f"
                    }

                    Label {
                        text: "Host"
                        color: "#d6e3f2"
                    }

                    TextField {
                        id: hostField
                        Layout.preferredWidth: 180
                        text: "0.0.0.0"
                        placeholderText: "IP Address"
                        selectByMouse: true
                    }

                    Label {
                        text: "Port"
                        color: "#d6e3f2"
                    }

                    SpinBox {
                        id: portField
                        Layout.preferredWidth: 100
                        from: 1
                        to: 65535
                        value: 3030
                    }

                    Button {
                        text: root.inverterClientObj.connected ? "Disconnect" : "Connect"
                        onClicked: {
                            if (root.inverterClientObj.connected) {
                                root.inverterClientObj.disconnectFromDevice()
                            } else {
                                root.inverterClientObj.connectToDevice(hostField.text, portField.value)
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        radius: 8
                        color: "#1a4f84"

                        Label {
                            anchors.fill: parent
                            anchors.margins: 8
                            text: root.inverterClientObj.status
                            color: "#eaf2fb"
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                    }
                }
            }

            SplitView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                orientation: Qt.Horizontal

                Rectangle {
                    SplitView.minimumWidth: 390
                    SplitView.preferredWidth: 430
                    SplitView.fillHeight: true
                    radius: 10
                    color: root.panelColor
                    border.width: 1
                    border.color: root.panelBorder

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 40
                            radius: 7
                            color: root.panelHeader

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8

                                Label {
                                    text: "Configuration and Stream Setup"
                                    font.bold: true
                                    color: root.textPrimary
                                }
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            rowSpacing: 6
                            columnSpacing: 8

                            Button {
                                Layout.fillWidth: true
                                text: "Load Dictionary"
                                onClicked: root.inverterClientObj.requestDictionary()
                            }

                            Button {
                                Layout.fillWidth: true
                                text: "Read All Values"
                                onClicked: root.inverterClientObj.readAllDictionaryValues()
                            }

                            Button {
                                Layout.fillWidth: true
                                text: "Commit Configuration"
                                onClicked: root.inverterClientObj.commitConfig()
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 1
                            color: "#e2e8ef"
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            rowSpacing: 8
                            columnSpacing: 8

                            Label {
                                text: "Stream ID"
                                color: root.textMuted
                            }

                            SpinBox {
                                id: streamIdField
                                from: 0
                                to: 255
                                value: 1
                                editable: true
                            }

                            Label {
                                text: "Loop Divider"
                                color: root.textMuted
                            }

                            SpinBox {
                                id: streamDividerField
                                from: 1
                                to: 255
                                value: 1
                                editable: true
                            }

                            Button {
                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                                text: "Stream control moved to Play/Pause"
                                enabled: false
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 1
                            color: "#e2e8ef"
                        }

                        Label {
                            text: "Tag Browser"
                            font.bold: true
                            color: root.textPrimary
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            TextField {
                                Layout.fillWidth: true
                                placeholderText: "Search name, address, unit, type"
                                selectByMouse: true
                                onTextChanged: root.searchText = text
                            }

                            CheckBox {
                                text: "Plot only"
                                checked: root.showPlotOnly
                                onToggled: root.showPlotOnly = checked
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 30
                            radius: 6
                            color: root.panelHeader

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                spacing: 6

                                Label { Layout.preferredWidth: 42; text: "Plot"; font.bold: true }
                                Label { Layout.preferredWidth: 78; text: "Addr"; font.bold: true }
                                Label { Layout.preferredWidth: 165; text: "Name"; font.bold: true }
                                Label { Layout.preferredWidth: 50; text: "Type"; font.bold: true }
                                Label { Layout.preferredWidth: 50; text: "Acc"; font.bold: true }
                                Label { Layout.preferredWidth: 90; text: "Value"; font.bold: true }
                                Label { text: "Write"; font.bold: true }
                            }
                        }

                        ListView {
                            id: dictList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 2
                            reuseItems: true
                            model: root.dictionaryModelObj

                            delegate: Rectangle {
                                id: rowDelegate
                                required property int index
                                required property int address
                                required property string name
                                required property string typeName
                                required property string accessName
                                required property string valueText
                                required property string unit
                                required property bool editable
                                required property bool plotEnabled

                                readonly property bool visibleByFilter: root.rowMatchesFilter(
                                                                          rowDelegate.address,
                                                                          rowDelegate.name,
                                                                          rowDelegate.unit,
                                                                          rowDelegate.typeName,
                                                                          rowDelegate.accessName,
                                                                          rowDelegate.plotEnabled)

                                width: dictList.width
                                height: visibleByFilter ? 34 : 0
                                visible: visibleByFilter
                                color: index % 2 === 0 ? "#fcfdff" : "#f6f9fc"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 8
                                    anchors.rightMargin: 8
                                    spacing: 6

                                    CheckBox {
                                        Layout.preferredWidth: 42
                                        checked: rowDelegate.plotEnabled
                                        onToggled: {
                                            root.dictionaryModelObj.setPlotEnabledByRow(rowDelegate.index, checked)
                                            root.plotControllerObj.setSeriesEnabled(rowDelegate.address, rowDelegate.name, checked)
                                            if (!checked) {
                                                root.removeSeriesForAddress(rowDelegate.address)
                                            }
                                        }
                                    }

                                    Label {
                                        Layout.preferredWidth: 78
                                        text: "0x" + rowDelegate.address.toString(16).padStart(4, "0")
                                        color: root.textPrimary
                                    }

                                    Label {
                                        Layout.preferredWidth: 165
                                        text: rowDelegate.name
                                        color: root.textPrimary
                                        elide: Text.ElideRight
                                    }

                                    Label {
                                        Layout.preferredWidth: 50
                                        text: rowDelegate.typeName
                                        color: root.textMuted
                                    }

                                    Label {
                                        Layout.preferredWidth: 50
                                        text: rowDelegate.accessName
                                        color: root.textMuted
                                    }

                                    Label {
                                        Layout.preferredWidth: 90
                                        text: rowDelegate.valueText
                                        color: "#1d5f7a"
                                        elide: Text.ElideRight
                                    }

                                    TextField {
                                        id: writeField
                                        Layout.fillWidth: true
                                        enabled: rowDelegate.editable
                                        placeholderText: rowDelegate.editable ? "new value" : "read only"
                                        selectByMouse: true
                                    }

                                    Button {
                                        Layout.preferredWidth: 56
                                        text: "Write"
                                        enabled: rowDelegate.editable
                                        onClicked: root.inverterClientObj.writeValue(rowDelegate.address, writeField.text)
                                    }

                                    Button {
                                        Layout.preferredWidth: 52
                                        text: "Read"
                                        onClicked: root.inverterClientObj.readAddresses([rowDelegate.address])
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    SplitView.minimumWidth: 620
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    radius: 10
                    color: root.panelColor
                    border.width: 1
                    border.color: root.panelBorder

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 42
                            radius: 7
                            color: root.panelHeader

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                spacing: 10

                                Label {
                                    text: "Realtime Trends"
                                    font.bold: true
                                    color: root.textPrimary
                                }

                                Rectangle {
                                    width: 1
                                    Layout.fillHeight: true
                                    color: "#d8e0ea"
                                }

                                Label { text: "Points"; color: root.textMuted }

                                SpinBox {
                                    from: 200
                                    to: 8000
                                    value: root.plotControllerObj.maxPointsPerSeries
                                    editable: true
                                    onValueModified: root.plotControllerObj.maxPointsPerSeries = value
                                }

                                Button {
                                    text: "Auto Follow"
                                    onClicked: {
                                        root.xFollowSpan = root.currentXSpan()
                                        root.xFollowEnabled = true
                                        root.xViewManual = true
                                        root.xViewMax = root.plotControllerObj.bufferMaxX
                                        root.xViewMin = Math.max(root.plotControllerObj.bufferMinX, root.xViewMax - root.xFollowSpan)
                                        root.updateRenderResolutionFromView()
                                        root.plotControllerObj.requestRefresh()
                                    }
                                }

                                Button {
                                    text: root.inverterClientObj.streamActive ? "Pause Live" : "Play Live"
                                    onClicked: {
                                        if (root.inverterClientObj.streamActive) {
                                            root.pauseLiveStream()
                                        } else {
                                            root.playLiveStream()
                                        }
                                    }
                                }

                                Label {
                                    text: "Wheel: X zoom | Drag plot: X pan | Drag Y axis: offset | Shift+Drag Y axis: scale"
                                    color: root.textMuted
                                }

                                Item { Layout.fillWidth: true }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#f9fcff"
                            border.width: 1
                            border.color: "#dae3ed"
                            radius: 6

                            GraphsView {
                                id: mainTrendView
                                anchors.fill: parent
                                anchors.margins: 6
                                onWidthChanged: {
                                    root.plotControllerObj.displayPointBudget = Math.max(800, Math.floor(width * 2.2))
                                }

                                theme: GraphsTheme {
                                    backgroundColor: "transparent"
                                    plotAreaBackgroundColor: "#f6fbff"
                                }

                                axisX: ValueAxis {
                                    min: root.xViewMin
                                    max: root.xViewMax
                                    labelDecimals: 2
                                }

                                axisY: ValueAxis {
                                    min: root.yViewMin
                                    max: root.yViewMax
                                    labelDecimals: 4
                                }
                            }

                            MouseArea {
                                id: trendInteraction
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton
                                hoverEnabled: true
                                propagateComposedEvents: true

                                property real lastMouseX: 0
                                property real lastMouseY: 0
                                property bool draggingYAxis: false

                                onPressed: function(mouse) {
                                    lastMouseX = mouse.x
                                    lastMouseY = mouse.y
                                    draggingYAxis = mouse.x >= (width - 48)
                                }

                                onPositionChanged: function(mouse) {
                                    if (!pressed) {
                                        return
                                    }

                                    const xSpan = Math.max(root.minXSpan, root.xViewMax - root.xViewMin)
                                    const ySpan = Math.max(root.minYSpan, root.yViewMax - root.yViewMin)
                                    const dx = mouse.x - lastMouseX
                                    const dy = mouse.y - lastMouseY

                                    if (draggingYAxis) {
                                        if (mouse.modifiers & Qt.ShiftModifier) {
                                            const factor = Math.exp(dy * 0.01)
                                            const clampedSpan = Math.max(root.minYSpan, Math.min(1.0e9, ySpan * factor))
                                            const center = (root.yViewMin + root.yViewMax) * 0.5
                                            root.yViewMin = center - clampedSpan * 0.5
                                            root.yViewMax = center + clampedSpan * 0.5
                                        } else {
                                            const delta = (dy / height) * ySpan
                                            root.yViewMin += delta
                                            root.yViewMax += delta
                                        }
                                        root.yViewManual = true
                                    } else {
                                        const xDelta = (-dx / width) * xSpan
                                        root.xViewMin += xDelta
                                        root.xViewMax += xDelta
                                        root.clampXViewToBuffer()
                                        root.xViewManual = true
                                        root.xFollowEnabled = false
                                    }

                                    root.updateRenderResolutionFromView()

                                    lastMouseX = mouse.x
                                    lastMouseY = mouse.y
                                }

                                onWheel: function(wheel) {
                                    if (!wheel.angleDelta.y) {
                                        return
                                    }

                                    const currentSpan = Math.max(root.minXSpan, root.xViewMax - root.xViewMin)
                                    const zoomFactor = Math.pow(0.86, wheel.angleDelta.y / 120.0)
                                    const bufferSpan = Math.max(root.minXSpan, root.plotControllerObj.bufferMaxX - root.plotControllerObj.bufferMinX)
                                    const nextSpan = Math.max(root.minXSpan, Math.min(bufferSpan, currentSpan * zoomFactor))
                                    if (root.xFollowEnabled) {
                                        root.xFollowSpan = nextSpan
                                        root.xViewMax = root.plotControllerObj.bufferMaxX
                                        root.xViewMin = Math.max(root.plotControllerObj.bufferMinX, root.xViewMax - nextSpan)
                                    } else {
                                        const pivotRatio = Math.max(0.0, Math.min(1.0, wheel.x / width))
                                        const pivotX = root.xViewMin + currentSpan * pivotRatio
                                        root.xViewMin = pivotX - nextSpan * pivotRatio
                                        root.xViewMax = root.xViewMin + nextSpan
                                        root.clampXViewToBuffer()
                                    }
                                    root.xViewManual = true
                                    root.updateRenderResolutionFromView()
                                    wheel.accepted = true
                                }
                            }

                            Rectangle {
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                width: 30
                                color: "#eef4fb"
                                border.width: 1
                                border.color: "#d9e3ee"

                                Label {
                                    anchors.centerIn: parent
                                    text: "Y"
                                    color: root.textMuted
                                    font.bold: true
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 34
                            radius: 6
                            color: "#f5f8fc"

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Label {
                                    text: "X: " + root.xViewMin.toFixed(2)
                                        + " .. " + root.xViewMax.toFixed(2)
                                          + " s"
                                    color: root.textMuted
                                }

                                Item { Layout.fillWidth: true }

                                Label {
                                    text: "Y: " + root.yViewMin.toFixed(3)
                                        + " .. " + root.yViewMax.toFixed(3)
                                    color: root.textMuted
                                }

                                Label {
                                    text: "RX " + root.formatSampleRate(root.plotControllerObj.inputSamplesPerSecond)
                                    color: root.inputRateColor()
                                    font.bold: true
                                }

                                Label {
                                    text: "Draw " + root.formatSampleRate(root.plotControllerObj.outputPointsPerSecond)
                                        + " @ " + root.plotControllerObj.renderFramesPerSecond.toFixed(1) + " FPS"
                                    color: root.textMuted
                                }

                                Label {
                                    text: "Frame " + root.plotControllerObj.lastFrameBuildMs.toFixed(2)
                                        + " ms (avg " + root.plotControllerObj.avgFrameBuildMs.toFixed(2) + ")"
                                    color: root.textMuted
                                }

                                Label {
                                    text: "Decim " + root.plotControllerObj.decimationRatio.toFixed(2) + "x"
                                        + " | visible " + root.formatNumberCompact(root.plotControllerObj.displayedPointCount)
                                        + " | budget " + root.formatNumberCompact(root.plotControllerObj.displayPointBudget)
                                    color: root.textMuted
                                }

                                Label {
                                    text: "Coalesced req " + root.formatNumberCompact(root.plotControllerObj.coalescedFrameRequests)
                                    color: root.textMuted
                                }

                                Rectangle {
                                    width: 9
                                    height: 9
                                    radius: 5
                                    color: root.inverterClientObj.connected ? root.liveGreen : "#d45151"
                                }

                                Label {
                                    text: root.inverterClientObj.streamActive ? "stream active" : "stream idle"
                                    color: root.textMuted
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
