#include <QCoreApplication>
#include <sys/time.h>
#include <ExtractVOI.h>
#include <Render3D.h>
#include <Thresholding.h>
#include <Segmentation.h>
#include <stdio.h>


vtkImageData *generateTestVolume(int x, int y, int z);
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


//    vtkImageData* myTestVolume = generateTestVolume(80, 80, 80);

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

    extractor->setInputData(mandibleSegmentation->getSegmentedData());
    extractor->setRange(75, 450, 145, 420, 390, 525);

    gettimeofday(&start, NULL);
    Thresholding* teethThresholding = new Thresholding(mandibleSegmentation->getSegmentedData(), 2500);
    gettimeofday(&end, NULL);
    tThresholdingTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;



    gettimeofday(&start, NULL);
    Segmentation* teethSegmentation = new Segmentation(teethThresholding->getThresholdedData());
    gettimeofday(&end, NULL);
    tsegmentationTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;

    printf("====================================================== \n");
    printf("Statistics \n");
    printf("Mandible Thresholding Time : %d min : %f sec \n", (int) mThresholdingTime /60,
           mThresholdingTime - 60*((int) mThresholdingTime /60));
    printf("Mandible Segmentation Time : %d min : %f sec \n", (int) msegmentationTime /60,
           msegmentationTime - 60*((int) msegmentationTime /60));

    printf("Teeth Thresholding Time : %d min : %f sec \n", (int) tThresholdingTime /60,
           tThresholdingTime - 60*((int) tThresholdingTime /60));
    printf("Teeth Segmentation Time : %d min : %f sec \n", (int) tsegmentationTime /60,
           tsegmentationTime - 60*((int) tsegmentationTime /60));
    //volRenderer->rayCastingRendering(teethSegmentation->getSegmentedData());
    volRenderer->extractSurfaces(2500, teethSegmentation->getSegmentedData());
    gettimeofday(&end, NULL);

    return EXIT_SUCCESS;

}

vtkImageData* generateTestVolume(int x, int y, int z)
{
    vtkImageData* testVolume = vtkImageData::New();
    testVolume->SetDimensions(x, y, z);
    testVolume->SetScalarType(VTK_UNSIGNED_SHORT, testVolume->GetInformation());
    testVolume->SetNumberOfScalarComponents(1, testVolume->GetInformation());
    testVolume->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

    for(int i = 0.25*x; i <  0.75*x; i++)
    {
        for(int j = 0.25*y ; j < 0.75*y; j++)
        {
            for(int k = 0.25*z ; k <0.75*z ; k++)
            {
                testVolume->SetScalarComponentFromDouble(i, j, k, 0, 1500);
            }
        }
    }
    return testVolume;
}
