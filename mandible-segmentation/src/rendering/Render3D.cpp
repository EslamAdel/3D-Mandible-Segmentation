#include "Render3D.h"

Render3D::Render3D()
{
    LOG_DEBUG("Initializing The 3D Renderer");
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    window_ = vtkSmartPointer<vtkRenderWindow>::New();
    window_->AddRenderer(renderer_);

    interactor_ = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor_->SetRenderWindow(window_);
    actor_ = vtkSmartPointer<vtkActor>::New();
    camera_ = vtkSmartPointer<vtkCamera>::New();
    setCameraParameters();
    isDataLoaded = false;
    isDataRescaled = false;

}

void Render3D::loadData(char *dirName)
{
    LOG_DEBUG("loading Data from %s", dirName);
    reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader_->SetDirectoryName(dirName);
    reader_->Update();
    isDataLoaded = true;
}

vtkSmartPointer<vtkImageShiftScale> Render3D::getShifter()
{
    return shifter_;
}

void Render3D::setShifter(vtkSmartPointer<vtkImageShiftScale> shifter)
{
    shifter_ = shifter;
    isDataLoaded = true;
    isDataRescaled = true;
}

float Render3D::getScale()
{
    return reader_->GetRescaleSlope();
}

float Render3D::getShift()
{
    return -1 * reader_->GetRescaleOffset();
}

void Render3D::rescaleData(float scale, float shift)
{
    if(!isDataLoaded)
        LOG_ERROR("You must Load Data First");
    LOG_DEBUG("Rescalling Data");
    shifter_ = vtkSmartPointer<vtkImageShiftScale>::New();
    shifter_->SetScale(scale);
    shifter_->SetShift(shift);
    shifter_->SetOutputScalarTypeToUnsignedShort();
    shifter_->SetInputData(reader_->GetOutput());
    shifter_->Update();
    isDataRescaled = true;
}

void Render3D::extractSurfaces(double isoValue, vtkImageData* data)
{
    LOG_DEBUG("Iso surface extraction for iso Value :");
    std::cout << isoValue << std::endl;
    if(!isDataRescaled)
        LOG_ERROR("Rescale Data at First");
    isoExtractor_ = vtkSmartPointer<vtkContourFilter>::New();
//    isoExtractor_->SetInputConnection(shifter_->GetOutputPort());
    isoExtractor_->SetInputData(data);
    isoExtractor_->SetValue(0, isoValue);

    vtkSmartPointer<vtkPolyDataNormals> dataNormals =
            vtkSmartPointer<vtkPolyDataNormals>::New();

    dataNormals->SetInputConnection(isoExtractor_->GetOutputPort());
    dataNormals->SetFeatureAngle(60.0);

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(dataNormals->GetOutputPort());
    mapper->ScalarVisibilityOff();

    setActormapper(mapper);
    updateRenderer();
}

void Render3D::cubeMarchingExtraction(double isoValue, vtkImageData *data)
{
    LOG_DEBUG("Marching Cubes ");
    if(!isDataRescaled)
        LOG_ERROR("Rescale Data at First");
    marchCubes_ = vtkSmartPointer<vtkMarchingCubes>::New();
    marchCubes_->SetInputData(data);
    marchCubes_->SetValue(0, isoValue);

    vtkSmartPointer<vtkPolyDataNormals> dataNormals =
            vtkSmartPointer<vtkPolyDataNormals>::New();

    dataNormals->SetInputConnection(marchCubes_->GetOutputPort());
    dataNormals->SetFeatureAngle(60.0);

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(dataNormals->GetOutputPort());
    mapper->ScalarVisibilityOff();

    setActormapper(mapper);
    updateRenderer();

}

void Render3D::rayCastingRendering(vtkImageData* data)
{
    if(!isDataRescaled)
        LOG_ERROR("Rescale Data At First");

    LOG_DEBUG("Ray Casting Volume Rendering");
    rayCasting_ = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();

    vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper =
            vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(data);
    volumeMapper->SetVolumeRayCastFunction(rayCasting_);

    vtkSmartPointer<vtkColorTransferFunction>volumeColor =
            vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeColor->AddRGBPoint(0,    0.0, 0.0, 0.0);
    volumeColor->AddRGBPoint(500,  1.0, 0.5, 0.3);
    volumeColor->AddRGBPoint(1000, 1.0, 0.5, 0.3);
    volumeColor->AddRGBPoint(1150, 1.0, 1.0, 0.9);

    vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeScalarOpacity->AddPoint(0,    0.0);
    volumeScalarOpacity->AddPoint(500,  0.25);
    volumeScalarOpacity->AddPoint(1000, 0.5);
    volumeScalarOpacity->AddPoint(1150, 0.85);

    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(0,   0.0);
    volumeGradientOpacity->AddPoint(90,  0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);

    vtkSmartPointer<vtkVolumeProperty> volumeProperty =
            vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkSmartPointer<vtkVolume> volume =
            vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);
    renderer_->AddViewProp(volume);

    updateRenderer();


}

void Render3D::setActormapper(vtkSmartPointer<vtkMapper> dataMapper)
{
    actor_->SetMapper(dataMapper);
}

void Render3D::setCameraParameters()
{
    camera_->SetViewUp(0, 0, -1);
    camera_->SetPosition(0, 1, 0);
    camera_->SetFocalPoint(0, 0, 0);
    camera_->ComputeViewPlaneNormal();
    camera_->Azimuth(30.0);
    camera_->Elevation(30.0);
}

void Render3D::updateRenderer()
{
    renderer_->AddActor(actor_);
    renderer_->SetActiveCamera(camera_);
    renderer_->ResetCamera();
    renderer_->SetBackground(0, 0, 0);
    window_->SetSize(640, 480);
    interactor_->Initialize();
    interactor_->Start();
}



