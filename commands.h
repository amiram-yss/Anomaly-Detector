/*
 * commands.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    //Reads and files to a new file
    void readAndFile(string fileName) {
        ofstream out(fileName);
        string s = "";
        while ((s = read()) != "done") {
            out << s << endl;
        }
        out.close();
    }


};

// you may add here helper classes

struct positiveReport {
    int start;
    int end;
    string description;
    bool tp;
};

//Status of the situation that passes between
// the various commands until leaving the program
struct status {
    float threshold;
    vector<AnomalyReport> AnomalyReport_vec;
    vector<positiveReport> positiveReport_vec;
    int testFileSize;

    status() {
        threshold = 0.9;
        testFileSize = 0;
    }
};


// you may edit this class
class Command {
protected:
    DefaultIO *dio;

public:
    const string description;

    Command(DefaultIO *dio, const string description) : dio(dio), description(description) {}

    string getDescription() { return description; }

    virtual void execute(status *myStatus) = 0;

    virtual ~Command() {}
};

// implement here your command classes

class uploadCSVfile : public Command {
public:
    uploadCSVfile(DefaultIO *dio) : Command(dio, "upload a time series csv file") {}

    /**
     *
     * @param myStatus
     * Using the "Read and Save" function - CSV files are read
     */
    virtual void execute(status *myStatus) {
        dio->write("Please upload your local train CSV file.\n");
        dio->readAndFile("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->readAndFile("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};

class algorithmSetting : public Command {
public:
    algorithmSetting(DefaultIO *dio) : Command(dio, "algorithm settings") {}

    /**
     *
     * @param myStatus
     * Possibility of changing the threshold
     */
    virtual void execute(status *myStatus) {

        float newThreshold;
        bool standartNum = false;

        while (!standartNum) {
            dio->write("The current correlation threshold is ");
            dio->write(myStatus->threshold);
            dio->write("\nType a new threshold\n");
            dio->read(&newThreshold);

            //the threshold is considered normal only between the values 0 and 1
            if (newThreshold > 0 && newThreshold <= 1) {
                myStatus->threshold = newThreshold;
                standartNum = true;
            } else {
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class detectAnomalies : public Command {
public:
    detectAnomalies(DefaultIO *dio) : Command(dio, "detect anomalies") {}

    /**
     *
     * @param myStatus
     * learnNormal + detect, and ordering the anomalies to positiveReport
     */
    virtual void execute(status *myStatus) {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        myStatus->testFileSize = test.getRowsNum();
        HybridAnomalyDetector ad;
        ad.setBRINK(myStatus->threshold);
        ad.learnNormal(train);
        myStatus->AnomalyReport_vec = ad.detect(test);

        positiveReport fr;
        fr.start = 0;
        fr.end = 0;
        fr.description = "";
        fr.tp = false;

        //ordering the anomalies to positiveReport
        for_each(myStatus->AnomalyReport_vec.begin(), myStatus->AnomalyReport_vec.end(),
                 [&fr, myStatus](AnomalyReport &ar) {

                     //If the report can join an existing positiveReport,
                     // then add it and do not open another positiveReport,
                     if (ar.timeStep == fr.end + 1 && ar.description == fr.description)
                         fr.end++;

                         // otherwise it is necessary to open another positiveReport for it.
                     else {
                         myStatus->positiveReport_vec.push_back(fr);
                         fr.start = ar.timeStep;
                         fr.end = fr.start;
                         fr.description = ar.description;
                     }
                 });
        myStatus->positiveReport_vec.push_back(fr);
        myStatus->positiveReport_vec.erase(myStatus->positiveReport_vec.begin());

        dio->write("anomaly detection complete.\n");
    }
};

class displayResults : public Command {
public:
    displayResults(DefaultIO *dio) : Command(dio, "display results") {}

    /**
     *
     * @param myStatus
     * Simple printing of AnomalyReport_vec
     */
    virtual void execute(status *myStatus) {
        for (auto report: myStatus->AnomalyReport_vec) {
            dio->write(report.timeStep);
            dio->write("\t " + report.description + "\n");
        }
        dio->write("Done.\n");
    }
};


class upAnomAnalyze : public Command {
public:
    upAnomAnalyze(DefaultIO *dio) : Command(dio, "upload anomalies and analyze results") {}


    /**
     *
     * @param as - start of current deviation
     * @param ae - end of current deviation
     * @param bs - start of member of positiveReport_vec
     * @param be - end of member of positiveReport_vec
     *
     * @return Is there any overlap between the range of the
     *         member of positiveReport_vec and the range of current deviation.
     */
    bool covering(int as, int ae, int bs, int be) {
        return (ae >= bs && be >= as);
    }


    /**
     *
     * @param start - start of current deviation
     * @param end - end of current deviation
     * @param myStatus
     * @return Is there any overlap between the current deviation,
     *         and one positiveReport from positiveReport_vec.
     */
    bool isTP(int start, int end, status *myStatus) {
        for (size_t i = 0; i < myStatus->positiveReport_vec.size(); i++) {
            positiveReport fr = myStatus->positiveReport_vec[i];
            if (covering(start, end, fr.start, fr.end)) {
                myStatus->positiveReport_vec[i].tp = true;
                return true;
            }
        }
        return false;
    }


    /**
     *
     * @param myStatus
     * Reading anomalies, and drawing conclusions about
     * true Positive Rate and False Positive Rate.
     */
    virtual void execute(status *myStatus) {

        for (size_t i = 0; i < myStatus->positiveReport_vec.size(); i++) {
            myStatus->positiveReport_vec[i].tp = false;
        }

        dio->write("Please upload your local anomalies file.\n");
        string s = "";
        float TP = 0, sum = 0, P = 0;
        while ((s = dio->read()) != "done") {
            size_t t = 0;

            //Run on the indexes of the string until you reach a comma,
            // which marks the boundary between the 2 numbers
            for (; s[t] != ','; t++);

            //Extract the numbers from the string to the int configuration
            string st = s.substr(0, t);
            string en = s.substr(t + 1, s.length());
            int start = stoi(st);
            int end = stoi(en);

            //Is there any overlap between the current deviation,
            //and one positiveReport from positiveReport_vec
            if (isTP(start, end, myStatus))
                TP++;

            //The sum of the lines in them was deviation
            sum += end + 1 - start;

            //The amount of deviation
            P++;
        }
        dio->write("Upload complete.\n");
        float FP = 0;
        for (size_t i = 0; i < myStatus->positiveReport_vec.size(); i++)
            if (!myStatus->positiveReport_vec[i].tp)
                FP++;
        float N = myStatus->testFileSize - sum;
        float tpr = ((int) (1000.0 * TP / P)) / 1000.0f;
        float fpr = ((int) (1000.0 * FP / N)) / 1000.0f;
        dio->write("True Positive Rate: ");
        dio->write(tpr);
        dio->write("\nFalse Positive Rate: ");
        dio->write(fpr);
        dio->write("\n");

    }
};

class exitProgram : public Command {
public:
    exitProgram(DefaultIO *dio) : Command(dio, "exit") {}

    /**
     *
     * @param myStatus
     */
    virtual void execute(status *myStatus) {
    }
};


#endif /* COMMANDS_H_ */