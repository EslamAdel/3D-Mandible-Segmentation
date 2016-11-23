#include <QCoreApplication>
#include <sys/time.h>
#include <ExtractVOI.h>
#include <Render3D.h>
#include <Thresholding.h>
#include <Segmentation.h>
#include <stdio.h>


int main(int argc , char ** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " DATADIR/" << endl;
        return EXIT_FAILURE;
    }

    timeval start;
    timeval end;
    double msegmentationTime;
    double mThresholdingTime;

    double tsegmentationTime;
    double tThresholdingTime;

    Render3D* volRenderer = new Render3D;

    volRenderer->loadData(argv[1]);
    volRenderer->rescaleData(volRenderer->getScale(),
                             volRenderer->getShift());

    ExtractVOI* extractor = new ExtractVOI;
    extractor->setInputData(volRenderer->getShifter()->GetOutput());
    extractor->setRange(75, 450, 145, 420, 270, 525);


    gettimeofday(&start, NULL);
    Thresholding *thresholdFilter = new Thresholding(extractor->getOutputData(), 1400);
    gettimeofday(&end, NULL);
    mThresholdingTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;

    gettimeofday(&start, NULL);
    Segmentation* mandibleSegmentation = new Segmentation(thresholdFilter->getThresholdedData());
    gettimeofday(&end, NULL);
    msegmentationTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;

    printf("====================================================== \n");
    printf("Statistics \n");
    printf("Mandible Thresholding Time : %d min : %f sec \n", (int) mThresholdingTime /60,
           mThresholdingTime - 60*((int) mThresholdingTime /60));
    printf("Mandible Segmentation Time : %d min : %f sec \n", (int) msegmentationTime /60,
           msegmentationTime - 60*((int) msegmentationTime /60));

    volRenderer->rayCastingRendering(mandibleSegmentation->getSegmentedData());
    gettimeofday(&end, NULL);

    return EXIT_SUCCESS;

}
