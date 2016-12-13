#include "Segmentation.h"

Segmentation::Segmentation(vtkImageData* input):inputData_(input)
{

    LOG_DEBUG("initialize Segmentation");
    //Initialization
    currentId_ = 0;
    visitedSegments_.clear();
    pointsLabels_.clear();
    outputData_ = vtkSmartPointer<vtkImageData>::New();
    segmentsList_ = new QList<Segment>;
    if(inputData_ != NULL)
    {
    //Get input Data Dimentions
    dimentions_ = inputData_->GetDimensions();

    //Get Extent (Rage of data that extracted from the original Volume)
    extent_ = inputData_->GetExtent();
    initializeLabelMap_();
    //createOutputData_();

    //Run segmentation Algorithm
    startLabeling_();
    }
}

vtkImageData* Segmentation::getSegmentedData()
{
    return inputData_;
}

void Segmentation::setInputData(vtkImageData *inData)
{
    inputData_ = inData;
    //Get input Data Dimentions
    dimentions_ = inputData_->GetDimensions();

    //Get Extent (Rage of data that extracted from the original Volume)
    extent_ = inputData_->GetExtent();
    initializeLabelMap_();
    //createOutputData_();

    //Run segmentation Algorithm
    startLabeling_();
}

void Segmentation::createOutputData_()
{
    LOG_DEBUG("Creating Image Data of Output Volume");
    outputData_->SetDimensions(dimentions_[0], dimentions_[1], dimentions_[2]);
    outputData_->SetScalarType(VTK_UNSIGNED_SHORT, outputData_->GetInformation());
    outputData_->SetNumberOfScalarComponents(1, outputData_->GetInformation());
    outputData_->SetExtent(extent_[0], extent_[1],
            extent_[2], extent_[3],
            extent_[4], extent_[5]);
    outputData_->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
}

void Segmentation::initializeLabelMap_()
{
    LOG_DEBUG("Initialize the points -> segment map");
    for(int k = extent_[4] ; k <= extent_[5]; k++)
    {
        for(int j = extent_[2] ; j <= extent_[3]; j++)
        {
            for(int i = extent_[0] ; i <= extent_[1]; i++)
            {
                if(inputData_->GetScalarComponentAsDouble(i, j, k, 0) > 0)
                {
                    int ijk[3] = {i, j,k};
                    pointsLabels_[inputData_->ComputePointId(ijk)] = -1;
                }
            }
        }
    }
}
void Segmentation::startLabeling_()
{
    LOG_DEBUG("Start Labeling");
    for(int k = extent_[4] + 1; k <= extent_[5]; k++)
    {
        for(int j = extent_[2] + 1; j < extent_[3]; j++)
        {
            for(int i = extent_[0] + 1; i < extent_[1]; i++)
            {
                if(inputData_->GetScalarComponentAsDouble(i, j, k, 0) > 0)
                    setLabel_(i, j, k);
            }
        }
    }
    setOutputData(getLargestSegment_());

}

void Segmentation::setLabel_(int i, int j, int k)
{
    QSet<int> neighbourSegments = getNeighbours_(i, j, k);
    int ijk[3] = {i, j, k};
    if(neighbourSegments.empty())
    {
        pointsLabels_[inputData_->ComputePointId(ijk)] = createSegment();
    }
    else
    {
        pointsLabels_[inputData_->ComputePointId(ijk)] = updateSegmentsList(neighbourSegments);
    }
}

QSet<int> Segmentation::getNeighbours_(int i, int j, int k)
{
    QSet<int> segmentsIds;
    int ijk[3];
    for(int x = -1; x < 2; x++ )
    {
        for(int y = -1; y < 2; y++)
        {
            ijk[0] = i + x;
            ijk[1] = j + y;
            if((x == -1 && y == 0) ||
                    (y == -1 && x == 0) ||
                    (x == -1 && y == -1) ||
                    (x == -1 && y == 1))
            {
                ijk[2] = k;
                if(inputData_->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0) > 0)
                {
                    if(pointsLabels_[inputData_->ComputePointId(ijk)] >= 0)
                    {
                        segmentsIds.insert(pointsLabels_[inputData_->ComputePointId(ijk)]);
                    }
                }
            }
            ijk[0] = i + x;
            ijk[1] = j + y;
            ijk[2] = k - 1;
            if(inputData_->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0) > 0)
            {
                if(pointsLabels_[inputData_->ComputePointId(ijk)] >= 0)
                {
                    segmentsIds.insert(pointsLabels_[inputData_->ComputePointId(ijk)]);
                }
            }
        }
    }
    return segmentsIds;
}

int Segmentation::createSegment()
{
    LOG_DEBUG("Creating a new Segment with id = %d", currentId_);
    Segment newSegment;
    newSegment.id_ = currentId_;
    newSegment.pointsCount_ = 1;
    segmentsList_->push_back(newSegment);
    currentId_++;

    return newSegment.id_;
}

int Segmentation::updateSegmentsList(QSet<int> segmentsIds)
{
    int minId = *segmentsIds.begin();
    for(int i : segmentsIds)
    {
        if( i < minId)
        {
            minId = i;
        }
    }
    for(int it : segmentsIds)
    {
        if(it == minId)
            segmentsList_->at(it).pointsCount_++;
        for(int it2 : segmentsIds)
        {
            if(it2 != it )
                segmentsList_->at(it).connectedSegmentsIds_.insert(it2);
        }
    }


    return minId;
}

QSet<int > Segmentation::getLargestSegment_()
{
    LOG_DEBUG("Getting The Largest Segment");
    double maxCount = segmentsList_->at(0).pointsCount_;
    QSet<int> largeSegments = BFS(segmentsList_->at(0));
    for(auto it = segmentsList_->begin() ; it != segmentsList_->end(); ++it)
    {
        if(visitedSegments_.contains(it->id_))
            continue;

        it->totalCount_ = it->pointsCount_;
        QSet<int> segments = BFS(*it);
        for(auto it2 : segments)
        {
            it->totalCount_ += segmentsList_->at(it2).pointsCount_;
        }
        if(it->totalCount_ > maxCount)
        {
            maxCount = it->totalCount_;
            largeSegments = segments;
        }
    }

    return largeSegments;
}

void Segmentation::setOutputData(QSet<int> segments)
{
    LOG_DEBUG("Writing The Output Data");
#pragma omp parallel for shared(segments)
    for(int k = extent_[4]; k <= extent_[5]; k++)
    {
        for(int j = extent_[2]; j <= extent_[3]; j++)
        {
            for(int i = extent_[0]; i <= extent_[1]; i++)
            {
                int ijk[3] = {i, j,k};
                if(inputData_->GetScalarComponentAsDouble(i, j, k, 0) > 0)
                {
                    if(!segments.contains(pointsLabels_[inputData_->ComputePointId(ijk)]))
                    {
                        inputData_->SetScalarComponentFromDouble(i,
                                                                  j,
                                                                  k,
                                                                  0,
                                                                  0);
                    }
                }

            }
        }
    }
    freeMemory();
}

QSet<int> Segmentation::BFS(Segment largestSegment)
{
    LOG_DEBUG("Starting BFS");
    //Segments Set
    QSet<int> segments;
    QList<int> visited;

    // Create a queue for BFS
    QList<int> queue;

    queue.push_back(largestSegment.id_);
    visited.push_back(largestSegment.id_);
    visitedSegments_.insert(largestSegment.id_);

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int s = queue.front();
        segments.insert(s);
        queue.pop_front();

        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for(auto i : segmentsList_->at(s).connectedSegmentsIds_)
        {
            if(!visited.contains(i))
            {
                queue.push_back(i);
                visited.push_back(i);
                visitedSegments_.insert(i);
                segments.insert(i);
            }
        }
    }
    LOG_DEBUG("Number of Segments Connected to %d Segment = %d", largestSegment.id_,segments.size());
    return segments;
}

void Segmentation::freeMemory()
{
    //Reset Id
    currentId_ = 0;
    //Clear the map
    pointsLabels_.clear();
    //Clear The List
    segmentsList_->clear();
}

