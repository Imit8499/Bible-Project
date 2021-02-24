#include <iostream>
#include "Verse.h"
#include "Ref.h"
using namespace std;

Verse::Verse() { // Default
    verseText = "No Verse";
    verseRef = Ref();
}

Verse::Verse(const string s) { //Verse buffer and token grab
    string buffer = s; //split verse into sections
    string refToken = GetNextToken(buffer, " "); //splits on whitespace
    verseRef = Ref(refToken);
    verseText = buffer;
}

string Verse::getVerse() { //Accessor for verse
    return verseText;
}

Ref Verse::getRef() {     //Accessor for ref
    return verseRef;
}

// Show verse and Reference
void Verse::display() {
    verseRef.display();
    cout << " \n" << verseText << endl;
}