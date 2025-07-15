import QtQuick
import QtQuick.Controls
import QtCore

ApplicationWindow {
    id: window
    visible: true

    property bool isLandscape: width > height

    // // Збереження позиції і розмірів -------------------------------------
    Settings {
        id: app_settings
        property real win_x:  100
        property real win_y:  100
        property real win_w: 1200
        property real win_h:  720
    }

    x: app_settings.win_x
    y: app_settings.win_y
    width:  app_settings.win_w
    height: app_settings.win_h

    onXChanged: app_settings.win_x = x
    onYChanged: app_settings.win_y = y
    onWidthChanged:  app_settings.win_w = width
    onHeightChanged: app_settings.win_h = height
    // ===================================================================

    Main {
        anchors.fill: parent
    }
}
