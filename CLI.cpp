/*
 * CLI.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
		this->dio=dio;
		commands.push_back(new UploadCSV(dio));
		commands.push_back(new Settings(dio));
		commands.push_back(new Detect(dio));
		commands.push_back(new Results(dio));
		commands.push_back(new UploadAnom(dio));
		commands.push_back(new Exit(dio));
}

void CLI::start(){
	SharedState sharedState;
	int index=-1;
	while(index!=5){
		dio->write("Welcome to the Anomaly Detection Server.\n");
		dio->write("Please choose an option:\n");
		for(size_t i=0;i<commands.size();i++){
			string s("1.");
			s[0]=((char)(i+1+'0'));
			dio->write(s);
			dio->write(commands[i]->description+"\n");
		}
		string input = dio->read();
		index=input[0]-'0'-1;
		if(index>=0 && index<=6)
			commands[index]->execute(&sharedState);
	}
}


CLI::~CLI() {
	for(size_t i=0;i<commands.size();i++){
		delete commands[i];
	}
}

