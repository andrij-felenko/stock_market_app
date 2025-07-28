import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import StockCpp 1.0  // SeetingsNetwork

Page {
    id: root
    title: qsTr("Network Settings")

    ScrollView {
        anchors.fill: parent
        contentWidth: parent.width

        Frame {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(parent.width - 32, 700)
            padding: 24

            background: Rectangle {
                color: Qt.darker(palette.window, 1.02)
                radius: 12
            }

            ColumnLayout {
                spacing: 16
                anchors.fill: parent

                Label {
                    text: qsTr("API Keys")
                    font.pixelSize: 22
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                // Alpha Vantage
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Alpha Vantage:"
                        Layout.preferredWidth: 130
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter Alpha Vantage key"
                        text: SeetingsNetwork.key_alpha_vantage
                        onTextChanged: SeetingsNetwork.setAlphaVantageKey(text)
                    }
                }

                // EOD (split key, horizontal)
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "EOD:"
                        Layout.preferredWidth: 130
                    }

                    // Перша частина ключа
                    TextField {
                        id: eodPart1
                        Layout.preferredWidth: 1
                        Layout.fillWidth: true
                        placeholderText: "Part 1"
                        text: SeetingsNetwork.key_eod.split(".")[0]
                        onTextChanged: SeetingsNetwork.setEodKey(eodPart1.text + "." + eodPart2.text)
                    }

                    // Друга частина ключа
                    TextField {
                        id: eodPart2
                        Layout.preferredWidth: 1
                        Layout.fillWidth: true
                        placeholderText: "Part 2"
                        text: SeetingsNetwork.key_eod.split(".").length > 1 ? SeetingsNetwork.key_eod.split(".")[1] : ""
                        onTextChanged: SeetingsNetwork.setEodKey(eodPart1.text + "." + eodPart2.text)
                    }
                }


                // Finnhub
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Finnhub:"
                        Layout.preferredWidth: 130
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter Finnhub key"
                        text: SeetingsNetwork.key_finnhub
                        onTextChanged: SeetingsNetwork.setFinnHubKey(text)
                    }
                }

                // MarketStack
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Market Stack:"
                        Layout.preferredWidth: 130
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter Market Stack key"
                        text: SeetingsNetwork.key_marketstack
                        onTextChanged: SeetingsNetwork.setMarketStackKey(text)
                    }
                }

                // Twelve Data
                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Twelve Data:"
                        Layout.preferredWidth: 130
                    }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Enter Twelve Data key"
                        text: SeetingsNetwork.key_twelve_data
                        onTextChanged: SeetingsNetwork.setTwelveDataKey(text)
                    }
                }

                // OpenAI (multiline)
                ColumnLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "OpenAI:"
                        Layout.preferredWidth: 130
                    }

                    TextArea {
                        Layout.fillWidth: true
                        wrapMode: TextArea.WrapAnywhere
                        placeholderText: "Enter OpenAI API key"
                        text: SeetingsNetwork.key_open_ai
                        onTextChanged: SeetingsNetwork.setOpenaiKey(text)
                        selectByMouse: true
                        readOnly: false
                        persistentSelection: true
                        // height: Math.max(50, contentHeight)
                        Layout.preferredHeight: Math.max(50, implicitHeight)
                    }
                }
            }
        }
    }
}
