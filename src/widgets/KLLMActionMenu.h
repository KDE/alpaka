// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmwidgets_export.h"
#include <KActionMenu>

namespace KLLMWidgets
{
/**
 * @brief The KLLMActionMenu class
 */
class KLLMWIDGETS_EXPORT KLLMActionMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit KLLMActionMenu(QObject *parent = nullptr);
    ~KLLMActionMenu() override;

private:
    KLLMWIDGETS_NO_EXPORT void updateMenu();
};
}
