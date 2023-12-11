// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMActionMenu.h"
#include <KLocalizedString>
#include <QMenu>

using namespace KLLMWidgets;
KLLMActionMenu::KLLMActionMenu(QObject *parent)
    : KActionMenu{parent}
{
    setText(i18nc("List of message viewer closed", "Closed Reader"));
    delete menu();
    auto subMenu = new QMenu;
    setMenu(subMenu);
}

KLLMActionMenu::~KLLMActionMenu() = default;

void KLLMActionMenu::updateMenu()
{
}

#include "moc_KLLMActionMenu.cpp"
