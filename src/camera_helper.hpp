#pragma once
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QPointer>

class CameraHelper : public QObject
{
Q_OBJECT
	Q_PROPERTY( QCamera* m_cam READ getCamera WRITE setCamera NOTIFY cameraChanged )
public:
	CameraHelper();
	~CameraHelper();
	QCamera* getCamera() const;
	void setCamera( QCamera* camera );

public slots:
	void capture();
	void handleImageCapture( int , const QImage& preview );

signals:
	void cameraChanged();
	void imageChanged( const QImage& image );

private:
private:
	QCamera* m_camera;
	QImageCapture* m_capture;
};