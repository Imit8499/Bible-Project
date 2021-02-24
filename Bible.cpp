#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <map>
#include <cstdlib>
using namespace std;

static std::map<std::string, std::string> bibleVersions = {
        {"kjv", "/home/class/csc3004/Bibles/kjv-complete"},
        {"web", "/home/class/csc3004/Bibles/web-complete"},
        {"dby", "/home/class/csc3004/Bibles/dby-complete"},
        {"webster", "/home/class/csc3004/Bibles/webster-complete"},
        {"ylt", "/home/class/csc3004/Bibles/ylt-complete"},
};

bool Bible::versionExists(std::string version) {
    return bibleVersions.count(version) > 0;
}

std::string Bible::versionToFile(std::string version) {
    return versionExists(version) ? bibleVersions.at(version) : "";
}

Bible::Bible() { //Default Constructor
    infile = "/home/class/csc3004/Bibles/webster-complete";
    isOpen = hasScanned = hasPrevious = false;
}

Bible::Bible(const string s) {
    infile = s;
    isOpen = hasScanned = hasPrevious = false;
}

bool Bible::resetFile() {
    hasScanned = false;
    hasPrevious = false;

    if(isOpen) {
        instream.clear();
        instream.seekg(0, ios::beg);
        return true;
    }

    else {
        instream.open(infile, ios::in);
        if(instream) {
            isOpen = true; //If file is open
            instream.unsetf(ios::skipws);
            return true;
        }

        else {
            cerr << "Error" << infile << endl;
            return false;
        }
    }
}

bool Bible::scanNext() { // Reads next line
    string buffer;
    getline(instream, buffer);
    bool check = !instream.fail() && !buffer.empty();
    if(check) {
        previousVerse = currentVerse; //Preserve prior verse
        hasPrevious = hasScanned;
        currentVerse = Verse(buffer); //Update verse
        hasScanned = true;
    }
    return check;
}

void Bible::scanTo(Ref ref, LookupResult& status) {
    if((!isOpen) || (hasScanned && getCurrentVerse().getRef() > ref)) {
        if(!resetFile()) {status = ELSE;return;}
    }

    if(!hasScanned) {scanNext();}

    bool foundBook = false;
    bool foundChapter = false;

    for(bool check = hasScanned; check; check = scanNext()) {
        Ref currentRef = getCurrentVerse().getRef();
        if(ref.getBook() == currentRef.getBook()) {
            foundBook = true;
            if(ref.getChapter() == currentRef.getChapter()) {
                foundChapter = true;
            }
        }
        if(ref == currentRef) {status = SUCCESS;return;}
    }
    if(!foundBook) {status = NO_BOOK;}
    else if(!foundChapter) {status = NO_CHAPTER;}
    else {status = NO_VERSE;}
}

const Verse Bible::lookup(Ref ref, LookupResult& status) {
    scanTo(ref, status);
    return status == SUCCESS ? getCurrentVerse() : Verse();
}

const Ref Bible::next(Ref ref, LookupResult& status) {
    scanTo(ref, status);
    if(status == SUCCESS) {
        if(!scanNext()){
            status = NO_BOOK;}
    }
    return status == SUCCESS ? getCurrentVerse().getRef() : Ref();
}

const Ref Bible::prev(Ref ref, LookupResult& status) {
    scanTo(ref, status);
    if(status == SUCCESS && !hasPrevious){
        status = NO_BOOK;
    }
    return status == SUCCESS ? getPreviousVerse().getRef() : Ref();
}

const string Bible::error(LookupResult status) {
    switch(status) {
        case SUCCESS:
            return "Success";
        case NO_BOOK:
            return "No Book";
        case NO_CHAPTER:
            return "No Chapter";
        case NO_VERSE:
            return "No Verse";
        case ELSE:
        default:
            return "Unknown Error";
    }
}

void Bible::display() {
    cout << "Bible: " << infile << endl;
}
