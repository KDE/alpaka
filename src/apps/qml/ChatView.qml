// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.coreaddons as KCoreAddons
import org.kde.kirigamiaddons.components as KirigamiComponents
import org.kde.alpaka

Kirigami.Page {
    title: i18n("Alpaka")
    actions: [
        Kirigami.Action {
            text: i18n("Start over")
            icon.name: "view-refresh"
            onTriggered: chat.resetConversation()
        },
        Kirigami.Action {
            text: i18n("Settings")
            icon.name: "settings-configure"
            onTriggered: pageStack.pushDialogLayer(settingsPage)
        }
    ]

    header: KirigamiComponents.Banner {
        id: errorBanner

        visible: chat.llm.hasError
        type: Kirigami.MessageType.Error
        width: parent.width
        actions: [
            Kirigami.Action {
                text: i18n("Try again")
                icon.name: "view-refresh"
                onTriggered: chat.llm.reload()
            }
        ]
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
                clip: true

                delegate: MessageDelegate {
                    width: chatView.width
                }
            }
        }

        Controls.TextField {
            id: messageInput

            placeholderText: i18n("Enter a message")
            enabled: chat.llm.ready && !chat.replyInProgress
            Layout.fillWidth: true
            focus: true
            onAccepted: {
                chat.sendMessage(messageInput.text);
                messageInput.text = "";
            }
        }
    }
}
