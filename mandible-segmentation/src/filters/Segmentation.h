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

    /**
     * @brief id_ : a unique id of the segment
     */
    int id_;

    /**
     * @brief pointsCount_
     * The count of points belongs to this segment.
     */
    double pointsCount_;

    /**
     * @brief totalCount_
     * The count of all points belongs to this
     * segment including other connected segments
     */
    double totalCount_;

    /**
     * @brief connectedSegmentsIds_
     * A Set of Ids of the equivalent (connected) segments.
     */
    QSet<int> connectedSegmentsIds_;
};

class Segmentation
{
public:

    /**
     * @brief Segmentation
     * @param input
     * vtkImageData source of volume after extracting the VOI (volume of
     * interest) and thresholding.
     */
    explicit Segmentation(vtkSmartPointer<vtkImageData> input);

    /**
     * @brief getSegmentedData
     * Get the result of segmentation
     * @return
     * A pointer to the segmented image Data.
     */
    vtkSmartPointer<vtkImageData> getSegmentedData();

private:

    /**
     * @brief createOutputData_
     * Manually create a new image Data to set the output of
     * segmentation in it.
     */
    void createOutputData_();

    /**
     * @brief initializeLabelMap_
     * Initialize pointsLabels_ map that maps every none zero point to
     * Its segment.
     */
    void initializeLabelMap_();

    /**
     * @brief startLabeling_
     * Start labeling none zero points of the input data
     * according to the connectivity with nieghbouring points
     */
    void startLabeling_();

    /**
     * @brief setLabel_
     * @param i
     * @param j
     * @param k
     * Set A label to the point at i, j, and k position
     * if there is no labled points at nieghbourhood create
     * a new segment. if there is many different labels
     * Get Segment of minimum Id and label it with this Segment
     * and update the corresponding Set of connectedSegmetsIds_.
     */
    void setLabel_(int i, int j, int k);

    /**
     * @brief getNeighbours_
     * Get the neighbours of a specific point at i, j, and k.
     * @param i
     * @param j
     * @param k
     * @return
     * A Set of Ids of Segments connected to this point
     * 13 none zero and connected Points 4 at the same slice
     * and 9 at the previeous slice.
     */
    QSet<int> getNeighbours_(int i, int j, int k);

    /**
     * @brief createSegment
     * Create a new segment and insert in in the segmentsList_
     * @return
     * the id of the new segment.
     */
    int createSegment();

    /**
     * @brief updateSegmentsList
     * Update the pointsCount, connectedSegmentsIds of the smallest segment id
     * among the segmentsIds Set.
     * @param segmentsIds
     * A Set of segments in the neighbourhood of the point.
     * @return
     * Minimum segment id in the segmentsIds set after updating it.
     */
    int updateSegmentsList(QSet<int> segmentsIds);

    /**
     * @brief getLargestSegment_
     * Compute the total count for all segments
     * this is done over two layers, points of segments
     * connected to this segment and points connected to
     * segments of each segment connected to that segment.
     * and finally optain the segment with maximum points count.
     * @return
     * Segment with max points count.
     */
    Segment getLargestSegment_();

    /**
     * @brief setOutputData
     * Set the points connected to largest segment to the  final image.
     * @param largestSeg
     */
    void setOutputData(Segment largestSeg);

private:

    /**
     * @brief inputData_
     */
    vtkSmartPointer<vtkImageData> inputData_;

    /**
     * @brief outputData_
     */
    vtkSmartPointer<vtkImageData> outputData_;

    /**
     * @brief pointsLabels_
     * A mapof pointId and segmentId
     */
    QMap<vtkIdType, int> pointsLabels_;

    /**
     * @brief segmentsList_
     * A list of segments provides informations
     * about the size of each segement and connected
     * segments
     */
    QList<Segment>* segmentsList_;

    /**
     * @brief dimentions_
     * Dimentions of input volume
     */
    int* dimentions_;

    /**
     * @brief extent_
     * Range of extraced volume in the original volume
     */
    int* extent_;

    /**
     * @brief currentId_
     * Stores the id of the next segment
     */
    int currentId_;

    /**
     * @brief count_
     * Stores the count of points
     * that actutally have maped to output volume.
     */
    int count_;
};


#endif
