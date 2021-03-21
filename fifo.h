/***************************************************************************
* fifo.cpp  -  code to allow interprocess communication via a fifo, or "names pipe"
 *
* copyright : (C) 2009 by Jim Skon
*
* This code permits the creation and use of FIFOs for communication
* between processes.
*
* the named piped is created and used in /tmp
*
***************************************************************************/
#ifndef SOFT_PROJECT1_FIFO_H
#define SOFT_PROJECT1_FIFO_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

#define MaxMess 1000
const string PATH  = "/tmp/";
const string SIG = "isatanner_"; //unique signature
#define MODE 0777
#define MESSTERM '\n'

class Fifo {
private:
    int f; //file descriptor
    string pipe;
public:
    Fifo();
    Fifo(string);

    void openread();    // Start a new read transaction
    void openwrite();   // Start a new write transaction
    void fifoclose();       // Finish a transaction

    string recv();    // Get the next record
    void send(string);    // Send a record
};
#endif //SOFT_PROJECT1_FIFO_H
