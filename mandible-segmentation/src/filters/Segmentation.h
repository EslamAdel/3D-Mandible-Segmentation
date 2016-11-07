#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <unordered_map>
#include <set>
#include <list>
#include <QSet>
#include <QList>

#include "Logger.h"

typedef std::unordered_map<vtkIdType, int> LabelsMap;

struct Segment{
    int id_;
    double pointsCount_;
    std::set<int> connectedSegmentsIds_;
};

class Segmentation
{
public:

    /**
     * @brief Segmentation
     * @param input
     */
    explicit Segmentation(vtkSmartPointer<vtkImageData> input);

private:
    void createOutputData_();
    void initializeLabelMap_();
    void startLabeling_();
    void setLabel_(int i, int j, int k);
    std::list<vtkIdType>* getNeighbours_(int i, int j, int k);
    int createSegment();
    int updateSegmentsList(QSet<int> segmentsIds);

private:
    vtkSmartPointer<vtkImageData> inputData_;
    vtkSmartPointer<vtkImageData> outputData_;
    LabelsMap pointsLabels_;
    QList<Segment> segmentsList_;
    int* dimentions_;
    int* extent_;
    int currentId_;
};


#endif
