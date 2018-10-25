#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>

#include "gamecontroller.h"
#include "constants.h"
#include "food.h"
#include "snake.h"
#include "wall.h"
#include "scoreboard.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this)),
    scoreboard(new Scoreboard(*this))
{
    timer.start(15);  //游戏循环的定时器（15毫秒1帧）

    scene.installEventFilter(this);

    QTimer::singleShot(0, this, SLOT(showMainMenu()));

}

GameController::~GameController()
{
}

void GameController::showMainMenu()
{
    _inMainMenu = true;
    _inGaming = false;
    _inGameOver = false;

    scene.clear();

    QGraphicsTextItem *text = new QGraphicsTextItem("Funny Snake");
    text->setFont(QFont("Times New Roman", 50, 75, true));
    text->setPos(QPointF(- text->boundingRect().width() / 2, -150));
    text->setDefaultTextColor(TEXT_COLOR_1);
    scene.addItem(text);
}

void GameController::restartGame()
{
    qDebug() << "In restartGame().";
    scene.clear();

    Wall *boundaryWall = new Wall();
    scene.addItem(boundaryWall);

    snake = new Snake(*this);
    scene.addItem(snake);

    scoreboard = new Scoreboard(*this);
    scene.addItem(scoreboard);

    Food *fd1 = new Food(0, -100, GO_Food_Normal);
    scene.addItem(fd1);
    Food *fd2 = new Food(0, 100, GO_Food_Normal);
    scene.addItem(fd2);
    _foodEaten = 0;

    resume();
}

void GameController::addNewFood()
{
    _foodEaten++;
    qreal x, y;

    do {
        int radius = static_cast<int>((qrand() % 190));
        qreal angle = static_cast<qreal>( (static_cast<double>(qrand()) / RAND_MAX) * PI*2 );
        x = radius * cos(angle);
        y = radius * sin(angle);

    } while (snake->isTheBody(QPointF(x, y), FOOD_SIZE));

    GameObjectTypes foodKind = GO_Food_Normal;
    if (_foodEaten > 10) {
        // special food
        qreal randnum = static_cast<qreal>( static_cast<double>(qrand()) / RAND_MAX );
        if (randnum > 0.8) {
            int buff = static_cast<int>((qrand() % 4));
            switch (buff) {
                case 0:
                    foodKind = GO_Food_Accelerate;
                    break;
                case 1:
                    foodKind = GO_Food_SlowDown;
                break;
                case 2:
                    foodKind = GO_Food_MoreFood;
                    break;
                case 3:
                    foodKind = GO_Food_AddLength;
                    break;
            }
        }
    }

    Food *food = new Food(x, y, foodKind);
    scene.addItem(food);
    qDebug() << "Add new food.  x:" << x << " y:" << y << " kind:" << static_cast<int>(foodKind);

    if ((_foodEaten % 10 == 0) && _foodEaten > 0) {
        addNewFood();  // add another
    }
}

void GameController::snakeAteFood(Snake *snake, Food *food)
{
    // check special food
    GameObjectTypes buff = food->getFoodKind();
    if (buff == GO_Food_Accelerate
            || buff == GO_Food_SlowDown
            || buff == GO_Food_AddLength) {
        snake->setBuff(buff, 50);
        scoreboard->addScore(3);
    }
    else if (buff == GO_Food_MoreFood) {
        addNewFood();
        scoreboard->addScore(3);
    }
    else if (buff == GO_Food_Normal) {
        scoreboard->addScore(1);
    }

    scene.removeItem(food);

    food = nullptr;
    //delete food;  // 好像是这里出问题，会异常退出，等待解决

    addNewFood();
}

void GameController::snakeHitWall(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::snakeAteItself(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::gameOver()
{
    qDebug() << "Game over.";

    pause();
    _inMainMenu = false;
    _inGaming = false;
    _inGameOver = true;

    QGraphicsTextItem *text = new QGraphicsTextItem("Game Over");
    text->setFont(QFont("Times New Roman", 50, 75, true));
    text->setPos(QPointF(- text->boundingRect().width() / 2, -150));
    text->setDefaultTextColor(TEXT_COLOR_2);
    scene.addItem(text);
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
}

void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
}

void GameController::mainMenu_handleKeyPressed(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        qDebug() << "Press the key (in main menu).";
        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                // 退出主菜单
                disconnect(&timer, SIGNAL(timeout()),
                        this, SLOT(showMainMenu()));

                _inMainMenu = false;
                _inGaming = true;
                _inGameOver = false;
                restartGame();
                break;
            default:
                break;
        }
    }
}

void GameController::gaming_handleKeyPressed(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        qDebug() << "Press the key (in gaming).";
        switch (event->key()) {
            case Qt::Key_Up:
                if (snake->getMoveDirection() == Snake::NoMove) {
                    snake->setMoveDirection(Snake::MoveForward);
                }
                break;
            case Qt::Key_Left:
                if (snake->getMoveDirection() == Snake::MoveForward) {
                    snake->setMoveDirection(Snake::TurnLeft);
                }
                else if (snake->getMoveDirection() == Snake::TurnRight) {
                    snake->setMoveDirection(Snake::MoveForward);
                }
                break;
            case Qt::Key_Right:
                if (snake->getMoveDirection() == Snake::MoveForward) {
                    snake->setMoveDirection(Snake::TurnRight);
                }
                else if (snake->getMoveDirection() == Snake::TurnLeft) {
                    snake->setMoveDirection(Snake::MoveForward);
                }
                break;
        }
    }
}

void GameController::gaming_handleKeyReleased(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        qDebug() << "Release the key (in gaming).";
        switch (event->key()) {
            case Qt::Key_Left:
                if (snake->getMoveDirection() == Snake::TurnLeft) {
                    snake->setMoveDirection(Snake::MoveForward);
                }
                else if (snake->getMoveDirection() == Snake::MoveForward) {
                    snake->setMoveDirection(Snake::TurnRight);
                }
                break;
            case Qt::Key_Right:
                if (snake->getMoveDirection() == Snake::TurnRight) {
                    snake->setMoveDirection(Snake::MoveForward);
                }
                else if (snake->getMoveDirection() == Snake::MoveForward) {
                    snake->setMoveDirection(Snake::TurnLeft);
                }
                break;
        }
    }
}

void GameController::gameOver_handleKeyPressed(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        qDebug() << "Press the key (in game over).";
        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                showMainMenu();
                break;
        }
    }
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (_inMainMenu) {
            mainMenu_handleKeyPressed(dynamic_cast<QKeyEvent *>(event));
        }
        else if (_inGaming) {
            gaming_handleKeyPressed(dynamic_cast<QKeyEvent *>(event));
        }
        else if (_inGameOver) {
            gameOver_handleKeyPressed(dynamic_cast<QKeyEvent *>(event));
        }
        else {
        }
        return true;
    }
    else if (event->type() == QEvent::KeyRelease) {
        if (_inGaming) {
            gaming_handleKeyReleased(dynamic_cast<QKeyEvent *>(event));
        }
        else {
        }
        return true;
    }
    else {
        return QObject::eventFilter(object, event);
    }
}