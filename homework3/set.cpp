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

unsigned Set::replace(Block value, unsigned numBytes, unsigned numBlocks) {
    //int index = this->find(value);
    /*std::vector<Block>::iterator it = blockVector->begin();
    it = std::find(blockVector->begin(), blockVector->end(), value);
    Block *temp = &(blockVector->at(it - blockVector->begin()));
    temp->changeValue(value.getData());
    */
    //sanity check, make sure oldest block is evicted
    
    
    //to get back to what we had earlier, delete this ^ and replace oldestBlockPos with zero
    //^^checking if our "move to front" heuristic works
    
        unsigned total_cycles = 0;
        Block temp = Block(value);
        blockVector->push_back(temp);
        if(blockVector->size() == numBlocks + 1) {
            if (blockVector->at(0).isDirty()) {
                total_cycles += 100 * (numBytes / 4);
            }
            blockVector->erase(blockVector->begin());
        }
    //blockVector->insert(blockVector->begin() + index, value);
    //blockVector->erase(blockVector->begin() + 1);
    return total_cycles;
}



void Set::setDirty(unsigned tag) {
    for (unsigned i = 0; i < (unsigned) blockVector->size(); i++) {
        if (blockVector->at(i).getData() == tag) {
            blockVector->at(i).makeDirty();
            break;
        }
    }
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
    bool test = false; //debugging, delete later
    for (unsigned i = 0; i < (unsigned) blockVector->size(); i++) {
        if (blockVector->at(i).getData() == tag) {
            blockVector->push_back(blockVector->at(i));
            blockVector->erase(blockVector->begin() + i);
            test = true; //debugging, delete later
            break;
        }
    
    }
    if (!test) { //debugging, delete later
        std::cout << "this shouldn't happen" << '\n';
    }
    
}