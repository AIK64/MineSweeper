#include "mainwindow.h"
#include "ui_mainwindow.h"

QPainter *mypainter;

int ROW=9;
int COL=9;
int MINESUM=10;
//int click_x,click_y;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{//主界面的默认构造函数

    ui->setupUi(this);
    timecount=0;
    minecount=MINESUM;
    ftimer=new QTimer(this);
    ftimer->stop();
    ftimer->setInterval(1000);  //设置定时器的周期 单位为毫秒
    connect(ftimer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));
    //ftimer->start();
    block=new mine(ROW,COL,MINESUM);
    timeend=1;
    STATE_FLAG=FirstOpen;

    blocklength=21;
    blockwidth=21;
    LENGTH=block->getcol()*blockwidth;   //窗口的长度
    HEIGHT=block->getrow()*blocklength +TITLE_HEIGHT;   //窗口不包含菜单栏的高度
    this->setMinimumSize(LENGTH,HEIGHT);       //两个语句的作用是固定窗口的大小
    this->setMaximumSize(LENGTH,HEIGHT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{//用户鼠标点击，判断左右键，确定落点，修改块信息，确定是否结束
    click_x=event->x();
    click_y=event->y();
    if(event->buttons()==(Qt::LeftButton)&&(click_x>LENGTH*100/200-11)&&(click_x<LENGTH*100/200+11+EXPRESSION_LENGTH)&&(click_y>7+TITLE_BEGIN)
            &&(click_y<7+TITLE_BEGIN+EXPRESSION_HEIGHT))
    {
        Restart(block->getrow(),block->getcol(),block->getminenum());
        STATE_FLAG=FirstOpen;
        isend=0;
        timeend=1;
        ftimer->stop();
        update();
        return;
    }
    else if(click_y<TITLE_HEIGHT)return; //点击到笑脸以外的其他位置则不做任何处理
    if(isend==1)
        return;
    //剩下的情况是点击到下面雷区的情况
    click_x=click_x/blockwidth;
    click_y=(click_y-TITLE_HEIGHT)/blocklength;
    if(event->buttons()==(Qt::LeftButton))
    {
        if(timeend==1)
        {
            ftimer->start();
        }
        if(block->p[click_y][click_x].ismine)
        {
            block->p[click_y][click_x].isopen=1;
            STATE_FLAG=MineClicked;
            isend=1;
            timeend=1;
            ftimer->stop();
            update();
        }
        else if(block->p[click_y][click_x].ismine==0)
        {//点击的方块不是雷的情况
            if(block->p[click_y][click_x].Srdcount==0)
            {
                showmine(click_y,click_x); //对周围方块递归打开
            }
            else
            {
                block->p[click_y][click_x].isopen=1;
            }
            STATE_FLAG=LeftClicked;
            if((isend = iswin())==1)
            {
                STATE_FLAG=GameOver;
                timeend=1;
                ftimer->stop();
            }
            update();
            if(isend)
            {
                QMessageBox message(QMessageBox::NoIcon,"you are win.","very good");
                message.exec();
            }
        }
        else
        {
            return;
        }
    }
    else if(event->buttons()==(Qt::RightButton))
    {//点击的是右键的情况
        if(block->p[click_y][click_x].isflaged==0)
        {
         block->p[click_y][click_x].isflaged=1;
         block->addflagnum();
        }
        else if(block->p[click_y][click_x].isflaged==1)
        {
            block->p[click_y][click_x].isflaged=0;
            block->minflagnum();
        }
        STATE_FLAG=RightClicked;
        if((isend=iswin())==1)
        {
            STATE_FLAG=GameOver;
            timeend=1;
            ftimer->stop();
        }
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    mypainter=&painter;

    QPixmap block_close(":/images/images/unOpendBlock.png");
    QPixmap block_open(":/images/images/block_open.bmp");
    QPixmap red_flag(":/images/images/redflag.bmp");
    QPixmap keil_boom(":/images/images/keilboom.bmp");
    QPixmap click_boom(":/images/images/clickboom.bmp");
    QPixmap boom(":/images/images/boom.bmp");
    QPixmap blk_1(":/images/images/blk1.bmp");
    QPixmap blk_2(":/images/images/blk2.bmp");
    QPixmap blk_3(":/images/images/blk3.bmp");
    QPixmap blk_4(":/images/images/blk4.bmp");
    QPixmap blk_5(":/images/images/blk5.bmp");
    QPixmap blk_6(":/images/images/blk6.bmp");
    QPixmap blk_7(":/images/images/blk7.bmp");
    QPixmap blk_8(":/images/images/blk8.bmp");

    if(STATE_FLAG==FirstOpen)
    {//第一次打开的情况
        painttitle(mypainter);
        for(int i=0;i<block->getcol()*blockwidth;i=i+blockwidth)
            for(int j=TITLE_HEIGHT;j<TITLE_HEIGHT+block->getrow()*blocklength;j=j+blocklength)
                painter.drawPixmap(i,j,blockwidth,blocklength,block_close);
    }
    else if(STATE_FLAG==MineClicked)
    {//点到炸弹的情况
        painttitle(mypainter);
        for(int i=0;i<block->getrow();i++)
        {
            for(int j=0;j<block->getcol();j++)
            {
                if((block->p[i][j].ismine==0)&&(block->p[i][j].isopen==0)&&(block->p[i][j].isflaged==0))
                {//不是雷并且还没有被打开也没有红旗
                    painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,block_close);
                }
                else if(block->p[i][j].ismine==1)
                {//该块为雷的情况
                    painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,boom);
                }
                else if((block->p[i][j].ismine==1)&&(block->p[i][j].isflaged==1))
                {//是雷并且有红旗的情况
                    painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,red_flag);
                }
                else if((block->p[i][j].ismine==0)&&(block->p[i][j].isflaged==1))
                {//不是雷并且有红旗的情况
                    painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,keil_boom);
                }
                else if((block->p[i][j].ismine==0)&&(block->p[i][j].isopen==1))
                {//不是雷并且已经被打开的情况
                    int blk_num=block->p[i][j].Srdcount;
                    switch (blk_num) {
                    case 0:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,block_open);break;
                    case 1:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_1);break;
                    case 2:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_2);break;
                    case 3:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_3);break;
                    case 4:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_4);break;
                    case 5:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_5);break;
                    case 6:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_6);break;
                    case 7:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_7);break;
                    case 8:painter.drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_8);break;

                    }
                }
            }
        }
        //绘制点击到的炸弹会将原来的图片覆盖
        painter.drawPixmap(click_x*blockwidth,click_y*blocklength+TITLE_HEIGHT,blockwidth,blocklength,click_boom);
    }
    else if(STATE_FLAG==LeftClicked)
    {//左键点击到空白情况
        painttitle(mypainter);
        paintmine(mypainter);
    }
    else if(STATE_FLAG==RightClicked)
    {//右键点击到空白的情况
        painttitle(mypainter);
        paintmine(mypainter);
    }
    else if(STATE_FLAG==GameOver)
    {
        painttitle(mypainter);
        paintmine(mypainter);
    }
}

void MainWindow::showmine(int y,int x)
{//对空白的方块周围方块进行遍历的函数
    for(int blockx=x-1;blockx<=x+1;blockx++)
        for(int blocky=y-1;blocky<=y+1;blocky++)
        {
            if(blockx<0||blocky<0||blockx>=block->getcol()||blocky>=block->getrow())
                continue;

            if((blockx==x)&&(blocky==y))
            {
                block->p[blocky][blockx].isopen=1;
                continue;
            }

            if((block->p[blocky][blockx].ismine==0)&&(block->p[blocky][blockx].isopen==0))
            {
                block->p[blocky][blockx].isopen=1;
                if(block->p[blocky][blockx].Srdcount==0)
                    showmine(blocky,blockx);
            }

        }
}

int MainWindow::iswin()
{
    int cnt=0;
    for(int row=0;row<block->getrow();row++)
        for(int col=0;col<block->getcol();col++)
        {
            if((block->p[row][col].isopen==1)&&(block->p[row][col].ismine==0))
                cnt++;
        }
    if(cnt>=((block->getrow()*block->getcol())-block->getminenum()))
        return 1;
    else
        return 0;
}
//重新开始游戏
void MainWindow::Restart(int row ,int col,int minenum)
{
    isend=0;
    timeend=1;
    timecount=0;
    ftimer->stop();
    block= new mine(row,col,minenum);
    LENGTH=block->getcol()*blockwidth;
    HEIGHT=block->getrow()*blocklength+TITLE_HEIGHT;
    this->setMinimumSize(LENGTH,HEIGHT);
    this->setMaximumSize(LENGTH,HEIGHT);
}

void MainWindow::painttitle(QPainter *painter)
{
    QPixmap mine_title(":/images/images/mine_title.bmp");
    QPixmap qq_smile(":/images/images/SmileFace.png");
    QPixmap qq_lose(":/images/images/UnSmileFace.png");
    QPixmap qq_pround(":/images/images/CoolFace.png");
    QPixmap ele_0(":/images/images/ele0.bmp");
    QPixmap ele_1(":/images/images/ele1.bmp");
    QPixmap ele_2(":/images/images/ele2.bmp");
    QPixmap ele_3(":/images/images/ele3.bmp");
    QPixmap ele_4(":/images/images/ele4.bmp");
    QPixmap ele_5(":/images/images/ele5.bmp");
    QPixmap ele_6(":/images/images/ele6.bmp");
    QPixmap ele_7(":/images/images/ele7.bmp");
    QPixmap ele_8(":/images/images/ele8.bmp");
    QPixmap ele_9(":/images/images/ele9.bmp");
    QPixmap ele_10(":/images/images/ele10.bmp");
    //调整大小
    mine_title=mine_title.scaled(QSize(LENGTH,40));
    //显示标题部分
    painter->drawPixmap(0,TITLE_BEGIN,mine_title);
    if(STATE_FLAG==MineClicked)
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_lose);
    else if(STATE_FLAG==GameOver)
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_pround);
    else
    {
        painter->drawPixmap((int)LENGTH*100/200-11,7+TITLE_BEGIN,qq_smile);
    }
    //显示红旗数量
    int redflagnum=block->getflagnum();
    if(redflagnum<0)
    {
        redflagnum=-redflagnum;
        painter->drawPixmap((int)(LENGTH*40)/200-30,5+TITLE_BEGIN,ele_10);
    }
    else
        painter->drawPixmap((int)(LENGTH*40/200-30),5+TITLE_BEGIN,ele_0);
    switch (redflagnum/10)
    {//红旗十位的数字
            case 0:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_0);break;
            case 1:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_1);break;
            case 2:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_2);break;
            case 3:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_3);break;
            case 4:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_4);break;
            case 5:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_5);break;
            case 6:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_6);break;
            case 7:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_7);break;
            case 8:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_8);break;
            case 9:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_9);break;
    }
    switch(redflagnum%10)
    {//红旗个位的数字
        case 0:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_0);break;
        case 1:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_1);break;
        case 2:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_2);break;
        case 3:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_3);break;
        case 4:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_4);break;
        case 5:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_5);break;
        case 6:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_6);break;
        case 7:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_7);break;
        case 8:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_8);break;
        case 9:painter->drawPixmap((int)(LENGTH*40)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_9);break;
    }
    //显示时间
    int timenum=timecount;
    switch(timenum/100)
    {//时间的百位数字
        case 0:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_0);break;
        case 1:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_1);break;
        case 2:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_2);break;
        case 3:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_3);break;
        case 4:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_4);break;
        case 5:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_5);break;
        case 6:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_6);break;
        case 7:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_7);break;
        case 8:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_8);break;
        case 9:painter->drawPixmap((int)(LENGTH*163)/200-30,5+TITLE_BEGIN,ele_9);break;
    }
    switch(timenum/10-(timenum/100)*10)
    {//时间的十位数字
        case 0:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_0);break;
        case 1:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_1);break;
        case 2:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_2);break;
        case 3:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_3);break;
        case 4:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_4);break;
        case 5:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_5);break;
        case 6:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_6);break;
        case 7:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_7);break;
        case 8:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_8);break;
        case 9:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH,5+TITLE_BEGIN,ele_9);break;
    }
    switch(timenum%10)
    {//时间的个位数字
        case 0:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_0);break;
        case 1:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_1);break;
        case 2:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_2);break;
        case 3:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_3);break;
        case 4:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_4);break;
        case 5:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_5);break;
        case 6:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_6);break;
        case 7:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_7);break;
        case 8:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_8);break;
        case 9:painter->drawPixmap((int)(LENGTH*163)/200-30+ELENUM_LENGTH+ELENUM_LENGTH,5+TITLE_BEGIN,ele_9);break;
    }
}

void MainWindow::paintmine(QPainter *painter)
{//绘制正常游戏中的雷区
    //加载位图
        QPixmap block_close(":/images/images/unOpendBlock.png");
        QPixmap block_open(":/images/images/block_open.bmp");
        QPixmap red_flag(":/images/images/redflag.bmp");
        QPixmap keil_boom(":/images/images/keilboom.bmp");
        QPixmap click_boom(":/images/images/clickboom.bmp");
        QPixmap boom(":/images/images/boom.bmp");
        QPixmap blk_1(":/images/images/blk1.bmp");
        QPixmap blk_2(":/images/images/blk2.bmp");
        QPixmap blk_3(":/images/images/blk3.bmp");
        QPixmap blk_4(":/images/images/blk4.bmp");
        QPixmap blk_5(":/images/images/blk5.bmp");
        QPixmap blk_6(":/images/images/blk6.bmp");
        QPixmap blk_7(":/images/images/blk7.bmp");
        QPixmap blk_8(":/images/images/blk8.bmp");
        for(int i=0;i<block->getrow();i++)
        {
            for(int j=0;j<block->getcol();j++)
            {
                if((block->p[i][j].ismine==0)&&(block->p[i][j].isopen==0)&&(block->p[i][j].isflaged==0))
                {//方块不是雷没有被打开没有被旗子标记的情况000
                    painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,block_close);
                }
                else if((block->p[i][j].isopen==0)&&(block->p[i][j].isflaged==1))
                {//不是雷没有被打开但是旗子标记001
                    painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,red_flag);
                }
                else if((block->p[i][j].ismine==1)&&(block->p[i][j].isopen==0)&&(block->p[i][j].isflaged==0))
                {//是雷但是没有被打开100
                    painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,block_close);
                }
                else if((block->p[i][j].ismine==1)&&(block->p[i][j].isopen==1)&&(block->p[i][j].isflaged==0))
                {//是雷并且被打开110
                    painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,click_boom);
                }
                else if((block->p[i][j].isopen==1)&&(block->p[i][j].ismine==0))
                {//已经被打开并且不是雷的情况
                    int blk_num=block->p[i][j].Srdcount;
                    switch(blk_num)
                    {
                        case 0:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,block_open);break;
                        case 1:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_1);break;
                        case 2:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_2);break;
                        case 3:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_3);break;
                        case 4:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_4);break;
                        case 5:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_5);break;
                        case 6:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_6);break;
                        case 7:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_7);break;
                        case 8:painter->drawPixmap(j*blockwidth,i*blocklength+TITLE_HEIGHT,blockwidth,blocklength,blk_8);break;

                    }
                }
            }
        }
}

void MainWindow::on_timer_timeout()
{
    timecount++;
    update();
}

void MainWindow::on_actionPaimary_triggered()
{
    Restart(9,9,10);
}

void MainWindow::on_actionMiddle_triggered()
{
    Restart(16,16,40);
}

void MainWindow::on_actionSenior_triggered()
{
    Restart(16,30,99);
}

void MainWindow::on_actionquit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionuser_define_triggered()
{
    int Row,Col,Mine=0;
    bool NumRight=0;
    Dialog *dlginput= new Dialog(this);
    Qt::WindowFlags  flags=dlginput->windowFlags();
    dlginput->setWindowFlags(flags| Qt::MSWindowsFixedSizeDialogHint);   //以模态方式显示对话框
    while(NumRight==0)
    {
        int ret=dlginput->exec();
        if(ret==QDialog::Accepted)
        {//ok按键被按下时
            Row=dlginput->getRowcount();
            Col=dlginput->getColcount();
            Mine=dlginput->getMinecount();
            if((Row<user_defRowMAX)||(Col<user_defColMAX))
            {//提示输入数据有误
                QMessageBox::warning(dlginput,"错误","输入数据有误");
            }
            else if((Mine<=0)||(Mine>=Row*Col))
            {//提示输入雷数有误
                QMessageBox::warning(dlginput,"错误","输入雷数有误");
            }
            else
            {
                NumRight=1;
            }

        }
        else if(ret==QDialog::Rejected)
        {//当cancel按键按下时
            return;
        }
    }
    delete dlginput;
    Restart(Row,Col,Mine);
}
