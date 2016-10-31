#ifndef RENDER3D_H
#define RENDER3D_H

#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkMarchingCubes.h>
#include <vtkVolume.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

#include "Logger.h"

class Render3D
{

public:

    /**
     * @brief Render3D
     */
    explicit Render3D();

    /**
     * @brief loadData
     * @param dirName
     */
    void loadData(char* dirName);

    /**
     * @brief getShifter
     * @return
     */
    vtkSmartPointer<vtkImageShiftScale> getShifter();

    /**
     * @brief setShifter
     * @param shifter
     */
    void setShifter(vtkSmartPointer<vtkImageShiftScale> shifter);

    /**
     * @brief getScale
     * @return
     */
    float getScale();

    /**
     * @brief getShift
     * @return
     */
    float getShift();

    /**
     * @brief rescaleData
     * @param scale
     * @param shift
     */
    void rescaleData(float scale, float shift);

    /**
     * @brief extractSurfaces
     * @param isoValue
     */
    void extractSurfaces(double isoValue, vtkImageData *data);

    /**
     * @brief cubeMarchingExtraction
     * @param isoValue
     */
    void cubeMarchingExtraction(double isoValue, vtkImageData *data);

    /**
     * @brief rayCastingRendering
     */
    void rayCastingRendering(vtkImageData *data);

private:

    /**
    * @brief setActormapper
    * @param dataMapper
    */
    void setActormapper(vtkSmartPointer<vtkMapper> dataMapper);

    /**
    * @brief setCameraParameters
    */
    void setCameraParameters();

    /**
    * @brief updateRenderer
    */
    void updateRenderer();


private:

    /**
     * @brief renderer_
     */
    vtkSmartPointer<vtkRenderer> renderer_;
    /**
     * @brief window_
     */
    vtkSmartPointer<vtkRenderWindow> window_;

    /**
     * @brief interactor_
     */
    vtkSmartPointer<vtkRenderWindowInteractor> interactor_;

    /**
     * @brief reader_
     */
    vtkSmartPointer<vtkDICOMImageReader> reader_;

    /**
     * @brief shifter_
     */
    vtkSmartPointer<vtkImageShiftScale> shifter_;

    /**
     * @brief actor_
     */
    vtkSmartPointer<vtkActor> actor_;

    /**
     * @brief isoExtractor_
     */
    vtkSmartPointer<vtkContourFilter> isoExtractor_;

    /**
     * @brief marchCubes_
     */
    vtkSmartPointer<vtkMarchingCubes> marchCubes_;

    /**
     * @brief rayCasting_
     */
    vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCasting_;

    /**
     * @brief camera_
     */
    vtkSmartPointer<vtkCamera> camera_;

public:

    /**
     * @brief isDataLoaded
     */
    bool isDataLoaded;

    /**
     * @brief isDataRescaled
     */
    bool isDataRescaled;
};


#endif
