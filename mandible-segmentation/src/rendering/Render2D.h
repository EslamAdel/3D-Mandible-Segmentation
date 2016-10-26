#ifndef RENDER2D_H
#define RENDER2D_H

#include <vtkDICOMImageReader.h>
#include <vtkSmartPointer.h>
#include <vtkResliceImageViewer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMatrix4x4.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>
#include <vtkImageShiftScale.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkCommand.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkObjectFactory.h>

#include "Logger.h"

class Render2D
{
public:
    explicit Render2D();
    void loadData(char* dirName);
    void rescaleData(float scale, float shift);
    void setAxialSlicing();
    void setCronalSlicing();
    void setSagittalSLicing();
    float getScaleVaue();
    float getShiftValue();

private:
    void setProjectionMatrices();
    void updateRenderer();


private:
    vtkSmartPointer<vtkDICOMImageReader> reader_;
    vtkSmartPointer<vtkResliceImageViewer> imViewer_;
    vtkSmartPointer<vtkRenderer> renderer_;
    vtkSmartPointer<vtkRenderWindow> renderWindow_;
    vtkSmartPointer<vtkRenderWindowInteractor> rwInteractor_;
    vtkSmartPointer<vtkImageReslice> reslicer_;
    vtkSmartPointer<vtkImageShiftScale> shifter_;
    vtkSmartPointer<vtkMatrix4x4> axial_;
    vtkSmartPointer<vtkMatrix4x4> cronal_;
    vtkSmartPointer<vtkMatrix4x4> sagittal_;
    bool isDataLoaded_;
    bool isDataRescaled_;

};


#endif
