#include <string>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QObject>

#include "inc/scoreboard.h"

Scoreboard::Scoreboard() :
    _score(0),
    _normalHiscore(0),
    _hardHiscore(0)
{
    _scorestr = new QString("");
    _hiscorestr = new QString("");
    openHiscoreFile();
    readHiscores();
}

Scoreboard::~Scoreboard()
{
    writeHiscores();
    closeHiscoreFile();
    delete _scorestr;
    delete _hiscorestr;
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
    QString tempstr1 = QString(_scorestr->data());
    path.addText( QPointF(-200, -240), QFont("Times New Roman", 22, 75, true), tempstr1 );

    if (_mode == GM_Normal) {
        _hiscorestr->replace(0, 99, QString("Hiscore: %1").arg(_normalHiscore));
        QString tempstr2 = QString(_hiscorestr->data());
        path.addText( QPointF(60, -240), QFont("Times New Roman", 22, 75, true), tempstr2 );
    }
    else if (_mode == GM_Hard) {
        _hiscorestr->replace(0, 99, QString("Hiscore: %1").arg(_hardHiscore));
        QString tempstr2 = QString(_hiscorestr->data());
        path.addText( QPointF(60, -240), QFont("Times New Roman", 22, 75, true), tempstr2 );
    }

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

void Scoreboard::openHiscoreFile()
{
    qDebug() << "Scoreboard file opened.";
    _hiscoreFile.setFileName("hiscdata.dat");
    _hiscoreFile.open(QIODevice::ReadWrite);
}

void Scoreboard::closeHiscoreFile()
{
    qDebug() << "Scoreboard file closed.";
    _hiscoreFile.close();
}

void Scoreboard::readHiscores()
{
    qDebug() << "Scoreboard file read.";
    QDataStream in(&_hiscoreFile);
    in.device()->seek(0);

    // 检查魔术数字
    quint32 magic;
    in >> magic;
    if (magic != 0xA0B0C0D0) {
        qDebug() << "Hiscore data file read error, initialize a new file.";
        _normalHiscore = 0;
        _hardHiscore = 0;
    }
    else {
        qint32 nhs, hhs;
        in >> nhs >> hhs;
        _normalHiscore = static_cast<int>(nhs);
        _hardHiscore = static_cast<int>(hhs);
    }

}

void Scoreboard::setNewHiscore(int newhisc)
{
    if (_mode == GM_Normal) {
        _normalHiscore = newhisc;
    }
    else if (_mode == GM_Hard) {
        _hardHiscore = newhisc;
    }
}

void Scoreboard::writeHiscores()
{
    qDebug() << "Scoreboard file written.";
    QDataStream out(&_hiscoreFile);
    out.device()->seek(0);

    // 写入魔术数字和最高分
    qint32 nhs = static_cast<qint32>(_normalHiscore);
    qint32 hhs = static_cast<qint32>(_hardHiscore);
    out << static_cast<quint32>(0xA0B0C0D0) << nhs << hhs;
}

void Scoreboard::cleanHiscores()
{
    _normalHiscore = 0;
    _hardHiscore = 0;
    writeHiscores();
}

void Scoreboard::setGameMode(GameMode mode)
{
    _mode = mode;
}

void Scoreboard::addScore(int score)
{
    _score += score;
    if ((_mode == GM_Normal && _score > _normalHiscore)
            || (_mode == GM_Hard && _score > _hardHiscore)) {
        setNewHiscore(_score);
    }
}

int Scoreboard::getNormalHiscore()
{
    return _normalHiscore;
}

int Scoreboard::getHardHiscore()
{
    return _hardHiscore;
}

void showHiscoreWindow(bool isInMainMenu)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(QObject::tr("Hiscore"));

    Scoreboard sbd;
    int nhs = sbd.getNormalHiscore(), hhs = sbd.getHardHiscore();
    QString hstext = QString("Normal mode:    %1\nHard   mode:    %2").arg(nhs).arg(hhs);

    msgBox.setText(QObject::tr("  ###  Hiscore  ###"));
    msgBox.setInformativeText(hstext);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Apply);
    msgBox.setButtonText(QMessageBox::Apply, "Reset");
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        qDebug() << "Choose OK.";
        break;
    case QMessageBox::Apply:
        qDebug() << "Choose reset.";
        bool result = showAssureResetWindow();
        if (result && !isInMainMenu) {
            QMessageBox submsgBox;
            submsgBox.setWindowTitle(QObject::tr("Warning"));
            submsgBox.setIcon(QMessageBox::Information);
            submsgBox.setText(QObject::tr("Back to main menu first."));
            submsgBox.exec();
        }
        else if (result) {
            sbd.cleanHiscores();
        }
        break;
    }
}

bool showAssureResetWindow()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(QObject::tr("Assure"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(QObject::tr("Sure to reset hiscore data?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        qDebug() << "Assure reset.";
        return true;
        break;
    case QMessageBox::Cancel:
        qDebug() << "Cancel reset.";
        return false;
        break;
    default:
        return false;
        break;
    }
}
