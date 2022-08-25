#pragma once
#include <QtQuick/QQuickPaintedItem>
#include <QtQmlIntegration>
#include <QtGui/QPainter>

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