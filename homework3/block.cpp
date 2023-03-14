#include <vector>
#include "block.h"

Block::Block() {
    //todo : implement
    //byteVector.push_back() = "abcdef12";
    /*for (int i = 0; i < 8; i++) {
        data[i] = '0';
    }
    */
    this->data = "00000000";
}

//constructor with string value argument
Block::Block(std::string input) {
    this->data = input;
}
Block::~Block() {
    //TODO: implement
    //Don't think we need anything, nothing to free
}

void Block::changeValue(std::string input) {
    data = input;
}

std::string Block::getData() {
    return data;
}

bool Block::operator == (Block compareTo) {
    /*
    if (compareTo.getData() == this->getData()) {
        return true;
    }
    */
   //pretty sure need to use compare for strings
    if(this->getData().compare(compareTo.getData()) == 0) {
        return true;
    }
      return false; 
    
}