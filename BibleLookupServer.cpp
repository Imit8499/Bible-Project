#include <sstream>
#include <iostream>
#include <memory>
#include "Bible.h"
#include "Ref.h"
#include "fifo.h"

static const std::string pipe_id_receive = "B-Request"; //Communication pipes
static const std::string pipe_id_send = "B-Reply";

std::map<std::string, std::shared_ptr<Bible>> loadAllBibles() { //load all possible versions
    std::map<std::string, std::shared_ptr<Bible>> bibles;       //returns all versions in a map, and uses shared_ptr to not leak memory

    for(auto version : Bible::getVersionList()) {
        std::cout << "Loading Bible: " << version << std::endl;
        bibles[version] = std::make_shared<Bible>(Bible::versionToFile(version));

        if(!bibles[version]->valid()) {
            std::cout << "Could not open Bible: " << version << std::endl;
            bibles.erase(version);
        }
    }
    return bibles;
}

int main() {
    std::map<std::string, std::shared_ptr<Bible>> bibles = loadAllBibles(); //Load all versions
    Fifo pipe_receive(pipe_id_receive); // Open pipe
    Fifo pipe_send(pipe_id_send);
    std::cout << "Opening pipes..." << std::endl;

    for(;;) {
        pipe_receive.openread(); //Get request
        std::string request = pipe_receive.recv();
        pipe_receive.fifoclose();
        std::cout << "Request received: " << request << std::endl;

        std::string version = GetNextToken(request, " "); //Split params
        std::string requestType = GetNextToken(request, " ");
        std::string refText = GetNextToken(request, " ");
        LookupResult result;
        Ref ref(refText);

        pipe_send.openwrite(); //Reply
        std::stringstream out;

        if(bibles.count(version) == 0) { //Check for errors
            result = ELSE;
            out << result;
        }
        else {
            std::shared_ptr<Bible> bible = bibles[version]; //Access right version

            if(requestType == "lookup") {
                Verse verse = bible->lookup(ref, result);
                out << result << " " << ref.toString() << " " << verse.getVerse();
            }
            else if(requestType == "next") {
                Ref nextRef = bible->next(ref, result);
                out << result << " " << nextRef.toString();
            }
            else if(requestType == "prev") {
                Ref prevRef = bible->prev(ref, result);
                out << result << " " << prevRef.toString();
            }
            else {
                result = ELSE;
                out << result;
            }
        }
        pipe_send.send(out.str());
        pipe_send.fifoclose();
        std::cout << "Request complete: " << Bible::error(result) << endl;
    }
}
