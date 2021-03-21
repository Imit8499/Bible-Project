#include "Ref.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

string GetNextToken(string& str, const string& delimiters = " ") {
    string::size_type startPos = str.find_first_not_of(delimiters, 0); //Skip delimiter
    string::size_type endPos = str.find_first_of(delimiters, startPos); //Find delimiter pos
    string next = str.substr(startPos, endPos - startPos); //Token found, remove and return
    string rest = str.substr(endPos - startPos + 1, string::npos);
    str = rest;
    return(next);
}

Ref::Ref() { //Default Constructor
    book = 0;
    chapter = 0;
    verse = 0;
}

Ref::Ref(const string s){
    string rtext = s;
    string strb = GetNextToken(rtext, ":"); //Get book
    book = atoi(strb.c_str());
    string strc = GetNextToken(rtext, ":"); //Get chapter
    chapter = atoi(strc.c_str());
    string strv = GetNextToken(rtext, " "); //Get verse
    verse = atoi(strv.c_str());
}

Ref::Ref(const Ref::book_ref b, const Ref::chapter_ref c, const Ref::verse_ref v) {
    book = b;
    chapter = c;
    verse = v;
}

Ref::book_ref Ref::getBook(){
    return book;
}
Ref::chapter_ref Ref::getChapter(){
    return chapter;
}
Ref::verse_ref Ref::getVerse(){
    return verse;
}

bool Ref::operator==(const Ref &r) const{
    return book == r.book && chapter == r.chapter && verse == r.verse;
}
bool Ref::operator<(const Ref &r) const{
    return book < r.book || (book == r.book && chapter < r.chapter) || (book == r.book && chapter == r.chapter && verse < r.verse);
}

std::string Ref::toString() const {
    std::stringstream ss;
    ss << book << ":" << chapter << ":" << verse;
    return ss.str();
}

void Ref::display() { //Use book name instead of number
    cout << getBookName() << " " << chapter << ":" << verse;
}

string Ref::getBookName(){
    static string book_names[66] = {
            "Genesis",
            "Exodus",
            "Leviticus",
            "Numbers",
            "Deuteronomy",
            "Joshua",
            "Judges",
            "Ruth",
            "1 Samuel",
            "2 Samuel",
            "1 Kings",
            "2 Kings",
            "1 Chronicles",
            "2 Chronicles",
            "Ezra",
            "Nehemiah",
            "Esther",
            "Job",
            "Psalms",
            "Proverbs",
            "Ecclesiastes",
            "Song of Solomon",
            "Isaiah",
            "Jeremiah",
            "Lamentations",
            "Ezekiel",
            "Daniel",
            "Hosea",
            "Joel",
            "Amos",
            "Obadiah",
            "Jonah",
            "Micah",
            "Nahum",
            "Habakkuk",
            "Zephaniah",
            "Haggai",
            "Zechariah",
            "Malachi",
            "Matthew",
            "Mark",
            "Luke",
            "John",
            "Acts",
            "Romans",
            "1 Corinthians",
            "2 Corinthians",
            "Galatians",
            "Ephesians",
            "Philippians",
            "Colossians",
            "1 Thessalonians",
            "2 Thessalonians",
            "1 Timothy",
            "2 Timothy",
            "Titus",
            "Philemon",
            "Hebrews",
            "James",
            "1 Peter",
            "2 Peter",
            "1 John",
            "2 John",
            "3 John",
            "Jude",
            "Revelation"
    };
    return (book >= 1 && book <= 66) ? book_names[book - 1] : "";
}