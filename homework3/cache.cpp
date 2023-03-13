#include "cache.h"
#include <vector>
#include "set.h"
#include "block.h"

Cache::Cache(int numSets, int numBlocks, int numBytes) {
    this->numSets = numSets;
    this->numBlocks = numBlocks;
    this->numBytes = numBytes;
    this->setVector = new std::vector<Set>;
    for (int i = 0; i < numSets; i++) {
        Set *temp = new Set();
        setVector->push_back(*temp);
        for (int j = 0; j < numBlocks; j++) {
            Block *bloq = new Block();
            temp->add(*bloq);
        }
    }
}

Cache::~Cache() {

}