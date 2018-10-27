#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QPainter>

enum GameMode {
    GM_Normal,
    GM_Hard
};

enum GameObjectsData {
    GD_Type
};

enum GameObjectTypes {
    GO_Wall,
    GO_Food_Normal,
    GO_Food_Accelerate,
    GO_Food_SlowDown,
    GO_Food_MoreFood,
    GO_Food_AddLength,
    GO_Food_RotateSwift
};

const int GRID_SIZE = 40;
const qreal PI = 3.1415926536;

const int SNAKE_SIZE = 10;
const int FOOD_SIZE = 4;
const int WALL_RADIUS = 220;

const QColor BG_COLOR_1 = QColor(235, 235, 235);
const QColor BG_COLOR_2 = QColor(160, 160, 160);
const QColor TEXT_COLOR_1 = QColor(51, 200, 85);
const QColor TEXT_COLOR_2 = QColor(254, 67, 101);
const QColor TEXT_COLOR_3 = QColor(122, 122, 200);
const QColor TEXT_COLOR_YES = QColor(30, 144, 200);
const QColor TEXT_COLOR_NO = QColor(156, 156, 156);
const QColor WALL_COLOR_1 = QColor(255, 102, 0);
const QColor WALL_COLOR_2 = QColor(200, 112, 0);
const QColor SNAKE_COLOR_NORMAL = QColor(60, 154, 196);
const QColor SNAKE_COLOR_BUFF = QColor(100, 210, 232);

#endif // CONSTANTS_H
