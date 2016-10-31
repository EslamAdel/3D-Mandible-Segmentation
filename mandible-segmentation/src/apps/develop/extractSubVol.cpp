#include <ExtractVOI.h>
#include <Render3D.h>

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    Render3D* volRenderer = new Render3D;
    volRenderer->loadData(argv[1]);
    volRenderer->rescaleData(volRenderer->getScale(),
                             volRenderer->getShift());
    vtkExtractVOI* extractor = vtkExtractVOI::New();
    extractor->SetInputConnection(volRenderer->getShifter()->GetOutputPort());
    extractor->SetVOI(0, 511, 0, 511, 50, 100);
    extractor->SetSampleRate(1, 1, 1);
    extractor->Update();


    vtkImageData* shifted = volRenderer->getShifter()->GetOutput();
     std::cout << shifted->GetScalarComponentAsFloat(100, 50, 80, 0) << std::endl;

    vtkImageData* extracted = extractor->GetOutput();
    int* extractedDims = extracted->GetDimensions();
     std::cout << "Dims: " << " x: " << extractedDims[0]
                           << " y: " << extractedDims[1]
                           << " z: " << extractedDims[2] << std::endl;
     std::cout << "Number of points: " << extracted->GetNumberOfPoints() << std::endl;
     std::cout << "Number of cells: " << extracted->GetNumberOfCells() << std::endl;

     std::cout << extracted->GetScalarComponentAsDouble(200, 100, 50, 0) << std::endl;
    volRenderer->extractSurfaces(1200, extractor->GetOutput());


    return EXIT_SUCCESS;

}
