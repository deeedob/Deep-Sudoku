#include <QGuiApplication>
#include <QQmlContext>
#include <filesystem>
#include "app.hpp"
#include "media_capture.hpp"

#ifdef ANDROID_BUILD
	#include <QQmlApplicationEngine>
#else
	#include "enhanced_engine.hpp"

#endif
#include <QtQml/qqmlextensionplugin.h>

Q_IMPORT_PLUGIN( ImagePainterPlugin )

int main( int argc, char* argv[] )
{
	QGuiApplication app( argc, argv );
	QGuiApplication::setOrganizationName( "DeepSudoku" );
	QGuiApplication::setApplicationName( "DeepSudoku" );
	QGuiApplication::setApplicationVersion( "1.0" );
	QGuiApplication::setQuitOnLastWindowClosed( true );
	MediaCapture mediaHelper;
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
	std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path() += "/src/qml/main_hot_reload.qml";
	QUrl url( path.string().c_str());
	QObject::connect( &engine, &QQmlApplicationEngine::objectCreated, &app, [ url ]( QObject* obj, const QUrl& objUrl ) {
		if( !obj && url == objUrl )
			QCoreApplication::exit( -1 );
	}, Qt::QueuedConnection );
	//QQmlEngine::setObjectOwnership()
	engine.rootContext()->setContextProperty( "$QmlEngine", &engine );
	engine.rootContext()->setContextProperty( "mediaHelper", &mediaHelper );
	auto logic = dsdk::App::create( &engine, qjsEngine( &engine ));
	
	engine.load( url );
#endif
	
	return app.exec();
}