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
    unsigned replace(unsigned index, Block value, unsigned numBytes, unsigned numBlocks);
    bool get(unsigned value);
    void setDirty(unsigned tag);
    void moveToBack(unsigned tag);
};
#endif