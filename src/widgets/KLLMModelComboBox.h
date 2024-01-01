// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "kllmwidgets_export.h"
#include <QComboBox>

namespace KLLMWidgets
{
/**
 * @brief The KLLMModelComboBox class
 */
class KLLMWIDGETS_EXPORT KLLMModelComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit KLLMModelComboBox(QWidget *parent = nullptr);
    ~KLLMModelComboBox() override;

    void setModels(const QStringList &models);
};
}
