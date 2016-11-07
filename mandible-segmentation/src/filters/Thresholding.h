#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPointData.h>

#include "Logger.h"

class Thresholding
{
public:

    /**
     * @brief ExtractVOI
     */
    explicit Thresholding(vtkSmartPointer<vtkImageData> input,
                          unsigned short threshold);

    /**
     * @brief getThresholdedData
     * @return
     */
    vtkSmartPointer<vtkImageData> getThresholdedData();

private:

    /**
     * @brief doThresholding_
     */
    void doThresholding_();

    /**
     * @brief createImageData_
     */
    void createImageData_();

private:

    /**
     * @brief inputData_
     */
    vtkSmartPointer<vtkImageData> inputData_;

    /**
     * @brief outputData_
     */
    vtkSmartPointer<vtkImageData> outputData_;

    /**
     * @brief threshold_
     */
    unsigned short threshold_;

    /**
     * @brief dimentions_
     */
    int* dimentions_;

    /**
     * @brief extent_
     */
    int* extent_;
};


#endif
