/***************************************************************************
 fifo.cpp  -  code to allow interprocess communication via a fifo, or "names pipe"
 *
* copyright : (C) 2009 by Jim Skon
*
* This code permits the creation and use of FIFOs for communication
* between processes.
*
* the named piped is created and used in /tmp
*
***************************************************************************/
#include "fifo.h"
using namespace std;
Fifo::Fifo(){
    f = 0;
}

Fifo::Fifo(string name){
    pipe = PATH + SIG + name;
    umask(0);

    int result = mknod(pipe.c_str(),MODE | S_IFIFO, 0);
    if ((result == -1) && (errno != EEXIST)) {
        cerr << "Error creating pipe: " << name << endl;
        return;
    }
    f = 0;
    return;
}

void Fifo::openwrite() {
    if (f !=0) {
        cerr << "Pipe already opened: " << pipe << endl;
        return;
    }

    f = open(pipe.c_str(),O_WRONLY); //Open pipe

    if (f ==-1) {
        cerr << "Error: " << pipe << endl;
        return;
    }
}
void Fifo::openread() {
    if (f !=0) {
        cerr << "Pipe already opened: " << pipe << endl;
        return;
    }

    f = open(pipe.c_str(),O_RDONLY); // Open Pipe

    if (f ==-1) {
        cerr << "Error: " << pipe << endl;
        return;
    }
}

void Fifo::fifoclose() {
    close(f);
    f = 0;
}


// Receive a message from a FIFO (named pipe)
string Fifo::recv() {
    if (f ==0) {
        cerr << "Pipe not open for read: " << pipe << endl;
        return ("\n");
    }

    int i = 0, bytes;
    string message;
    bool done;;
    char buff;
    message = "";
    done = false;

    while (i<MaxMess && !done) { // read until we see an end of message line
        bytes = read(f, &buff,1); // Read the next character in the pipe
        if (bytes ==-1) {
            cerr << "Error: " << pipe << endl;
            return("\n");
        }
        if (bytes > 0) {
            if (buff == MESSTERM && (i > 0)) {
                done = true;
            }
            else {
                i++;
                message += buff;
            }
        }
        else {
            fifoclose();
            openread();
        }
    }
    return(message);
}

void Fifo::send(string message) { // Send a message to a pipe
    if (f ==0) {
        cerr << "Pipe not open for send: " << pipe << endl;
        return;
    }

    int bytes;
    message = message + MESSTERM;
    bytes = write(f, message.c_str(),message.length());
    if (bytes ==-1) {
        cerr << "Error: " << pipe << endl;
        return;
    }
    if (bytes == 0) {
        cerr << "Error: " << pipe << endl;
        return;
    }
    return;
}

