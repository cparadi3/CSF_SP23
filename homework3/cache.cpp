#include "cache.h"
#include <vector>
#include "set.h"

Cache::Cache(int numSets, int numBlocks, int numBytes) {
    this->numSets = numSets;
    this->numBlocks = numBlocks;
    this->numBytes = numBytes;
    this->setVector = new std::vector<Set>;
    for (int i = 0; i < numSets; i++) {
        for (int j = 0; j < numBlocks; j++) {
            
        }
    }
}
