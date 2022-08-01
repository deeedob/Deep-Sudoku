#include "media_capture.hpp"
#include <iostream>

MediaCapture::MediaCapture( QObject* parent )
	: QObject( parent ) , m_session( nullptr ) {
	
}

void MediaCapture::setSession( QMediaCaptureSession* session ) {
	if( session == m_session || session == nullptr )
		return;
	m_session = session;
	/* will result in Format_RGB32 */
	m_session->imageCapture()->setFileFormat( QImageCapture::JPEG );
	connect( m_session->imageCapture() , &QImageCapture::imageCaptured , this , &MediaCapture::handleImageCaptured );
}

QMediaCaptureSession* MediaCapture::getSession() const {
	return m_session;
}

void MediaCapture::handleImageCaptured( int id , const QImage& preview ) {
	std::cout << " id: " << id << " w: " << preview.format() << std::endl;
}