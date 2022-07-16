#include <QGuiApplication>
#include <fdeep/fdeep.hpp>
#include "app.hpp"
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
    auto path = std::filesystem::current_path();
    path = path.parent_path().parent_path();
    ///* TODO: Make this a module! */
    //const auto model = fdeep::load_model(path.string() + "/ai_model/fdeep_model.json");
    //const fdeep::tensor_shape ts(28, 28, 1);
    //const fdeep::tensors tss( {fdeep::tensor(ts, 1) });
    //const auto result = model.predict(tss);
    //std::cout << fdeep::show_tensors(result) << std::endl;

    EnhancedEngine engine;
    path.append("src/qml/MainWindow.qml");
    //path.append("src/qml/main_hot_reload.qml");
    QUrl url(path.c_str());
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);

    //qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");
    //QQmlEngine::setObjectOwnership()
    engine.rootContext()->setContextProperty("$QmlEngine", &engine);
    engine.rootContext()->setContextProperty("Logic", &App::getInstance());

    engine.load(url);
#endif


    return app.exec();
}