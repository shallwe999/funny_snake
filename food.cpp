#include <QPainter>

#include "constants.h"
#include "food.h"

Food::Food(qreal x, qreal y, GameObjectTypes kind)
{
    setPos(x, y);
    setData(GD_Type, kind);
    _foodKind = kind;
}

QRectF Food::boundingRect() const
{
    // 求food的外接矩形
    return QRectF(-GRID_SIZE,    -GRID_SIZE,
                   GRID_SIZE * 2, GRID_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    if (_foodKind == GO_Food_Normal) {
        painter->fillPath(shape(), Qt::red);
    }
    else {
        painter->fillPath(shape(), Qt::blue);
    }

    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(GRID_SIZE / 2, GRID_SIZE / 2), FOOD_SIZE, FOOD_SIZE);
    return p;
}

GameObjectTypes Food::getFoodKind() const
{
    return _foodKind;
}
