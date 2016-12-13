#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include "Logger.h"

class Thresholding
{
public:

    /**
     * @brief Thresholding
     * @param input
     * @param threshold
     */
    explicit Thresholding(vtkImageData* input = NULL,
                          unsigned short threshold = 0);

    /**
     * @brief getThresholdedData
     * @return
     */
    vtkSmartPointer<vtkImageData> getThresholdedData();

    void setInputData(vtkImageData* inData);
    void setThreshold(unsigned int thr);

private:

    /**
     * @brief doThresholding_
     */
    void doThresholding_();

private:

    /**
     * @brief inputData_
     */
    vtkImageData* inputData_;

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
