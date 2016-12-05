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
    renderer_->rayCastingRendering(renderer_->getShifter()->GetOutput());


}
