#include "cache.h"
#include <vector>
#include <iostream>
#include <math.h>
#include "set.h"
#include "block.h"

Cache::Cache(int numSets, int numBlocks, int numBytes) {
    this->numSets = numSets;
    //std::cout << numSets;
    this->numBlocks = numBlocks;
    this->numBytes = numBytes;
    this->setVector = new std::vector<Set>;
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_misses = 0;
    total_cycles = 0;
    for (int i = 0; i < numSets; i++) {
        Set temp = Set();
        setVector->push_back(temp);
        for (int j = 0; j < numBlocks; j++) {
            Block bloq = Block();
            temp.add(bloq);
            //std::cout << bloq.getData() << " " << i << " " << j << '\n' ;
        }
    }
}

Cache::~Cache() {
   /* for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        Set *temp = it;
        delete temp;
    }
    */
    delete this->setVector;
}

void Cache::attempt(std::string lors, unsigned memLoc) {

    //look for tag in memory
    unsigned offsetBits = (unsigned) log2(numBlocks);
    unsigned indexBits = (unsigned) log2(numSets);
    unsigned index = getIndex(memLoc, offsetBits, indexBits);
    unsigned offset = getOffset(memLoc, offsetBits);
    unsigned tag = getTag(memLoc, offsetBits, indexBits);
    if (find(index, offset, tag)) {
        hit();
    } else {
        miss();
    }
    for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        if (it->get(tag)) {
            hit = true;
        }
    }
    //perform appropriate operation depending on hit or miss & store or load
   if (lors.compare("s") == 0) {
        //for now, cache is write-through and no-write-allocate
        if (hit) {
            total_cycles += 101;
            store_hits += 1;
        } else {
            //don't modify the cache (for no-write allocate)
            store_misses += 1;
            total_cycles += 100;
        }
   } else if (lors.compare("l") == 0) {
        if (hit) {
            load_hits += 1;
            total_cycles += 1;
        } else {
            load_misses += 1;
            setVector->erase(setVector->begin());
            Set temp = Set();
            temp.add(tag);
            setVector->push_back(temp);
            total_cycles += 100;
        }
   } else {
    //TODO: change this to a stderr message
    std::cout << "error. ivalid input (lors): " << '\n';
   }
}

//get the tag from unsigned memory location
bool find(unsigned index, unsigned offset, unsigned tag) {
     for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        if (it->get(tag)) {
            return true;
        }
     }
     return false;
}

//get the index from the unsigned memory location
unsigned getIndex(unsigned memLoc, unsigned offsetBits, unsigned indexBits) {
    unsigned indexNum = memLoc >> offsetBits;
    //unsigned indexBits = log2(numSets);
    //shift back and forth to isolate the bits 
    indexNum = indexNum << (32 - indexBits);
    indexNum = indexNum >> (32 - indexBits);
}

//get the offset from unsigned memory location
unsigned getOffset(unsigned memLoc, unsigned offsetBits) {
    //int numbits = log2(numBlocks);
    //TODO : add error case for wrong num of blocks (also sets and byte)
    unsigned offsetNum = memLoc >> (32 - offsetBits);
    offsetNum = offsetNum = memLoc << (32 - offsetBits);
    return offsetNum; 
}

//get the tag from unsigned memory location
 unsigned getTag(unsigned memLoc, unsigned offsetBits, unsigned indexBits) {
    unsigned tagNum = memLoc >> (indexBits + offsetBits);
 } 

// perform the appropriate operation on a hit
void hit() {

}

// perform the appropriate operation on a miss
void miss() {

}
void Cache::print() {
    std::cout << "load hits: " << load_hits << '\n';
    std::cout << "load misses: " << load_misses << '\n';
    std::cout << "store hits: " << store_hits << '\n';
    std::cout << "store misses: " << store_misses << '\n';
    std::cout << "total cycles: " << total_cycles << '\n';
 }
/*
 void Cache::attempt(std::string lors, unsigned memLoc) {
    //remove the "0x" from the memory tag
    std::string tag = memLoc;
    tag.erase(0,1);
    //"hit" variable
    bool hit = false;
    /* for (int i = 0; i < memLoc.length(); i++) {
        if (i > 1) {
            tag[i] = memLoc[i];
        }
        //tag[i+1] = 0;
    }
    
    //look for tag in memory
    for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        if (it->get(tag)) {
            hit = true;
        }
    }
    //perform appropriate operation depending on hit or miss & store or load
   if (lors.compare("s") == 0) {
        //for now, cache is write-through and no-write-allocate
        if (hit) {
            total_cycles += 101;
            store_hits += 1;
        } else {
            //don't modify the cache (for no-write allocate)
            store_misses += 1;
            total_cycles += 100;
        }
   } else if (lors.compare("l") == 0) {
        if (hit) {
            load_hits += 1;
            total_cycles += 1;
        } else {
            load_misses += 1;
            setVector->erase(setVector->begin());
            Set temp = Set();
            temp.add(tag);
            setVector->push_back(temp);
            total_cycles += 100;
        }
   } else {
    //TODO: change this to a stderr message
    std::cout << "error. ivalid input (lors): " << '\n';
   }
}
*/