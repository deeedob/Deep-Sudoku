#include <QGuiApplication>
#ifdef ANDROID_BUILD
    #include <QQmlApplicationEngine>
#else
    #include <filesystem>
    #include "enhanced_engine.hpp"
    #include <QQmlContext>
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifdef ANDROID_BUILD
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/MainWindow.qml"));
#else
    EnhancedEngine engine;
    engine.rootContext()->setContextProperty("$QmlEngine", &engine);

    auto path = std::filesystem::current_path();
    path = path.parent_path().parent_path();
    path.append("src/qml/main_hot_reload.qml");
    engine.load(path.c_str());
#endif


    return app.exec();
}