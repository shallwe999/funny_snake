#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsItem>

#include "inc/constants.h"

class Food : public QGraphicsItem
{
public:
    Food(qreal x, qreal y, GameObjectTypes kind);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QPainterPath shape() const;

    GameObjectTypes getFoodKind() const;

private:
    GameObjectTypes _foodKind;
};

#endif // FOOD_H
