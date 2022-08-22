#include "app.hpp"
#include <QQuickItem>

App::App( QObject* parent )
	: QObject( parent ), m_boardModel( new BoardModel( 9, 9 ))
{
	
	#ifdef ANDROID_BUILD
	QFile f("assets:/fdeep_model.json");
m_deepSolver = std::make_unique<DeepSolver>(f);
	#else
	m_deepSolver = std::make_unique<DeepSolver>( "model/fdeep_model.json" );
	#endif
	
}

App* App::create( QQmlEngine* engine, QJSEngine* jsEngine )
{
	auto* instance = new App( nullptr );
	return instance;
}

BoardModel* App::getModel() const
{
	return m_boardModel;
}

/* this gets the image from qml with the width and height from the scene */
bool App::getQmlImage( QQuickItem* src )
{
	QSharedPointer<const QQuickItemGrabResult> grabber = src->grabToImage();
	connect( grabber.data(), &QQuickItemGrabResult::ready, [ = ]() {
		auto img = grabber->image();
		std::cout << "width: " << img.width() << " height: " << img.height() << std::endl;
		return m_deepSolver->solveFromImage( img );
	} );
	
	return false;
}

bool App::solveGame( const QUrl& url )
{
	qDebug() << url.url();
	bool success = false;
	auto engine = qmlEngine( this );
	if( engine == nullptr ) {
		return success;
	}
	/* the file is inside a image provider */
	if( url.toString().startsWith( "image", Qt::CaseInsensitive )) {
		auto provider = reinterpret_cast<QQuickImageProvider*>(engine->imageProvider( url.host()));
		if( provider->imageType() == QQuickImageProvider::Image ) {
			QImage image = provider->requestImage( url.path().remove( 0, 1 ), nullptr, QSize());
			success = m_deepSolver->solveFromImage( image );
		}
	} else { /* the file is on the system! */
		QImage image( url.url());
		success = m_deepSolver->solveFromImage( image );
	}
	
	if( success ) {
		auto solved_model = m_deepSolver->getBoard();
		m_boardModel->setBoard( solved_model );
	}
	
	return success;
}