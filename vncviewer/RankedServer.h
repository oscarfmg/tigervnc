#ifndef __RANKEDHOSTNAME_H__
#define __RANKEDHOSTNAME_H__

#include <string>

class RankedServer {

protected:
    int rank;
    std::string name;
    bool pinned;

public:
    RankedServer(int rank, std::string name, bool pinned);
    ~RankedServer();

    int getRank() const;
    const std::string& getName() const;
    bool isPinned() const;

    void setRank(int&);

    bool operator< (const RankedServer& h2) const;
    bool operator== (const RankedServer& h2) const;
    bool operator== (const std::string& str) const;
};

#endif // __RANKEDHOSTNAME_H__
