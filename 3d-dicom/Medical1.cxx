/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Medical1.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example reads a volume dataset, extracts an isosurface and displays it.
//

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkNew.h>
#include <vtkImageData.h>
#include <vtkType.h>
#include <vtkDataArray.h>
#include <vtkIdTypeArray.h>
#include <vtkImageShiftScale.h>
#include <vtkThresholdPoints.h>

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    // Create the renderer, the render window, and the interactor. The renderer
    // draws into the render window, the interactor enables mouse- and
    // keyboard-based interaction with the data within the render window.
    vtkSmartPointer<vtkRenderer> aRenderer =
            vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renWin =
            vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(aRenderer);

    vtkSmartPointer<vtkRenderWindowInteractor> iren =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);


    //Read DICOM Images
    std::string folder = argv[1];
    vtkSmartPointer<vtkDICOMImageReader> reader =
            vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(folder.c_str());
    reader->Update();

    //================================================================================
    //Get Patient Name
    const char* name = new char[100];
    name = reader->GetPatientName();

    // Returns the pixel spacing (in X, Y, Z). Note: if there is only one slice,
    // the Z spacing is set to the slice thickness. If there is more
    // than one slice, it is set to the distance between the first two slices.
    double* pixelSpacing = new double;
    pixelSpacing = reader->GetPixelSpacing();

    //Returns the image width.
    int width = reader->GetWidth();

    //Returns the image height.
    int height = reader->GetHeight();

    //Get the (DICOM) x,y,z coordinates of the first pixel in the image
    //(upper left hand corner) of the last image processed by the DICOMParser
    float* imagePositionPatient = new float[3];
    imagePositionPatient = reader->GetImagePositionPatient();


    //Get the (DICOM) directions cosines. It consist of the components of the first two
    //vectors. The third vector needs to be computed to form an orthonormal basis
    float* imageOrientationPatient = new float;
    imageOrientationPatient = reader->GetImageOrientationPatient();

    //Get the number of bits allocated for each pixel in the file.
    int bitsAllocated = reader->GetBitsAllocated();

    //Get the pixel representation of the last image processed by the DICOMParser.
    //A zero is a unsigned quantity. A one indicates a signed quantity
    int pixelRepresentation = reader->GetPixelRepresentation();

    //Get the number of components of the image data for the last image processed.
    int numberOfComponents = reader->GetNumberOfComponents();

    //Get the rescale offset for the pixel data.
    float rescaleOffset = reader->GetRescaleOffset();

    //Get the rescale slope for the pixel data
    float rescaleSlope  =  reader->GetRescaleSlope();

    //#define VTK_SHORT   4
    int dataScalarType =  reader->GetDataScalarType();

    //Set/Get the number of scalar components
    int numberOfScalarComponents = reader->GetNumberOfScalarComponents();

    //The number of dimensions stored in a file. This defaults to two
    int fileDimentionality = reader->GetFileDimensionality();

    //little or big indian
    //#define VTK_FILE_BYTE_ORDER_LITTLE_ENDIAN 1
    int byteOrder = reader->GetDataByteOrder();

    vtkSmartPointer<vtkImageData> imageData =
            vtkSmartPointer<vtkImageData>::New();
    imageData = reader->GetOutput();

    //X Y Z
    int dimentions[3];
    imageData->GetDimensions(dimentions);

    // 3D dimentions
    int dataDimentions = imageData->GetDataDimension();

    vtkIdType numberOfCells = imageData->GetNumberOfCells();

    //X * Y * Z
    vtkIdType numberOfPoints = imageData->GetNumberOfPoints();

    //
    int pointijk[3] = {100, 100 ,100};

    vtkIdType pointId = imageData->ComputePointId(pointijk);

    double point[3];

    imageData->GetPoint(pointId, point);

    short* scalar = (short*) imageData->GetScalarPointer(pointijk);
    double minScalar = imageData->GetScalarTypeMin();
    double maxScalar = imageData->GetScalarTypeMax();
    const char* scalarTypeName = imageData->GetScalarTypeAsString();

    vtkImageShiftScale * shifter = vtkImageShiftScale::New();
    shifter->SetScale(rescaleSlope);
    shifter->SetShift(-1*rescaleOffset);
    shifter->SetOutputScalarTypeToShort();
    shifter->SetInputConnection(reader->GetOutputPort());
    shifter->ReleaseDataFlagOff();
    shifter->Update();

    vtkImageData* rescaledImageData = shifter->GetOutput();
    short* rescaledValue = (short*) rescaledImageData->GetScalarPointer(pointijk);

    vtkThresholdPoints * thresholdedVolume = vtkThresholdPoints::New();
    thresholdedVolume->SetInputConnection(shifter->GetOutputPort());
    thresholdedVolume->ThresholdByUpper(-100);
    thresholdedVolume->Update();


    //=======================================================================================

    // An isosurface, or contour value of 500
    // Once generated, a vtkPolyDataNormals filter is used to create normals
    // for smooth surface shading during rendering.
//    vtkSmartPointer<vtkContourFilter> isoExtractor =
//            vtkSmartPointer<vtkContourFilter>::New();
//    isoExtractor->SetInputConnection(thresholdedVolume->GetOutputPort());
//    isoExtractor->SetValue(0, 1);

    vtkSmartPointer<vtkPolyDataNormals> dataNormals =
            vtkSmartPointer<vtkPolyDataNormals>::New();
    dataNormals->SetInputConnection(thresholdedVolume->GetOutputPort());
    dataNormals->SetFeatureAngle(60.0);

    double range[2];
    thresholdedVolume->GetOutput()->GetScalarRange(range);

    vtkSmartPointer<vtkPolyDataMapper> dataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    dataMapper->SetInputConnection(thresholdedVolume->GetOutputPort());
    dataMapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> extractedData =
            vtkSmartPointer<vtkActor>::New();
//    extractedData->GetProperty()->SetColor(1, 0.5, 0.25);
    extractedData->SetMapper(dataMapper);

    // An outline provides context around the data.
    //
    vtkSmartPointer<vtkOutlineFilter> outlineData =
            vtkSmartPointer<vtkOutlineFilter>::New();
    outlineData->SetInputConnection(shifter->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> mapOutline =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapOutline->SetInputConnection(outlineData->GetOutputPort());

    vtkSmartPointer<vtkActor> outline =
            vtkSmartPointer<vtkActor>::New();
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0,0,0);

    // It is convenient to create an initial view of the data. The FocalPoint
    // and Position form a vector direction. Later on (ResetCamera() method)
    // this vector is used to position the camera to look at the data in
    // this direction.
    vtkSmartPointer<vtkCamera> aCamera =
            vtkSmartPointer<vtkCamera>::New();
    aCamera->SetViewUp (0, 0, -1);
    aCamera->SetPosition (0, 1, 0);
    aCamera->SetFocalPoint (0, 0, 0);
    aCamera->ComputeViewPlaneNormal();
    aCamera->Azimuth(30.0);
    aCamera->Elevation(30.0);

    // Actors are added to the renderer. An initial camera view is created.
    // The Dolly() method moves the camera towards the FocalPoint,
    // thereby enlarging the image.
    //  aRenderer->AddActor(outline);
    aRenderer->AddActor(extractedData);
    aRenderer->SetActiveCamera(aCamera);
    aRenderer->ResetCamera ();
    aCamera->Dolly(1.5);

    // Set a background color for the renderer and set the size of the
    // render window (expressed in pixels).
    aRenderer->SetBackground(0, 0, 0);
    renWin->SetSize(640, 480);

    // Note that when camera movement occurs (as it does in the Dolly()
    // method), the clipping planes often need adjusting. Clipping planes
    // consist of two planes: near and far along the view direction. The
    // near plane clips out objects in front of the plane; the far plane
    // clips out objects behind the plane. This way only what is drawn
    // between the planes is actually rendered.
    aRenderer->ResetCameraClippingRange ();

    // Initialize the event loop and then start it.
    iren->Initialize();
    iren->Start();

    return EXIT_SUCCESS;
}
