#include "Render3D.h"
#include <memory>

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<Render3D> surfaceRenderer(new Render3D);
    surfaceRenderer->loadData(argv[1]);
    surfaceRenderer->rescaleData(surfaceRenderer->getScale(),
                                 surfaceRenderer->getShift());

    surfaceRenderer->cubeMarchingExtraction(1200);

    std::unique_ptr<Render3D> rayCastingRenderer(new Render3D);
    rayCastingRenderer->setShifter(surfaceRenderer->getShifter());
    rayCastingRenderer->rayCastingRendering();


    return EXIT_SUCCESS;

}
