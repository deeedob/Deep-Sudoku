#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qcvimageprocessor.hpp>
//#include <private/qandroidextras_p.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//dauto result = QtAndroidPrivate::checkPermission(QString("android.permission.CAMERA"));
    QQmlApplicationEngine engine;

    //const QUrl url(u"qrc:/qtsudoku/src/qml/main.qml"_qs);
    const QUrl url = QUrl::fromLocalFile("src/qml/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,[url](QObject *obj, const QUrl &objUrl) {
        if(!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

   // qmlRegisterType<Producer>("com.dev.prod", 1, 0, "Producer");

    engine.load(url);

    return app.exec();
}
