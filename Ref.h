#ifndef SOFT_PROJECT1_REF_H
#define SOFT_PROJECT1_REF_H

#include <string>
#include <cstdlib>
using namespace std;

string GetNextToken(string& str, const string& delimiters);

class Ref {
public:

    typedef short book_ref, chapter_ref, verse_ref;

    static const book_ref MIN_BOOK_REF = 1; // Limits for books, chapters, and verses
    static const book_ref MAX_BOOK_REF = 66; // Revelation
    static const chapter_ref MIN_CHAPTER_REF = 1;
    static const chapter_ref MAX_CHAPTER_REF = 150; // Psalm 150
    static const verse_ref MIN_VERSE_REF = 1;
    static const verse_ref MAX_VERSE_REF = 176; // Psalm 119:176

private:

    book_ref book;
    chapter_ref chapter;
    verse_ref verse;

public:

    Ref();
    Ref(string s);
    Ref(const book_ref,const chapter_ref,const verse_ref);

    book_ref getBook();
    chapter_ref getChapter();
    verse_ref getVerse();

    string getBookName();
    std::string toString() const;


    bool operator==(const Ref &) const;
    bool operator<(const Ref &) const;
    bool operator!=(const Ref &r) const {return !(*this == r);}
    bool operator>(const Ref &r) const {return !(*this == r || *this < r);}

    void display();
};
#endif //SOFT_PROJECT1_REF_H