#ifndef SOFT_PROJECT1_BIBLELOOKUPCLIENT_H
#define SOFT_PROJECT1_BIBLELOOKUPCLIENT_H

#include <string>
#include "fifo.h"
#include "Bible.h"
#include "Verse.h"
#include "Ref.h"

class BibleLookupClient {
private:
    Fifo pipe_request;
    Fifo pipe_reply;
    std::string bibleVersion;

    struct ServerReply { // Structure for the generic server reply.
        LookupResult result;
        Ref ref;
        std::string verseText;
    };
    ServerReply request(std::string action, const Ref &ref);
public:
    //Requesting pipe id's
    BibleLookupClient(std::string pipe_request_id, std::string pipe_reply_id, std::string bibleVersion);
    Verse lookup(const Ref &ref, LookupResult &result); //Getting verse identified from Ref
    Ref next(const Ref &ref, LookupResult &result); //Getting ref after specified ref
    Ref prev(const Ref &ref, LookupResult &result); //Getting ref before specified ref
};
#endif //SOFT_PROJECT1_BIBLELOOKUPCLIENT_H
