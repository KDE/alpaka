// SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once
#include "kllmcore_export.h"
#include <QUrl>
namespace KLLMCore
{
/**
 * @brief Return default Ollama Url
 * @return default Ollama Url.
 */
[[nodiscard]] KLLMCORE_EXPORT QUrl ollamaUrl();

}
