/*
 * timeseries.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include "timeseries.h"

#include <iostream>
#include <fstream>
#include <algorithm>

/**
* Init dataset
* input: vector of strings
* output:
*
**/
void TimeSeries::initDataset(vector<string> &raw_lines) {
    // Settings titles
    setCriteriaMap(raw_lines[0]);
    raw_lines.erase(raw_lines.begin());
    // Setting dataset
    setDatasetDims(raw_lines);
    initMatrixes();
    setRecordGuidedDataset(raw_lines);

    setFeaturesGuidedDataset();
}

/**
* Get columns number
* input:
* output: number of records
**/
int TimeSeries::getColumnsNum() const {
    return this->features_count;

}

/**
* Get rows number
* input:
* output: number of features in each record.
**/
int TimeSeries::getRowsNum() const {
    return this->records_count;

}

/**
* Get array of all records in feature number i
* input: index (=i)
* output: array of all records in the feature i
**/
float *TimeSeries::getFeatureArray(int index) const {

    return feature_guided_dataset[index];
}

/**
* Get array of all records in feature with the name in the param
* input: string = feature name
* output: array of all records in the feature
**/
float *TimeSeries::getFeatureArray(string feature) const {

    auto it = criterion_index_map.find(feature);
    return feature_guided_dataset[it->second];
}

/**
* get i's feature name
* input: index
* output: name of the i's feature
**/
string TimeSeries::getFeatureName(int i) const {
    for (auto var: criterion_index_map) {
        if (var.second == i)
            return var.first;
    }
    return "NOT FOUND";
}

/**
* Returns Vector of strings, each line in the csv becomes a value in the vector
* input: csv file name
* output: vector of strings
**/
vector<string> TimeSeries::GetCSVrawDataLines(const char *CSVfileName) {
    vector<string> vtr = vector<string>();
    string line;
    ifstream data_stream(CSVfileName);
    // EDIT: Removed 0 sized lines.
    while (getline(data_stream, line)) {
        if (line.length() != 0)
            vtr.push_back(line);
    }
    data_stream.close();
    return vtr;
}

/**
* mapping the features and their index
* input: string representing features
* output: string array of features
**/
void TimeSeries::setCriteriaMap(string line) {
    criterion_index_map = map<string, int>();
    // Start scanning tokens
    int indexer = 0;
    int start_index = 0;
    int end_index = line.find(",");

    while (end_index != -1) {
        this->criterion_index_map[line.substr(start_index, (end_index - start_index))] = indexer;
        start_index = end_index + 1;
        end_index = line.find(",", start_index);
        indexer++;
    }
    this->criterion_index_map[line.substr(start_index, line.size())] = indexer;
}

/**
* creates a matrix based on the records string
* input: list of strings
* output: matrix of the data represented in those lines
**/
void TimeSeries::setRecordGuidedDataset(vector<string> &raw_text) {
    for (size_t i = 0; i < records_count; i++) {
        this->record_guided_dataset[i] = lineToDoubleArray(raw_text[i]);
    }
}

/**
* converts rows to cols (from records to features quick access)
* input:
* output:
**/
void TimeSeries::setFeaturesGuidedDataset() {
    for (size_t i = 0; i < this->features_count; i++) {
        feature_guided_dataset[i] = new float[records_count];
        for (size_t j = 0; j < this->records_count; j++) {
            feature_guided_dataset[i][j] = record_guided_dataset[j][i];
        }
    }
}

/**
* updates the dataset dims
* input: lines
* output: updating the size of the matrix
**/
void TimeSeries::setDatasetDims(vector<string> &raw_dataset) {
    this->features_count = this->criterion_index_map.size();
    this->records_count = raw_dataset.size();
}

/**
* allocates mem for the matrixes
* input:
* output:
**/
void TimeSeries::initMatrixes() {
    this->feature_guided_dataset = vector<float *>(features_count);
    this->record_guided_dataset = vector<float *>(records_count);
}

/**
* takes a single line of text and converts it to float arr
* input: line
* output: float array represented by the string
**/
float *TimeSeries::lineToDoubleArray(string line) {
    // Setup array to return
    int token_num = count(line.begin(), line.end(), ',') + 1;
    float *ptr = new float[token_num];
    if (line.length() == 0)
        return nullptr;
    // Start scanning tokens
    int indexer = 0;
    int start_index = 0;
    int end_index = line.find(",");

    int lstCommaIndex = 0;

    string numHolder;

    while (end_index != -1) {
        // Holds a number between 2 commas
        numHolder = line.substr(start_index, (end_index - start_index));
        // And puts it, converted to float, in the array.
        ptr[indexer] = stof(numHolder);
        // Next variable pwease :3
        start_index = end_index + 1;
        lstCommaIndex = end_index;
        end_index = line.find(",", start_index);
        indexer++;
    }

    // Sets last variable in the array.
    numHolder = line.substr(lstCommaIndex + 1, line.length() - 1);
    ptr[indexer] = stof(numHolder);

    return ptr;
}

/**
* Constructor
* input:
* output:
**/
TimeSeries::TimeSeries(const char *CSVfileName) {
    vector<string> raw_lines = GetCSVrawDataLines(CSVfileName);
    initDataset(raw_lines);
}

void TimeSeries::print() {

}