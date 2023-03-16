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
    int total_loads;
    int total_stores;
    int load_hits;
    int load_misses;
    int store_hits;
    int store_misses;
    int total_cycles;

    public:
    Cache(int numSets, int numBlocks, int numBytes);
    ~Cache();
    void attempt(std::string lors, unsigned memLoc);
    void print();
    //try to find tag value in cache. true means hit, false means miss
    bool find(unsigned index, unsigned offset, unsigned tag);
    //get the index from the unsigned memory location
    unsigned getIndex(unsigned memLoc, unsigned offsetBits, unsigned indexBits);
    //get the offset from unsigned memory location
    unsigned getOffset(unsigned memLoc, unsigned offsetBits);
    //get the tag from unsigned memory location
    unsigned getTag(unsigned memLoc, unsigned offsetBits, unsigned indexBits);
    // perform the appropriate operation on a hit
    void hit(unsigned tag, unsigned index, unsigned offset, std::string command, std::string writeThrough, std::string writeAllocate);
    // perform the appropriate operation on a miss
    void miss(unsigned tag, unsigned index, unsigned offset, std::string command, std::string writeThrough, std::string writeAllocate);
};
#endif