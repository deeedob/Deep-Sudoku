#pragma once
#include <QObject>
#include <QMediaCaptureSession>
#include <QImageCapture>

namespace dsdk
{
	/*!
 * @ingroup QT
 * @brief Connecting with QML MediaCaptureSession at runtime.
 * @author Dennis Oberst
 * @todo add more functionality. Live Detection?
 */
	class MediaCapture : public QObject
	{
	Q_OBJECT
		Q_PROPERTY( QMediaCaptureSession* session READ getSession WRITE setSession )
	public:
		explicit MediaCapture( QObject* parent = nullptr );
		void setSession( QMediaCaptureSession* session );
		QMediaCaptureSession* getSession() const;
	
	public slots:
		void handleImageCaptured( int id, const QImage& preview );
	
	private:
		QMediaCaptureSession* m_session;
	};
}