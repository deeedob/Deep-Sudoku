#include <QGuiApplication>
#include <fdeep/fdeep.hpp>
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
    /* TODO: Make this a module! */
    const auto model = fdeep::load_model("fdeep_model.json");
    const auto result = model.predict(
            {fdeep::tensor(fdeep::tensor_shape(static_cast<std::size_t>(4)),
                           std::vector<float>{1, 2, 3, 4})});
    std::cout << fdeep::show_tensors(result) << std::endl;

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