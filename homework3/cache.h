#ifndef cache
#define cache
#include <vector>
#include "set.h"
#include "block.h"

class Cache {
    private:
    std::vector<Set> *setVector;
    int numSets;
    int numBlocks;
    int numBytes;
    //metadata
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cycles;

    public:
    Cache(int numSets, int numBlocks, int numBytes);
    ~Cache();
    void attempt(std::string lors, std::string memLoc);
    void print();
};
#endif