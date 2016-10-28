#include "Render3D.h"
#include "Render2D.h"
#include <memory>

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    Render2D* myRenderer = new Render2D;
//    std::unique_ptr<Render3D> myRenderer(new Render3D);
//    Render3D* myRenderer = new Render3D;
    myRenderer->loadData(argv[1]);

//    myRenderer->cubeMarchingExtraction(1200);
    myRenderer->setCronalSlicing();


    return EXIT_SUCCESS;

}
