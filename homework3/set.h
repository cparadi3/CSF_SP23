#ifndef set 
#define set
#include <vector>
#include "block.h"

class Set {
    //private:
    public:
    std::vector<Block> *blockVector;
    //public:
    Set();
    ~Set();
    //void changeValue(Block value);
    void add(Block value);
    void replace(Block value);
};
#endif