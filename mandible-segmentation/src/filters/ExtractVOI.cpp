#include "ExtractVOI.h"



ExtractVOI::ExtractVOI()
{
    LOG_DEBUG("Initializing Sub Volume Extractor");
    extractor_ = vtkSmartPointer<vtkExtractVOI>::New();
}

void ExtractVOI::setRange(int xi, int xf, int yi, int yf, int zi, int zf)
{
    LOG_DEBUG("Setting Sub Volume Range");
    extractor_->SetVOI(xi, xf,
                       yi, yf,
                       zi, zf);

    extractor_->SetSampleRate(1, 1, 1);

}

void ExtractVOI::setInputData(vtkImageData *data)
{
    LOG_DEBUG("Setting Input Data");
    extractor_->SetInputData(data);
    extractor_->ReleaseDataFlagOff();
     extractor_->Update();
}

vtkImageData* ExtractVOI::getOutputData()
{
    LOG_DEBUG("Getting Output Data");
    return extractor_->GetOutput();
}
