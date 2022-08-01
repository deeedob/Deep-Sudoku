#include "camera_helper.hpp"

CameraHelper::CameraHelper()
	: m_camera( nullptr ) , m_capture( nullptr ) {
}

CameraHelper::~CameraHelper() {
	delete m_camera;
	delete m_capture;
}

void CameraHelper::capture() {
	if( m_capture )
		m_capture->capture();
}

void CameraHelper::handleImageCapture( int , const QImage& preview ) {
	emit imageChanged( preview );
}

QCamera* CameraHelper::getCamera() const {
	return m_camera;
}

void CameraHelper::setCamera( QCamera* camera ) {
	if( m_camera == camera )
		return;
	if( m_capture )
		m_capture->deleteLater();
	
	m_camera = camera;
	m_capture = new QImageCapture( this );
	m_capture->captureSession()->setCamera( m_camera );
	//QCamera::connect( m_capture , &QImageCapture::imageCaptured , this , &CameraHelper::handleImageCapture );
	emit cameraChanged();
	
}