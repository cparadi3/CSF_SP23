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
    this->total_loads = 0;
    this->total_stores = 0;
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
    bool test = find(index, offset, tag); //delete later
    std::string write = "write-through" ; //DELETE LATER
    std::string allocate = "no-write-allocate"; //delete later
    if (test) {
        hit(tag, index, offset, lors, write, allocate);
    } else {
        miss(tag, index, offset, lors, write, allocate);
    }
    
    //perform appropriate operation depending on hit or miss & store or load
   
    //TODO: change this to a stderr message
    
}

//get the tag from unsigned memory location
bool Cache::find(unsigned index, unsigned offset, unsigned tag) {
     for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        if (it->get(tag)) {
            return true;
        }
     }
     return false;
}

//get the index from the unsigned memory location
unsigned Cache::getIndex(unsigned memLoc, unsigned offsetBits, unsigned indexBits) {
    unsigned indexNum = memLoc >> offsetBits;
    //unsigned indexBits = log2(numSets);
    //shift back and forth to isolate the bits 
    if (indexBits == 0) {
        return 0; // if there's only one set the index will always be zero
    }
    indexNum = indexNum << (32 - indexBits);
    indexNum = indexNum >> (32 - indexBits);
    return indexNum;
}

//get the offset from unsigned memory location
unsigned Cache::getOffset(unsigned memLoc, unsigned offsetBits) {
    //int numbits = log2(numBlocks);
    //TODO : add error case for wrong num of blocks (also sets and byte)
    if (offsetBits == 0) {
        return 0; //no offset bits, offset will always be zero (direct mapped i think)
    }
    unsigned offsetNum = memLoc >> (32 - offsetBits);
    offsetNum = memLoc << (32 - offsetBits);
    return offsetNum; 
}

//get the tag from unsigned memory location
 unsigned Cache::getTag(unsigned memLoc, unsigned offsetBits, unsigned indexBits) {
    unsigned tagNum = memLoc >> (indexBits + offsetBits);
    return tagNum;
 } 

// perform the appropriate operation on a hit
void Cache::hit(unsigned tag, unsigned index, unsigned offset, std::string command, std::string writeThrough, std::string writeAllocate) {
    if(command.compare("l") == 0) {
        total_loads += 1;
        load_hits += 1;
        total_cycles += 1;
    }
    //Store Hit
    else {
        total_stores +=1;
        if(writeThrough.compare("write-through") == 0) {
            if(writeAllocate.compare("write-allocate") == 0) {
                //write-through write-allocate
                store_hits+=1;
                total_cycles+=100;
            }
            //write-through no-write-allocate
            else {
            store_hits += 1;
            total_cycles += 100;
            }

        }
        else {
            //write_back write-allocate
            if(writeAllocate.compare("write-allocate") == 0) {
                store_hits += 1;
                setVector->at(index).setDirty(offset);
            }
            else {
            //write-back no-write-allocate
            store_hits += 1;
            total_cycles += 100;
            }
            //Mark block dirty
        }
    }
}

// perform the appropriate operation on a miss
void Cache::miss(unsigned tag, unsigned index, unsigned offset, std::string command, std::string writeThrough, std::string writeAllocate) {
     if(command.compare("l") == 0) {
        total_loads += 1;
        Block tempBlock = Block(tag);
        setVector->at(index).replace(offset, tempBlock);
        load_misses += 1;
        total_cycles += 100;
        /*
        if(writeThrough.compare("write-through") == 0) {
            if(writeAllocate.compare("write-allocate") == 0) {
                //write-through write-allocate

            }
            else {
                //write-through no-write-allocate
                Block tempBlock = Block(tag);
                setVector->at(index).replace(offset, tempBlock);
                load_misses += 1;
                total_cycles += 100;
            }
            
        }
        else {
            //write-back
            if(writeAllocate.compare("write-allocate") == 0) {
                //write-back write-allocate
            }
            else {
                //write-back no-write-allocate
            }
        }
        */
    }
    else {
        total_stores += 1;
        store_misses += 1;
          if(writeThrough.compare("write-through") == 0) {
            if(writeAllocate.compare("write-allocate") == 0) {
                total_cycles += 200;
            }
            else {
                //write-through no-write-allocate
                total_cycles += 100;
            }
        }
        else {
            //write-back
            if(writeAllocate.compare("write-allocate") == 0) {
                //write-back write-allocate
                total_cycles += 1;
                Block tempBlock = Block(tag);
                setVector->at(index).replace(offset, tempBlock);
                setVector->at(index).setDirty(offset);
            }
            else {
                //write-back no-write-allocate
            }
        }
    }
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
      for (int i = 0; i < memLoc.length(); i++) {
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