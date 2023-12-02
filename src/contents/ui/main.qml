import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kandalf 0.1

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Kandalf")

    LLMInterface {
        id: llm

        onFinished: display.text = replyText
    }

    pageStack.initialPage: Kirigami.Page {
        ColumnLayout {
            spacing: 10
            anchors.fill: parent

            Controls.Label {
                id: display

                Layout.alignment: Qt.AlignCenter
                horizontalAlignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: i18n("<i>Awaiting message...</i>")
                wrapMode: Controls.Label.WordWrap
            }

            Controls.TextField {
                id: messageInput

                placeholderText: "Enter a message"
                Layout.fillWidth: true
                onAccepted: {
                    llm.getCompletion(messageInput.text);
                    messageInput.text = "";
                }
            }
        }
    }
}
