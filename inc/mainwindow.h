#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;
class GameController;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void adjustViewSize();

    void on_actionStartNM_triggered();
    void on_actionStartHM_triggered();
    void on_actionRules_triggered();
    void on_actionAbout_triggered();
    void on_actionQuit_triggered();
    void on_actionPause_triggered();
    void on_actionHiscores_triggered();

private:
    void initScene();
    void initSceneBackground();

    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsView *view;
    GameController *game;

};

#endif // MAINWINDOW_H
