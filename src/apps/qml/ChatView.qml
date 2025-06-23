// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.coreaddons as KCoreAddons
import org.kde.alpaka

Kirigami.ScrollablePage {
    title: i18n("Alpaka")

    Component.onCompleted: {
        modelCombo.currentIndex = modelCombo.indexOfValue(AlpakaSettings.model);
    }

    Kirigami.Theme.colorSet: Kirigami.Theme.Window
    Connections {
        target: chat.llm

        function onErrorOccurred(message: string) {
            errorBanner.text = message;
        }
    }


    leftPadding: 30
    actions: [

        Kirigami.Action {
            text: i18nc("@action:intoolbar", "Start Over")
            icon.name: "view-refresh"
            onTriggered: chat.resetConversation()
            enabled: chat.llm.ready
        },
        Kirigami.Action {
            text: i18nc("@action:intoolbar", "Settings")
            icon.name: "settings-configure"
            onTriggered: pageStack.pushDialogLayer(settingsPage)
        }
    ]

    header: Kirigami.InlineMessage {
        id: errorBanner

        visible: chat.llm.hasError
        type: Kirigami.MessageType.Error
        width: parent.width
        position: Kirigami.InlineMessage.Position.Header
        actions: [
            Kirigami.Action {
                text: i18nc("@action:button", "Try Again")
                icon.name: "view-refresh"
                onTriggered: chat.llm.reload()
            }
        ]
    }

    footer: ColumnLayout {
        spacing: 0
        Kirigami.Separator {
            Layout.fillWidth: true
        }

        Controls.TextField {
            id: messageInput
            property var history: []
            property int historyIndex: -1
            property string currentPrompt: ""

            Keys.onPressed: (event) => {
                if (event.key === Qt.Key_Down) {
                    if (history.length > 0 && historyIndex > 0) {
                        historyIndex--
                        messageInput.text = history[historyIndex]
                        event.accepted = true
                    } else if (historyIndex === 0 && history.length > 0) {
                        historyIndex--
                        messageInput.text = currentPrompt
                        event.accepted = true
                    }
                } else if (event.key === Qt.Key_Up) {
                    if (history.length > 0 && historyIndex < history.length - 1) {
                        if (historyIndex == -1 && messageInput.text) currentPrompt = messageInput.text
                        historyIndex++
                        messageInput.text = history[historyIndex]
                        event.accepted = true
                    }
                }
            }

            background: Rectangle {
                color: Kirigami.Theme.backgroundColor
            }
            placeholderText: i18nc("@info:placeholder", "Enter a message")
            enabled: chat.llm.ready && !chat.replyInProgress
            Layout.fillWidth: true
            focus: true
            onAccepted: {
                if (messageInput.text) {
                    chat.sendMessage(messageInput.text);
                    history.unshift(text)
                    historyIndex = -1
                    messageInput.text = "";
                }
            }
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }
        RowLayout {
            Layout.margins: Kirigami.Units.smallSpacing
            Controls.Label {
                text: i18n("LLM:")

            }
            Controls.ComboBox {
                id: modelCombo
                model: chat.llm.models
                onCurrentTextChanged: AlpakaSettings.model = currentText
            }
            Controls.Button {
                text: i18nc("@action:button", "Info")
                enabled: modelCombo.count > 0
                onClicked: chat.getModelInfo()
            }
        }

    }

    Kirigami.PlaceholderMessage {
        parent: chatView
        anchors.centerIn: parent

        text: i18nc("@info:placeholder", "Interface Not Found")
        explanation: i18n("Make sure Ollama is installed and is running. Visit the project's website for setup information.")
        helpfulAction: Kirigami.Action {
            icon.name: "globe-symbolic"
            text: i18nc("@action:button", "Visit Website")
            onTriggered: Qt.openUrlExternally("https://ollama.com/")
        }
        visible: !chat.llm.ready
    }

    ListView {
        id: chatView
        spacing: Kirigami.Units.largeSpacing * 2
        model: chat
        clip: true
        property bool autoScroll: true

        function isCloseToYEnd() {
            return (contentY + height) * (1 - (visibleArea.yPosition + visibleArea.heightRatio)) <=  height / 5;
        }

        onContentYChanged: {
            if (isCloseToYEnd()) autoScroll = true
            else autoScroll = false
        }

        Connections {
            function onContentHeightChanged() {
                if (chatView.autoScroll) chatView.positionViewAtEnd()
            }
        }

        delegate: MessageDelegate {
            width: chatView.width
        }
    }
}
