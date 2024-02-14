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


RowLayout {
    id: messageDelegate

    required property string message
    required property var sender
    required property bool finished
    required property double tokensPerSecond

    height: messageBubble.height

    Rectangle {
        id: messageBubble

        radius: 5
        color: palette.alternateBase
        Layout.preferredWidth: messageLayout.implicitWidth + 20
        Layout.preferredHeight: messageLayout.implicitHeight + 20
        Layout.maximumWidth: messageDelegate.width * 0.75
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

                    text: messageDelegate.sender === ChatModel.LLM ? i18n("Alpaka") : localUser.fullName
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

            Controls.ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
                visible: AlpakaSettings.showDebugInfo && messageDelegate.sender === ChatModel.LLM && messageDelegate.finished
            }

            RowLayout {
                Layout.fillWidth: true
                visible: AlpakaSettings.showDebugInfo && messageDelegate.sender === ChatModel.LLM && messageDelegate.finished

                Controls.Label {
                    text: i18n("%1 tok/s", messageDelegate.tokensPerSecond.toFixed(2))
                    color: palette.disabled.text
                }
            }
        }
    }
}
