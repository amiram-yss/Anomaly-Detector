/*
 * HybridAnomalyDetector.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */


#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();


    float superDev(Point p1, correlatedFeatures c);
    correlatedFeatures curr_with_reg_Shape(struct correlatedFeatures &theStruct, int rows, Point** ps);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
