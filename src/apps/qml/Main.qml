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

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Alpaka")

    ChatModel { id: chat }

    SystemPalette { id: palette }

    KCoreAddons.KUser { id: localUser }


    ConfigurationView {
        id: configurationView
        window: root
    }

    pageStack.initialPage: ChatView {}
}
