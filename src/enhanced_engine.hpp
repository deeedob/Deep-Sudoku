#pragma once
#include <QQmlApplicationEngine>

class EnhancedEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit EnhancedEngine(QObject* parent = nullptr);
    Q_INVOKABLE void clearCache();
};
