import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import StockCpp 1.0  // Твій сінглтон Market

Item {
    Item {
        id: search_view
        width: parent.width
        height: 30
    }

    ScrollView {
        id: scroll_view
        width: parent.width
        anchors.top: search_view.bottom
        anchors.bottom: parent.bottom
        clip: true
        padding: 8

        GridView {
            id: grid
            property int columnCount: Math.max(1, Math.floor(width / 320))

            width: scroll_view.width - scroll_view.padding * 2
            model: MarketCpp
            cellWidth: width / columnCount

            delegate: StockShort {
                width: GridView.view.cellWidth - 8
            }
        }
    }
}
