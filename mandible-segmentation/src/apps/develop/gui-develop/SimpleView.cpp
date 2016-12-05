#include "SimpleView.h"
#include "ui_SimpleView.h"

SimpleView::SimpleView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleView)
{
    ui->setupUi(this);
}

SimpleView::~SimpleView()
{
    delete ui;
}
