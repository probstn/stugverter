pragma Singleton
import QtQuick

QtObject {
    // This internal palette fetches colors from macOS system settings
    property SystemPalette _sysPalette: SystemPalette { colorGroup: SystemPalette.Active }

    // Dynamic Backgrounds
    readonly property color windowBg: _sysPalette.window
    readonly property color controlBg: _sysPalette.button

    // Brand Colors (These remain constant usually)
    readonly property color accent: "#3a86ff"
    readonly property color error: "#ff4d4d"
    readonly property color success: "#00ff7f"

    // Text Colors - Automatically flips between Black (Light) and White (Dark)
    readonly property color textMain: _sysPalette.windowText
    readonly property color textDimmed: _sysPalette.text

    // Dimensions
    readonly property int radius: 4
    readonly property int spacing: 10
}