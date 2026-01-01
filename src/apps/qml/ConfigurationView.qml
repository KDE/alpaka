// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import org.kde.kirigamiaddons.settings as KirigamiSettings

KirigamiSettings.ConfigurationView {
    id: settingsRoot

    modules: [
        KirigamiSettings.ConfigurationModule {
            moduleId: "alpaka_general_settings"
            text: i18n("General")
            page: () => Qt.createComponent("org.kde.alpaka", "GeneralSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "llm_settings"
            text: i18n("LLM")
            page: () => Qt.createComponent("org.kde.alpaka", "LLMSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "ollama_settings"
            text: i18n("Ollama")
            page: () => Qt.createComponent("org.kde.alpaka", "OllamaSettingsPage")
        }
    ]
}
