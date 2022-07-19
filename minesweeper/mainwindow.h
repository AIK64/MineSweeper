#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"mine.h"
#include"dialog.h"
#include <QMainWindow>
#include <iostream>
#include <QTimer>
#include<QPainter>
#include <QPixmap>
#include<QPaintEvent>
#include<QMouseEvent>
#include<QMessageBox>
#include<QDialog>
#include<ctime>
#include<QDebug>

#define ELENUM_LENGTH 21
#define TITLE_HEIGHT (35+TITLE_BEGIN)
#define TITLE_BEGIN 23
#define EXPRESSION_LENGTH 24
#define EXPRESSION_HEIGHT 24
#define user_defRowMAX 8
#define user_defColMAX 8


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);
    void painttitle(QPainter *painter);
    void paintmine(QPainter *painter);
    void mousePressEvent(QMouseEvent *event); //鼠标点击响应函数
    void showmine(int x,int y);
    int iswin();
    void Restart(int row,int col,int minenum); //重新开始函数
    enum StateFlag
    {
        FirstOpen,
        LeftClicked,
        RightClicked,
        MineClicked,
        GameOver
    }STATE_FLAG;

private:
    Ui::MainWindow *ui;

    int blocklength,blockwidth;
    QTimer *ftimer; //定时器
    int timecount;   //计时器
    int minecount;  //记录雷的总数
    int LENGTH,HEIGHT;
    int timeend=1;  //初始时不计时
    int isend=0;    //初始时置为0
    mine *block=NULL;
    int click_x=0,click_y=0;
private slots:
    void on_timer_timeout();
    void on_actionPaimary_triggered();
    void on_actionMiddle_triggered();
    void on_actionquit_triggered();
    void on_actionSenior_triggered();
    void on_actionuser_define_triggered();
};


#endif // MAINWINDOW_H
