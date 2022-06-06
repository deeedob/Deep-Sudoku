#include <QGuiApplication>
#include <filesystem>
#ifdef ANDROID_BUILD
    #include <QQmlApplicationEngine>
#else
    #include "enhanced_engine.hpp"
    #include <QQmlContext>
#endif
#include <iostream>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    auto path = std::filesystem::current_path();
    std::cout << path.parent_path().c_str() << std::endl;


#ifdef ANDROID_BUILD
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/MainWindow.qml"));
#else
    EnhancedEngine engine;
    engine.rootContext()->setContextProperty("$QmlEngine", &engine);
    engine.load(QStringLiteral("qml/main_hot_reload.qml"));
#endif


    return app.exec();
}