#include <sstream>
#include "BibleLookupClient.h"
#include "Ref.h"

BibleLookupClient::BibleLookupClient(std::string pipe_request_id, std::string pipe_reply_id, std::string bibleVersion)
: pipe_request(pipe_request_id), pipe_reply(pipe_reply_id), bibleVersion(bibleVersion) {}

BibleLookupClient::ServerReply BibleLookupClient::request(std::string action, const Ref &ref) {
    ServerReply reply;

    pipe_request.openwrite(); //Construct request
    std::stringstream out;
    out << bibleVersion << " " << action << " " << ref.toString();

    pipe_request.send(out.str());
    pipe_request.fifoclose();

    pipe_reply.openread(); //Receive reply
    std::string replyText = pipe_reply.recv();
    pipe_reply.fifoclose();

    std::string statusText = GetNextToken(replyText, " "); // Split reply
    std::string verseText = replyText;

    reply.result = static_cast<LookupResult>(atoi(statusText.c_str())); //Convert reply
    reply.ref = Ref(verseText);
    reply.verseText = verseText;
    return reply;
}
Verse BibleLookupClient::lookup(const Ref &ref, LookupResult &result) { //Wrapping request
    ServerReply reply = request("lookup", ref);
    result = reply.result;
    return Verse(reply.verseText);
}

Ref BibleLookupClient::next(const Ref &ref, LookupResult &result) {
    ServerReply reply = request("next", ref);
    result = reply.result;
    return reply.ref;
}

Ref BibleLookupClient::prev(const Ref &ref, LookupResult &result) {
    ServerReply reply = request("prev", ref);
    result = reply.result;
    return reply.ref;
}
