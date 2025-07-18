import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import StockCpp 1.0  // Твій сінглтон Market

Item {
    width: 400
    height: 400

    TextArea {
        id: logOutput
        anchors.fill: parent
        objectName: "logOutput"
        readOnly: true
        wrapMode: TextArea.WrapAnywhere
        text: ""
    }
}
