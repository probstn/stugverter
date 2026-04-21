import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import stuglab

Page {
    id: root
    padding: 0
    background: Rectangle { color: "transparent" }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 30

        // --- Header Actions ---
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Device Configuration"
                font.pixelSize: 26
                font.weight: Font.Medium
                color: Style.textMain
            }

            Item { Layout.fillWidth: true }

            Row {
                spacing: 10
                Button {
                    text: "Read Config"
                    onPressed: if (Window.activeFocusItem) Window.activeFocusItem.focus = false
                    onClicked: inverterManager.readConfiguration()
                }

                Button {
                    text: "Write Config"
                    highlighted: true
                    onPressed: if (Window.activeFocusItem) Window.activeFocusItem.focus = false
                    onClicked: inverterManager.writeConfiguration()
                }
            }
        }

        // --- Multi-Column Soft Grid ---
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            GridView {
                id: grid
                model: inverterManager.dictionary
                cellWidth: width / 2
                cellHeight: 60
                flow: GridView.FlowLeftToRight

                delegate: Item {
                    width: grid.cellWidth
                    height: grid.cellHeight

                    // Subtle row background (Soft UI tone)
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 5
                        anchors.rightMargin: 15
                        color: Style.controlBg
                        radius: 6
                        opacity: 0.5

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 15
                            anchors.rightMargin: 10
                            spacing: 10 // Slightly reduced spacing to accommodate the new address label

                            // 1. Parameter Address (NEW)
                            Label {
                                // Formats the quint16 paramId into a 4-digit hex format (e.g., "0x001F")
                                text: "0x" + Number(model.paramId).toString(16).toUpperCase().padStart(4, '0')
                                font.pixelSize: 12
                                font.family: "Menlo" // Monospace font for addresses
                                color: Style.textDimmed
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 2. Parameter Label
                            Label {
                                text: model.name
                                Layout.preferredWidth: 90 // Reduced width to make room for the hex address
                                font.pixelSize: 13
                                color: Style.textMain
                                elide: Text.ElideRight
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 3. Value Input Container (Styled to match Main.qml TextFields)
                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 32 // Height matching standard macOS inputs

                                // Matches the look of inputs in Main
                                color: Style.windowBg
                                radius: 5

                                // Border logic: Visible only on editable fields, highlights on focus
                                border.color: valueInput.activeFocus ? Style.accent : Style.textDimmed
                                border.width: 1

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 10
                                    anchors.rightMargin: 10
                                    spacing: 5

                                    TextField {
                                        id: valueInput
                                        Layout.fillWidth: true
                                        text: model.currentValue !== undefined ? model.currentValue : ""
                                        readOnly: model.access === 0
                                        font.pixelSize: 13
                                        font.family: "Menlo"
                                        font.italic: readOnly ? false : true
                                        color: Style.textMain

                                        // Clean background to match Main's native feel without console warnings
                                        background: null
                                        verticalAlignment: TextInput.AlignVCenter
                                        selectByMouse: true

                                        onEditingFinished: {
                                            if (!readOnly) {
                                                inverterManager.setUserInputValue(index, text)
                                            }
                                        }
                                    }

                                    // Unit Label inside the "box"
                                    Label {
                                        text: model.unit
                                        color: Style.textMain
                                        font.pixelSize: 11
                                        font.weight: Font.Light
                                        Layout.alignment: Qt.AlignVCenter
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