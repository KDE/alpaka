import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kandalf 0.1

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Kandalf")

    ChatModel { id: chat }

    pageStack.initialPage: Kirigami.Page {
        ColumnLayout {
            spacing: 10
            anchors.fill: parent

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Controls.Label {
                    text: "Model:"
                }

                Controls.ComboBox {
                    model: chat.llm.models
                    onCurrentTextChanged: chat.model = currentText
                }
            }

            ListView {
                id: chatView

                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10
                model: chat

                delegate: ColumnLayout {
                    id: messageDelegate

                    required property string message
                    required property var sender

                    spacing: 10
                    width: chatView.width

                    Controls.Label {
                        text: messageDelegate.sender === ChatModel.LLM ? "Kandalf" : "You"
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

            Controls.TextField {
                id: messageInput

                placeholderText: "Enter a message"
                enabled: chat.llm.ready
                Layout.fillWidth: true
                onAccepted: {
                    chat.sendMessage(messageInput.text);
                    messageInput.text = "";
                }
            }
        }
    }
}
