pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Rectangle {
    GlobalStyle {
        id: style
    }

    implicitHeight: 72
    radius: 8
    color: style.topBarBackground
    border.color: style.border

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        Label {
            text: "Connection"
            font.bold: true
            color: style.textPrimary
        }

        Label {
            text: "IP"
            color: style.textMuted
        }

        TextField {
            id: ipInput
            Layout.preferredWidth: 160
            placeholderText: "192.168.1.100"
            text: "192.168.1.100"
        }

        Label {
            text: "TCP"
            color: style.textMuted
        }

        TextField {
            id: tcpPortInput
            Layout.preferredWidth: 86
            placeholderText: "3030"
            text: backend.tcpPort.toString()
            validator: IntValidator { bottom: 1; top: 65535 }
        }

        Label {
            text: "UDP"
            color: style.textMuted
        }

        TextField {
            id: udpPortInput
            Layout.preferredWidth: 86
            placeholderText: "3040"
            text: backend.udpPort.toString()
            validator: IntValidator { bottom: 1; top: 65535 }
        }

        Label {
            text: "Hz"
            color: style.textMuted
        }

        TextField {
            id: streamHzInput
            Layout.preferredWidth: 78
            placeholderText: "100"
            text: backend.streamFrequencyHz.toString()
            validator: IntValidator { bottom: 1; top: 10000 }
        }

        Button {
            text: backend.connected ? "Reconnect Device" : "Connect Device"
            onClicked: {
                backend.tcpPort = Number(tcpPortInput.text)
                backend.udpPort = Number(udpPortInput.text)
                backend.streamFrequencyHz = Number(streamHzInput.text)
                backend.connectSystem(ipInput.text)
            }
        }

        Button {
            text: "Disconnect"
            enabled: backend.connected
            onClicked: backend.disconnectSystem()
        }

        Item {
            Layout.fillWidth: true
        }

        Rectangle {
            width: 10
            height: 10
            radius: 5
            color: backend.connected ? style.success : style.warning
            border.color: style.border
        }

        Label {
            Layout.maximumWidth: 520
            horizontalAlignment: Text.AlignRight
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            color: style.accent
            text: backend.statusText
        }
    }
}
