#include <vector>
#include "block.h"

Block::Block() {
    //todo : implement
    //byteVector.push_back() = "abcdef12";
    /*for (int i = 0; i < 8; i++) {
        data[i] = '0';
    }
    */
    this->tag = 0;
}

//constructor with string value argument
Block::Block(unsigned input) {
    this->tag = input;
    this->dirty = false;
}
Block::~Block() {
    //TODO: implement
    //Don't think we need anything, nothing to free
}

void Block::changeValue(unsigned input) {
    tag = input;
}

unsigned Block::getData() {
    return tag;
}

void Block::makeDirty() {
    dirty = true;
}
bool Block::operator == (Block compareTo) {
    if (compareTo.getData() == this->getData()) {
        return true;
    }
    return false; 
}
