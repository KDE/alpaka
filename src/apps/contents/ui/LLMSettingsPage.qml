// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kognos

FormCard.FormCardPage {
    title: i18n("LLM settings")

    Component.onCompleted: {
        prompt.text = KognosSettings.systemPrompt;
        modelCombo.currentIndex = modelCombo.indexOfValue(KognosSettings.model);
    }

    FormCard.FormHeader {
        title: i18n("Model settings")
    }

    FormCard.FormCard {
        FormCard.FormComboBoxDelegate {
            id: modelCombo

            text: i18n("LLM model")
            model: chat.llm.models
            onCurrentTextChanged: KognosSettings.model = currentText
        }

        FormCard.FormTextFieldDelegate {
            id: prompt

            label: i18n("System prompt")
            placeholderText: i18n("No system prompt")
            onTextChanged: KognosSettings.systemPrompt = text
        }
    }
}
