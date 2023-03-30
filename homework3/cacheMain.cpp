#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "cache.h"

int main(int argc, char *argv[]) {
    int sets = atoi(argv[1]);
    int blocks = atoi(argv[2]);
    int bytes = atoi(argv[3]);
    std::string throughOrBack = argv[5];
    std::string allocateNoAllocate = argv[4];
    std::string eviction = argv[6];
    // TODO: add other arg parameters 
    //std::ifstream inf{argv[7]}; //Don't think we need to do this
    if(sets % 2 != 0 && sets != 1) {
      std::cerr << "Number of sets not a power of 2\n";
      return 1;
    }
    if (bytes %2 != 0) {
      std::cerr << "Block size not a power of 2\n";
      return 1;
    }
    if (bytes < 4) {
      std::cerr << "Block size must be greater than or equal to 4\n";
    }
    if(throughOrBack.compare("write-back") == 0 && allocateNoAllocate.compare("no-write-allocate") == 0) {
      std::cerr << "Cannot make cache with 'write-back and 'no-write-allocate\n";
      return 1;
    }
    Cache *myCache = new Cache(sets, blocks, bytes);
    std::string  inf;
    //std::cin >> inf;
    while (std::cin) {
      //getline(std::cin, inf)
      //std::string lors = inf.substr(0, inf.find(" "));
      std::string lors;
      std::cin >> lors;
      if (lors.compare("") == 0) {
        break;
      } 
      //inf.erase(0,1);
      if(lors.compare("l")!= 0 && lors.compare("s") != 0) {
        std::cerr << "Incorrect command\n";
      }
      std::string temp;
      //temp = inf.substr(0, inf.find(" "));
      
      //unsigned memLoc = (unsigned) stoul(temp);
      //cin >> temp;
      //temp.erase(0,1);
      unsigned memLoc;
      std::cin >> std::hex >> memLoc;

      std::string garbage;
      std::cin >> garbage;
      //std::cout << lors << " " << memLoc << '\n';
      myCache->attempt(lors, memLoc, blocks, throughOrBack, allocateNoAllocate, eviction); 
    }
    //std::string test
     //Set temp = myCache->setVector->at(0);
     //Block tempy = temp.blockVector->at(0);
    //std::string test = tempy.getData();
    //std::cout << test;
    myCache->print();
    delete myCache;
    return 0;
    //int argc, char *argv[]
}