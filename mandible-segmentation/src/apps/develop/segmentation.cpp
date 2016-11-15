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
    double segmentationTime;
    double ThresholdingTime;

    Render3D* volRenderer = new Render3D;

    volRenderer->loadData(argv[1]);
    volRenderer->rescaleData(volRenderer->getScale(),
                             volRenderer->getShift());


//    vtkImageData* myTestVolume = generateTestVolume(80, 80, 80);

    ExtractVOI* extractor = new ExtractVOI;
    extractor->setInputData(volRenderer->getShifter()->GetOutputPort());
    extractor->setRange(75, 450, 150, 420, 350, 525);


    gettimeofday(&start, NULL);
    Thresholding *thresholdFilter = new Thresholding(extractor->getOutputData(), 1400);
    gettimeofday(&end, NULL);
    ThresholdingTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;

    gettimeofday(&start, NULL);
    Segmentation* mandibleSegmentation = new Segmentation(thresholdFilter->getThresholdedData());
    gettimeofday(&end, NULL);
    segmentationTime = (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec)/1000000;


    printf("====================================================== \n");
    printf("Statistics \n");
    printf("Thresholding Time : %d min : %f sec \n", (int) ThresholdingTime /60,
           ThresholdingTime - 60*((int) ThresholdingTime /60));
    printf("Segmentation Time : %d min : %f sec \n", (int) segmentationTime /60,
           segmentationTime - 60*((int) segmentationTime /60));
//    volRenderer->rayCastingRendering(mandibleSegmentation->getSegmentedData());
    volRenderer->extractSurfaces(1500, mandibleSegmentation->getSegmentedData());
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
