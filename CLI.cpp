/*
 * CLI.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include "CLI.h"
#include <string.h>
#include <unistd.h>
#include "commands.h"



CLI::CLI(DefaultIO* dio) {
    this->dio = dio;

    //Placing the commands into the command vector
    commands_vec.push_back(new uploadCSVfile (dio));
    commands_vec.push_back(new algorithmSetting(dio));
    commands_vec.push_back(new detectAnomalies(dio));
    commands_vec.push_back(new displayResults(dio));
    commands_vec.push_back(new upAnomAnalyze(dio));
    commands_vec.push_back(new exitProgram(dio));

}

void CLI::start(){

    status myStatus;
    string input;
    int choice = 0;

    while (choice != 6 ) {

        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");

        dio->write("1." + commands_vec[0]->getDescription() + "\n");
        dio->write("2." + commands_vec[1]->getDescription()+ "\n");
        dio->write("3." + commands_vec[2]->getDescription() + "\n");
        dio->write("4." + commands_vec[3]->getDescription() + "\n");
        dio->write("5." + commands_vec[4]->getDescription() + "\n");
        dio->write("6." + commands_vec[5]->getDescription() + "\n");

        input = dio->read();
        choice = input[0] - '0';

        if (choice >= 1 && choice <= 6) {
            commands_vec[choice - 1]->execute(&myStatus);
        }
    }






}


CLI::~CLI() {
    for (int j = 0; j < commands_vec.size(); ++j) {
        delete commands_vec[j];
    }
}

