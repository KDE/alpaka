// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    title: i18n("General")
    id: generalSettings
    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.largeSpacing
        FormCard.FormCheckDelegate {
            text: i18n("Show in System Tray")
            checked: AlpakaSettings.systemTray
            visible: Controller.supportSystemTray
            enabled: !AlpakaSettings.isSystemTrayImmutable
            onToggled: {
                AlpakaSettings.systemTray = checked
                AlpakaSettings.save()
            }
        }
        FormCard.FormCheckDelegate {
            text: i18n("Auto scroll down when a new prompt is posted")
            checked: AlpakaSettings.autoScrollDown
            onToggled: {
                AlpakaSettings.autoScrollDown = checked
                AlpakaSettings.save()
            }
        }
    }
}
