#include <QCoreApplication>
#include <ExtractVOI.h>
#include <Render3D.h>
#include <Thresholding.h>
#include <Segmentation.h>
#include <vtkImageResample.h>

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

    ExtractVOI* extractor = new ExtractVOI;

    extractor->setInputData(volRenderer->getShifter()->GetOutput());
    extractor->setRange(75, 450, 150, 420, 390, 520);


    Thresholding *thresholdFilter = new Thresholding(extractor->getOutputData(), 2500);
    Segmentation* teeth = new Segmentation(thresholdFilter->getThresholdedData());

    vtkSmartPointer<vtkImageResample> sampleUp = vtkSmartPointer<vtkImageResample>::New();
    sampleUp->SetInputData(teeth->getSegmentedData());
    sampleUp->SetAxisMagnificationFactor(0, 1.5);
    sampleUp->SetAxisMagnificationFactor(1, 1.5);
    sampleUp->SetAxisMagnificationFactor(2, 1.5);
    sampleUp->Update();


//  volRenderer->rayCastingRendering(extractor->getOutputData());
//    volRenderer->rayCastingRendering(thresholdFilter->getThresholdedData());
//    volRenderer->extractSurfaces(2550, sampleUp->GetOutput());
    volRenderer->rayCastingRendering(sampleUp->GetOutput());
    return EXIT_SUCCESS;

}
