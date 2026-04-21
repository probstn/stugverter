pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Rectangle {
    GlobalStyle {
        id: style
    }

    radius: 8
    color: style.pageBackground
    border.color: style.border

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        Label {
            Layout.fillWidth: true
            text: "Workflow: 1) Load Dictionary  2) Read Values  3) Edit pending values  4) Write Values  5) Commit Config"
            color: style.textMuted
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "1. Load Dictionary"
                enabled: backend.connected
                onClicked: backend.requestDictionary()
            }

            Button {
                text: "2. Read Values"
                enabled: backend.connected
                onClicked: backend.readConfig()
            }

            Button {
                text: "4. Write Values"
                enabled: backend.connected
                onClicked: backend.writeConfig()
            }

            Button {
                text: "5. Commit Config"
                enabled: backend.connected
                onClicked: backend.commitConfig()
            }

            Label {
                text: "Dictionary entries: " + configList.count
                color: style.textPrimary
            }

            Label {
                text: "Selected for streaming: " + backend.selectedStreamCount()
                color: style.textPrimary
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Clear Stream Selection"
                enabled: configList.count > 0
                onClicked: backend.dictionaryModel.clearStreamSelection()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            color: style.elevatedBackground
            radius: 4
            border.color: style.subtleBorder

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                spacing: 10

                Label { text: "Stream"; Layout.preferredWidth: 62; color: style.textPrimary }
                Label { text: "Addr"; Layout.preferredWidth: 80; color: style.textPrimary }
                Label { text: "Name"; Layout.preferredWidth: 220; color: style.textPrimary }
                Label { text: "Type"; Layout.preferredWidth: 80; color: style.textPrimary }
                Label { text: "Access"; Layout.preferredWidth: 150; color: style.textPrimary }
                Label { text: "Unit"; Layout.preferredWidth: 70; color: style.textPrimary }
                Label { text: "Current"; Layout.preferredWidth: 120; color: style.textPrimary }
                Label { text: "Pending"; Layout.fillWidth: true; color: style.textPrimary }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                clip: true

                ListView {
                    id: configList
                    model: backend.dictionaryModel
                    spacing: 2

                    delegate: DictionaryEntryRow {
                        width: configList.width
                        index: index
                        addressHex: addressHex
                        name: name
                        typeName: typeName
                        accessName: accessName
                        unit: unit
                        value: value
                        pendingValue: pendingValue
                        writable: writable
                        modified: modified
                        streamSelected: streamSelected
                    }
                }
            }

            Label {
                anchors.centerIn: parent
                visible: configList.count === 0
                text: "No dictionary loaded yet. Click '1. Load Dictionary'."
                color: style.textMuted
            }
        }
    }
}
