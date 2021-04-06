#ifndef __RANKEDHOSTNAME_H__
#define __RANKEDHOSTNAME_H__

#include <string>

class RankedHostName {

protected:
    int rank;
    std::string name;
    bool pinned;

public:
    RankedHostName(int rank, std::string name, bool pinned);
    ~RankedHostName();

    int getRank() const;
    const std::string& getName() const;
    bool getPinned() const;

    void setRank(int&);

    bool operator< (const RankedHostName& h2) const;
    bool operator== (const RankedHostName& h2) const;
    bool operator== (const std::string& str) const;
};

#endif // __RANKEDHOSTNAME_H__
