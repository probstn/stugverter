pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Window {
    id: root
    width: 1024
    height: 768
    visible: true
    minimumWidth: 980
    minimumHeight: 680
    title: "SIC Studio"
    color: style.windowBackground
    required property var backendObj

    GlobalStyle {
        id: style
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 10

        AppHeader {
            Layout.fillWidth: true
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton {
                text: "Configuration"
            }

            TabButton {
                text: "Live Scopes"
            }
        }

        StackLayout {
            currentIndex: tabBar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            ConfigPage {
            }

            ScopesPage {
            }
        }
    }
}
