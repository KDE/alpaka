// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "KLLMContext.h"
#include "kllmcore_export.h"
#include <QDebug>
#include <QJsonArray>
namespace KLLMCore
{
/**
 * @brief The KLLMRequest class contains a representation of a message to an LLM.
 *
 * Before calling KLLMInterface::getCompletion(), you need to create a KLLMRequest with information about the desired
 * completion.
 */
class KLLMCORE_EXPORT KLLMRequest
{
public:
    /**
     * @brief Creates a KLLMRequest with context.
     * @param context The contect object containing messages that will be sent to the LLM.
     * @param parent
     */
    explicit KLLMRequest(KLLMContext *context);

    /**
     * @brief Use this to get the outgoing context for the LLM.
     * @return Returns the KLLMContext containing messages to be sent to the LLM.
     */
    [[nodiscard]] KLLMContext *context() const;

    /**
     * @brief Gets the LLM model that will be used by the backend.
     * @return The name of the model to be used for this request.
     */
    [[nodiscard]] QString model() const;

    /**
     * @brief Sets the context to be sent to the LLM.
     * @param context The context object to be sent to the LLM.
     */
    void setContext(const KLLMContext *context);

    /**
     * @brief Sets the model to be used for this request.
     *
     * You should set this to one of the values returned by KLLMInterface::models(); failure to do so will likely produce an
     * error from the backend.
     *
     * @param model The name of the model to be used for this request.
     */
    void setModel(const QString &model);

private:
    KLLMContext *m_context;
    QString m_model;
};
}
KLLMCORE_EXPORT QDebug operator<<(QDebug d, const KLLMCore::KLLMRequest &t);
