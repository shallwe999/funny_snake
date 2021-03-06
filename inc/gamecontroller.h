#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsItem>

#include "constants.h"

class QGraphicsScene;
class QKeyEvent;

class Snake;
class Food;
class Wall;
class Scoreboard;

class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QGraphicsScene &scene, QObject *parent = nullptr);
    ~GameController();

    void mainMenu_buttonPressed(GameMode mode);

    void snakeAteFood(Snake *snake, Food *food);
    void snakeHitWall(Snake *snake);
    void snakeAteItself(Snake *snake);

    bool getPaused();
    bool isInMainMenu();

public slots:
    void showMainMenu();
    void showReadyText();
    void hideReadyText();
    void pause();
    void resume();
    void gameOver();
    void restartGame();
    void restartHard();

protected:
     bool eventFilter(QObject *object, QEvent *event);

private:
    void mainMenu_handleKeyPressed(QKeyEvent *event);
    void gaming_handleKeyPressed(QKeyEvent *event);
    void gaming_handleKeyReleased(QKeyEvent *event);
    void gameOver_handleKeyPressed(QKeyEvent *event);
    void addNewFood();

    QTimer timer;
    QGraphicsScene &scene;

    Snake *snake;
    Wall *hardModeWall;
    Scoreboard *scoreboard;
    QGraphicsTextItem *readyText, *hintText, *pauseText;

    GameMode _modePointed;
    GameMode _gameMode;

    int _foodEaten;

    bool _paused = true;
    bool _inMainMenu = true;
    bool _inGaming = false;
    bool _inGameOver = false;
};

#endif // GAMECONTROLLER_H
