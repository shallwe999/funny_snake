#include <QGraphicsView>
#include <QTimer>
#include <QDebug>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    game(new GameController(*scene, this))
{
    setCentralWidget(view);
    resize(600, 600);

    initScene();
    initSceneBackground();

    // 在0ms之后，调用receiver的member槽函数，即在下一次事件循环开始时，立刻调用指定的槽函数
    QTimer::singleShot(0, this, SLOT(adjustViewSize()));

}

MainWindow::~MainWindow()
{

}


void MainWindow::initScene()
{
    // 设置了场景的坐标系 500x500
    scene->setSceneRect(-250, -250, 500, 500);

}

void MainWindow::initSceneBackground()
{
    QPixmap bg(GRID_SIZE, GRID_SIZE);
    QPainter p(&bg);
    p.setPen(BG_COLOR_2);
    p.setBrush(QBrush(BG_COLOR_1));
    p.drawRect(0, 0, GRID_SIZE, GRID_SIZE);

    // 使用QBrush绘制背景，铺满整个视图
    view->setBackgroundBrush(QBrush(bg));
}

void MainWindow::adjustViewSize()
{
    qDebug() << "In function adjustViewSize().";
}

