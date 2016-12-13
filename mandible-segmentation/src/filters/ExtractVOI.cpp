#include "ExtractVOI.h"



ExtractVOI::ExtractVOI()
{
    LOG_DEBUG("Initializing Sub Volume Extractor");
    extractor_ = vtkSmartPointer<vtkExtractVOI>::New();
}

void ExtractVOI::setRange(int xi, int xf, int yi, int yf, int zi, int zf)
{
    LOG_DEBUG("Setting Sub Volume Range %d:%d, %d:%d, %d:%d",xi,xf,yi,yf,zi,zf);
    extractor_->SetVOI(xi, xf,
                       yi, yf,
                       zi, zf);

    extractor_->SetSampleRate(1, 1, 1);
    extractor_->Update();

}

void ExtractVOI::setInputData(vtkImageData* input)
{
    LOG_DEBUG("Setting Input Data");
    extractor_->SetInputData(input);
}

vtkImageData* ExtractVOI::getOutputData()
{
    LOG_DEBUG("Getting Output Data");
    int* x = new int[3];
    x = extractor_->GetOutput()->GetDimensions();
    LOG_DEBUG("x = %d, y = %d, z = %d", x[0], x[1], x[2]);
    return extractor_->GetOutput();
}
