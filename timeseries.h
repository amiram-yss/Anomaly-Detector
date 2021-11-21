

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

class TimeSeries {
private:

    map<string, int> criterion_index_map;
    vector<float*> feature_guided_dataset, record_guided_dataset;
    int records_count, features_count;
    void initDataset(vector<string>&);
    float* lineToDoubleArray(string);
    vector<string>& GetCSVrawDataLines(const char*);
    void setCriteriaMap(string);
    void setRecordGuidedDataset(vector<string>&);
    void setFeaturesGuidedDataset();
    void setDatasetDims(vector<string>& raw_dataset);
    void initMatrixes();

public:
    int getColumnsNum() const;
    int getRowsNum() const;
    float *getFeatureArray(int);
    float *getFeatureArray(string);
    string getFeatureName(int);
    TimeSeries(const char* CSVfileName);
};


#endif /* TIMESERIES_H_ */
