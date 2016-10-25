#include "Render3D.h"
#include <memory>

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<Render3D> myRenderer(new Render3D);
//    Render3D* myRenderer = new Render3D;
    myRenderer->loadData(argv[1]);
    myRenderer->rescaleData(myRenderer->getScale(),
                            myRenderer->getShift());

//    myRenderer->cubeMarchingExtraction(1200);
    myRenderer->rayCastingRendering();


    return EXIT_SUCCESS;

}
