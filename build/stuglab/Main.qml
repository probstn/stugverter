import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import stuglab

Window {
    width: 1024
    height: 700
    visible: true
    title: "Inverter Dashboard"
    color: Style.windowBg

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // --- Styled Header Area ---
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: Style.controlBg // Subtle contrast from window background

            // Bottom border for the header
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: Style.textDimmed
                opacity: 0.2
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 20

                // Serial Group
                RowLayout {
                    spacing: 8
                    ComboBox {
                        id: portCombo
                        model: serial.portList
                        editable: true
                        enabled: !serial.connected
                        implicitWidth: 160
                    }

                    TextField {
                        id: baudInput
                        text: "115200"
                        enabled: !serial.connected
                        implicitWidth: 80
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Button {
                        text: serial.connected ? "Disconnect" : "Connect"
                        highlighted: !serial.connected
                        onClicked: {
                            if (serial.connected) serial.disconnectDevice();
                            else serial.connectDevice(portCombo.editText, parseInt(baudInput.text));
                        }
                    }
                }

                // Spacer to push Navbar to the right or center
                Item { Layout.fillWidth: true }

                Navbar {
                    id: customNav
                    Layout.preferredWidth: 350
                }
            }
        }

        // --- Page Content ---
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: customNav.selectedIndex
            
            ScopeDashboard { }
            ConfigPage { }
        }
    }
}