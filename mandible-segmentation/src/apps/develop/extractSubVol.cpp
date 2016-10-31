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
    vtkSmartPointer<vtkExtractVOI> extractor = vtkSmartPointer<vtkExtractVOI>::New();
    extractor->SetInputConnection(volRenderer->getShifter()->GetOutputPort());
//    extractor->SetVOI(0, 100, 0, 100, 0, 100);
//    extractor->SetSampleRate(1, 1, 1);
//    extractor->ReleaseDataFlagOff();
//    extractor->Update();

    volRenderer->extractSurfaces(0, extractor->GetOutput());


    return EXIT_SUCCESS;

}
