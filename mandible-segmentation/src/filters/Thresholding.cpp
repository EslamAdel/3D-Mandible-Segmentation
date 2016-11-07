#include "Thresholding.h"


Thresholding::Thresholding(vtkSmartPointer<vtkImageData> input, unsigned short threshold):
    inputData_(input), threshold_(threshold)
{
    LOG_DEBUG("Initializing Thresholding, threshold = %d", threshold_);
    outputData_ = vtkSmartPointer<vtkImageData>::New();

    dimentions_ = inputData_->GetDimensions();

    LOG_DEBUG("dimentions: %d x %d x %d", dimentions_[0],
            dimentions_[1],
            dimentions_[2]);
    extent_ = inputData_->GetExtent();
    doThresholding_();
}

vtkSmartPointer<vtkImageData> Thresholding::getThresholdedData()
{
    return outputData_;
}

void Thresholding::doThresholding_()
{
    LOG_DEBUG("Applying Thresholding");

    LOG_DEBUG("%d, %d, %d, %d, %d, %d", extent_[0],
            extent_[1],
            extent_[2],
            extent_[3],
            extent_[4],
            extent_[5]);


#pragma omp parallel for
    for(int i = extent_[0]; i <= extent_[1]; i++)
    {
        for(int j = extent_[2]; j <= extent_[3]; j++)
        {
            for(int k = extent_[4]; k <= extent_[5]; k++)
            {
                double data = inputData_->GetScalarComponentAsDouble(i, j, k,0);
                if(data < threshold_)
                {
                    outputData_->SetScalarComponentFromDouble(i, j, k, 0, 0);
                }
                else
                {
                    outputData_->SetScalarComponentFromDouble(i, j, k, 0, data);
                }

            }
        }
    }
}

void Thresholding::createImageData_()
{

    outputData_->SetDimensions(dimentions_[0], dimentions_[1], dimentions_[2]);
    outputData_->SetScalarType(VTK_UNSIGNED_SHORT, inputData_->GetInformation());
    outputData_->SetNumberOfScalarComponents(1, inputData_->GetInformation());
    double* origin = inputData_->GetOrigin();
    double* spacing = inputData_->GetSpacing();
    outputData_->SetOrigin(origin[0],
            origin[1], origin[2]);
    outputData_->SetSpacing(spacing[0],
            spacing[1],
            spacing[2]);
    outputData_->SetExtent(extent_[0], extent_[1],
            extent_[2], extent_[3],
            extent_[4], extent_[5]);
    outputData_->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
}

