#include "SimpleView.h"
#include "ui_SimpleView.h"

SimpleView::SimpleView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleView)
{
    //    QVTKWidget w;
    //    w.GetRenderWindow()

    ui->setupUi(this);
    renderer_ = new Render3D( ui->qvtkWidget->GetRenderWindow());
    extractor_ = new ExtractVOI;
    thresholder_ = new Thresholding;
    segment_ = new Segmentation;
}

SimpleView::~SimpleView()
{
    delete ui;
}

void SimpleView::setSliderRanges()
{
    ui->horizontalSlider_2->setRange(0,dataDimentions_[0]-1);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_5->setRange(0,dataDimentions_[1]-1);
    ui->horizontalSlider_5->setValue(0);
    ui->horizontalSlider_3->setRange(0,dataDimentions_[2]-1);
    ui->horizontalSlider_3->setValue(0);


    ui->horizontalSlider->setRange(0,dataDimentions_[0]-1);
    ui->horizontalSlider->setValue(dataDimentions_[0]-1);
    ui->horizontalSlider_6->setRange(0,dataDimentions_[1]-1);
    ui->horizontalSlider_6->setValue(dataDimentions_[1]-1);
    ui->horizontalSlider_4->setRange(0,dataDimentions_[2]-1);
    ui->horizontalSlider_4->setValue(dataDimentions_[2]-1);
}

void SimpleView::on_pushButton_clicked()
{
    volumeDir_ = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "/home",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);


    renderer_->loadData((char*) volumeDir_.toStdString().c_str());
    renderer_->rescaleData(renderer_->getScale(),
                           renderer_->getShift());
    dataDimentions_ = renderer_->getShifter()->GetOutput()->GetDimensions();
    setSliderRanges();
    extractor_->setInputData(renderer_->getShifter()->GetOutput());
    setExtractorRange();
}



void SimpleView::on_horizontalSlider_9_sliderReleased()
{
    renderer_->extractSurfaces( ui->horizontalSlider_9->value(), data_);
}

void SimpleView::on_horizontalSlider_9_sliderMoved(int position)
{
    ui->label_9->setText(QString::number(position));
}

void SimpleView::on_radioButton_clicked(bool checked)
{
    if(checked)
    {
        renderer_->rayCastingRendering(data_);
    }
}

void SimpleView::on_radioButton_2_clicked(bool checked)
{
    if(checked)
        on_horizontalSlider_9_sliderReleased();
}


void SimpleView::on_horizontalSlider_2_sliderReleased()
{
    setExtractorRange();
}

void SimpleView::setExtractorRange()
{
    extractor_->setRange(ui->horizontalSlider_2->value(), ui->horizontalSlider->value(),
                         ui->horizontalSlider_5->value(), ui->horizontalSlider_6->value(),
                         ui->horizontalSlider_3->value(), ui->horizontalSlider_4->value());
    data_ = extractor_->getOutputData();
    ui->qvtkWidget->GetRenderWindow()->Render();

}

void SimpleView::on_horizontalSlider_sliderReleased()
{
    setExtractorRange();
}


void SimpleView::on_horizontalSlider_5_sliderReleased()
{
    setExtractorRange();
}

void SimpleView::on_horizontalSlider_6_sliderReleased()
{
    setExtractorRange();
}

void SimpleView::on_horizontalSlider_3_sliderReleased()
{
    setExtractorRange();
}

void SimpleView::on_horizontalSlider_4_sliderReleased()
{
    setExtractorRange();
}

void SimpleView::on_horizontalSlider_8_sliderReleased()
{
    thresholder_->setInputData(extractor_->getOutputData());
    thresholder_->setThreshold(ui->horizontalSlider_8->value());
    data_ = thresholder_->getThresholdedData();
    ui->qvtkWidget->GetRenderWindow()->Render();

}

void SimpleView::on_pushButton_3_clicked()
{
    segment_->setInputData(thresholder_->getThresholdedData());
    data_ = segment_->getSegmentedData();
    ui->qvtkWidget->GetRenderWindow()->Render();
}
