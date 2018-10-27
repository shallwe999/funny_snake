#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>

class Wall : public QGraphicsItem
{
public:
    Wall();
    Wall(int minWallRadius, int maxWallradius);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QPainterPath shape() const;

    int getWallRadius();
    void addWallRadius(int added);

private:
    int _wallRadius;
    int _limitRadius;
};

#endif // WALL_H
