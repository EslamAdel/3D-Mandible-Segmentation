#include "Render2D.h"
#include <memory>
#include <thread>

int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    Render2D* axial = new Render2D;
    Render2D* cronal = new Render2D;
    Render2D* sagittal = new Render2D;

    axial->loadData(argv[1]);
    cronal->setReader(axial->getReader());
    sagittal->setReader(axial->getReader());

    axial->setAxialSlicing();
    cronal->setCronalSlicing();
    sagittal->setSagittalSLicing();


    return EXIT_SUCCESS;

}
