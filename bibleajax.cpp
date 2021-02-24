#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <limits>
using namespace std;

#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

#include "Bible.h"


static bool checkInteger(std::string s) { //checks if a string is an integer
    static const std::string integer_chars = "-0123456789";
    return !s.empty() && s.find_first_not_of(integer_chars) == std::string::npos;
}

class BibleCGIRequest { //Got some advice to wrap the CGICC
public:
    BibleCGIRequest() : cgi(), failed(false), errorMessage("") {
        // Get the CGI input data.
        form_iterator bible = cgi.getElement("bible");
        form_iterator book = cgi.getElement("book");
        form_iterator chapter = cgi.getElement("chapter");
        form_iterator verse = cgi.getElement("verse");
        form_iterator nv = cgi.getElement("num_verse");

        bibleVersion = inputBibleVersion(bible, "bible version");

        ref = Ref(inputToInteger<Ref::book_ref>(book, "book", Ref::MIN_BOOK_REF,
                                                Ref::MAX_BOOK_REF), //Ref construction from input
                  inputToInteger<Ref::chapter_ref>(chapter, "chapter", Ref::MIN_CHAPTER_REF, Ref::MAX_CHAPTER_REF),
                  inputToInteger<Ref::verse_ref>(verse, "verse", Ref::MIN_VERSE_REF, Ref::MAX_VERSE_REF)
        );

        numberOfVerses = inputToInteger<int>(nv, "verse count", 1, std::numeric_limits<int>::max());
    }

    bool getFailed() {
        return failed; //Get failure message
    }

    std::string getErrorMessage() {
        return errorMessage;
    }

    Ref getRef() { //Get request reference
        return ref;
    }

    int getNumberOfVerses() { //Get # of verses after success
        return numberOfVerses;
    }

    std::string getBibleVersion() {
        return bibleVersion;
    };
private:
    Cgicc cgi;
    std::string errorMessage, bibleVersion;
    int numberOfVerses;
    bool failed;
    Ref ref;

    void fail(std::string message) {
        failed = true;
        errorMessage = message;
    }

    bool hasElement(const form_iterator &element) { //Is true if it is nonempty and element exists
        return element != cgi.getElements().end() && !element->getValue().empty();
    }

    std::string inputBibleVersion(const form_iterator &element, std::string name) {
        std::string result;
        if (!failed) {
            if (!hasElement(element))
                fail("the " + name + " was not specified");
            else if (!Bible::versionExists(element->getValue())) //Is Valid?
                fail("the specified " + name + " is not a recognized bible version");
            else
                result = element->getValue();
        }
        return result;
    }

    template<typename T>
    T inputToInteger(const form_iterator &element, std::string name, const T min, const T max) {
        T result = min; //Default to min
        if (!failed) {

            if (!hasElement(element)) //If element is not specified
                fail("the " + name + " was not specified");

            else if (!checkInteger(element->getValue()))  //Integer check
                fail("the specified " + name + " is not an integer");

            else {
                long value = element->getIntegerValue();

                if (value < min)
                    fail("the specified " + name + " is below " + std::to_string(min));

                else if (value > max)
                    fail("the specified " + name + " is above " + std::to_string(max));

                result = value;
            }
        }
        return result;
    }
};

int main() {
  cout << "Content-Type: text/plain\n\n";
  BibleCGIRequest request;

  if(request.getFailed())
        cout << "<p>Input error: <em>" << request.getErrorMessage() << "</em></p>";
  else{
      Bible bible(Bible::versionToFile(request.getBibleVersion()));
      LookupResult result;
      Verse verse = bible.lookup(request.getRef(), result);

      if(result == SUCCESS) { //If success look up verses
          int currentChapter = -1;
          for(int i = 0; i < request.getNumberOfVerses() && verse.getRef().getBook() == request.getRef().getBook() && result == SUCCESS; i++) {
              if(verse.getRef().getChapter() != currentChapter) { //Print header for chapter
                  currentChapter = verse.getRef().getChapter(); //Set curr chapter to next
                  cout << "<h2>" << verse.getRef().getBookName() << " " << verse.getRef().getChapter() << "</h2>" << endl;
              }
              cout << "<p><em>" << verse.getRef().getVerse() << ".</em> " << verse.getVerse() << "</p>" << endl; //Output
              Ref nextRef = bible.next(verse.getRef(), result); //Next reference
              if(result == SUCCESS) //Next verse search
                  verse = bible.lookup(nextRef, result);
          }
      }
      else {
          cout << "Lookup error: <em>" << bible.error(result); //Lookup failed
          switch(result) {
              case NO_CHAPTER:
                  cout << " in " << request.getRef().getBookName();
                  break;
              case NO_VERSE:
                  cout << " in " << request.getRef().getBookName() << " " << request.getRef().getChapter();
                  break;
              default:
                  break;
          }
          cout << "</em>" << endl;
      }
  }
}

