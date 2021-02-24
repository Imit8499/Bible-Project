#ifndef SOFT_PROJECT1_BIBLE_H
#define SOFT_PROJECT1_BIBLE_H

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Ref.h"
#include "Verse.h"
using namespace std;

enum LookupResult {SUCCESS, NO_BOOK, NO_CHAPTER, NO_VERSE, ELSE};

class Bible{
private:
    string infile;
    ifstream instream;

    bool isOpen; //Scan Status
    bool hasScanned;
    bool hasPrevious;

    Verse currentVerse; //Result
    Verse previousVerse;

    bool resetFile();
    void scanTo(Ref ref, LookupResult &status); // Scan the input file and set status.
    bool scanNext();                            // True if next verse exist.

    Verse getCurrentVerse(){
        return currentVerse;
    };
    
    Verse getPreviousVerse(){
        return previousVerse;
    };

public:
    Bible();
    Bible(const string s);

    const Verse lookup(Ref ref, LookupResult& status); // Verse lookup by ref & set status
    const Ref next(Ref ref, LookupResult& status);     // Return reference
    const Ref prev(Ref ref, LookupResult& status);     // Return reference before verse

    const string error(LookupResult status); // Lookup status
    void display();
    static bool versionExists(std::string version);
    static std::string versionToFile(std::string version);
};
#endif //SOFT_PROJECT1_BIBLE_H