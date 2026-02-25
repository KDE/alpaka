// SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "KLLMContext.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QMimeDatabase>

using namespace Qt::StringLiterals;
using namespace KLLMCore;

KLLMContext::KLLMContext(QObject *parent)
    : QObject{parent}
{
}

void KLLMContext::handleFileUrl(const QUrl &url, QString &replacement, QString &error, QVector<QByteArray> &imagesOut)
{
    const QString localPath = url.toLocalFile();
    QFileInfo info(localPath);

    if (!info.exists()) {
        error = error % QStringLiteral("File does not exist: %1\n").arg(localPath);
        return;
    }

    if (info.isDir()) {
        QDirIterator it(info.absoluteFilePath(), QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
        QString concatenator;
        while (it.hasNext()) {
            it.next();
            handleFileUrl(QUrl::fromLocalFile(it.fileInfo().absoluteFilePath()), concatenator, error, imagesOut);
            replacement = replacement % concatenator;
            concatenator.clear();
        }
        return;
    }

    if (!info.isFile()) {
        error = error % QStringLiteral("Not a regular file: %1\n").arg(localPath);
        return;
    }

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(info, QMimeDatabase::MatchContent);
    replacement = localPath + QStringLiteral(":\r"); // Giving the model the path will give some context
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly)) {
        error += QStringLiteral("Cannot open file: %1\n").arg(localPath);
        return;
    }

    if (mime.name().startsWith(QStringLiteral("image/"))) {
        imagesOut.append(file.readAll().toBase64());
        return;
    }

    if (mime.name().startsWith(QStringLiteral("text/")) || mime.name() == QStringLiteral("application/x-shellscript")
        || mime.name() == QStringLiteral("application/json") || mime.name() == QStringLiteral("application/xml")
        || mime.name() == QStringLiteral("application/javascript")) {
        QByteArray data = file.readAll();
        replacement = replacement % QString::fromUtf8(data);
        return;
    }

    error += QStringLiteral("Unsupported file type (%1): %2\n").arg(mime.name(), localPath);
    return;
}

QString KLLMContext::parseMentions(const QString &message, QVector<QByteArray> &imagesOut)
{
    QString result = message;

    static const QRegularExpression re(QStringLiteral(R"(@\[\[([^\]]+)\]\])"));

    QRegularExpressionMatchIterator it = re.globalMatch(message);

    int offset = 0;

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();

        QString urlString = match.captured(1);

        QUrl url(urlString);
        if (!url.isValid())
            continue;

        QString replacement;
        QString scheme = url.scheme().toLower();

        if (scheme == QStringLiteral("file")) {
            QString error;
            handleFileUrl(url, replacement, error, imagesOut);
            if (!error.isEmpty())
                qWarning() << error;
        } else if (scheme == QStringLiteral("http") || scheme == QStringLiteral("https")) {
            // TODO: Handle URLs
            // QNetworkReply *reply = manager->get(QNetworkRequest(url));

            // connect(reply, &QNetworkReply::metaDataChanged, this, [reply]() {
            //     QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            //     if (contentType != QStringLiteral("application/json")) {
            //         qWarning() << "Wrong content type, aborting:" << contentType;
            //         reply->abort();
            //     }
            // });
            replacement = QString(QStringLiteral("<a href=\"%1\">%1</a>")).arg(url.toString());

        } else {
            replacement = url.toString();
        }

        int start = match.capturedStart(0) + offset;
        int length = match.capturedLength(0);

        result.replace(start, length, replacement);

        offset += replacement.length() - length;
    }

    return result;
}

void KLLMContext::addUserMessage(const QString &content)
{
    QVector<QByteArray> extractedImages;
    QString parsedContent = parseMentions(content, extractedImages);
    m_messages.append(KLLMMessage("user"_L1, parsedContent, extractedImages));
}

void KLLMContext::addAssistantMessage(const QString &content)
{
    m_messages.append(KLLMMessage("assistant"_L1, content));
}

void KLLMContext::addSystemMessage(const QString &content)
{
    m_messages.append(KLLMMessage("system"_L1, content));
}

const QVector<KLLMMessage> &KLLMContext::messages() const
{
    return m_messages;
}

void KLLMContext::clear()
{
    m_messages.clear();
}

int KLLMContext::size() const
{
    return m_messages.size();
}

bool KLLMContext::isEmpty() const
{
    return m_messages.isEmpty();
}

QJsonArray KLLMContext::toJsonArray() const
{
    QJsonArray array;
    for (const auto &msg : m_messages)
        array.append(msg.toJson());
    return array;
}

QJsonDocument KLLMContext::toJsonDocument() const
{
    return QJsonDocument(toJsonArray());
}

KLLMMessage::KLLMMessage(const QString &role, const QString &content, const QVector<QByteArray> &images)
    : role(role)
    , content(content)
    , images(images)
{
}

QJsonObject KLLMMessage::toJson() const
{
    QJsonObject obj;

    obj.insert("role"_L1, role);
    obj.insert("content"_L1, content);

    if (!images.isEmpty()) {
        QJsonArray imageArray;
        for (const QByteArray &img : images)
            imageArray.append(QString::fromUtf8(img));

        obj.insert("images"_L1, imageArray);
    }

    return obj;
}