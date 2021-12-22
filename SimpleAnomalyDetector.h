/*
 * SimpleAnomalyDetector.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#define ELIMINATES_FALSE_ALARMS 1.1f
#define LINE_BRINK 0.9f //columns pearsons above it correlated to line
#define MIN_BRINK 0.5f //only pearsons above it will consider
#define HYPHEN "-"


#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Point circle_center;
    float threshold;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
private:
    vector<correlatedFeatures> cf;


protected:
    correlatedFeatures collideTwoFeatures(const TimeSeries &ts, int i, int j);

    virtual float superDev(Point p1, correlatedFeatures c);

    virtual correlatedFeatures curr_with_reg_Shape(struct correlatedFeatures &theStruct, int rows, Point **ps);


public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }

};


#endif /* SIMPLEANOMALYDETECTOR_H_ */
