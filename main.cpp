#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Verse.h"
#include "Ref.h"
#include "Bible.h"
using namespace std;

int main (int argc, char **argv) {
    Bible bible("/home/class/csc3004/Bibles/web-complete"); //Bible Object
    int length = 1; //# of verses
    int book, chap, ver; //Bible param

    switch(argc) {
        case 0: //Success
        case 1:
            cerr << "Empty BOOK field" << endl;
                return EXIT_FAILURE;
        case 2:
            cerr << "Empty CHAPTER field" << endl;
                return EXIT_FAILURE;
        case 3:
            cerr << "Empty VERSE field" << endl;
                return EXIT_FAILURE;
        default:
            break;
    }

    book = atoi(argv[1]); //Args as ints
    chap = atoi(argv[2]);
    ver = atoi(argv[3]);

    if(argc >= 5) {
        length = atoi(argv[4]);
    }

    Ref ref(book, chap, ver); //Creating reference
    LookupResult result;
    Verse verse = bible.lookup(Ref(book, chap, ver), result);

    if(result == SUCCESS) {
        int currentChapter = -1;
        for(int i = 0; i < length && result == SUCCESS; i++) {
            if(verse.getRef().getChapter() != currentChapter) {
                currentChapter = verse.getRef().getChapter();
                cout << verse.getRef().getBook() << " \n" << verse.getRef().getChapter() << endl;
            }
            cout << " \n" << verse.getRef().getVerse() << " " << verse.getVerse() << endl;
            Ref nextRef = bible.next(verse.getRef(), result);

            if(nextRef.getBook() != ref.getBook())
                break;
            if(result == SUCCESS)
                verse = bible.lookup(nextRef, result);
        }
    }

    else {
        switch(result) {
            case NO_CHAPTER:
                cerr << " in " << ref.getBook();
                break;
            case NO_VERSE:
                cerr << " in " << ref.getBook() << " " << chap;
                break;
            case SUCCESS:
                break;
            case NO_BOOK:
                break;
            case ELSE:
                break;
        }
        cerr << endl;
    }
}