#include "RankedServer.h"

RankedServer::RankedServer(int rank, std::string name, bool pinned)
: rank(rank)
, name(name)
, pinned(pinned) {

}

RankedServer::~RankedServer() {
}

int RankedServer::getRank() const {
    return rank;
}

const std::string& RankedServer::getName() const {
    return name;
}

bool RankedServer::isPinned() const {
    return pinned;
}

void RankedServer::setRank(int &rank) {
    this->rank = rank;
}

bool RankedServer::operator< (const RankedServer& h2) const {
    if (pinned == h2.pinned)
        return rank < h2.rank;
    else if (pinned)
        return true;
    return false;
}

bool RankedServer::operator== (const RankedServer& h2) const {
    return name == h2.name;
}

bool RankedServer::operator== (const std::string& str) const {
    return name == str;
}