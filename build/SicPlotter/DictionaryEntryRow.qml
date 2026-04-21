pragma ComponentBehavior: Bound

import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

Rectangle {
    id: rowRoot

    required property int index
    required property string addressHex
    required property string name
    required property string typeName
    required property string accessName
    required property string unit
    required property string value
    required property string pendingValue
    required property bool writable
    required property bool modified
    required property bool streamSelected

    GlobalStyle {
        id: style
    }

    implicitHeight: 40
    radius: 4
    color: index % 2 === 0 ? style.rowEvenBackground : style.rowOddBackground
    border.color: modified ? style.rowModifiedBorder : "transparent"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        spacing: 10

        CheckBox {
            Layout.preferredWidth: 62
            checked: rowRoot.streamSelected
            onToggled: backend.dictionaryModel.setStreamSelected(rowRoot.index, checked)
        }

        Label { Layout.preferredWidth: 80; color: style.textPrimary; text: rowRoot.addressHex }
        Label { Layout.preferredWidth: 220; color: style.textPrimary; text: rowRoot.name }
        Label { Layout.preferredWidth: 80; color: style.textPrimary; text: rowRoot.typeName }
        Label { Layout.preferredWidth: 150; color: style.textPrimary; text: rowRoot.accessName }
        Label { Layout.preferredWidth: 70; color: style.textPrimary; text: rowRoot.unit }
        Label { Layout.preferredWidth: 120; color: style.textPrimary; text: rowRoot.value }

        TextField {
            id: pendingEditor
            Layout.fillWidth: true
            enabled: rowRoot.writable
            color: rowRoot.writable ? style.textPrimary : style.textMuted
            placeholderText: rowRoot.writable ? "edit value" : "read only"

            Binding {
                target: pendingEditor
                property: "text"
                value: rowRoot.pendingValue
                when: !pendingEditor.activeFocus
            }

            onEditingFinished: {
                if (!backend.dictionaryModel.setPendingValue(rowRoot.index, text)) {
                    text = rowRoot.pendingValue
                }
            }
        }
    }
}
