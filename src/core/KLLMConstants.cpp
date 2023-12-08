// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMConstants.h"

QUrl KLLMCore::ollamaUrl()
{
    return QUrl(QStringLiteral("http://0.0.0.0:11434"));
}
