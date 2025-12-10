import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Cpp 1.0  // SeetingsNetwork

Page {
    id: root
    title: qsTr("Account")

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
                    text: qsTr("Authentication")
                    font.pixelSize: 22
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    spacing: 12
                    Layout.fillWidth: true

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        TextField {
                            id: usernameField
                            Layout.fillWidth: true
                            placeholderText: qsTr("Username")
                            text: Nexus.session.username
                        }

                        TextField {
                            id: emailField
                            Layout.fillWidth: true
                            placeholderText: qsTr("Email")
                            inputMethodHints: Qt.ImhEmailCharactersOnly
                            text: Nexus.session.email
                        }

                        TextField {
                            id: passwordField
                            Layout.fillWidth: true
                            placeholderText: qsTr("Password (min 8 symbols)")
                            echoMode: TextInput.Password
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Button {
                                text: qsTr("Register")
                                onClicked: Nexus.session.registerAccount(usernameField.text, emailField.text, passwordField.text)
                            }

                            Button {
                                text: qsTr("Login")
                                onClicked: Nexus.session.login(emailField.text.length > 0 ? emailField.text : usernameField.text, passwordField.text)
                            }

                            Button {
                                text: qsTr("Refresh token")
                                enabled: Nexus.session.refreshToken.length > 0
                                onClicked: Nexus.session.refreshAccess()
                            }

                            Button {
                                text: qsTr("Logout")
                                onClicked: Nexus.session.logout()
                            }
                        }

                        GroupBox {
                            title: qsTr("Remembered on this device")
                            Layout.fillWidth: true

                            ColumnLayout {
                                spacing: 8
                                Layout.fillWidth: true

                                ComboBox {
                                    id: rememberedUsers
                                    Layout.fillWidth: true
                                    model: Nexus.session.rememberedUsers
                                    editable: false
                                    placeholderText: qsTr("Select a saved account")
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 8

                                    Button {
                                        text: qsTr("Switch account")
                                        enabled: rememberedUsers.currentText.length > 0
                                        onClicked: Nexus.session.activateRemembered(rememberedUsers.currentText)
                                    }

                                    Button {
                                        text: qsTr("Load details")
                                        enabled: rememberedUsers.currentText.length > 0
                                        onClicked: {
                                            if (rememberedUsers.currentText.length > 0) {
                                                usernameField.text = rememberedUsers.currentText
                                                emailField.text = rememberedUsers.currentText
                                            }
                                        }
                                    }
                                }

                                Label {
                                    Layout.fillWidth: true
                                    wrapMode: Text.WordWrap
                                    color: "gray"
                                    text: qsTr("Previously signed-in accounts keep refresh tokens for quick switching during their validity window.")
                                }
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            color: Nexus.session.authenticated ? "#2e7d32" : "#c62828"
                            text: Nexus.session.authenticated
                                  ? qsTr("Logged in as %1 (%2)").arg(Nexus.session.username).arg(Nexus.session.email)
                                  : qsTr("No active session")
                        }

                        Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            color: "#c62828"
                            visible: Nexus.session.lastError.length > 0
                            text: Nexus.session.lastError
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: 260
                        spacing: 8

                        Label {
                            text: qsTr("Access token (30 min)")
                            font.bold: true
                        }
                        TextArea {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 70
                            readOnly: true
                            wrapMode: Text.WrapAnywhere
                            text: Nexus.session.accessToken
                        }
                        Label {
                            text: Nexus.session.accessExpiry.toLocaleString()
                            color: "gray"
                        }

                        Label {
                            text: qsTr("Refresh token (90 days)")
                            font.bold: true
                            topPadding: 8
                        }
                        TextArea {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 70
                            readOnly: true
                            wrapMode: Text.WrapAnywhere
                            text: Nexus.session.refreshToken
                        }
                        Label {
                            text: Nexus.session.refreshExpiry.toLocaleString()
                            color: "gray"
                        }
                    }
                }
            }
        }

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
                                ? Nexus.settings_network.key_eod.split(".")[1]
                                : ""
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
