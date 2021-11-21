#include "timeseries.h"

#include <iostream>
#include <fstream>
#include <algorithm>

void TimeSeries::initDataset(vector<string>& raw_lines)
{
	// Settings titles
	setCriteriaMap(raw_lines[0]);
	raw_lines.erase(raw_lines.begin());
	// Setting dataset
	setDatasetDims(raw_lines);
	initMatrixes();
	setRecordGuidedDataset(raw_lines);
	setFeaturesGuidedDataset();
}

int TimeSeries::getColumnsNum() const
{
	return this->records_count;
}

int TimeSeries::getRowsNum() const
{
	return this->features_count;
}

float* TimeSeries::getFeatureArray(int index)
{
	return feature_guided_dataset[index];
}

float* TimeSeries::getFeatureArray(string feature)
{
	return feature_guided_dataset[criterion_index_map[feature]];
}

//TODO: Upgradeable: Try avoiding loop by adding a reverse map.
string TimeSeries::getFeatureName(int i)
{
	for each (auto var in criterion_index_map)
	{
		if (var.second == i)
			return var.first;
	}
	return "NOT FOUND";
}


vector<string>& TimeSeries::GetCSVrawDataLines(const char* CSVfileName) {
	vector<string>* vtr = new vector<string>();
	string line;
	ifstream data_stream(CSVfileName);
	while (getline(data_stream, line)) {
		vtr->push_back(line);
	}
	data_stream.close();
	return *vtr;
}

void TimeSeries::setCriteriaMap(string line)
{
	criterion_index_map = map<string, int>();
	// Start scanning tokens
	int indexer = 0;
	int start_index = 0;
	int end_index = line.find(",");

	while (end_index != -1) {
		this -> criterion_index_map[line.substr(start_index, (end_index - start_index))] = indexer;
		start_index = end_index + 1;
		end_index = line.find(",", start_index);
		indexer++;
	}
	this -> criterion_index_map[line.substr(start_index, line.size())] = indexer;
}

void TimeSeries::setRecordGuidedDataset(vector<string>& raw_text)
{
	for (size_t i = 0; i < records_count; i++)
	{
		this->record_guided_dataset[i] = lineToDoubleArray(raw_text[i]);
	}
}

void TimeSeries::setFeaturesGuidedDataset()
{
	for (size_t i = 0; i < this->features_count; i++)
	{
		feature_guided_dataset[i] = new float[records_count];
		for (size_t j = 0; j < this -> records_count; j++)
		{
			feature_guided_dataset[i][j] = record_guided_dataset[j][i];
		}
	}
}

void TimeSeries::setDatasetDims(vector<string>& raw_dataset)
{
	this->features_count = this->criterion_index_map.size();
	this->records_count = raw_dataset.size();
}

void TimeSeries::initMatrixes()
{
	this->feature_guided_dataset = vector<float *>(features_count);
	this->record_guided_dataset = vector<float*>(records_count);
}

float* TimeSeries::lineToDoubleArray(string line)
{
	// Setup array to return
	int token_num = count(line.begin(), line.end(), ',') + 1;
	float* ptr = new float[token_num];
	// Start scanning tokens
	int indexer = 0;
	int start_index = 0;
	int end_index = line.find(",");

	while (end_index != -1) {
		ptr[indexer] = stof(line.substr(start_index, (end_index - start_index)));
		start_index = end_index + 1;
		end_index = line.find(",", start_index);
		indexer++;
	}
	ptr[indexer] = stof(line.substr(start_index, line.size()));
	return ptr;
}

TimeSeries::TimeSeries(const char* CSVfileName)
{
	vector<string>& raw_lines = GetCSVrawDataLines(CSVfileName);
	initDataset(raw_lines);
	delete & raw_lines;
}


