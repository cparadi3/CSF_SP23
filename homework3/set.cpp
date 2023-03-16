#include <vector>
#include <algorithm>
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

void Set::replace(unsigned index, Block value) {
    //int index = this->find(value);
    /*std::vector<Block>::iterator it = blockVector->begin();
    it = std::find(blockVector->begin(), blockVector->end(), value);
    Block *temp = &(blockVector->at(it - blockVector->begin()));
    temp->changeValue(value.getData());
    */
   blockVector->push_back(value);
   blockVector->erase(blockVector->begin());
    //blockVector->insert(blockVector->begin() + index, value);
    //blockVector->erase(blockVector->begin() + 1);

}

//check if a value is in the set (true if it is, false if it isnt)
bool Set::get(unsigned value) {
    for(std::vector<Block>::iterator it = blockVector->begin(); it != blockVector->end(); it++) {
        if (it->getData() == value) {
            return true;
        }
    }
    return false;
}

void Set::setDirty(unsigned offset) {
    blockVector->at(offset).makeDirty();
}

void Set::moveToBack(unsigned offset) {
    Block temp = blockVector->at(offset);
    blockVector->push_back(temp);
    blockVector->erase(blockVector->begin() + offset);
}