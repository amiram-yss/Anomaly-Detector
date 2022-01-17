/*
 * CLI.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO *dio;
    vector<Command*> commands_vec;

    // you can add data members
public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};

#endif /* CLI_H_ */
