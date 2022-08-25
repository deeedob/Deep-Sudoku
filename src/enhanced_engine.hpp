#pragma once
#include <QQmlApplicationEngine>

/*!
 * @ingroup QT
 * @brief Little Hack to force re-refreshing the QML Scene
 * @author Dennis Oberst
 */
class EnhancedEngine : public QQmlApplicationEngine
{
Q_OBJECT
public:
	explicit EnhancedEngine( QObject* parent = nullptr );
	Q_INVOKABLE void clearCache();
};