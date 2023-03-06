#include <stdlib.h>
#include <stdio.h>
#include "cache.h"

int main(int argc, char *argv[]) {
    int sets = atoi(argv[1]);
    int blocks = atoi(argv[2]);
    int bytes = atoi(argv[3]);
    Cache myCache(sets, blocks, bytes);
    return 0;
}