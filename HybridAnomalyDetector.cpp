/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */


#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

/******************************************************************************
 *
 * @param theStruct - correlatedFeatures
 * @param rows - num of rows in the columns
 * @param ps - the array of points
 * @return theStruct of correlatedFeatures after
 *         inserting the correlative line or circle
 *         (depending on the pearson) and the corresponding threshold.
 *****************************************************************************/
correlatedFeatures HybridAnomalyDetector::curr_with_reg_Shape(struct correlatedFeatures &theStruct,
                                                              int rows, Point **ps) {

    // corrlation to a line_reg
    if (theStruct.corrlation >= LINE_BRINK) {
        theStruct.lin_reg = linear_reg(ps, rows);
        float theMaxDev = maxDev(ps, rows, theStruct.lin_reg);
        theStruct.threshold = theMaxDev * ELIMINATES_FALSE_ALARMS;
        return theStruct;
    }

    // corrlation to a surrounding circle
    Circle circle = findMinCircle(ps, rows);
    float theMaxDev = circle.radius;
    theStruct.threshold = theMaxDev * ELIMINATES_FALSE_ALARMS;
    theStruct.circle_center = circle.center;
    return theStruct;
}


/****************************************************
 *
 * @param p1 - point
 * @param c - correlatedFeatures
 * @return the distance between the point and the
 *         linear reg or the surrounding circle
 *         (depending on the corrlation).
 ****************************************************/
float HybridAnomalyDetector::superDev(Point p1, correlatedFeatures c) {

    if (c.corrlation >= LINE_BRINK)
        return distance(p1, c.lin_reg);

    return distance(p1, c.circle_center);
}
