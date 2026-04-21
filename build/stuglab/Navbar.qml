import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import stuglab

TabBar {
    id: control
    property alias selectedIndex: control.currentIndex

    background: Item {} // Transparent background

    component CustomTabButton : TabButton {
        id: tabButton
        implicitHeight: 50

        contentItem: Text {
            text: tabButton.text
            font.pixelSize: 14
            font.bold: tabButton.checked
            color: tabButton.checked ? Style.accent : Style.textMain
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            opacity: tabButton.checked || tabButton.hovered ? 1.0 : 0.7
            Behavior on opacity { NumberAnimation { duration: 150 } }
        }

        background: Item {
            // Blue underline indicator
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.7
                height: 3
                color: Style.accent
                visible: tabButton.checked
                radius: 1.5
            }
        }
    }

    CustomTabButton { text: "Scopes" }
    CustomTabButton { text: "Config" }
}