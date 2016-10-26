#include "Render2D.h"

// Define own interaction style
class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static myVtkInteractorStyleImage* New();
    //   vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
    vtkResliceImageViewer* _ImageViewer;
    int _Slice;
    int _MinSlice;
    int _MaxSlice;

public:
    void SetImageViewer(vtkResliceImageViewer* imageViewer) {
        _ImageViewer = imageViewer;
        _MinSlice = imageViewer->GetSliceMin();
        _MaxSlice = imageViewer->GetSliceMax();
        _Slice = _MinSlice;
        cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
    }

protected:
    void MoveSliceForward() {
        if(_Slice < _MaxSlice) {
            _Slice += 1;
            cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
            _ImageViewer->SetSlice(_Slice);
            _ImageViewer->Render();
        }
    }

    void MoveSliceBackward() {
        if(_Slice > _MinSlice) {
            _Slice -= 1;
            cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
            _ImageViewer->SetSlice(_Slice);
            _ImageViewer->Render();
        }
    }


    virtual void OnKeyDown() {
        std::string key = this->GetInteractor()->GetKeySym();
        if(key.compare("Up") == 0) {
            //cout << "Up arrow key was pressed." << endl;
            MoveSliceForward();
        }
        else if(key.compare("Down") == 0) {
            //cout << "Down arrow key was pressed." << endl;
            MoveSliceBackward();
        }
        // forward event
        vtkInteractorStyleImage::OnKeyDown();
    }


    virtual void OnMouseWheelForward() {
        //std::cout << "Scrolled mouse wheel forward." << std::endl;
        MoveSliceForward();
        // don't forward events, otherwise the image will be zoomed
        // in case another interactorstyle is used (e.g. trackballstyle, ...)
        // vtkInteractorStyleImage::OnMouseWheelForward();
    }


    virtual void OnMouseWheelBackward() {
        //std::cout << "Scrolled mouse wheel backward." << std::endl;
        if(_Slice > _MinSlice) {
            MoveSliceBackward();
        }
        // don't forward events, otherwise the image will be zoomed
        // in case another interactorstyle is used (e.g. trackballstyle, ...)
        // vtkInteractorStyleImage::OnMouseWheelBackward();
    }
};

vtkStandardNewMacro(myVtkInteractorStyleImage)

vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
        vtkSmartPointer<myVtkInteractorStyleImage>::New();
Render2D::Render2D()
{
    LOG_DEBUG("Initializing Slicer");


    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    renderWindow_ = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow_->AddRenderer(renderer_);
    rwInteractor_ = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwInteractor_->SetRenderWindow(renderWindow_);
    reslicer_ = vtkSmartPointer<vtkImageReslice>::New();
    imViewer_ = vtkSmartPointer<vtkResliceImageViewer>::New();



    // make imageviewer2 and sliceTextMapper visible to our interactorstyle
    // to enable slice status message updates when scrolling through the slices
    rwInteractor_->SetInteractorStyle(myInteractorStyle);
    isDataLoaded_ = false;
    isDataRescaled_ = false;

}

void Render2D::loadData(char *dirName)
{
    LOG_DEBUG("Loading Data");
    reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader_->SetDirectoryName(dirName);
    reader_->Update();
    isDataLoaded_ = true;
    setProjectionMatrices();
}

void Render2D::rescaleData(float scale, float shift)
{
    if(!isDataLoaded_)
        LOG_ERROR("Load Data first");
    shifter_ = vtkSmartPointer<vtkImageShiftScale>::New();
    shifter_->SetScale(scale);
    shifter_->SetShift(shift);
    shifter_->SetOutputScalarTypeToUnsignedShort();
    shifter_->SetInputConnection(reader_->GetOutputPort());
    reslicer_->SetInputConnection(shifter_->GetOutputPort());
    imViewer_->SetInputData(reader_->GetOutput());
    imViewer_->SetSliceOrientation(2);
    imViewer_->SetResliceModeToAxisAligned();
    LOG_DEBUG("Min Slides = %d, max = %d", imViewer_->GetSliceMin(), imViewer_->GetSliceMax());
    myInteractorStyle->SetImageViewer(imViewer_.Get());
    imViewer_->SetupInteractor(rwInteractor_);
    isDataRescaled_=true;
}

void Render2D::setAxialSlicing()
{
    if(!isDataRescaled_)
        LOG_ERROR("Load and rescale data first");
    reslicer_->SetResliceAxes(axial_);
    updateRenderer();
}

void Render2D::setCronalSlicing()
{
    if(!isDataRescaled_)
        LOG_ERROR("Load and rescale data first");
    reslicer_->SetResliceAxes(cronal_);
    updateRenderer();
}

void Render2D::setSagittalSLicing()
{
    if(!isDataRescaled_)
        LOG_ERROR("Load and rescale data first");
    reslicer_->SetResliceAxes(sagittal_);
    updateRenderer();
}

float Render2D::getScaleVaue()
{
    return reader_->GetRescaleSlope();
}

float Render2D::getShiftValue()
{
    return -1 * reader_->GetRescaleOffset();
}

void Render2D::setProjectionMatrices()
{
    if(!isDataLoaded_)
        LOG_ERROR("Load Data First");
    axial_ = vtkSmartPointer<vtkMatrix4x4>::New();
    cronal_ = vtkSmartPointer<vtkMatrix4x4>::New();
    sagittal_ = vtkSmartPointer<vtkMatrix4x4>::New();
    int dimentions[3];
    reader_->GetOutput()->GetDimensions(dimentions);
    double axialMat[16] = {1, 0, 0, round(dimentions[0]/2),
                           0, 1, 0, round(dimentions[1]/2),
                           0, 0, 1, round(dimentions[2]/2),
                           0, 0, 0, 1};

    double cronalMat[16] = {1, 0, 0, round(dimentions[0]/2),
                            0, 0, 1, round(dimentions[1]/2),
                            0,-1, 0, round(dimentions[2]/2),
                            0, 0, 0, 1};

    double sagittalMat[16] = {0, 0,-1, round(dimentions[0]/2),
                              1, 0, 0, round(dimentions[1]/2),
                              0,-1, 0, round(dimentions[2]/2),
                              0, 0, 0, 1};
    axial_->DeepCopy(axialMat);
    cronal_->DeepCopy(cronalMat);
    sagittal_->DeepCopy(sagittalMat);
}

void Render2D::updateRenderer()
{
    vtkSmartPointer<vtkLookupTable> table =
            vtkSmartPointer<vtkLookupTable>::New();
    table->SetRange(0, 4095); // image intensity range
    table->SetValueRange(0.0, 1.0); // from black to white
    table->SetSaturationRange(0.0, 0.0); // no color saturation
    table->SetRampToLinear();
    table->Build();

    // Map the image through the lookup table
    vtkSmartPointer<vtkImageMapToColors> color =
            vtkSmartPointer<vtkImageMapToColors>::New();
    color->SetLookupTable(table);
    color->SetInputConnection(reslicer_->GetOutputPort());

    // Display the image
    vtkSmartPointer<vtkImageActor> actor =
            vtkSmartPointer<vtkImageActor>::New();
    actor->GetMapper()->SetInputConnection(color->GetOutputPort());
    renderer_->AddActor(actor);
    imViewer_->GetRenderer()->ResetCamera();
    imViewer_->Render();
    rwInteractor_->Start();

}

