#ifndef set 
#define set
#include <vector>
#include "block.h"

class Set {
    private:
    //std::vector<Block> *blockVector;
    public:
    std::vector<Block> *blockVector;
    Set();
    ~Set();
    //void changeValue(Block value);
    void add(Block value);
    void replace(unsigned index, Block value);
    bool get(unsigned value);
    void setDirty(unsigned offset);
};
#endif