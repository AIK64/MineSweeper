#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::getRowcount()
{
    return ui->spinBoxRow->value();
}
int Dialog::getColcount()
{
    return ui->spinBoxCol->value();
}
int Dialog::getMinecount()
{
    return ui->spinBoxMine->value();
}
