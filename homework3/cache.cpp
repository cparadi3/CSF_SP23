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
        /*
        for (unsigned j = 0; j < numBlocks; j++) {
            Block bloq = Block();
            temp.add(bloq);
            //std::cout << bloq.getData() << " " << i << " " << j << '\n' ;
        }
        */
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

void Cache::attempt(std::string lors, unsigned memLoc, unsigned numBlocks, std::string write, std::string allocate, std::string eviction) {

    //look for tag in memory
    unsigned indexBits = (unsigned) log2(numSets);
    unsigned index = getIndex(memLoc, indexBits);
    unsigned tag = getTag(memLoc);
    //bool test = find(index, offset, tag); //delete later
    //std::string write = "write-through" ; //DELETE LATER
    //std::string allocate = "no-write-allocate"; //delete later
    std::pair<int, bool> setNum = find(index, tag);
    if (setNum.second) {
        hit(tag, index, lors, write, allocate, eviction);
    } else {
        miss(numBlocks, tag, index, lors, write, allocate);
    }
    
    //perform appropriate operation depending on hit or miss & store or load
   
    //TODO: change this to a stderr message
    
}

//get the tag from unsigned memory location
std::pair<int, bool> Cache::find(unsigned index, unsigned tag) {
     int returnVal = -1;
     //set containing the oldest value
     int oldestSet = 0;
     std::pair<int, bool> tempPair;
     //actual oldest value
     unsigned oldestVal = 0;
     for (std::vector<Set>::iterator it = setVector->begin(); it != setVector->end(); it++) {
        /*if (it->get(tag)) {
            returnVal = it - setVector->begin();
            
        }
        */
        //edge case, since we initialize everything in the cache to zero
        
        
        if (index < (it - setVector->begin())) {
            break;
        }
        

        for(std::vector<Block>::iterator it2 = it->blockVector->begin(); it2 != it->blockVector->end(); it2++) {
        //increment all the ages here. should only have to iterate through everything once
        
        if (it2->getData() == tag) {
            returnVal = it - setVector->begin();
        } //Useless DELETE
        else if (it2->getAge() > oldestVal) {
            oldestVal = it2->getAge();
            oldestSet = it - setVector ->begin();
        }
        it2->incAge();
    }
     }
     if ((returnVal < 0)|| (index != (unsigned) returnVal)) {
        tempPair.first = oldestSet;
        tempPair.second = false;
        return tempPair;
     }
     tempPair.first = returnVal;
     tempPair.second = true;
     return tempPair;
}

//get the index from the unsigned memory location
unsigned Cache::getIndex(unsigned memLoc, unsigned indexBits) {
    ////unsigned tagEnd = (1 << indexBits -1);
    ////indexNum = indexNum & tagEnd;
    //unsigned indexBits = log2(numSets);
    //shift back and forth to isolate the bits 
    
    if (indexBits == 0) {
        return 0; // if there's only one set the index will always be zero
    }
    /*
    indexNum = indexNum << (32 - indexBits);
    indexNum = indexNum >> (32 - indexBits);
    */
   //unsigned endTag = (1 << indexBits) - 1;
   //unsigned index = indexNum & endTag;
   //WARNING
    unsigned index = (memLoc >> (int) log2(numBytes)) & (numSets - 1);//WARNING parentheses
    return index;
}

//get the tag from unsigned memory location
 unsigned Cache::getTag(unsigned memLoc) {
    //unsigned tagNum = memLoc >> (indexBits + offsetBits);
    //return tagNum;
    
        return memLoc >> (int) log2(numSets * numBytes);

 } 

// perform the appropriate operation on a hit
void Cache::hit(unsigned tag, unsigned index, std::string command, std::string writeThrough, std::string writeAllocate, std::string lruORFifo) {
   //load hit
    if(command.compare("l") == 0) {
        total_loads += 1;
        load_hits += 1;
        total_cycles += 1;
        //std::cout << tag << " " << index << " " << offset << '\n'; //delete later
        if (lruORFifo.compare("lru") == 0) {
        moveToBack(index, tag);
        }
    }
    //Store Hit
    else {
        total_stores +=1;
        if (lruORFifo.compare("lru") == 0) {
        moveToBack(index, tag);
        }
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
            store_hits += 1;
            total_cycles += 1;
            //need to fix this
            setVector->at(index).setDirty(tag);
            
        }
    }
}

void Cache::moveToBack(unsigned index, unsigned tag) {
   setVector->at(index).moveToBack(tag);
   
   /*for (int j = 0; j < setVector->length(); j++) {
        for (unsigned i = 0; i < (unsigned) setVector->at(j).blockVector->size(); i++) {
        if (setVector->at(j).blockVector->at(i).getData() == tag) {
            setVector->at(j).blockVector->push_back(blockVector->at(i).getData());
            setVector->at(j).blockVector->erase(blockVector->begin() + i);
            break;
        }
    }
   }
   */
}

// perform the appropriate operation on a miss
void Cache::miss(unsigned numBlocks, unsigned tag, unsigned index, std::string command, std::string writeThrough, std::string writeAllocate) {
     if(command.compare("l") == 0) {
        total_loads += 1;
        Block tempBlock = Block(tag);
        total_cycles += setVector->at(index).replace(tempBlock, numBytes, numBlocks);
        load_misses += 1;
        total_cycles += (100 * (numBytes / 4));
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
                Block tempBlock = Block(tag);
                total_cycles += setVector->at(index).replace(tempBlock, numBytes, numBlocks);
                total_cycles += (100 * (numBytes/4) + 2);
            }
            else {
                //write-through no-write-allocate
                //Block tempBlock = Block(tag);
                //total_cycles += setVector->at(index).replace(offset, tempBlock, numBytes);
                total_cycles += 100;
            }
        }
        else {
                //write-back write-allocate
            total_cycles += 1 + (100 * (numBytes/4));
            Block tempBlock = Block(tag);
            total_cycles += setVector->at(index).replace(tempBlock, numBytes, numBlocks);
            setVector->at(index).setDirty(tag);
        }
    }
}

void Cache::print() {
    std::cout << "Total loads: " << total_loads << '\n';
    std::cout << "Total stores: " << total_stores << '\n';
    std::cout << "Load hits: " << load_hits << '\n';
    std::cout << "Load misses: " << load_misses << '\n';
    std::cout << "Store hits: " << store_hits << '\n';
    std::cout << "Store misses: " << store_misses << '\n';
    std::cout << "Total cycles: " << total_cycles << '\n';
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
