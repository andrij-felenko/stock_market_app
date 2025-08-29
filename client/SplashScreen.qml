import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    id: splash
    width: 256
    height: 256
    flags: Qt.FramelessWindowHint | Qt.SplashScreen
    visible: true

    Rectangle {
        anchors.fill: parent
        color: "#111111" // темний фон (можна змінити)

        Image {
            id: logo
            anchors.centerIn: parent
            source: "qrc:/rc/images/Stock_app_logo2.png"
            fillMode: Image.PreserveAspectFit
            width: Math.min(parent.width, parent.height)
            height: width
            smooth: true
        }
    }
}
