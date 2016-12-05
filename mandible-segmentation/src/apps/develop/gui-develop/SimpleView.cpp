#include "SimpleView.h"
#include "ui_SimpleView.h"

SimpleView::SimpleView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleView)
{
    //    QVTKWidget w;
    //    w.GetRenderWindow()

    ui->setupUi(this);
    renderer_ = new Render3D;
    renderer_->setRenderWindow( ui->qvtkWidget->GetRenderWindow());
}

SimpleView::~SimpleView()
{
    delete ui;
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
    on_horizontalSlider_9_sliderReleased();

}



void SimpleView::on_horizontalSlider_9_sliderReleased()
{

    renderer_->extractSurfaces( ui->horizontalSlider_9->value(), renderer_->getShifter()->GetOutput());
}

void SimpleView::on_horizontalSlider_9_sliderMoved(int position)
{
    ui->label_9->setText(QString::number(position));
}
