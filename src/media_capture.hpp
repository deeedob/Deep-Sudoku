#pragma once
#include <QObject>
#include <QMediaCaptureSession>
#include <QImageCapture>

class MediaCapture : public QObject
{
Q_OBJECT
	Q_PROPERTY( QMediaCaptureSession * session READ getSession WRITE setSession )
public:
	explicit MediaCapture( QObject* parent = nullptr );
	void setSession( QMediaCaptureSession* session );
	QMediaCaptureSession* getSession() const;

public slots:
	void handleImageCaptured( int id , const QImage& preview );

private:
	QMediaCaptureSession* m_session;
};