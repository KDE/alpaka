// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once
#include "kllmwidgets_export.h"
#include <QDialog>

namespace KLLMWidgets
{
class KLLMWIDGETS_EXPORT KLLMConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KLLMConfigureDialog(QWidget *parent = nullptr);
    ~KLLMConfigureDialog() override;
};
}
