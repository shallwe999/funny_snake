#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <ctime>
#include <QPushButton>

#include "inc/constants.h"
#include "inc/gamecontroller.h"
#include "inc/mainwindow.h"
#include "inc/scoreboard.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    game(new GameController(*scene, this))
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/data/game-icon"));
    setWindowTitle(tr("Funny Snake"));
    setCentralWidget(view);
    resize(620, 620);
    setMinimumSize(620, 620);

    initScene();
    initSceneBackground();

    // 在0ms之后，调用receiver的member槽函数，即在下一次事件循环开始时，立刻调用指定的槽函数
    QTimer::singleShot(0, this, SLOT(adjustViewSize()));

    // 生成随机种子
    qsrand(static_cast<uint>(time(nullptr)));
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

void MainWindow::on_actionRules_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Instructions"));
    msgBox.setText(tr("                    ##  Game  Rules  ##\n"\
                      "- A simple snake game!\n"\
                      "- You can move all directions inside the wall.\n"\
                      "- The red bean is normal (+1 pt).\n"\
                      "- The blue bean is special (+3pts).\n"\
                      "- Blue beans have many buffs, just have a try.\n"\
                      "\n"\
                      "                  ##  Keyboard Config  ##\n"\
                      "- Main menu\n"\
                      "    - Press ENTER to start.\n"\
                      "    - Press UP or DOWN to select mode.\n"\
                      "- Playing\n"\
                      "    - Press UP to start moving.\n"\
                      "    - Press LEFT or RIGHT to change the direction.\n"\
                      "- Game over\n"\
                      "    - Press ENTER to back to the main menu.\n"));
    msgBox.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(tr("Name: funny snake\n"\
                      "Author: Shallwe\n"\
                      "Date: 2018/10/27\n"\
                      "Version: Beta\n"\
                      "Instruction: A simple snake game based on Qt5.\n"));
    msgBox.exec();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionStartNM_triggered()
{
    game->mainMenu_buttonPressed(GM_Normal);
}

void MainWindow::on_actionStartHM_triggered()
{
    game->mainMenu_buttonPressed(GM_Hard);
}

void MainWindow::on_actionPause_triggered()
{
    if (game->getPaused()) {
        game->resume();
    }
    else {
        game->pause();
    }
}

void MainWindow::on_actionHiscores_triggered()
{
    showHiscoreWindow();
}
