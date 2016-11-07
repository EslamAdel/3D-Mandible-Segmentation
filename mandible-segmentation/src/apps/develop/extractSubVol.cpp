#include <ExtractVOI.h>
#include <Render3D.h>
#include <Thresholding.h>

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

    extractor->setInputData(volRenderer->getShifter()->GetOutputPort());
    extractor->setRange(75, 450, 150, 420, 392, 525);


    Thresholding *thresholdFilter = new Thresholding(extractor->getOutputData(), 1300);

//    volRenderer->rayCastingRendering(extractor->getOutputData());
    volRenderer->rayCastingRendering(thresholdFilter->getThresholdedData());
    return EXIT_SUCCESS;

}
