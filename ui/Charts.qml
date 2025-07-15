import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import StockCpp 1.0  // Твій сінглтон Market

// Column {
//     id: _
//     anchors.fill: parent
//     spacing: 8
//     anchors.margins: 12
//     clip: true

//     TextField {
//         id: searchField
//         placeholderText: "Пошук акції..."
//         width: parent.width
//         onAccepted: {
//             if (searchField.text.toUpperCase.endsWith(".US")){
//                 TwelveData.add_by_tag(searchField.text)
//             }
//             else {
//                 //
//             }

//             searchField.text = ""
//         }
//     }

//     ScrollView {
//         width: parent.width
//         height: parent.height - searchField.height - 20
//         clip: true

//         ListView {
//             id: tickerList
//             width: parent.width
//             model: Market

//             delegate: Item {
//                 width: _.width
//                 height: 40

//                 Row {
//                     spacing: 8
//                     Text { text: ticker; width: 80 }
//                     Text { text: title; width: 250 }
//                     Text { text: country; width: 80 }
//                     Text { text: industry; width: 160 }
//                     Text { text: parseFloat(price).toFixed(2); width: 160 }
//                     Image {
//                         source: logo
//                         width: 24
//                         height: 24
//                         fillMode: Image.PreserveAspectFit
//                         cache: true
//                         asynchronous: true
//                     }
//                 }
//             }
//         }
//     }
// }


Item {
    //
}
