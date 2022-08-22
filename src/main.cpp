#include <QGuiApplication>
#include <QQmlContext>
#include "app.hpp"
#include "media_capture.hpp"

#ifdef ANDROID_BUILD
	#include <QQmlApplicationEngine>
#else
#include <filesystem>
#include "enhanced_engine.hpp"

#endif
/* __ TEST __ */
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <string>

const QString assetsPath = "assets:/";

QString resolveModelFilePath(QString modelName)
{
    QString file = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QDir::separator() + modelName;
    QString def  = assetsPath + QDir::separator() + modelName;

    return file;
}

int main( int argc, char* argv[] )
{
	QGuiApplication app( argc, argv );
	QGuiApplication::setOrganizationName( "DeepSudoku" );
	QGuiApplication::setApplicationName( "DeepSudoku" );
	QGuiApplication::setApplicationVersion( "1.0" );
	QGuiApplication::setQuitOnLastWindowClosed( true );

	
	
	MediaCapture mediaHelper;
	QDir dir( "assets:" ); //with / and without /
	QFileInfoList list = dir.entryInfoList();
	for( int i = 0; i < list.size(); ++i ) {
		QFileInfo fileInfo = list.at( i );
		qDebug() << fileInfo;
	}
	
	QString modelPath = resolveModelFilePath("fdeep_model.json");
	qDebug() << "Model Path os: " << modelPath;
	QString randPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
	qDebug() << "rand path: " << randPath;
	QFile model("assets:/fdeep_model.json");
	model.copy(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	QDir dirr(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
	model.open(QFile::ReadOnly);
	for (int i = 0; i < 100; i++) {
	    qDebug() << "READ:" << model.read(100)q;
	}
	
	QFileInfoList listt = dirr.entryInfoList();
	for (int i = 0; i < listt.size(); i++) {
	    QFileInfo fileInfo = listt.at(i);
		qDebug() << fileInfo;
	}
	
	std::ifstream in_stream( modelPath.toStdString().c_str() );
	if( !in_stream.is_open(), std::ios_base::in ) {
		qDebug() << "in stream no open!";
	}
	std::string line;
	while ( std::getline( in_stream, line )) {
		qDebug() << line.c_str();
	}

#ifdef ANDROID_BUILD
	QQmlApplicationEngine engine;
	QUrl url( "qrc:/MainWindow.qml" );
	
	QObject::connect( &engine, &QQmlApplicationEngine::objectCreated, &app, [ url ]( QObject* obj, const QUrl& objUrl ) {
		if( !obj && url == objUrl )
			QCoreApplication::exit( -1 );
	}, Qt::QueuedConnection );
	
	engine.rootContext()->setContextProperty( "mediaHelper", &mediaHelper );
	engine.load( QUrl( "qrc:/MainWindow.qml" ));
#else
	EnhancedEngine engine;
	QUrl url( "qrc:/main_hot_reload.qml" );
	QObject::connect( &engine, &QQmlApplicationEngine::objectCreated, &app, [ url ]( QObject* obj, const QUrl& objUrl ) {
		if( !obj && url == objUrl )
			QCoreApplication::exit( -1 );
	}, Qt::QueuedConnection );
	//QQmlEngine::setObjectOwnership()
	engine.rootContext()->setContextProperty( "$QmlEngine", &engine );
	engine.rootContext()->setContextProperty( "mediaHelper", &mediaHelper );
	
	engine.load( url );
#endif
	
	return app.exec();
}