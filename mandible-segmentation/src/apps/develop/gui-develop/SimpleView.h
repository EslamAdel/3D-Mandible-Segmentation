#ifndef SIMPLEVIEW_H
#define SIMPLEVIEW_H

#include <QMainWindow>
#include <Render3D.h>
#include <vtkSmartPointer.h>
#include <QFileDialog>
#include <QString>
#include <QVTKWidget.h>

namespace Ui {
class SimpleView;
}

class SimpleView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleView(QWidget *parent = 0);
    ~SimpleView();

private slots:
    void on_pushButton_clicked();


    void on_horizontalSlider_9_sliderReleased();

    void on_horizontalSlider_9_sliderMoved(int position);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

private:
    Ui::SimpleView *ui;
    Render3D* renderer_;
    int IsoValue_;
    QString volumeDir_;
};

#endif // SIMPLEVIEW_H
