#include "Segmentation.h"

Segmentation::Segmentation(vtkSmartPointer<vtkImageData> input):inputData_(input)
{

    LOG_DEBUG("initialize Segmentation");
    //Get input Data Dimentions
    dimentions_ = inputData_->GetDimensions();

    //Get Extent (Rage of data that extracted from the original Volume)
    extent_ = inputData_->GetExtent();

    //Initialization
    currentId_ = 0;
    outputData_ = vtkSmartPointer<vtkImageData>::New();
    segmentsList_ = new QList<Segment>;
    initializeLabelMap_();
    createOutputData_();

    //Run segmentation Algorithm
    startLabeling_();
}

vtkSmartPointer<vtkImageData> Segmentation::getSegmentedData()
{
    return outputData_;
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
#pragma omp parallel for
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
    Segment largestSegment =  getLargestSegment_();
    setOutputData(largestSegment);

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

    for(auto seg = segmentsList_->begin(); seg != segmentsList_->end(); ++seg)
    {
        if(seg->id_ == minId)
        {
            seg->pointsCount_++;
            for(int it : segmentsIds)
            {
                if(it != seg->id_ && !seg->connectedSegmentsIds_.contains(it))
                    seg->connectedSegmentsIds_.insert(it);
            }
            break;
        }
    }

    return minId;
}

Segment Segmentation::getLargestSegment_()
{
    LOG_DEBUG("Getting The Largest Segment");
    double maxCount = segmentsList_->at(segmentsList_->size() - 1).pointsCount_;
    int largestSegmentId = segmentsList_->size() - 1;
    for(auto it = segmentsList_->end() - 1; it != segmentsList_->begin(); --it)
    {
        it->totalCount_ = it->pointsCount_;
        for(int it2 : it->connectedSegmentsIds_)
        {
            it->totalCount_ += segmentsList_->at(it2).pointsCount_;
            for(auto it3 : segmentsList_->at(it2).connectedSegmentsIds_)
            {
                it->totalCount_ += segmentsList_->at(it3).pointsCount_;
            }
        }
        if(it->totalCount_ > maxCount)
        {
            maxCount = it->totalCount_;
            largestSegmentId = it->id_;
        }
    }

    return segmentsList_->at(largestSegmentId);
}

void Segmentation::setOutputData(Segment largestSeg)
{
    LOG_DEBUG("Writing The Output Data");
    QSet<int> segments = BFS(largestSeg);
#pragma omp parallel for shared(segments)
    for(int k = extent_[4] ; k <= extent_[5]; k++)
    {
        for(int j = extent_[2] ; j <= extent_[3]; j++)
        {
            for(int i = extent_[0] ; i <= extent_[1]; i++)
            {
                int ijk[3] = {i, j,k};
                if(segments.contains(pointsLabels_[inputData_->ComputePointId(ijk)]))
                {
                    outputData_->SetScalarComponentFromDouble(i,
                                                              j,
                                                              k,
                                                              0,
                                                              inputData_->GetScalarComponentAsDouble(i, j, k, 0));
                }
            }
        }
    }
}

QSet<int> Segmentation::BFS(Segment largestSegment)
{
    LOG_DEBUG("Starting BFS");
    //Segments Set
    QSet<int> segments;

    // Create a queue for BFS
    QList<int> queue;

    queue.push_back(largestSegment.id_);

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
            queue.push_back(i);
            segments.insert(i);
        }
    }
    return segments;
}

