// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    title: i18n("Ollama")

    Component.onCompleted: {
        serverUrl.text = AlpakaSettings.serverUrl;
    }

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.largeSpacing
        Layout.bottomMargin: Kirigami.Units.largeSpacing
        Layout.fillWidth: true
        FormCard.FormTextFieldDelegate {
            id: serverUrl
            label: i18n("Ollama server URL")
            placeholderText: "http://localhost:11434"
            inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase
            onTextChanged: AlpakaSettings.serverUrl = text
        }
        
        FormCard.FormHeader {
            title: i18n("Advanced Parameters")
        }

        FormCard.FormSpinBoxDelegate {
            label: i18n("Context size")
            from: 1
            to: 65536
            value: AlpakaSettings.numCtx
            onValueChanged: AlpakaSettings.numCtx = value
        }

	  FormCard.FormSpinBoxDelegate {
            label: i18n("Repeat penalty lookback tokens")
            description: i18n("How far back to look for repeated tokens (0 = disabled, -1 = context size)")
            from: -1
            to: 4096
            value: AlpakaSettings.repeatLastN
            onValueChanged: AlpakaSettings.repeatLastN = value
        }
    
        FormDoubleSpinBoxDelegate {
            label: i18n("Repeat penalization factor")
            description: i18n("Sets how strongly to penalize repetitions (0 = disabled)")
            decimals: 1
            realFrom: 0.0
            realTo: 5.0
            realStepSize: 0.1
            realValue: AlpakaSettings.repeatPenalty
            onRealValueModified: value => AlpakaSettings.repeatPenalty = value
        }

        FormDoubleSpinBoxDelegate {
            label: i18n("Temperature")
            description: i18n("Higher = more creative/random, lower = more focused/deterministic")
            decimals: 1
            realFrom: 0.0
            realTo: 2.0
            realStepSize: 0.1
            realValue: AlpakaSettings.temperature
            onRealValueModified: value => AlpakaSettings.temperature = value
        }
    
        FormCard.FormSpinBoxDelegate {
            label: i18n("Seed (-1 = random)")
            from: -1
            to: 999999999
            value: AlpakaSettings.seed
            onValueChanged: AlpakaSettings.seed = value
        }
    
        FormCard.FormSpinBoxDelegate {
            label: i18n("Maximum tokens to predict (-1 = unlimited)")
            from: -1
            to: 32768
            value: AlpakaSettings.numPredict
            onValueChanged: AlpakaSettings.numPredict = value
        }
    
        FormCard.FormSpinBoxDelegate {
            label: i18n("Top K")
            from: 0
            to: 500
            value: AlpakaSettings.topK
            onValueChanged: AlpakaSettings.topK = value
        }
    
        FormDoubleSpinBoxDelegate {
            label: i18n("Top P")
            description: i18n("Nucleus sampling – consider tokens that make up this cumulative probability")
            realFrom: 0.0
            realTo: 1.0
            realStepSize: 0.05
            realValue: AlpakaSettings.topP
            onRealValueModified: value => AlpakaSettings.topP = value
        }

        FormDoubleSpinBoxDelegate {
            label: i18n("Min P")
            description: i18n("Minimum probability a token must have to be considered, relative to the probability of the most likely token")
            realFrom: 0.0
            realTo: 1.0
            realStepSize: 0.05
            realValue: AlpakaSettings.minP
            onRealValueModified: value => AlpakaSettings.minP = value
        }
        
        FormCard.FormButtonDelegate {
            text: i18n("Reset to defaults")
            icon.name: "edit-undo-symbolic"
            description: i18n("Restore all settings to their recommended default values")
            onClicked: {
                // These are very common / sensible Ollama defaults in 2025–2026
                AlpakaSettings.numCtx       = 2048
                AlpakaSettings.repeatLastN  = 64
                AlpakaSettings.repeatPenalty= 1.1
                AlpakaSettings.temperature  = 0.8
                AlpakaSettings.seed         = -1
                AlpakaSettings.numPredict   = -1
                AlpakaSettings.topK         = 40
                AlpakaSettings.topP         = 0.9
                AlpakaSettings.minP         = 0.0
        
                // Force UI to update (important in some cases)
                serverUrl.text = AlpakaSettings.serverUrl;
            }
        }
    }
}
