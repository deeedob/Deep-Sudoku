#pragma once
#include <QPointer>
#include <memory>
#include "board_model.hpp"
#include "img_proc/deep_solver.hpp"

class App : public QObject
{
Q_OBJECT
	Q_PROPERTY( BoardModel* model READ getModel CONSTANT )
public:
	App() = delete;
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;
	App( App&& ) = delete;
	App& operator=( App&& ) = delete;
	
	static App& getInstance();
	BoardModel* getModel() const;
	Q_INVOKABLE void solveGame( const QImage& src );
private:
	explicit App( QObject* parent = nullptr );
	~App() override = default;

private:
	QPointer<BoardModel> m_boardModel;
	std::unique_ptr<DeepSolver> m_deepSolver;
};