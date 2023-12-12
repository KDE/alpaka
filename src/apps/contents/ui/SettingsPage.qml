// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.coreaddons as KCoreAddons
import org.kde.kirigamiaddons.components as KirigamiComponents
import org.kde.kirigamiaddons.settings as KirigamiSettings
import org.kde.kognos

KirigamiSettings.CategorizedSettings {
    id: settingsRoot

    actions: [
        KirigamiSettings.SettingAction {
            actionName: "general_settings"
            text: i18n("General")
            page: "qrc:/GeneralSettingsPage.qml"
        },
        KirigamiSettings.SettingAction {
            actionName: "llm_settings"
            text: i18n("LLM")
            page: "qrc:/LLMSettingsPage.qml"
        },
        KirigamiSettings.SettingAction {
            actionName: "ollama_settings"
            text: i18n("Ollama")
            page: "qrc:/OllamaSettingsPage.qml"
        }
    ]
}
