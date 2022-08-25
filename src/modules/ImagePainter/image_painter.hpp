#pragma once
#include <QtQuick/QQuickPaintedItem>
#include <QtQmlIntegration>
#include <QtGui/QPainter>

namespace dsdk
{
	/*!
 * @ingroup QT
 * @brief Sending images from C++ to QML
 * @author Dennis Oberst
 * @todo fuly implement this.
 */
	class ImagePainter : public QQuickPaintedItem
	{
		QML_ELEMENT
	public:
		ImagePainter( QQuickItem* parent = nullptr );
		void paint( QPainter* painter ) override;
		void setImage( const QImage& other ) noexcept;
		const QImage& getImage() const;
	
	protected:
		void itemChange( ItemChange change, const ItemChangeData& data ) override;
	signals:
		void imageChanged();
	private:
		QImage m_image;
	};
}