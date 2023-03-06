#ifndef cache
#define cache
#include <vector>
#include "set.h"

class Cache {
    private:
    std::vector<Set> setVector;
    int numSets;
    int numBlocks;
    int numBytes;
    public:
    Cache();
    Cache(int numSets, int numBlocks, int numBytes);
};
#endif