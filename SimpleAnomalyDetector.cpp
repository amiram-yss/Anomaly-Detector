/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include "SimpleAnomalyDetector.h"

#define ELIMINATES_FALSE_ALARMS 1.2f
#define BRINK 0.9f //only pearsons above it will consider
#define HYPHEN "-"


SimpleAnomalyDetector::SimpleAnomalyDetector() {}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}

/************************************************************************
 *
 * @param ts - the "TimeSeries"
 * The function learns the DATA and arranges in the class' vector
 * all the correlations that exist between all 2 Features.
 **********************************************************************/
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {

    int columns = ts.getColumnsNum();
    int rows = ts.getRowsNum();
    int c;
    float m;
    float p;  //pearson

    //All column pairs
    for (int i = 0; i < columns; ++i) {
        for (int j = i + 1; j < columns; ++j) {
            m = 0;
            c = -1;

            float *a = ts.getFeatureArray(i);
            float *b = ts.getFeatureArray(j);

            p = fabsf(pearson(a, b, rows));
            if (p > m) {
                m = p;
                c = j;
            }

            //If there is a correlation and it is also greater than the brink
            if (c != -1 && m >= BRINK) {

                correlatedFeatures s = collideTwoFeatures(ts, i, c);
                this->cf.push_back(s);
            }
        }
    }
}

/************************************************
 *
 * @param ts - the "TimeSeries"
 * @param i - index of feature 1
 * @param j - index of feature 2
 * @return Boot and construction 'correlatedFeatures'
 * struct of the 2 features
 ************************************************/
correlatedFeatures SimpleAnomalyDetector::collideTwoFeatures(const TimeSeries &ts, int i, int j) {
    correlatedFeatures theStruct;
    theStruct.corrlation = pearson(ts.getFeatureArray(i), ts.getFeatureArray(j), ts.getRowsNum());
    theStruct.feature1 = ts.getFeatureName(i);
    theStruct.feature2 = ts.getFeatureName(j);

    Point *ps[ts.getRowsNum()];
    for (int k = 0; k < ts.getRowsNum(); k++) {
        float x = ts.getFeatureArray(i)[k];
        float y = ts.getFeatureArray(j)[k];

        ps[k] = new Point(x, y);
    }

    theStruct.lin_reg = linear_reg(ps, ts.getRowsNum());

    float theMaxDev = maxDev(ps, ts.getRowsNum(), theStruct.lin_reg);

    theStruct.threshold = theMaxDev * ELIMINATES_FALSE_ALARMS;

    return theStruct;
}

/**********************************************
 *
 * @param ts - the "TimeSeries"
 * @return vector<AnomalyReport> that
 * contains all the AnomalyReport of
 * the new TimeSeries
 ********************************************/
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {

    vector<AnomalyReport> vec;
    int rows = ts.getRowsNum();
    Point point(0, 0);

    //all the rows
    for (int i = 0; i < rows; ++i) {

        //all the correlatedFeatures
        for (auto currFeatures: this->cf) {
            point.x = ts.getFeatureArray(currFeatures.feature1)[i];
            point.y = ts.getFeatureArray(currFeatures.feature2)[i];
            float theDev = dev(point, currFeatures.lin_reg);

            if (theDev > currFeatures.threshold) {

                vec.push_back(AnomalyReport(
                        currFeatures.feature1 + HYPHEN + currFeatures.feature2, i + 1));
            }
        }
    }
    return vec;
}

