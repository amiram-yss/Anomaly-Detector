

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

struct positiveReport {
    int start;
    int end;
    string description;
    bool truePositive;

    positiveReport() {
        start = 0;
        end = 0;
        description = "";
        truePositive = false;
    }
};

struct status {
    float threshold;
    vector<AnomalyReport> report;
    vector<positiveReport *> ppositiveReport;
    int testFileSize;

    status() {
        threshold = 0.9;
        testFileSize = 0;
    }
};


class DefaultIO {
public:
    virtual string read() {
    };

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    //virtual void readToNewFile(string name) = 0;
    void readToNewFile(string name) {
        ofstream out(name);
        string row;
        string done = "done";
        row = read();

        while (row != done) {
            out << row << endl;
            row = read();
        }
        out.close();
    }

    virtual void close() = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here




};

class StandardIO : public DefaultIO {
public:

    StandardIO();

    virtual string read() {
        string str;
        cin >> str;
        return str;
    };

    virtual void write(string text) {
        cout << text;
    };

    virtual void write(float f) {

    };

    virtual void read(float *f) {

    };

    void close() {};


};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    string description;
    DefaultIO *dio;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    string getDescription() { return description; }

    virtual void execute(status *myStatus) = 0;

    virtual ~Command() {}
};





// implement here your command classes

class uploadCSVfile : public Command {
public:
    uploadCSVfile(DefaultIO *dio) : Command(dio) {
        this->description = "upload a time series csv file";
    }

    void execute(status *myStatus) {
        dio->write("Please upload your local train CSV file.\n");
        dio->readToNewFile("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->readToNewFile("anomalyTest.csv");
        dio->write("Upload complete.\n");

    }
};


class algorithmSetting : public Command {
public:
    algorithmSetting(DefaultIO *dio) : Command(dio) {
        this->description = "algorithm settings";
    }

    void execute(status *myStatus) {
        float newThreshold;
        bool standartNum = false;

        while (!standartNum) {
            dio->write("The current correlation threshold is ");
            dio->write(myStatus->threshold);
            dio->write("\nType a new threshold\n");
            dio->read(&newThreshold);

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
    detectAnomalies(DefaultIO *dio) : Command(dio) {
        this->description = "detect anomalies";
    }

    void execute(status *myStatus) {

        HybridAnomalyDetector ad;
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");

        myStatus->testFileSize = test.getRowsNum();
        ad.setBRINK(myStatus->threshold);

        ad.learnNormal(train);
        myStatus->report = ad.detect(test);
        vector<AnomalyReport> reportCopy = myStatus->report;


        bool openNewPositive;

        for (auto report: myStatus->report) {
            openNewPositive = true;
            for (auto pr: myStatus->ppositiveReport) {
                if (pr->description == report.description
                    && pr->end + 1 == report.timeStep) {
                    pr->end++;
                    openNewPositive = false;

                }
            }

            if (openNewPositive) {

                positiveReport *pr = new positiveReport();
                pr->start = pr->end = report.timeStep;
                pr->description = report.description;
                myStatus->ppositiveReport.push_back(pr);
            }
        }

        dio->write("anomaly detection complete.\n");
    }


};


class displayResults : public Command {
public:
    displayResults(DefaultIO *dio) : Command(dio) {
        this->description = "display results";
    }

    void execute(status *myStatus) {
        for (auto report: myStatus->report) {
            dio->write(report.timeStep);
            dio->write("\t " + report.description + "\n");
        };
        dio->write("Done.\n");
    }
};


class upAnomAnalyze : public Command {
public:
    upAnomAnalyze(DefaultIO *dio) : Command(dio) {
        this->description = "upload anomalies and analyze results";

    }

    bool covering(int pr_a, int pr_b, int re_a, int re_b) {
        return ((pr_a >= re_a && pr_a <= re_b)
                || (pr_b >= re_a && pr_b <= re_b)
                || (pr_a <= re_a && pr_b >= re_b));
    }


    bool isTP(int start, int end, status *myStatus) {
        for (auto pr: myStatus->ppositiveReport) {
            if (covering(pr->start, pr->end, start, end)) {
                pr->truePositive = true;
                return true;
            }
        }
        return false;
    }



void execute(status *myStatus) {
    string row;
    string done = "done";
    string first_num;
    string second_num;
    int start;
    int end;
    int sum = 0;
    float positive = 0;
    float FP = 0;
    float TP = 0;
    float N;

    int index;
    char token;
    char comma = ',';

    dio->write("Please upload your local anomalies file.\n");
    row = dio->read();

    while (row != done) {
        index = 0;
        token = row[index];
        while (token != comma) {
            index++;
            token = row[index];
        }
        first_num = row.substr(0, index);
        second_num = row.substr(index + 1, row.length());
        start = stoi(first_num);
        end = stoi(second_num);

        sum += (end - start + 1);
        positive++;

        if (isTP(start, end, myStatus)) {
            TP++;
        }
        row = dio->read();
    }

    dio->write("Upload complete.\n");

    //
    N = myStatus->testFileSize - sum;

    //
    for (int i = 0; i < myStatus->ppositiveReport.size(); ++i) {
        if (!myStatus->ppositiveReport[i]->truePositive) {
            FP++;
        }
    }


    float truePositiveRate = ((int) (1000.0 * TP / positive)) / 1000.0f;
    float falseAlarmRate = ((int) (1000.0 * FP / N)) / 1000.0f;

    dio->write("True Positive Rate: ");
    dio->write(truePositiveRate);
    dio->write("\nFalse Positive Rate: ");
    dio->write(falseAlarmRate);
    dio->write("\n");


}

};

class exitProgram : public Command {
public:
    exitProgram(DefaultIO *dio) : Command(dio) {
        this->description = "exit";
    }

    void execute(status *myStatus) {
        return;
    }

};


#endif /* COMMANDS_H_ */
