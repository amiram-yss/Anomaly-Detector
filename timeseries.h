

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class TimeSeries {

public:

    TimeSeries(const char *CSVfileName) {

    };

    int getRowsNum();

    int getColumnsNum();

    float *getFeatureArray(int);

    float *getFeatureArray(string);

    string getFeatureName(int);


#endif /* TIMESERIES_H_ */
};