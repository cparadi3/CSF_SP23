#ifndef cache
#define cache
#include <vector>
#include "set.h"
#include "block.h"

class Cache {
    //private:
    public:
    std::vector<Set> *setVector;
    int numSets;
    int numBlocks;
    int numBytes;
    //public:
    Cache(int numSets, int numBlocks, int numBytes);
    ~Cache();
};
#endif