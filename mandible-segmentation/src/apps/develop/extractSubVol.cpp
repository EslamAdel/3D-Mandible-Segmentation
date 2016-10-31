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

    ExtractVOI* extractor = new ExtractVOI;

    extractor->setInputData(volRenderer->getShifter()->GetOutputPort());
    extractor->setRange(150, 350, 280, 400, 140, 230);
    volRenderer->extractSurfaces(1200, extractor->getOutputData());
//    volRenderer->rayCastingRendering(extractor->getOutputData());
    return EXIT_SUCCESS;

}
