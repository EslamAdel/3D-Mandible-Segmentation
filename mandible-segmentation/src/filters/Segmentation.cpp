#include "Segmentation.h"

Segmentation::Segmentation(vtkSmartPointer<vtkImageData> input):inputData_(input)
{
    dimentions_ = inputData_->GetDimensions();
    extent_ = inputData_->GetExtent();
    currentId_ = 0;
    initializeLabelMap_();
    createOutputData_();
}

void Segmentation::createOutputData_()
{
    LOG_DEBUG("Creating Image Data");
    outputData_->SetDimensions(dimentions_[0], dimentions_[1], dimentions_[2]);
    outputData_->SetScalarType(VTK_UNSIGNED_SHORT, inputData_->GetInformation());
    outputData_->SetNumberOfScalarComponents(1, inputData_->GetInformation());
    outputData_->SetExtent(extent_[0], extent_[1],
            extent_[2], extent_[3],
            extent_[4], extent_[5]);
    outputData_->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
}

void Segmentation::initializeLabelMap_()
{
#pragma omp parallel for
    for(int i = extent_[0] + 1; i < extent_[1]; i++)
    {
        for(int j = extent_[2] + 1; j < extent_[3]; j++)
        {
            for(int k = extent_[4] + 1; k <= extent_[5]; k++)
            {
                int ijk[3] = {i, j,k};
                pointsLabels_[inputData_->ComputePointId(ijk)] = -1;
            }
        }
    }
}

void Segmentation::startLabeling_()
{
    for(int i = extent_[0] + 1; i < extent_[1]; i++)
    {
        for(int j = extent_[2] + 1; j < extent_[3]; j++)
        {
            for(int k = extent_[4] + 1; k <= extent_[5]; k++)
            {
                setLabel_(i, j, k);
            }
        }
    }
}

void Segmentation::setLabel_(int i, int j, int k)
{
    std::list<vtkIdType>* neighbours = getNeighbours_(i, j, k);
    int ijk[3] = {i, j, k};
    std::set<int> segmentsIds;
    for(auto it = neighbours->begin(); it != neighbours->end(); ++it)
    {
        if(pointsLabels_[*it] >= 0)
        {
            segmentsIds.insert(pointsLabels_[*it]);
        }
    }
    if(segmentsIds.empty())
    {
        Segment newSegment;
        newSegment.id_ = currentId_;
        newSegment.pointsCount_ = 1;
        newSegment.connectedSegmentsIds_.insert(currentId_);
        segmentsList_.push_back(newSegment);
        pointsLabels_[inputData_->ComputePointId(ijk)] = currentId_;
        currentId_++;
    }
    else
    {
        if(segmentsIds.size() > 1)
        {
           int id = *segmentsIds.begin();
           for(auto it = segmentsList_.begin(); it != segmentsList_.end(); ++it)
           {
               if(it->id_ == id)
               {
                   it->pointsCount_++;
                   for(auto it2 = segmentsIds.begin(); it2 != segmentsIds.end(); ++it)
                   {
                      it->connectedSegmentsIds_.insert(*it2);
                      it->pointsCount_ +=
                   }
               }
           }
        }
    pointsLabels_[inputData_->ComputePointId(ijk)] = *segmentsIds.begin();
    }
}

std::list<vtkIdType> *Segmentation::getNeighbours_(int i, int j, int k)
{
    std::list<vtkIdType>* neighbours = new std::list<vtkIdType>;
    int ijk[3];
    for(int x = -1; x < 2; x++ )
    {
        for(int y = -1; y < 2; y++)
        {
            ijk[0] = i + x;
            ijk[1] = j + y;
            ijk[2] = k;
            if(x != 0 && y != 0)
                neighbours->push_back(inputData_->ComputePointId(ijk));
            ijk[2] = k - 1;
            neighbours->push_back(inputData_->ComputePointId(ijk));
        }
    }

}
