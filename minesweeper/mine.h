#ifndef MINE_H
#define MINE_H
#include<iostream>
#include<ctime>

using namespace std;

class mine
{
public:
    mine(int row,int col,int minenum);
    ~mine();
    int getcol(){return col;}
    int getrow(){return row;}
    int getminenum(){return minenum;}
    int getflagnum(){return flagnum;}
    void addflagnum();
    void minflagnum();
    struct info
    {
        bool ismine;
        bool isopen;
        bool isflaged;
        int Srdcount;
    }**p;

private:
    int row,col,minenum;
    int flagnum;

};

#endif // MINE_H
