#include "cache.h"
#include <vector>
#include <iostream>
#include "set.h"
#include "block.h"

Cache::Cache(int numSets, int numBlocks, int numBytes) {
    this->numSets = numSets;
    //std::cout << numSets;
    this->numBlocks = numBlocks;
    this->numBytes = numBytes;
    this->setVector = new std::vector<Set>;
    int load_hits = 0;
    int load_misses = 0;
    int store_hits = 0;
    int store_misses = 0;
    int total_cycles = 0;
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

void Cache::attempt(std::string lors, std::string memLoc) {
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
    */
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
            total_cycles += 100;
            store_hits += 1;
        } else {
            //don't modify the cache (for no-write allocate)
            store_misses += 1;
            total_cycles += 100;
        }
   } else if (lors.compare("l") == 0) {
        if (hit) {
            load_hits += 1;
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
    std::cout << "error. ivalid input (lors)";
   }
}

void Cache::print() {
    std::cout << "load hits: " << load_hits << '\n';
    std::cout << "load misses: " << load_misses << '\n';
    std::cout << "store hits: " << store_hits << '\n';
    std::cout << "store misses: " << store_misses << '\n';
    std::cout << "total cycles: " << total_cycles << '\n';
 }