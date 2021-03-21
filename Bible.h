#ifndef SOFT_PROJECT1_BIBLE_H
#define SOFT_PROJECT1_BIBLE_H

#include "Ref.h"
#include "Verse.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <list>
#include <string>
using namespace std;

enum LookupResult {SUCCESS, NO_BOOK, NO_CHAPTER, NO_VERSE, ELSE};

class Bible{
private:
    string infile;
    ifstream instream;
    bool isValid;
    std::map<Ref, std::streampos> index; //Ref positions in file index
    void buildIndex();
    LookupResult getRefLookupStatus(Ref ref);

public:
    Bible();
    Bible(const string s);

    bool valid();

    const Verse lookup(Ref ref, LookupResult& status); // Verse lookup by ref & set status
    const Ref next(Ref ref, LookupResult& status);     // Return reference
    const Ref prev(Ref ref, LookupResult& status);     // Return reference before verse

    static const string error(LookupResult status); // Lookup status
    void display();

    static bool versionExists(std::string version);
    static std::string versionToFile(std::string version);

    static std::list<std::string> getVersionList();
    static std::string getDefaultVersion();
};
#endif //SOFT_PROJECT1_BIBLE_H