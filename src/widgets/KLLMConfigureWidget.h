// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once
#include "kllmwidgets_export.h"

#include <QWidget>
namespace KLLMWidgets
{
class KLLMWIDGETS_EXPORT KLLMConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KLLMConfigureWidget(QWidget *parent = nullptr);
    ~KLLMConfigureWidget() override;
};
}
