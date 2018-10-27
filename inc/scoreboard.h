#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QGraphicsItem>
#include <QRectF>

#include "inc/constants.h"

class GameController;

class Scoreboard : public QGraphicsItem
{
public:
    Scoreboard(GameController & controller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void addScore(int score);

protected:
    void advance(int step);

private:
    int _score;
    QString *_scorestr;

    GameController &controller;
};

#endif // SCOREBOARD_H
