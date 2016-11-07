#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <unordered_map>
#include <set>
#include <list>

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

private:
    vtkSmartPointer<vtkImageData> inputData_;
    vtkSmartPointer<vtkImageData> outputData_;
    LabelsMap pointsLabels_;
    std::list<Segment> segmentsList_;
    int* dimentions_;
    int* extent_;
    int currentId_;
};


#endif
