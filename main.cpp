#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Verse.h"
#include "Ref.h"
#include "Bible.h"
#include "BibleLookupClient.h"
using namespace std;

static const std::string pipe_id_receive = "B-Reply";
static const std::string pipe_id_send = "B-Request";

int main (int argc, char **argv) {
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

    if(argc >= 5)
        length = atoi(argv[4]);

    Ref ref(book, chap, ver); //Creating reference
    BibleLookupClient client(pipe_id_send, pipe_id_receive, Bible::getDefaultVersion());
    LookupResult result;
    Verse verse = client.lookup(ref, result);

    if(result == SUCCESS) {
        int currentChapter = -1;
        for(int i = 0; i < length && result == SUCCESS; i++) {
            if(verse.getRef().getChapter() != currentChapter) {
                currentChapter = verse.getRef().getChapter();
                cout << verse.getRef().getBookName() << " \n" << verse.getRef().getChapter() << endl;
            }
            cout << " \n" << verse.getRef().getVerse() << " " << verse.getVerse() << endl;
            Ref nextRef = client.next(verse.getRef(), result);

            if(nextRef.getBook() != ref.getBook())
                break;
            if(result == SUCCESS)
                verse = client.lookup(nextRef, result);
        }
    }

    else {
        switch(result) {
            case NO_CHAPTER:
                cerr << " in " << ref.getBookName();
                break;
            case NO_VERSE:
                cerr << " in " << ref.getBookName() << " " << chap;
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