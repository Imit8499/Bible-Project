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

std::string Bible::getDefaultVersion() {
    return "web";
}

bool Bible::versionExists(std::string version) {
    return bibleVersions.count(version) > 0;
}

std::string Bible::versionToFile(std::string version) {
    return versionExists(version) ? bibleVersions.at(version) : "";
}

std::list<std::string> Bible::getVersionList(){
    std::list<std::string> result;
    for(auto const &pair : bibleVersions) {
        result.push_back(pair.first);
    }
    return result;
}

Bible::Bible() : Bible(versionToFile(getDefaultVersion())){ //Default Constructor
    //infile = "/home/class/csc3004/Bibles/webster-complete";
    //isOpen = hasScanned = hasPrevious = false;
}

Bible::Bible(const string s) : infile(s), isValid(false) {
    instream.open(infile, ios::in); //Build index if valid
    if(instream) {
        isValid = true;
        buildIndex();
    }
}

bool Bible::valid() {
    return isValid;
}

void Bible::buildIndex() {
    std::string buffer;
    std::streampos pos = instream.tellg();

    while(getline(instream, buffer)){
        if(!buffer.empty()) {
            Ref ref(buffer);
            index[ref] = pos;
        }
        pos = instream.tellg();
    }
}

LookupResult Bible::getRefLookupStatus(Ref ref) {
    if(index.count(ref))//Check if Ref exists
        return SUCCESS;
    else {
        Ref bookt(ref.getBook(), Ref::MIN_CHAPTER_REF, Ref::MIN_VERSE_REF);
        if(index.count(bookt)) { //Does the book exist?
            Ref chaptert(ref.getBook(), ref.getChapter(), Ref::MIN_VERSE_REF);
            if(index.count(chaptert)) //Does the chapter exist?
                return NO_VERSE;
            else
                return NO_CHAPTER;
        }
        else
            return NO_BOOK;
    }
}

const Verse Bible::lookup(Ref ref, LookupResult& status) {
    status = getRefLookupStatus(ref);
    if(status == SUCCESS) {
        std::string buffer;

        instream.clear(); // Reset and seek ref
        instream.seekg(index[ref]);
        getline(instream, buffer); // Get Verse line

        if(buffer.empty())
            status = ELSE;
        return Verse(buffer);
    }
    else
        return Verse(); // dummy return
}

const Ref Bible::next(Ref ref, LookupResult& status) {
    status = getRefLookupStatus(ref);
    if(status != SUCCESS)
        return Ref();

    std::map<Ref, std::streampos>::iterator i = index.find(ref); //Return next Ref after iterator can increment

    if(++i != index.end()) {
        status = SUCCESS;
        return i->first;
    }
    else {
        status = NO_BOOK;
        return Ref();
    }
}

const Ref Bible::prev(Ref ref, LookupResult& status) {
    status = getRefLookupStatus(ref);
    if(status != SUCCESS)
        return Ref();

    std::map<Ref, std::streampos>::iterator i = index.find(ref); //Return previous Ref after iterator can decrement
    if(i != index.begin()) {
        --i;
        status = SUCCESS;
        return i->first;
    }
    else {
        status = NO_BOOK;
        return Ref();
    }
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
