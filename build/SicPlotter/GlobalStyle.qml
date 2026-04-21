import QtQuick 6.5
import QtQuick.Controls 6.5

QtObject {
    id: style

    readonly property SystemPalette systemPalette: SystemPalette {
        colorGroup: SystemPalette.Active
    }

    readonly property font appFont: Qt.application.font
    readonly property string baseFontFamily: appFont.family
    readonly property int titleFontPx: 13
    readonly property int bodyFontPx: 12
    readonly property int smallFontPx: 11

    readonly property color windowBackground: systemPalette.window
    readonly property color topBarBackground: Qt.lighter(systemPalette.window, 1.04)
    readonly property color pageBackground: Qt.lighter(systemPalette.base, 1.02)
    readonly property color sectionBackground: Qt.lighter(systemPalette.base, 1.06)
    readonly property color elevatedBackground: Qt.lighter(systemPalette.base, 1.12)

    readonly property color border: Qt.darker(systemPalette.mid, 1.10)
    readonly property color subtleBorder: systemPalette.mid

    readonly property color textPrimary: systemPalette.windowText
    readonly property color textMuted: Qt.darker(systemPalette.windowText, 1.30)

    readonly property color accent: systemPalette.highlight
    readonly property color accentText: systemPalette.highlightedText
    readonly property color success: Qt.lighter(systemPalette.highlight, 1.20)
    readonly property color warning: Qt.lighter(systemPalette.linkVisited, 1.12)

    readonly property color scopeBackground: Qt.lighter(systemPalette.base, 1.04)
    readonly property color scopeGrid: Qt.darker(systemPalette.mid, 1.08)
    readonly property color scopeAxis: Qt.darker(systemPalette.windowText, 1.08)

    readonly property color rowEvenBackground: Qt.lighter(systemPalette.base, 1.03)
    readonly property color rowOddBackground: Qt.lighter(systemPalette.base, 1.08)
    readonly property color rowModifiedBorder: systemPalette.highlight
}
