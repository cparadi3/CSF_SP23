#include <stdlib.h>
#include <stdio.h>
#include "cache.h"

int main() {
    //int sets = atoi(argv[1]);
    //int blocks = atoi(argv[2]);
    //int bytes = atoi(argv[3]);
    int sets = 16;
    int blocks = 1;
    int bytes = 4;
    Cache *myCache = new Cache(sets, blocks, bytes);
    std::string test = myCache->setVector[0].blockVector[0].getData();
    std::cout << test;
    delete myCache;
    return 0;
    //int argc, char *argv[]
}