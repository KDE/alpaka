// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Dialogs as Dialogs
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

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
            onTriggered: configurationView.open()
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
        Controls.ScrollView {
            Layout.fillWidth: true
            clip: true
            implicitHeight: Math.min(Kirigami.Units.gridUnit * 12, Math.max(Kirigami.Units.gridUnit * 3, contentHeight))
            Controls.TextArea {
                id: messageInput
                wrapMode: TextEdit.Wrap
                placeholderText: i18nc("@info:placeholder", "Enter a message (Use @ to open the attachment menu)")
                enabled: chat.llm.ready && !chat.replyInProgress
                width: parent.width
                focus: true

                property var history: []
                property int historyIndex: -1
                property string currentPrompt: ""

                Keys.onPressed: event => {
                    if (event.key === Qt.Key_Down && !event.modifiers) {
                        if (history.length > 0 && historyIndex > 0) {
                            historyIndex--;
                            messageInput.text = history[historyIndex];
                            event.accepted = true;
                        } else if (historyIndex === 0 && history.length > 0) {
                            historyIndex--;
                            messageInput.text = currentPrompt;
                            event.accepted = true;
                        }
                    } else if (event.key === Qt.Key_Up && !event.modifiers) {
                        if (history.length > 0 && historyIndex < history.length - 1) {
                            if (historyIndex === -1 && messageInput.text)
                                currentPrompt = messageInput.text;

                            historyIndex++;
                            messageInput.text = history[historyIndex];
                            event.accepted = true;
                        }
                    }
                }

                background: Rectangle {
                    color: Kirigami.Theme.backgroundColor
                }

                Keys.onReturnPressed: event => {
                    if (event.modifiers & Qt.ShiftModifier) {
                        const pos = messageInput.cursorPosition;
                        messageInput.text = messageInput.text.slice(0, pos) + "\n" + messageInput.text.slice(pos);
                        messageInput.cursorPosition = pos + 1;
                        event.accepted = true;
                    } else {
                        if (messageInput.text) {
                            chat.sendMessage(messageInput.text.replace(/\n/g, "  \n"));
                            history.unshift(messageInput.text);
                            historyIndex = -1;
                            messageInput.text = "";
                            if (AlpakaSettings.autoScrollDown) chatView.autoScroll = true
                        }
                        event.accepted = true;
                    }
                }

                onTextChanged: {
                    // Check if the last typed character is "@"
                    if (text.length > 0 && text[cursorPosition - 1] === "@" && (text[cursorPosition - 2] === " " || text.length == 1)) {
                        const cursorPos = messageInput.cursorRectangle
                        mentionMenu.popup(
                            messageInput.mapToGlobal(Qt.point(cursorPos.x, cursorPos.y + cursorPos.height)),
                            messageInput
                        )
                    }
                }
            }
        }

        Controls.Menu {
            id: mentionMenu
            Controls.MenuItem {
                text: "file"
                onTriggered: {
                    mentionMenu.close();
                    fileDialog.open();
                }
            }
            Controls.MenuItem {
                text: "picture"
                onTriggered: {
                    mentionMenu.close();
                    imageDialog.open();
                }
            }
            Controls.MenuItem {
                text: "directory"
                onTriggered: {
                    mentionMenu.close();
                    dirDialog.open();
                }
            }
            // TODO: Uncomment when url handling was implemented
            // Controls.MenuItem {
            //     text: "url"
            //     onTriggered: {
            //         mentionMenu.close();
            //         urlDialog.open();
            //     }
            // }
        }

        Dialogs.FileDialog {
            id: fileDialog
            title: "Select a Text File"

            onAccepted: {
                messageInput.insert(messageInput.cursorPosition, "[[" + selectedFile  + "]] ");
            }
            
            onRejected: {
                messageInput.insert(messageInput.cursorPosition - 1, " ");
            }
        }

        Dialogs.FileDialog {
            id: imageDialog
            title: "Pick a Picture"
            nameFilters: ["Images (*.png *.jpg *.jpeg *.bmp *.gif)", "All files (*)"]
            onAccepted: {
                messageInput.insert(messageInput.cursorPosition, "[[" + selectedFile + "]] ");
            }
            
            onRejected: {
                messageInput.insert(messageInput.cursorPosition - 1, " ");
            }
        }
        
        Dialogs.FolderDialog {
            id: dirDialog
            title: "Select a Directory"
            options: Dialogs.FolderDialog.ReadOnly
            onAccepted: {
                messageInput.insert(messageInput.cursorPosition, "[[" + selectedFolder + "]] ");
            }
            
            onRejected: {
                messageInput.insert(messageInput.cursorPosition - 1, " ");
            }
        }
        
        Controls.Dialog {
            id: urlDialog
            title: "Insert URL"
            modal: true
            standardButtons: Controls.Dialog.Ok | Controls.Dialog.Cancel
        
            Column {
                spacing: 10
                padding: 12
        
                Controls.TextField {
                    id: urlField
                    width: 400
                    placeholderText: "https://example.com/file.txt"
                    inputMethodHints: Qt.ImhUrlCharactersOnly
                }
        
                Controls.Label {
                    visible: !urlField.acceptableInput
                    color: "red"
                    text: "Invalid URL"
                }
            }
        
            onAccepted: {
                if (isValidUrl(urlField.text)) {
                    messageInput.insert(messageInput.cursorPosition, "[[" + urlField.text + "]] ");
                } else {
                    Qt.callLater(function() {
                        urlDialog.open()
                    })
                }
            }
            
            onRejected: {
                messageInput.insert(messageInput.cursorPosition - 1, " ");
            }
        
            function isValidUrl(text) {
                return text.match(/^(https?):\/\/\S+$/)
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
            return (contentY + height) * (1 - (visibleArea.yPosition + visibleArea.heightRatio)) <= height / 5;
        }

        onContentYChanged: {
            if (isCloseToYEnd())
                autoScroll = true;
            else
                autoScroll = false;
        }

        Connections {
            function onContentHeightChanged() {
                if (chatView.autoScroll)
                    chatView.positionViewAtEnd();
            }
        }

        delegate: MessageDelegate {
            width: chatView.width
        }
    }
}
