#ifndef QGRAPHIC_ARC_ITEM_HPP
#define QGRAPHIC_ARC_ITEM_HPP

#include <QGraphicsEllipseItem>

class QGraphicsArcItem: public QGraphicsEllipseItem{

public:

  QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height,
		   QGraphicsItem *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QPointF centerPoint; // Store the center point
	QPointF startPoint;
	QPointF endPoint;
	double spanAngle = 0;    // Store the span angle
};


#endif
