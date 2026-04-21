pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Rectangle {
    id: panelRoot

    required property int scopeIndex
    required property string titleText

    GlobalStyle {
        id: style
    }

    radius: 6
    color: style.sectionBackground
    border.color: style.subtleBorder

    function drawScope(canvas) {
        const ctx = canvas.getContext("2d")
        const channels = backend.scopeSeries(scopeIndex)

        ctx.reset()
        ctx.clearRect(0, 0, canvas.width, canvas.height)

        const w = canvas.width
        const h = canvas.height
        const padLeft = 48
        const padRight = 18
        const padTop = 20
        const padBottom = 28
        const plotW = Math.max(1, w - padLeft - padRight)
        const plotH = Math.max(1, h - padTop - padBottom)

        const bgGrad = ctx.createLinearGradient(0, 0, 0, h)
        bgGrad.addColorStop(0.0, style.scopeBackground)
        bgGrad.addColorStop(1.0, Qt.darker(style.scopeBackground, 1.10))
        ctx.fillStyle = bgGrad
        ctx.fillRect(0, 0, w, h)

        ctx.strokeStyle = style.border
        ctx.lineWidth = 1
        ctx.strokeRect(padLeft, padTop, plotW, plotH)

        ctx.fillStyle = style.textPrimary
        ctx.font = "bold " + style.titleFontPx + "px " + style.baseFontFamily
        ctx.fillText(titleText, 12, 16)

        if (channels.length === 0) {
            ctx.fillStyle = style.textMuted
            ctx.font = style.bodyFontPx + "px " + style.baseFontFamily
            ctx.fillText("No stream channels assigned to this scope", 12, h / 2)
            return
        }

        let minX = Number.POSITIVE_INFINITY
        let maxX = Number.NEGATIVE_INFINITY
        let minY = Number.POSITIVE_INFINITY
        let maxY = Number.NEGATIVE_INFINITY
        let hasPoint = false

        for (let c = 0; c < channels.length; ++c) {
            const points = channels[c].points
            for (let i = 0; i < points.length; ++i) {
                const p = points[i]
                minX = Math.min(minX, p.x)
                maxX = Math.max(maxX, p.x)
                minY = Math.min(minY, p.y)
                maxY = Math.max(maxY, p.y)
                hasPoint = true
            }
        }

        if (!hasPoint) {
            ctx.fillStyle = style.textMuted
            ctx.font = style.bodyFontPx + "px " + style.baseFontFamily
            ctx.fillText("Waiting for UDP data...", 12, h / 2)
            return
        }

        if (Math.abs(maxX - minX) < 1e-9) {
            maxX = minX + 1.0
        }
        if (Math.abs(maxY - minY) < 1e-9) {
            maxY = minY + 1.0
        }

        const yPad = (maxY - minY) * 0.08
        minY -= yPad
        maxY += yPad

        ctx.strokeStyle = style.scopeGrid
        ctx.lineWidth = 1
        ctx.font = style.smallFontPx + "px " + style.baseFontFamily
        ctx.fillStyle = style.scopeAxis
        for (let i = 1; i < 5; ++i) {
            const gx = padLeft + (plotW * i / 5)
            ctx.beginPath()
            ctx.moveTo(gx, padTop)
            ctx.lineTo(gx, padTop + plotH)
            ctx.stroke()
        }
        for (let i = 1; i < 4; ++i) {
            const gy = padTop + (plotH * i / 4)
            ctx.beginPath()
            ctx.moveTo(padLeft, gy)
            ctx.lineTo(padLeft + plotW, gy)
            ctx.stroke()
        }

        function mapX(x) {
            return padLeft + ((x - minX) / (maxX - minX)) * plotW
        }
        function mapY(y) {
            return padTop + (1.0 - (y - minY) / (maxY - minY)) * plotH
        }

        for (let c = 0; c < channels.length; ++c) {
            const channel = channels[c]
            const points = channel.points
            if (points.length < 2) {
                continue
            }

            ctx.strokeStyle = channel.color
            ctx.lineWidth = 2
            ctx.beginPath()
            ctx.moveTo(mapX(points[0].x), mapY(points[0].y))
            for (let i = 1; i < points.length; ++i) {
                ctx.lineTo(mapX(points[i].x), mapY(points[i].y))
            }
            ctx.stroke()
        }

        ctx.fillStyle = style.scopeAxis
        ctx.font = style.smallFontPx + "px " + style.baseFontFamily
        ctx.fillText(minX.toFixed(3) + " s", padLeft, h - 8)
        ctx.fillText(maxX.toFixed(3) + " s", w - 80, h - 8)
        ctx.fillText(maxY.toPrecision(4), 8, padTop + 10)
        ctx.fillText(minY.toPrecision(4), 8, h - padBottom)

        const legendX = padLeft + 6
        let legendY = padTop + 16
        ctx.font = style.bodyFontPx + "px " + style.baseFontFamily
        for (let c = 0; c < channels.length; ++c) {
            const channel = channels[c]
            ctx.fillStyle = channel.color
            ctx.fillRect(legendX, legendY - 8, 10, 10)
            ctx.fillStyle = style.textPrimary
            ctx.fillText(channel.name + " = " + channel.latest, legendX + 16, legendY)
            legendY += 16
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "Start Streaming"
                enabled: backend.connected && backend.selectedStreamCount() > 0
                onClicked: backend.startStreaming()
            }

            Button {
                text: "Stop Streaming"
                enabled: backend.streaming
                onClicked: backend.stopStreaming()
            }

            Label {
                color: style.textPrimary
                text: "Selected channels: " + backend.selectedStreamCount()
            }

            Item { Layout.fillWidth: true }

            Label {
                color: backend.streaming ? style.success : style.warning
                text: backend.streaming ? "Streaming active" : "Streaming stopped"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 6
            color: style.sectionBackground
            border.color: style.subtleBorder

            Canvas {
                id: scopeCanvas
                anchors.fill: parent
                anchors.margins: 8
                onPaint: panelRoot.drawScope(scopeCanvas)
            }
        }
    }

    Connections {
        target: backend
        function onScopeDataChanged() {
            scopeCanvas.requestPaint()
        }
    }
}
