import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import StockCpp 1.0  // Твій сінглтон Market

Rectangle {
    id: _
    // width: ListView.view.width
    height: 90
    color: "gray"
    radius: 5
    border.width: 2
    border.color: "black"

    ColumnLayout {
        id: _column
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width * 0.2
        spacing: 0

        Text { text: "   price: " + Qt.locale("de_DE").toString(price, 'f', 2); horizontalAlignment: Text.AlignRight; width: parent.width; font.pointSize: 8 }
        Text { text: " country: " + country; horizontalAlignment: Text.AlignRight;    width: parent.width; font.pointSize: 8 }
        Text { text: "industry: " + industry; horizontalAlignment: Text.AlignRight; width: parent.width; font.pointSize: 8 }
        Text { text: "year min: " + Qt.locale("de_DE").toString(year_min, 'f', 2); horizontalAlignment: Text.AlignRight; width: parent.width; font.pointSize: 8 }
        Text { text: "year max: " + Qt.locale("de_DE").toString(year_max, 'f', 2); horizontalAlignment: Text.AlignRight; width: parent.width; font.pointSize: 8 }
    }

    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: _column.left
        height: parent.height / 2
        font.pointSize: 12
        text: title
        leftPadding: 8
    }

    Item {
        anchors.bottom: parent.bottom
        height: 2
        width: parent.width - 8
        anchors.leftMargin: 4
        anchors.rightMargin: 4

        Rectangle {
            id: left
            anchors.left: parent.left
            anchors.leftMargin: 4
            height: parent.height
            color: "green"
            width: parent.width * ((price - year_min) / (year_max - year_min))
        }

        Rectangle {
            anchors.left: left.right
            anchors.right: parent.right
            height: parent.height
            color: "red"
        }
    }
}
