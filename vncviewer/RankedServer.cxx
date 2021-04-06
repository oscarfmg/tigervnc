#include "RankedHostName.h"

RankedHostName::RankedHostName(int rank, std::string name, bool pinned)
: rank(rank)
, name(name)
, pinned(pinned) {

}

RankedHostName::~RankedHostName() {
}

int RankedHostName::getRank() const {
    return rank;
}

const std::string& RankedHostName::getName() const {
    return name;
}

bool RankedHostName::getPinned() const {
    return pinned;
}

void RankedHostName::setRank(int &rank) {
    this->rank = rank;
}

bool RankedHostName::operator< (const RankedHostName& h2) const {
    if (pinned == h2.pinned)
        return rank < h2.rank;
    else if (pinned)
        return true;
    return false;
}

bool RankedHostName::operator== (const RankedHostName& h2) const {
    return name == h2.name;
}

bool RankedHostName::operator== (const std::string& str) const {
    return name == str;
}