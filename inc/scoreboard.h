#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QGraphicsItem>
#include <QRectF>
#include <QFile>

#include "inc/constants.h"

void showHiscoreWindow(bool isInMainMenu);
bool showAssureResetWindow();

class Scoreboard : public QGraphicsItem
{
public:
    Scoreboard();
    ~Scoreboard();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void addScore(int score);
    void setGameMode(GameMode mode);
    void cleanHiscores();

    int getNormalHiscore();
    int getHardHiscore();

protected:
    void advance(int step);

private:
    void readHiscores();
    void writeHiscores();
    void setNewHiscore(int newhisc);
    void openHiscoreFile();
    void closeHiscoreFile();

    int _score;
    QString *_scorestr, *_hiscorestr;

    QFile _hiscoreFile;
    int _normalHiscore;
    int _hardHiscore;

    GameMode _mode;
};

#endif // SCOREBOARD_H
