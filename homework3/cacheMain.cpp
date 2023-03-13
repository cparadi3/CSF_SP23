#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "cache.h"

int main(int argc, char *argv[]) {
    int sets = atoi(argv[1]);
    int blocks = atoi(argv[2]);
    int bytes = atoi(argv[3]);
    // TODO: add other arg parameters
    std::ifstream inf{argv[4]};
    Cache *myCache = new Cache(sets, blocks, bytes);
    while (inf) {
      std::string lors;
      inf >> lors;
      std::string memLoc;
      inf >> memLoc;
      std::string garbage;
      inf >> garbage;
      std::cout << lors << " " << memLoc << '\n';
      //myCache.do(lors, memLoc); 
    }
    //std::string test
     Set temp = myCache->setVector->at(0);
     Block tempy = temp.blockVector->at(0);
    std::string test = tempy.getData();
    std::cout << test;
    delete myCache;
    return 0;
    //int argc, char *argv[]
}