#pragma once

#include <QObject>

// TODO: flesh this class out to represent context in an ongoing conversation
class KLLMContext : public QObject
{
    Q_OBJECT

public:
    explicit KLLMContext(QObject *parent = nullptr);

signals:

};
