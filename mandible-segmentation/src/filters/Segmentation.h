#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <QMap>
#include <set>
#include <list>
#include <QSet>
#include <QList>

#include "Logger.h"

struct Segment{
public:
    int id_;
    double pointsCount_;
    double totalCount_;
    QSet<int> connectedSegmentsIds_;
};

class Segmentation
{
public:

    /**
     * @brief Segmentation
     * @param input
     */
    explicit Segmentation(vtkSmartPointer<vtkImageData> input);

    vtkSmartPointer<vtkImageData> getSegmentedData();

private:
    void createOutputData_();
    void initializeLabelMap_();
    void startLabeling_();
    void setLabel_(int i, int j, int k);
    QList<vtkIdType> getNeighbours_(int i, int j, int k);
    int createSegment();
    int updateSegmentsList(QSet<int> segmentsIds);
    Segment getLargestSegment_();
    void setOutputData(Segment largestSeg);

private:
    vtkSmartPointer<vtkImageData> inputData_;
    vtkSmartPointer<vtkImageData> outputData_;
    QMap<vtkIdType, int> pointsLabels_;
    QList<Segment>* segmentsList_;
    int* dimentions_;
    int* extent_;
    int currentId_;
};


#endif
