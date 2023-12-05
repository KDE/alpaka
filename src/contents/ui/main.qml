// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kandalf 0.1

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Kandalf")

    ChatModel { id: chat }

    SystemPalette { id: palette }

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

                            ColumnLayout {
                                id: messageLayout

                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 10

                                Controls.Label {
                                    text: messageDelegate.sender === ChatModel.LLM ? i18n("Kandalf") : i18n("You")
                                    font.bold: true
                                    font.pixelSize: 15
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
