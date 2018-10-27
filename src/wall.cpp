#include <QPainter>

#include "inc/constants.h"
#include "inc/wall.h"

Wall::Wall()
{
    setData(GD_Type, GO_Wall);
}

QRectF Wall::boundingRect() const
{
    // 求wall的外接矩形
    return QRectF(-250, -250, 500, 500);
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen( QPen(WALL_COLOR_1, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->drawPath(shape());
    painter->setPen( QPen(WALL_COLOR_2, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    painter->drawPath(shape());

    painter->restore();
}

QPainterPath Wall::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(0, 0), WALL_RADIUS, WALL_RADIUS);
    return p;
}
