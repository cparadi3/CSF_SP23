#include <vector>
#include "block.h"

Block::Block() {
    //todo : implement
    //byteVector.push_back() = "abcdef12";
    for (int i = 0; i < 8; i++) {
        data[i] = '0';
    }
}
Block::~Block() {
    //TODO: implement
}

void Block::changeValue(std::string input) {
    data = input;
}

std::string Block::getData() {
    return data;
}

bool Block::operator == (Block compareTo) {
    if (compareTo.getData() == this->getData()) {
        return true;
    }
      return false; 
    
}