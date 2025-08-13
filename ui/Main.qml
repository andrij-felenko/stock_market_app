import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Cpp 1.0  // Твій сінглтон Market

Item {
    Item {
        id: base_window
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: row_menu.top
        }

        SwipeView {
            id: _swipe
            currentIndex: 0
            anchors.fill: parent

            Favorite {
                id: _fav
            }

            Market {
                id: _exch
            }

            Portfolio {
                id: _port
            }

            Charts {
                id: _chart
            }

            Planned {
                id: _plan
            }

            Settings {
                id: _sett
            }

            Account {
                id: _acc
            }
        }
    }

    component MenuButton: Button {
        property alias image_source: image_item.source
        property alias button_text: text_item.text

        leftPadding: 8
        topPadding: 8

        flat: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        checkable: true

        contentItem: Row {
            spacing: 10
            width: parent.width
            height: parent.height

            Image {
                id: image_item
                width: 24
                height: 24
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: text_item
                font.pixelSize: 15
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "white"
                font.bold: true
                visible: Qt.platform.os != "android"
            }
        }
    }

    RowLayout {
        id: row_menu
        anchors.bottom: parent.bottom
        width: parent.width
        height: Qt.platform.os == "android" ? parent.height / 12 : 40
        spacing: 0

        MenuButton {
            image_source: "qrc:/rc/images/folder.png"
            button_text: "Favorite"
            checked: _swipe.currentIndex == 0
            onClicked: _swipe.currentIndex = 0
        }

        MenuButton {
            image_source: "qrc:/rc/images/stock.png"
            button_text: "Exchange"
            checked: _swipe.currentIndex == 1
            onClicked: _swipe.currentIndex = 1
        }

        MenuButton {
            image_source: "qrc:/rc/images/briefcase.png"
            button_text: "Portfolio"
            checked: _swipe.currentIndex == 2
            onClicked: _swipe.currentIndex = 2
        }

        MenuButton {
            image_source: "qrc:/rc/images/presentation.png"
            button_text: "Charts"
            checked: _swipe.currentIndex == 3
            onClicked: _swipe.currentIndex = 3
        }

        MenuButton {
            image_source: "qrc:/rc/images/browser.png"
            button_text: "Planned"
            checked: _swipe.currentIndex == 4
            onClicked: _swipe.currentIndex = 4
        }

        MenuButton {
            image_source: "qrc:/rc/images/website.png"
            button_text: "Settings"
            checked: _swipe.currentIndex == 5
            onClicked: _swipe.currentIndex = 5
        }

        MenuButton {
            image_source: "qrc:/rc/images/person.png"
            button_text: "Account"
            checked: _swipe.currentIndex == 6
            onClicked: _swipe.currentIndex = 6
        }
    }
}
