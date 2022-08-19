#include <QGuiApplication>
#include <fdeep/fdeep.hpp>
#include "app.hpp"
#include "media_capture.hpp"

#ifdef ANDROID_BUILD
#include <QQmlApplicationEngine>
#else
#include <filesystem>
#include "enhanced_engine.hpp"
#include <QQmlContext>

#endif

int main( int argc, char* argv[] )
{
	QGuiApplication app( argc, argv );
	MediaCapture mediaHelper;
#ifdef ANDROID_BUILD
	QQmlApplicationEngine engine;
	engine.load(QUrl("qrc:/MainWindow.qml"));
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
	//engine.rootContext()->setContextProperty( "Logic", &App::create( 0, nullptr, nullptr ));
	
	engine.load( url );
#endif
	
	return app.exec();
}