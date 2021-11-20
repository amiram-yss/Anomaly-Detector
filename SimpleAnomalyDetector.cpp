
#include "SimpleAnomalyDetector.h"

#define eliminatesFalseAlarms 1.2; //c'mon!!!!!!!!!!!!!!!
#define brink 0.9; //only pearsons above it will consider

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // TODO Auto-generated destructor stub

    int columns = ts.getColumnsNum();
    int rows = ts.getRowsNum();
    int m = 0;
    int c = -1;
    float p;  //pearson


    for (int i = 0; i < rows; ++i) {
        for (int j = i + 1; j < rows; ++j) {

            p = fabsf(pearson(ts.getFeatureArray(i), ts.getFeatureArray(j)));
            if (p > m) {
                m = p;
                c = j;
            }
        }
        if (c != -1 && m >= brink) {
            auto it = this->cf.insert(this->cf.begin(), collideTwoFeatures(ts, i, c));
        }
    }


}

correlatedFeatures SimpleAnomalyDetector::collideTwoFeatures(const TimeSeries &ts, int i, int j) {
    correlatedFeatures theStruct;
    theStruct.corrlation = pearson(ts.getFeatureArray(i), ts.getFeatureArray(j));
    theStruct.feature1 = ts.getFeatureName(i);
    theStruct.feature2 = ts.getFeatureName(j);

    Point *ps[ts.getRowsNum()];
    for (int k = 0; k < ts.getRowsNum(); i++) {
        ps[i] = new Point(ts.getFeatureArray(i)[k], ts.getFeatureArray(j)[k]);
    }

    theStruct.lin_reg = linear_reg(ps, ts.getRowsNum());

    float maxDev = maxDev(ps, ts.getRowsNum(), theStruct.lin_reg)

//    float theDev = 0;
//    //float maxDev = 0;
//
//    for (int k = 0; k < ts.getRowsNum(); ++k) {
//        theDev = dev(ps[k], theStruct.lin_reg);
//        if (theDev > maxDev) {
//            maxDev = theDev;
//        }
//    }


    theStruct.threshold = maxDev * eliminatesFalseAlarms;

    return theStruct;
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {

    vector<AnomalyReport> vec;
    int rows = ts.getRowsNum();
    int columns = ts.getColumnsNum();
    Point point;

    for (int i = 0; i < rows; ++i) { //all the rows

        for (int j = 0; j < this->cf.size(); ++j) {
            correlatedFeatures currFeatures = this->cf[j];
            point.x = getFeatureArray(currFeatures.feature1)[i];
            point.y = getFeatureArray(currFeatures.feature2)[i];
            float dev = dev(point, currFeatures.lin_reg);

            if (dev > currFeatures.threshold) {
                AnomalyReport a = AnomalyReport::AnomalyReport
                        (currFeatures.feature1[i] + "-" + currFeatures.feature2[i], (long) i);

                auto it = vec.insert(vec.begin(), a);

            }
        }



        // TODO Auto-generated destructor stub
    }
    return vec;

}