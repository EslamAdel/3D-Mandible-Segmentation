#include "SimpleView.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleView w;
    w.show();

    return a.exec();
}
