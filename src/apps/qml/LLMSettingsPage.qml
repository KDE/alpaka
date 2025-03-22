// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.alpaka

FormCard.FormCardPage {
    title: i18n("LLM")

    Component.onCompleted: {
        prompt.text = AlpakaSettings.systemPrompt;
        llmDebugInfo.checked = AlpakaSettings.showDebugInfo;
    }

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.largeSpacing

        FormCard.AbstractFormDelegate {
            id: prompt

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.smallSpacing

                Controls.Label {
                    id: textLabel

                    text: i18n("System prompt")
                    elide: Text.ElideRight
                    color: root.enabled ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
                    wrapMode: Text.Wrap
                    maximumLineCount: 2

                    Accessible.ignored: true
                    Layout.fillWidth: true
                }
                Controls.TextArea {
                    id: textField

                    placeholderText: i18n("No system prompt")
                    text: prompt.text
                    activeFocusOnTab: false
                    wrapMode: TextEdit.Wrap

                    onTextChanged: editingFinished()
                    onEditingFinished: AlpakaSettings.systemPrompt = text

                    Accessible.name: textLabel.text
                    Layout.fillWidth: true
                }
            }
        }

        FormCard.FormCheckDelegate {
            id: llmDebugInfo

            text: i18n("Show debug information")
            onCheckedChanged: AlpakaSettings.showDebugInfo = checked
        }
    }
}
