#pragma once
#include <QtQml/qqmlregistration.h>
#include <QPointer>
#include <memory>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickImageProvider>
#include "board_model.hpp"
#include "deep_solver.hpp"

class App : public QObject
{
Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
	Q_PROPERTY( BoardModel* model READ getModel CONSTANT )
public:
	static App* create( QQmlEngine* engine, QJSEngine* jsEngine );
	App() = delete;
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;
	App( App&& ) = delete;
	App& operator=( App&& ) = delete;
	
	BoardModel* getModel() const;
	Q_INVOKABLE bool getQmlImage( QQuickItem* src );
	Q_INVOKABLE bool solveGame( const QUrl& url );
private:
	explicit App( QObject* parent = nullptr );
	~App() override = default;

private:
	QPointer<BoardModel> m_boardModel;
	std::unique_ptr<DeepSolver> m_deepSolver;
};