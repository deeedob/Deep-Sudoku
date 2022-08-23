#include "image_painter.hpp"

ImagePainter::ImagePainter( QQuickItem* parent )
	: QQuickPaintedItem( parent )
{
	QQuickPaintedItem::setAntialiasing( true );
	setFillColor( QColor::fromRgb( 255, 0, 0 ));
	fillColor();
	setRenderTarget( RenderTarget::Image );
}

void ImagePainter::paint( QPainter* painter )
{
	painter->drawImage( QPointF( 0.0f, 0.0f ), m_image );
}

void ImagePainter::itemChange( QQuickItem::ItemChange change, const QQuickItem::ItemChangeData& data )
{
	QQuickPaintedItem::itemChange( change, data );
}

void ImagePainter::setImage( const QImage& other ) noexcept
{
	m_image = other;
}

const QImage& ImagePainter::getImage() const
{
	return m_image;
}