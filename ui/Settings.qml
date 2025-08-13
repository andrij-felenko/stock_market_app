import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Cpp 1.0  // SeetingsNetwork

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
                        text: Nexus.settings_network.key_alpha_vantage
                        onTextChanged: Nexus.settings_network.setAlphaVantageKey(text)
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
                        text: Nexus.settings_network.key_eod.split(".")[0]
                        onTextChanged: Nexus.settings_network.setEodKey(eodPart1.text + "." + eodPart2.text)
                    }

                    // Друга частина ключа
                    TextField {
                        id: eodPart2
                        Layout.preferredWidth: 1
                        Layout.fillWidth: true
                        placeholderText: "Part 2"
                        text: Nexus.settings_network.key_eod.split(".").length > 1
                              ? Nexus.settings_network.key_eod.split(".")[1] : ""
                        onTextChanged: Nexus.settings_network.setEodKey(eodPart1.text + "." + eodPart2.text)
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
                        text: Nexus.settings_network.key_finnhub
                        onTextChanged: Nexus.settings_network.setFinnHubKey(text)
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
                        text: Nexus.settings_network.key_marketstack
                        onTextChanged: Nexus.settings_network.setMarketStackKey(text)
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
                        text: Nexus.settings_network.key_twelve_data
                        onTextChanged: Nexus.settings_network.setTwelveDataKey(text)
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
                        text: Nexus.settings_network.key_open_ai
                        onTextChanged: Nexus.settings_network.setOpenaiKey(text)
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
