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

Render2D::Render2D()
{
    LOG_DEBUG("Initializing Slicer");

    vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle =
            vtkSmartPointer<myVtkInteractorStyleImage>::New();
    imViewer_ = vtkSmartPointer<vtkResliceImageViewer>::New();
    rwInteractor_ = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwInteractor_->SetInteractorStyle(myInteractorStyle);
    imViewer_->SetupInteractor(rwInteractor_);

    // make imageviewer2 and sliceTextMapper visible to our interactorstyle
    // to enable slice status message updates when scrolling through the slices
    isDataLoaded_ = false;

}

void Render2D::loadData(char *dirName)
{
    LOG_DEBUG("Loading Data");
    reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader_->SetDirectoryName(dirName);
    reader_->Update();
    isDataLoaded_ = true;
}

void Render2D::setAxialSlicing()
{
    if(!isDataLoaded_)
        LOG_ERROR("Load and Load data first");
    imViewer_->SetSliceOrientation(0);
    imViewer_->SetInputData(reader_->GetOutput());
    imViewer_->Render();
    rwInteractor_->Start();

}

void Render2D::setCronalSlicing()
{
    if(!isDataLoaded_)
        LOG_ERROR("Load and Load data first");
    imViewer_->SetSliceOrientation(1);
    imViewer_->SetInputData(reader_->GetOutput());
    imViewer_->Render();
    rwInteractor_->Start();
}

void Render2D::setSagittalSLicing()
{
    if(!isDataLoaded_)
        LOG_ERROR("Load and load data first");
    imViewer_->SetSliceOrientation(2);
    imViewer_->SetInputData(reader_->GetOutput());
    imViewer_->Render();
    rwInteractor_->Start();
}

