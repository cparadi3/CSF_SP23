#ifndef block 
#define block
#include <vector>
#include <string>

class Block {
    private:
        std::vector<char[8]> byteVector; 
    public:
        Block();
        ~Block();
        void fillAddress(char *location[8], std::string memAddress);
};
#endif