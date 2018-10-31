#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QPushButton>

#include "inc/gamecontroller.h"
#include "inc/constants.h"
#include "inc/food.h"
#include "inc/snake.h"
#include "inc/wall.h"
#include "inc/scoreboard.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this)),
    scoreboard(nullptr),
    readyText(nullptr),
    _modePointed(GM_Normal)
{
    timer.start(18);  //游戏循环的定时器（18毫秒1帧）

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

    QGraphicsTextItem *modeText1 = new QGraphicsTextItem("Play Normal Mode");
    modeText1->setFont(QFont("Times New Roman", 25, 75, true));
    modeText1->setPos(QPointF(- modeText1->boundingRect().width() / 2, 40));

    QGraphicsTextItem *modeText2 = new QGraphicsTextItem("Play Hard Mode");
    modeText2->setFont(QFont("Times New Roman", 25, 75, true));
    modeText2->setPos(QPointF(- modeText2->boundingRect().width() / 2, 100));
    modeText2->setDefaultTextColor(TEXT_COLOR_NO);

    if (_modePointed == GM_Normal) {
        modeText1->setDefaultTextColor(TEXT_COLOR_YES);
        modeText2->setDefaultTextColor(TEXT_COLOR_NO);
    }
    else {
        modeText2->setDefaultTextColor(TEXT_COLOR_YES);
        modeText1->setDefaultTextColor(TEXT_COLOR_NO);
    }

    scene.addItem(modeText1);
    scene.addItem(modeText2);
}

void GameController::showReadyText()
{
    if (snake->getMoveDirection() == Snake::NoMove) {
        readyText = new QGraphicsTextItem("Ready?");
        readyText->setFont(QFont("Times New Roman", 30, 75, true));
        readyText->setPos(QPointF(- readyText->boundingRect().width() / 2, 25));
        readyText->setDefaultTextColor(TEXT_COLOR_3);
        scene.addItem(readyText);

        hintText = new QGraphicsTextItem("Press UP to move!");
        hintText->setFont(QFont("Times New Roman", 20, 75, true));
        hintText->setPos(QPointF(- hintText->boundingRect().width() / 2, 95));
        hintText->setDefaultTextColor(TEXT_COLOR_3);
        scene.addItem(hintText);
    }
}

void GameController::hideReadyText()
{
    if (readyText != nullptr) {
        scene.removeItem(readyText);
        readyText = nullptr;
        scene.removeItem(hintText);
        hintText = nullptr;
    }

}

void GameController::restartGame()
{
    qDebug() << "In restartGame().";
    scene.clear();

    Wall *boundaryWall = new Wall();
    scene.addItem(boundaryWall);

    snake = new Snake(*this);
    scene.addItem(snake);

    scoreboard = new Scoreboard();
    scoreboard->setGameMode(GM_Normal);
    scene.addItem(scoreboard);

    Food *fd1 = new Food(0, -100, GO_Food_Normal);
    scene.addItem(fd1);
    Food *fd2 = new Food(0, 100, GO_Food_Normal);
    scene.addItem(fd2);
    _foodEaten = 0;

    QTimer::singleShot(0, this, SLOT(showReadyText()));
}

void GameController::restartHard()
{
    qDebug() << "In restartHard().";
    scene.clear();

    hardModeWall = new Wall(110, 210);
    scene.addItem(hardModeWall);

    snake = new Snake(*this);
    snake->updateWallRadius(hardModeWall->getWallRadius());
    scene.addItem(snake);

    scoreboard = new Scoreboard();
    scoreboard->setGameMode(GM_Hard);
    scene.addItem(scoreboard);

    Food *fd1 = new Food(0, -60, GO_Food_Normal);
    scene.addItem(fd1);
    Food *fd2 = new Food(0, 60, GO_Food_Normal);
    scene.addItem(fd2);
    _foodEaten = 0;

    QTimer::singleShot(0, this, SLOT(showReadyText()));
}

void GameController::addNewFood()
{
    _foodEaten++;
    qreal x, y;

    if (_gameMode == GM_Normal) {
        do {
            int radius = qrand() % 190;
            qreal angle = static_cast<qreal>( (static_cast<double>(qrand()) / RAND_MAX) * PI*2 );
            x = static_cast<qreal>(radius) * cos(angle);
            y = static_cast<qreal>(radius) * sin(angle);

        } while (snake->isTheBody(QPointF(x, y), FOOD_SIZE));
    }
    else if (_gameMode == GM_Hard) {
        do {
            int limitRadius = hardModeWall->getWallRadius() - 10;
            qDebug() << limitRadius;
            int radius = qrand() % limitRadius;
            qreal angle = static_cast<qreal>( (static_cast<double>(qrand()) / RAND_MAX) * PI*2 );
            x = static_cast<qreal>(radius) * cos(angle);
            y = static_cast<qreal>(radius) * sin(angle);

        } while (snake->isTheBody(QPointF(x, y), FOOD_SIZE));
    }


    GameObjectTypes foodKind = GO_Food_Normal;
    if (_foodEaten > 10) {  //调试用1 运行用10
        // special food
        qreal randnum = static_cast<qreal>( static_cast<double>(qrand()) / RAND_MAX );
        if (randnum > 0.8) {  //调试用0.1 运行用0.8
            int buff = static_cast<int>((qrand() % 5));
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
                case 4:
                    foodKind = GO_Food_RotateSwift;
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
            || buff == GO_Food_AddLength
            || buff == GO_Food_RotateSwift) {
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

    if (_gameMode == GM_Hard) {
        // add wall radius (hard mode)
        hardModeWall->addWallRadius(1);
        snake->updateWallRadius(hardModeWall->getWallRadius());
    }

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

    _inMainMenu = false;
    _inGaming = false;
    _inGameOver = true;
    pause();

    QGraphicsTextItem *text = new QGraphicsTextItem("Game Over");
    text->setFont(QFont("Times New Roman", 50, 75, true));
    text->setPos(QPointF(- text->boundingRect().width() / 2, -150));
    text->setDefaultTextColor(TEXT_COLOR_2);
    scene.addItem(text);
}

void GameController::pause()
{
    if (!_paused) {
        qDebug() << "Paused.";
        disconnect(&timer, SIGNAL(timeout()),
                   &scene, SLOT(advance()));
        _paused = true;

        // show pause string
        if (_inGaming) {
            pauseText = new QGraphicsTextItem("Paused");
            pauseText->setFont(QFont("Times New Roman", 40, 75, true));
            pauseText->setPos(QPointF(- pauseText->boundingRect().width() / 2, -150));
            pauseText->setDefaultTextColor(TEXT_COLOR_YES);
            scene.addItem(pauseText);
        }
    }
}

void GameController::resume()
{
    if (_paused && !_inGameOver) {
        qDebug() << "Resumed.";
        connect(&timer, SIGNAL(timeout()),
                &scene, SLOT(advance()));
        _paused = false;

        if (_inGaming) {
            scene.removeItem(pauseText);
            pauseText = nullptr;
        }
    }
}

bool GameController::getPaused()
{
    return _paused;
}

void GameController::mainMenu_buttonPressed(GameMode mode)
{
    _gameMode = mode;
    qDebug() << "Press the startButton.";
    if (_inMainMenu) {
        disconnect(&timer, SIGNAL(timeout()),
                this, SLOT(showMainMenu()));
    }

    if (!_inGaming) {
        _inMainMenu = true;  // 就当作都是在main menu界面进入
        _inGaming = false;
        _inGameOver = false;
        resume();
    }
    _inMainMenu = false;
    _inGaming = true;
    _inGameOver = false;

    if (mode == GM_Normal) {
        restartGame();
    }
    else if (mode == GM_Hard) {
        restartHard();
    }
}

bool GameController::isInMainMenu()
{
    return _inMainMenu;
}

void GameController::mainMenu_handleKeyPressed(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {

        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                // 退出主菜单，进入游戏
                mainMenu_buttonPressed(_modePointed);
                break;
            case Qt::Key_Up:  //只有两个选项，并在一起
            case Qt::Key_Down:
                if (_modePointed == GM_Normal) {
                    _modePointed = GM_Hard;
                }
                else {
                    _modePointed = GM_Normal;
                }
                showMainMenu();
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
                    QTimer::singleShot(0, this, SLOT(hideReadyText()));
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
