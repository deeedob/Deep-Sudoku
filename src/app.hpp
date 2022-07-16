#pragma once
#include <QPointer>
#include "board_model.hpp"

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BoardModel* model READ getModel CONSTANT)
public:
    static App& getInstance();
    BoardModel* getModel() const;
private:
    App(QObject* parent = nullptr);
    ~App();

private:
    QPointer<BoardModel> m_boardModel;
};