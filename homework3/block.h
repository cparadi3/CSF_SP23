#ifndef block 
#define block
#include <vector>
#include <string>

class Block {
    private:
        //std::string data;
        unsigned tag;
        bool dirty;
        unsigned age;
    public:
        Block();
        Block(unsigned input);
        ~Block();
        //void fillAddress(char location[8], std::string memAddress);
        void changeValue(unsigned input);
        unsigned getData();
        void makeDirty();
        bool isDirty();
        unsigned getAge();
        void incAge();
        bool operator == (Block compareTo);
};
#endif