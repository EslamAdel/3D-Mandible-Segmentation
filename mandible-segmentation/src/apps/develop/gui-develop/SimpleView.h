#ifndef SIMPLEVIEW_H
#define SIMPLEVIEW_H

#include <QMainWindow>

namespace Ui {
class SimpleView;
}

class SimpleView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleView(QWidget *parent = 0);
    ~SimpleView();

private:
    Ui::SimpleView *ui;
};

#endif // SIMPLEVIEW_H
