#include "Thresholding.h"


Thresholding::Thresholding(vtkSmartPointer<vtkImageData> input, unsigned short threshold):
    inputData_(input), threshold_(threshold)
{
    LOG_DEBUG("Initializing Thresholding, threshold = %d", threshold_);

    dimentions_ = inputData_->GetDimensions();

    LOG_DEBUG("dimentions: %d x %d x %d", dimentions_[0],
            dimentions_[1],
            dimentions_[2]);
    extent_ = inputData_->GetExtent();
    doThresholding_();
}

vtkSmartPointer<vtkImageData> Thresholding::getThresholdedData()
{
    return inputData_;
}

void Thresholding::doThresholding_()
{
    LOG_DEBUG("Applying Thresholding");

#pragma omp parallel for
    for(int i = extent_[0]; i <= extent_[1]; i++)
    {
        for(int j = extent_[2]; j <= extent_[3]; j++)
        {
            for(int k = extent_[4]; k <= extent_[5]; k++)
            {
                double data = inputData_->GetScalarComponentAsDouble(i, j, k, 0);
                if(data < threshold_)
                {
                    inputData_->SetScalarComponentFromDouble(i, j, k, 0, 0);
                }

            }
        }
    }
}

