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

void Set::replace(Block value) {
    //int index = this->find(value);
    std::vector<Block>::iterator it = blockVector->begin();
    it = std::find(blockVector->begin(), blockVector->end(), value);
    Block *temp = &(blockVector->at(it - blockVector->begin()));
    temp->changeValue(value.getData());
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