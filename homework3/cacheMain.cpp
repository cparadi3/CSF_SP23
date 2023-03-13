#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "cache.h"

int main(int argc, char *argv[]) {
    int sets = atoi(argv[1]);
    int blocks = atoi(argv[2]);
    int bytes = atoi(argv[3]);
   
    Cache *myCache = new Cache(sets, blocks, bytes);
    //std::string test
     Set temp = myCache->setVector->at(0);
     Block tempy = temp.blockVector->at(0);
    std::string test = tempy.getData();
    std::cout << test;
    delete myCache;
    return 0;
    //int argc, char *argv[]
}