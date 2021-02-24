#ifndef SOFT_PROJECT1_VERSE_H
#define SOFT_PROJECT1_VERSE_H

#include <string>
#include <cstdlib>
#include "Ref.h"
using namespace std;

class Verse {
private:

    Ref verseRef;
    string verseText;

public:
    Verse(); // Default

    Verse(const string s); //Getting Verse
    string getVerse();
    Ref getRef();

    void display(); // Display verse with reference
};
#endif //SOFT_PROJECT1_VERSE_H