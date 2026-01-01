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

        FormCard.FormTextFieldDelegate {
            id: serverUrl

            label: i18n("Ollama server URL")
            placeholderText: "http://localhost:11434"
            onTextChanged: AlpakaSettings.serverUrl = text
        }
    }
}
