#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include "qcv_algorithm.hpp"
//#include <private/qandroidextras_p.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //auto result = QtAndroidPrivate::checkPermission(QString("android.permission.CAMERA"));
    QQmlApplicationEngine engine;

    //qmlRegisterType<QCvImageProcessor>("com.dev.prod", 1, 0, "CvImageProcessor");

    //const QUrl url(u"qrc:/qtsudoku/src/qml/main.qml"_qs);
    const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,[url](QObject *obj, const QUrl &objUrl) {
        if(!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    engine.load(url);

    return app.exec();
}
