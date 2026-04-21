import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs

Item {
    id: root
    width: 1000
    height: 800

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Style.spacing
        spacing: 15

        // ==========================================
        // TOP BAR
        // ==========================================
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "Oscilloscope Dashboard"
                font.pixelSize: 24
                font.bold: true
                color: Style.textMain
                Layout.fillWidth: true
            }

            // --- NEW: Frequency Input ---
            Text {
                text: "Freq (10Hz):"
                color: Style.textMain
            }

            SpinBox {
                id: frequencyInput
                from: 1
                to: 255       // Max value for quint8
                value: 100    // Default value
                editable: true
                Layout.preferredWidth: 120
            }
            Text {
                text: "Buff (Samples):"
                color: Style.textMain
            }

            SpinBox {
                id: buffersizeInput
                from: 100
                to: 100000
                value: 10000
                editable: true
                Layout.preferredWidth: 120
            }
            // ----------------------------

            Button {
                text: "+ Add"
                font.bold: true
                highlighted: true
                onClicked: inverterManager.scopeManager.addScope(frequencyInput.value, buffersizeInput.value)
            }
        }

        // ==========================================
        // MAIN CONTENT
        // ==========================================
        ListView {
            id: scopeListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20
            clip: true

            model: inverterManager.scopeManager.scopes

            delegate: Rectangle {
                id: scopeCard
                width: scopeListView.width - 15
                height: 600
                radius: Style.radius
                border.width: 1
                border.color: Qt.rgba(Style.textDimmed.r, Style.textDimmed.g, Style.textDimmed.b, 0.3)
                color: Style.windowBg

                property var scope: modelData
                property var seriesRefMap: ({})
                // Add a palette of distinct colors
                property var colorPalette: ["#ff3b30", "#34c759", "#007aff", "#ffcc00", "#ff9500", "#af52de", "#5ac8fa", "#ff2d55"]

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    // --- CARD HEADER ---
                    RowLayout {
                        Layout.fillWidth: true

                        TextEdit {
                            text: scope.name
                            font.pixelSize: 18
                            font.bold: true
                            color: Style.textMain
                            onEditingFinished: scope.name = text
                        }

                        Text {
                            text: scope.frequency*10 + " Hz"
                            font.pixelSize: 10
                            color: Style.textMain
                        }
                        Text {
                            text: scope.bufferSize + " Samples"
                            font.pixelSize: 10
                            color: Style.textMain
                        }

                        Item { Layout.fillWidth: true }

                        Button {
                            text: scope.running ? "⏸ Pause" : "▶ Play"
                            onClicked: scope.running ? scope.pause() : scope.play()
                        }

                        Button {
                            text: "Delete"
                            palette.buttonText: Style.error
                            onClicked: inverterManager.scopeManager.removeScope(scope.streamId)
                        }
                    }

                    // --- TOOLBAR: Add Variable & X-Scale ---
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 15

                        ComboBox {
                            id: dictCombo
                            Layout.preferredWidth: 250
                            model: inverterManager.dictionary
                            textRole: "name"
                        }

                        ToolButton {
                            text: "+"
                            font.pixelSize: 18
                            font.bold: true
                            ToolTip.text: "Add Variable to Chart"
                            ToolTip.visible: hovered
                            onClicked: {
                                if (dictCombo.currentIndex >= 0) {
                                    let address = inverterManager.dictionary.getAddress(dictCombo.currentIndex);
                                    let dtype = inverterManager.dictionary.getDataType(dictCombo.currentIndex);

                                    if (address !== -1) {
                                        let wasRunning = scope.running;
                                        if (wasRunning) scope.pause();
                                        scope.addAddress(address, dtype);
                                        if (wasRunning) scope.play();
                                    }
                                }
                            }
                        }

                        Item { Layout.fillWidth: true }

                        Text { text: "Time Scale:"; color: Style.textMain }

                        Slider {
                            id: timeSlider
                            Layout.preferredWidth: 200
                            from: 10
                            to: scope.bufferSize // Tied securely to C++ Buffer Size Limit
                            value: scope.bufferSize / 2
                        }

                        Text {
                            text: Math.round(timeSlider.value) + " pts"
                            color: Style.textDimmed
                            Layout.minimumWidth: 60
                        }
                    }

                    // --- ACTIVE VARIABLES ---
                    Flow {
                        Layout.fillWidth: true
                        spacing: 10

                        Repeater {
                            id: varRepeater
                            model: scope.addresses

                            delegate: Rectangle {
                                id: varChip
                                height: 36
                                width: chipLayout.implicitWidth + 20
                                radius: 4
                                color: Style.controlBg
                                border.color: Qt.rgba(Style.textDimmed.r, Style.textDimmed.g, Style.textDimmed.b, 0.2)

                                property int address: modelData

                                ValueAxis {
                                    id: localYAxis
                                    // Add a guard to ensure we don't send NaN to the axis
                                    min: scope ? scope.getYMin(address) : 0
                                    max: scope ? scope.getYMax(address) : 100
                                    labelDecimals: 2

                                    // Use formal parameters to fix the "Injection deprecated" warning
                                    onMinChanged: (newMin) => { if(!isNaN(newMin)) scope.setYMin(address, newMin) }
                                    onMaxChanged: (newMax) => { if(!isNaN(newMax)) scope.setYMax(address, newMax) }
                                }

                                LineSeries {
                                    id: localSeries
                                    name: "Reg: " + address
                                    axisX: xAxis
                                    axisY: localYAxis
                                    // Assign a unique color based on the Repeater's index
                                    color: scopeCard.colorPalette[index % scopeCard.colorPalette.length]
                                }

                                Component.onCompleted: {
                                    graphsView.addSeries(localSeries)
                                    scopeCard.seriesRefMap[address] = localSeries
                                }
                                Component.onDestruction: {
                                    // Remove from JS safe map instantly to prevent backend crash
                                    delete scopeCard.seriesRefMap[address]
                                    graphsView.removeSeries(localSeries)
                                }

                                RowLayout {
                                    id: chipLayout
                                    anchors.centerIn: parent
                                    spacing: 8

                                    Rectangle {
                                        width: 12; height: 12; radius: 6
                                        color: localSeries.color
                                    }

                                    Text {
                                        text: "Reg: " + address
                                        font.bold: true
                                        color: Style.textMain
                                    }

                                    TextField {
                                        text: localYAxis.min.toString()
                                        color: Style.textMain
                                        horizontalAlignment: TextInput.AlignRight
                                        Layout.preferredWidth: 40
                                        validator: DoubleValidator {}
                                        onEditingFinished: localYAxis.min = parseFloat(text)
                                    }

                                    Text { text: "to"; color: Style.textDimmed }

                                    TextField {
                                        text: localYAxis.max.toString()
                                        color: Style.textMain
                                        Layout.preferredWidth: 40
                                        validator: DoubleValidator {}
                                        onEditingFinished: localYAxis.max = parseFloat(text)
                                    }

                                    ToolButton {
                                        text: "×"
                                        onClicked: {
                                            let wasRunning = scope.running;
                                            if (wasRunning) scope.pause();
                                            scope.removeAddress(address);
                                            if (wasRunning) scope.play();
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // --- HIGH PERFORMANCE GRAPH ---
                    GraphsView {
                        id: graphsView
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // --- Theme Customization ---
                        theme: GraphsTheme {
                            backgroundColor: "transparent"
                            plotAreaBackgroundColor: "transparent"
                        }

                        axisX: ValueAxis {
                            id: xAxis
                            // Sliding window logic:
                            // Min is the current total points minus the desired window size
                            min: Math.max(0, scope.pointsCount - timeSlider.value)
                            max: Math.max(timeSlider.value, scope.pointsCount)
                            labelDecimals: 0
                        }

                        Timer {
                            interval: 16 // ~60 FPS
                            running: scope.running
                            repeat: true
                            onTriggered: {
                                // CRASH FIX: Loop over the JS Map instead of QML Repeater
                                // Prevents accessing nodes that are midway through destruction
                                for (let key in scopeCard.seriesRefMap) {
                                    let series = scopeCard.seriesRefMap[key];
                                    let addr = parseInt(key);
                                    if (series) {
                                        scope.updateGraphSeries(addr, series);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}