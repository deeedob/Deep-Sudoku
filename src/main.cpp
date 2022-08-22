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

#include <QFileInfo>
#include <QDir>
#include <string>

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
	qDebug() << "Readable? " << dir.isReadable();
	qDebug() << "root path" << dir.absolutePath();
	qDebug() << "pathhh" << dir.filesystemAbsolutePath();
	for( int i = 0; i < list.size(); ++i ) {
		QFileInfo fileInfo = list.at( i );
		qDebug() << fileInfo;
	}
	QFile file( "assets:/fdeep_model.json" );
	qDebug() << "Path: " << file.fileName();
	file.open( QFile::ReadOnly );
	qDebug() << file.readAll();
	
	std::ifstream in_stream( "assets:/fdeep_model.json" );
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