/*
 * timeseries.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

class TimeSeries {
private:

    map<string, int> criterion_index_map;
    vector<float *> feature_guided_dataset, record_guided_dataset;
    int records_count, features_count;

    // Inits the data stracture
    void initDataset(vector<string> &);

    //switches line to float arr
    float *lineToDoubleArray(string);

    //Reads csv data
    vector<string> GetCSVrawDataLines(const char *);

    // creates list of the criteria string
    void setCriteriaMap(string);

    // creates matrix according to records
    void setRecordGuidedDataset(vector<string> &);

    // creates matrix according to features
    void setFeaturesGuidedDataset();

    // sets the dims of the experiment
    void setDatasetDims(vector<string> &raw_dataset);

    // allocates mem for the matrix
    void initMatrixes();

public:
    // get num of features
    int getColumnsNum() const;

    // get num of records 
    int getRowsNum() const;

    // get array of data in the i'th feature
    float *getFeatureArray(int) const;

    // get array of data in the feature given as param
    float *getFeatureArray(string) const;

    // get the name of the i'th feature
    string getFeatureName(int) const;

    //constructor
    TimeSeries(const char *CSVfileName);
};


#endif /* TIMESERIES_H_ */