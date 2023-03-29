#include <vector>
#include <algorithm>
#include <iostream>
#include "set.h"
#include "block.h"

Set::Set() {
    this->blockVector = new std::vector<Block>;
}

Set::~Set() {
    //delete(blockVector);
}

void Set::add(Block value) {
    blockVector->push_back(value);
}

unsigned Set::replace(unsigned index, Block value, unsigned numBytes) {
    //int index = this->find(value);
    /*std::vector<Block>::iterator it = blockVector->begin();
    it = std::find(blockVector->begin(), blockVector->end(), value);
    Block *temp = &(blockVector->at(it - blockVector->begin()));
    temp->changeValue(value.getData());
    */
    //sanity check, make sure oldest block is evicted
    /*
    int oldestBlockPos = 0;
    for (std::vector<Block>::iterator it = blockVector->begin(); it != blockVector->end(); it++) {
        if(it->getAge() > blockVector->at(oldestBlockPos).getAge()) {
            oldestBlockPos = it - blockVector->begin();
        }
    }
    //to get back to what we had earlier, delete this ^ and replace oldestBlockPos with zero
    //^^checking if our "move to front" heuristic works
    */
        unsigned total_cycles = 0;
        blockVector->push_back(value);
        if (blockVector->at(0).isDirty()) {
           total_cycles += 100 * (numBytes / 4);
        }
        blockVector->erase(blockVector->begin());
    //blockVector->insert(blockVector->begin() + index, value);
    //blockVector->erase(blockVector->begin() + 1);
    return total_cycles;
}

//check if a value is in the set (true if it is, false if it isnt)
bool Set::get(unsigned value) {
    bool returnVal = false;
    for(std::vector<Block>::iterator it = blockVector->begin(); it != blockVector->end(); it++) {
        //increment all the ages here. should only have to iterate through everything once
        it->incAge();
        if (it->getData() == value) {
            returnVal = true;
        }
    }
    return returnVal;
}

void Set::setDirty(unsigned offset) {
    blockVector->at(offset).makeDirty();
}

void Set::moveToBack(unsigned tag) {
    /*
    make sure to add offset parameter back for this version
    Block temp = blockVector->at(offset);
    blockVector->push_back(temp);
    blockVector->erase(blockVector->begin() + offset);
    */
    /*
    for (std::vector<Block>::iterator it = blockVector.begin(); it != blockVector.end(); it++) {
        if (it->getData() == tag) {
            blockVector->push_back(it);
            blockVector->erase(it);
        }
    }
    */
    for (unsigned i = 0; i < (unsigned) blockVector->size(); i++) {
        if (blockVector->at(i).getData() == tag) {
            blockVector->push_back(blockVector->at(i).getData());
            blockVector->erase(blockVector->begin() + i);
            debug = true;
            break;
        }
    }
    
}