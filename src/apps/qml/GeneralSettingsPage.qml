// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kognos

FormCard.FormCardPage {
    title: i18n("General")

    FormCard.FormHeader {
        title: i18n("General settings")
    }

    FormCard.FormCard {
        FormCard.FormCheckDelegate {
            id: closeDelegate
            text: i18n("Show in System Tray")
            checked: KognosSettings.systemTray
            visible: Controller.supportSystemTray
            enabled: !KognosSettings.isSystemTrayImmutable
            onToggled: {
                KognosSettings.systemTray = checked
                KognosSettings.save()
            }
        }
    }
}
