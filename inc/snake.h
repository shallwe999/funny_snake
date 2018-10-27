#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

#include "inc/constants.h"

class GameController;

class Snake : public QGraphicsItem
{
public:
    enum Direction {
        NoMove,
        MoveForward,
        TurnLeft,
        TurnRight
    };

    Snake(GameController & controller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setMoveDirection(Direction direction);
    Snake::Direction getMoveDirection();
    bool isTheBody(QPointF pos, qreal distance);

    void setBuff(GameObjectTypes buff, int period);

protected:
    void advance(int step);

private:
    void moveForward();
    void turnLeft();
    void turnRight();

    void handleCollisions();

    QPointF        _head;
    qreal          _headDirection;
    int            _growing;
    int            _speed;
    QList<QPointF> _tail;
    int            _tickCounter;
    Direction      _moveDirection;
    QColor         _snakeColor;

    int            _buffTime;
    int            _lengthAdded;
    bool           _rotateBuff;

    GameController &controller;
};

#endif // SNAKE_H
