#include <QPainter>
#include <QDebug>
#include <cmath>

#include "inc/constants.h"
#include "inc/gamecontroller.h"
#include "inc/snake.h"

static const int RUNNING_LENGTH = 9;
static const qreal ROTATE_ANGLE = PI / 16;
static const qreal ROTATE_ANGLE_SWIFT = PI / 10;

Snake::Snake(GameController &controller) :
    _head(0, 0),
    _headDirection(PI/2),
    _growing(10),
    _speed(5),
    _moveDirection(NoMove),
    _snakeColor(SNAKE_COLOR_NORMAL),
    _buffTime(-1),
    _lengthAdded(0),
    _rotateBuff(false),
    controller(controller)
{
}

QRectF Snake::boundingRect() const
{
    qreal minX = _head.x();
    qreal minY = _head.y();
    qreal maxX = _head.x();
    qreal maxY = _head.y();

    foreach (QPointF p, _tail) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x() - SNAKE_SIZE/2,  // x
                          tl.y() - SNAKE_SIZE/2,  // y
                          br.x() - tl.x() + SNAKE_SIZE,      // width
                          br.y() - tl.y() + SNAKE_SIZE       //height
                          );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addEllipse(QRectF(- SNAKE_SIZE/2, - SNAKE_SIZE/2, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, _tail) {
        QPointF itemp = mapFromScene(p);
        path.addEllipse(QRectF(itemp.x() - SNAKE_SIZE/2, itemp.y() - SNAKE_SIZE/2, SNAKE_SIZE, SNAKE_SIZE));
    }

    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), _snakeColor);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction)
{
    _moveDirection = direction;
}

Snake::Direction Snake::getMoveDirection()
{
    return _moveDirection;
}

bool Snake::isTheBody(QPointF pos, qreal distance)
{
    bool result = false;
    foreach (QPointF p, _tail) {
        qreal totalDistance = sqrt(pow((pos.x() - p.x()), 2) + pow((pos.y() - p.y()), 2));
        if (totalDistance <= SNAKE_SIZE + distance) {
            result = true;
            break;
        }
    }
    return result;
}

void Snake::setBuff(GameObjectTypes buff, int period)
{
    _buffTime = period;
    _snakeColor = SNAKE_COLOR_BUFF;
    if (buff == GO_Food_Accelerate) {
        if (_speed <= 2) {
            _speed = 1;
        }
        else {
            _speed -= 2;
        }
    }
    else if (buff == GO_Food_SlowDown) {
        _speed += 3;
    }
    else if (buff == GO_Food_AddLength) {
        _growing += 10;
        _lengthAdded += 10;
    }
    else if (buff == GO_Food_RotateSwift) {
        _rotateBuff = true;
    }
}


void Snake::advance(int step)
{
    if (!step) {
        return;
    }
    if (_tickCounter++ % _speed != 0) {
        return;
    }
    if (_moveDirection == NoMove) {
        return;
    }

    if (_growing > 0) {
        QPointF _tailPoint = _head;
        _tail << _tailPoint;
        _growing -= 1;
    } else {
        _tail.takeFirst();
        _tail << _head;
    }

    switch (_moveDirection) {
        case MoveForward:
            moveForward();
            break;
        case TurnLeft:
            turnLeft();
            break;
        case TurnRight:
            turnRight();
            break;
        default:
            break;
    }

    // count buff time and cancel buff
    if (_buffTime > 0) {
        _buffTime--;
    }
    else if (_buffTime == 0) {
        _speed = 5;
        _buffTime--;
        if (_lengthAdded) {
            for (int idx = 0; idx < _lengthAdded; idx++)
                _tail.takeFirst();
        }
        _lengthAdded = 0;
        _rotateBuff = false;
        _snakeColor = SNAKE_COLOR_NORMAL;
    }

    setPos(_head);
    handleCollisions();
}

void Snake::moveForward()
{
    qreal offsetx = RUNNING_LENGTH * cos(_headDirection);
    qreal offsety = RUNNING_LENGTH * sin(_headDirection);
    _head.rx() += offsetx;
    _head.ry() -= offsety;
}

void Snake::turnLeft()
{
    if (_rotateBuff) {
        _headDirection += ROTATE_ANGLE_SWIFT;
    }
    else {
        _headDirection += ROTATE_ANGLE;
    }
    if (_headDirection > PI * 2) {
        _headDirection -= PI * 2;
    }
    moveForward();
}


void Snake::turnRight()
{
    if (_rotateBuff) {
        _headDirection -= ROTATE_ANGLE_SWIFT;
    }
    else {
        _headDirection -= ROTATE_ANGLE;
    }
    if (_headDirection < 0) {
        _headDirection += PI * 2;
    }
    moveForward();
}

void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    // Check collisions with other objects on screen
    foreach (QGraphicsItem *collidingItem, collisions) {
        if (collidingItem->data(GD_Type) == GO_Food_Normal
                || collidingItem->data(GD_Type) == GO_Food_Accelerate
                || collidingItem->data(GD_Type) == GO_Food_AddLength
                || collidingItem->data(GD_Type) == GO_Food_MoreFood
                || collidingItem->data(GD_Type) == GO_Food_SlowDown
                || collidingItem->data(GD_Type) == GO_Food_RotateSwift) {
            // Let GameController handle the event by putting another apple
            qDebug() << "Snake eat food.";
            controller.snakeAteFood(this, (Food *)collidingItem);
            _growing += 1;
        }
    }

    // Check snake eating itself
    if (isTheBody(_head, RUNNING_LENGTH - SNAKE_SIZE - 0.1)) {
        qDebug() << "Snake eat itself.";
        controller.snakeAteItself(this);
    }

    // Check snake hitting the wall
    if (pow(_head.rx(), 2) + pow(_head.ry(), 2) >= pow(WALL_RADIUS - 2, 2)) {
        qDebug() << "Snake hit the wall.";
        controller.snakeHitWall(this);
    }
}
