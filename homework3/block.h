#ifndef block 
#define block
#include <vector>
#include <string>

class Block {
    private:
        //std::string data;
        unsigned tag;
    public:
        Block();
        Block(std::string input);
        ~Block();
        //void fillAddress(char location[8], std::string memAddress);
        void changeValue(std::string input);
        unsigned getData();
        //bool operator == (Block compareTo);
};
#endif