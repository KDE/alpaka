// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.coreaddons 1.0 as KCoreAddons
import org.kde.kirigamiaddons.components 1.0 as KirigamiComponents
import org.kde.kandalf 0.1

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Kandalf")

    ChatModel { id: chat }

    SystemPalette { id: palette }

    KCoreAddons.KUser { id: localUser }

    Connections {
        target: chat.llm

        function onErrorOccurred(message: string) {
            errorBanner.text = message;
            errorBanner.visible = true;
        }
    }

    pageStack.initialPage: Kirigami.Page {
        title: i18n("Kandalf")
        actions: [
            Kirigami.Action {
                text: i18n("Model")
                displayComponent: RowLayout {
                    spacing: 10

                    Controls.Label {
                        text: i18n("Model:")
                    }

                    Controls.ComboBox {
                        model: chat.llm.models
                        onCurrentTextChanged: chat.model = currentText
                    }
                }
            },
            Kirigami.Action {
                text: i18n("Start over")
                icon.name: "view-refresh"
                onTriggered: chat.resetConversation()
            }
        ]

        header: KirigamiComponents.Banner {
            id: errorBanner
            type: Kirigami.MessageType.Error
            width: parent.width

            showCloseButton: true
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent

            text: i18n("Waiting for interface")
            visible: !chat.llm.ready
        }

        ColumnLayout {
            spacing: 10
            anchors.fill: parent

            Controls.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: chatView
                    spacing: 10
                    model: chat

                    delegate: RowLayout {
                        id: messageDelegate

                        required property string message
                        required property var sender
                        required property bool finished

                        width: chatView.width
                        height: messageBubble.height

                        Rectangle {
                            id: messageBubble

                            radius: 5
                            color: palette.alternateBase
                            Layout.preferredWidth: messageLayout.implicitWidth + 20
                            Layout.preferredHeight: messageLayout.implicitHeight + 20
                            Layout.maximumWidth: chatView.width * 0.75
                            Layout.alignment: messageDelegate.sender === ChatModel.LLM ? Qt.AlignLeft : Qt.AlignRight
                            border {
                                width: 1
                                color: palette.highlight
                            }

                            ColumnLayout {
                                id: messageLayout

                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 10

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 10

                                    KirigamiComponents.Avatar {
                                        source: messageDelegate.sender === ChatModel.User ? localUser.faceIconUrl + "?timestamp=" + Date.now() : ""
                                        Layout.preferredHeight: userName.height + 15
                                        Layout.preferredWidth: height
                                    }

                                    Controls.Label {
                                        id: userName

                                        text: messageDelegate.sender === ChatModel.LLM ? i18n("Kandalf") : localUser.fullName
                                        font.bold: true
                                        font.pixelSize: 15
                                    }

                                    TypingIndicator {
                                        visible: !messageDelegate.finished
                                    }
                                }

                                Controls.Label {
                                    text: messageDelegate.message
                                    wrapMode: Controls.Label.WordWrap
                                    Layout.fillWidth: true
                                    textFormat: Controls.Label.MarkdownText
                                }
                            }
                        }
                    }
                }
            }

            Controls.TextField {
                id: messageInput

                placeholderText: i18n("Enter a message")
                enabled: chat.llm.ready
                Layout.fillWidth: true
                focus: true
                onAccepted: {
                    chat.sendMessage(messageInput.text);
                    messageInput.text = "";
                }
            }
        }
    }
}
