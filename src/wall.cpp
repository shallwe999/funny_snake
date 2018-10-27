#include <QPainter>

#include "inc/constants.h"
#include "inc/wall.h"

Wall::Wall()
{
    _wallRadius = WALL_RADIUS;
    _limitRadius = WALL_RADIUS;
    setData(GD_Type, GO_Wall);
}

Wall::Wall(int minWallRadius, int maxWallradius)
{
    _wallRadius = minWallRadius;
    _limitRadius = maxWallradius;
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
    update();

    painter->restore();
}

QPainterPath Wall::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(0, 0), _wallRadius, _wallRadius);
    return p;
}

int Wall::getWallRadius()
{
    return _wallRadius;
}

void Wall::addWallRadius(int added)
{
    if (_wallRadius + added <= _limitRadius) {
        _wallRadius += added;
    }
}
