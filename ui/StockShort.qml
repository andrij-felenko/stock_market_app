import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    height: 90
    radius: 6
    // anchors.fill: parent
    color: "#444"
    clip: true

    function cutto(val) { return Number(val).toFixed(2) }

    // Right section (static fields)
    Column {
        id: _right
        width: 80
        spacing: 6
        anchors.top: parent.top
        anchors.bottom: _red_green.top
        anchors.right: parent.right
        anchors.margins: 8

        Text {
            text: ticker
            color: "white"
            horizontalAlignment: Text.AlignRight
            Layout.alignment:    Qt  .AlignRight
            font.bold: true
            height: 16
            width: parent.width
        }
        Text {
            text: currency
            color: "black"
            horizontalAlignment: Text.AlignRight
            Layout.alignment:    Qt  .AlignRight
            height: 12
            width: parent.width
        }
        Text {
            text: "price: " + cutto(price)
            color: "white"
            horizontalAlignment: Text.AlignRight
            Layout.alignment:    Qt  .AlignRight
            width: parent.width
        }

        // Text {
        //     text: "year min: " + _right.cutto(year_min)
        //     color: "black"
        //     horizontalAlignment: Text.AlignRight
        //     Layout.alignment:    Qt  .AlignRight
        // }
        // Text {
        //     text: "     max: " + _right.cutto(year_max)
        //     color: "black"
        //     horizontalAlignment: Text.AlignRight
        //     Layout.alignment:    Qt  .AlignRight
        // }
    }

    Image {
        id: _logo
        source: logo
        width: 32
        height: 32
        anchors.left: parent.left
        anchors.top: parent.top
        clip: true
    }

    Item {
        anchors.left: _logo.right
        anchors.right: _right.left
        anchors.top: _logo.top
        anchors.bottom: _logo.bottom
        anchors.leftMargin: 4

        Text {
            id: _title_item
            anchors.fill: parent
            text: title
            color: "white"
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            clip: true
            elide: Text.ElideRight
            renderType: Text.NativeRendering
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            ToolTip.visible: containsMouse && _title_item.implicitWidth >= parent.width
            ToolTip.text: title
            ToolTip.delay: 500
        }
    }

    Image {
        id: _flag
        anchors.left: _logo.left
        anchors.top: _logo.bottom
        source: country == "" ? "" : "https://flagsapi.com/" + country + "/flat/64.png"
        width: 32
        height: 24
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: _second_line
        anchors.left: _flag.right
        anchors.right: _right.left
        anchors.top: _flag.top
        height: 32
        anchors.leftMargin: 4
        text: industry
        font.italic: true
        color: "white"
        font.pointSize: 8
        verticalAlignment: Text.AlignVCenter
    }

    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: _right.left
        anchors.margins: 8

        //
    }

    Item {
        id: _red_green
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 4

        Rectangle {
            id: greenBar
            height: parent.height
            color: "palegreen"
            width: _red_green.width * ((price - year_min) / (year_max - year_min))
        }

        Rectangle {
            height: parent.height
            color: "salmon"
            anchors.left: greenBar.right
            anchors.right: parent.right
        }
    }

    Text {
        anchors.left: parent.left
        anchors.bottom: _red_green.top
        height: 12
        anchors.leftMargin: 4
        anchors.bottomMargin: 2
        text: cutto(year_min)
        font.italic: true
        horizontalAlignment: Text.AlignLeft
        color: "white"
        font.pointSize: 7
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        anchors.right: parent.right
        anchors.bottom: _red_green.top
        height: 12
        anchors.rightMargin: 4
        anchors.bottomMargin: 2
        text: cutto(year_max)
        font.italic: true
        horizontalAlignment: Text.AlignRight
        color: "white"
        font.pointSize: 7
        verticalAlignment: Text.AlignVCenter
    }
}
