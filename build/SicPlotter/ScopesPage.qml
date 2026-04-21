pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Item {
    GlobalStyle {
        id: style
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ScopePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            scopeIndex: 0
            titleText: "Scope A"
        }

        ScopePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            scopeIndex: 1
            titleText: "Scope B"
        }
    }
}
