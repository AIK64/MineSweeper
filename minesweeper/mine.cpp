#include "mine.h"
#include<cstdlib>

mine::mine(int row,int col,int minenum)
{
    //创建二维数组用于保存不同方块的信息
    p=new info *[row];
    for(int i=0;i<row;i++)
        p[i]=new info[col];
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
        {
            p[i][j].ismine=0;
            p[i][j].isflaged=0;
            p[i][j].isopen=0;
            p[i][j].Srdcount=0;
        }
    srand((unsigned)time(NULL));
    for(int i=1;i<=minenum;i++)
    {
        int minerow=rand()%row;
        int minecol=rand()%col;
        if(p[minerow][minecol].ismine==0)
        {
            p[minerow][minecol].ismine=1;
            continue;
        }
        i--;
    }
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
        {
            if(p[i][j].ismine==0)
            {
                int srandmine=0;
                for(int blockrow=i-1;blockrow<=i+1;blockrow++)
                {
                    for(int blockcol=j-1;blockcol<=j+1;blockcol++)
                    {
                        if(blockrow<0||blockcol<0||blockrow>=row||blockcol>=col)
                            continue;
                        if(p[blockrow][blockcol].ismine)
                            srandmine++;
                    }
                }
                p[i][j].Srdcount=srandmine;
            }
        }
    this->row=row;
    this->col=col;
    this->minenum=minenum;
    this->flagnum=0;
}

mine::~mine()
{
    for(int i=0;i<row;i++)
    {
        delete[] p[i];
    }
    delete[] p;
}

void mine::addflagnum()
{
    flagnum++;
}

void mine::minflagnum()
{
    flagnum--;
}
