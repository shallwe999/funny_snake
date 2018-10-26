#include <string>
#include <QDebug>

#include "scoreboard.h"

Scoreboard::Scoreboard(GameController &controller) :
    _score(0),
    controller(controller)
{
    _scorestr = new QString("");

}

QRectF Scoreboard::boundingRect() const
{

    QRectF bound = QRectF(-300,  // x
                          -300,  // y
                          600,      // width
                          300       //height
                          );
    return bound;
}

QPainterPath Scoreboard::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    _scorestr->replace(0, 99, QString("Score: %1").arg(_score));
    QString tempstr = QString(_scorestr->data());
    path.addText( QPointF(-200, -220), QFont("Times New Roman", 25, 70, true), tempstr );

    return path;
}

void Scoreboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), TEXT_COLOR_2);
    painter->restore();
    update();
}

void Scoreboard::advance(int step)
{
    return;
}

void Scoreboard::addScore(int score)
{
    _score += score;
}
