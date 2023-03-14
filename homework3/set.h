#ifndef set 
#define set
#include <vector>
#include "block.h"

class Set {
    private:
    std::vector<Block> *blockVector;
    public:
    Set();
    ~Set();
    //void changeValue(Block value);
    void add(Block value);
    void replace(Block value);
    bool get(std::string value);
};
#endif