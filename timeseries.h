

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>

using namespace std;

class TimeSeries {
    int colomns;
    int rows;

public:

    int getColumnsNum() const;

    int getRowsNum() const;

    float *getFeatureArray(int);

    float *getFeatureArray(string);

    string getFeatureName(int);



    TimeSeries(const char *CSVfileName) {
    }

};


#endif /* TIMESERIES_H_ */
