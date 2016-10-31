#ifndef EXTRACTVOI_H
#define EXTRACTVOI_H

#include <vtkExtractVOI.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include "Logger.h"

class ExtractVOI
{
public:

    /**
     * @brief ExtractVOI
     */
    explicit ExtractVOI();

    /**
     * @brief setRange
     * @param xi
     * @param xf
     * @param yi
     * @param yf
     * @param zi
     * @param zf
     */
    void setRange(int xi, int xf,
                  int yi, int yf,
                  int zi, int zf);

    /**
     * @brief setInputData
     * @param data
     */
    void setInputData(vtkAlgorithmOutput* input);

    /**
     * @brief getOutputData
     * @return
     */
    vtkImageData* getOutputData();

private:

    /**
     * @brief extractor_
     */
    vtkSmartPointer<vtkExtractVOI> extractor_;

};


#endif
