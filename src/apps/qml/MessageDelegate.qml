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
    required property int tokenCount
    required property double duration

    height: messageBubble.height

    Rectangle {
        id: messageBubble

        radius: 5
        color: palette.alternateBase
        Layout.leftMargin: Kirigami.Units.largeSpacing * 2
        Layout.topMargin: Kirigami.Units.largeSpacing * 2
        Layout.rightMargin: Kirigami.Units.largeSpacing * 2
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

            TextEdit {
                id: textEdit
                text: messageDelegate.message
                wrapMode: Controls.Label.WordWrap
                Layout.fillWidth: true
                readOnly: true
                textFormat: Controls.Label.MarkdownText
                color: palette.text
                selectionColor: palette.highlight

                onLinkActivated: function(link) {
                    Qt.openUrlExternally(link)
                }
                onLinkHovered: {
                    Qt.PointingHandCursor
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: contextMenu.open()
                }

                Controls.Menu {
                    id: contextMenu
                    Controls.MenuItem {
                        text: i18n("Copy")
                        enabled: textEdit.canCopy
                        onTriggered: textEdit.copy()
                    }
                    Controls.MenuItem {
                        text: i18n("Select All")
                        onTriggered: textEdit.selectAll()
                    }
                }
            }

            Controls.ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
                visible: AlpakaSettings.showDebugInfo && messageDelegate.sender === ChatModel.LLM && messageDelegate.finished
            }

            Controls.Label {
                Layout.fillWidth: true
                visible: AlpakaSettings.showDebugInfo && messageDelegate.sender === ChatModel.LLM && messageDelegate.finished
                text: i18n("%1 tok/s · %2 tokens · %3 seconds",
                           messageDelegate.tokensPerSecond.toFixed(2),
                           messageDelegate.tokenCount,
                           messageDelegate.duration.toFixed(2))
                color: palette.disabled.text
            }
        }
    }
}
