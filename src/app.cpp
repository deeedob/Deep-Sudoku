#include "app.hpp"

App::App(QObject* parent)
    : QObject(parent), m_boardModel(new BoardModel(9, 9))
{
}

App::~App() {

}

App &App::getInstance() {
    static auto* instance = new App();
    return *instance;
}

BoardModel* App::getModel() const{
    return m_boardModel;
}