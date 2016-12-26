#ifndef SIMPLEVIEW_H
#define SIMPLEVIEW_H

#include <QMainWindow>
#include <Render3D.h>
#include <vtkSmartPointer.h>
#include <QFileDialog>
#include <QString>
#include <QVTKWidget.h>
#include <vtkImageData.h>
#include <ExtractVOI.h>
#include <Thresholding.h>
#include <Segmentation.h>
#include <sys/time.h>
#include <vtkImageResample.h>

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
    void setSliderRanges();

private slots:
    void on_pushButton_clicked();


    void on_horizontalSlider_9_sliderReleased();

    void on_horizontalSlider_9_sliderMoved(int position);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_horizontalSlider_2_sliderReleased();
    void setExtractorRange();

    void on_horizontalSlider_sliderReleased();



    void on_horizontalSlider_5_sliderReleased();

    void on_horizontalSlider_6_sliderReleased();

    void on_horizontalSlider_3_sliderReleased();

    void on_horizontalSlider_4_sliderReleased();

    void on_horizontalSlider_8_sliderReleased();

    void on_pushButton_3_clicked();

private:
    Ui::SimpleView *ui;
    Render3D* renderer_;
    vtkImageData* data_;
    ExtractVOI* extractor_;
    Thresholding* thresholder_;
    Segmentation* segment_;
    int IsoValue_;
    QString volumeDir_;
    int* dataDimentions_;
};

#endif // SIMPLEVIEW_H
