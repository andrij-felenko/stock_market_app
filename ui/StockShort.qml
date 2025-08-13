import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Cpp 1.0

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
        cache: true
        asynchronous: true

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            ToolTip.visible: containsMouse
            ToolTip.text: logo_url
        }
    }

    Item {
        id: _manipulate_button
        width: 28
        height: 28

        anchors.right: _right.left
        anchors.top: parent.top
        property bool to_add: Nexus.user.isInAssetList(ticker)

        Text {
            text: parent.to_add ? "+" : "×"
            anchors.fill: parent
            font.pixelSize: 24
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: parent.to_add ? "green" : "#e53935"
            renderType: Text.NativeRendering
        }

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("Close clicked")
        }
    }

    function to_tag(s){
        if (s == "USA") return "US"
        if (s == "Germany") return "DE"
        if (s == "UK") return "GB"
        if (s == "France") return "FR"
        if (s == "Belgium") return "BE"
        if (s == "Sweden") return "SE"
        if (s == "Spain") return "ES"
        if (s == "Denmark") return "DK"
        if (s == "Norway") return "NO"
        if (s == "Finland") return "FI"
        if (s == "Switzerland") return "CH"
        return s
    }

    Image {
        id: _flag
        anchors.left: _manipulate_button.right
        y: _right.y - 4
        source: country == "" ? "" : "https://flagsapi.com/" + to_tag(country) + "/flat/64.png"
        width: 24
        height: 24
    }

    Item {
        anchors.left: _logo.right
        anchors.right: _manipulate_button.left
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

    Text {
        id: _second_line
        anchors.left: _logo.left
        anchors.right: _right.left
        anchors.top: _logo.bottom
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
        visible: cutto(year_max) != 0

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
        visible: cutto(year_max) != 0
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
        visible: cutto(year_max) != 0
    }
}
