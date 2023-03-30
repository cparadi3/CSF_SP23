#!/bin/bash

echo "Test 1: BIG BLOCK DIRECT MAPPED NWA WT LRU"
./csim 8192 1 64 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 2: BIG BLOCK DIRECT MAPPED NWA WT FIFO"
./csim 8192 1 64 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 3: BIG BLOCK DIRECT MAPPED WA WB LRU "
./csim 8192 1 64 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 4: BIG BLOCK DIRECT MAPPED WA WB FIFO"
./csim 8192 1 64 write-allocate write-back fifo < gcc.trace
echo "----------------------"


echo "Test 5: BIG BLOCK SET ASSOCIATIVE NWA WT LRU"
./csim 128 64 64 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 6: BIG BLOCK SET ASSOCIATIVE NWA WT FIFO"
./csim 128 64 64 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 7: BIG BLOCK SET ASSOCIATIVE WA WB LRU"
./csim 128 64 64 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 8: BIG BLOCK SET ASSOCIATIVE WA WB FIFO"
./csim 128 64 64 write-allocate write-back fifo < gcc.trace
echo "----------------------"



echo "Test 9: BIG BLOCK FULLY ASSOCIATIVE NWA WT LRU"
./csim 1 8192 64 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 10: BIG BLOCK FULLY ASSOCIATIVE NWA WT FIFO"
./csim 1 8192 64 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 11: BIG BLOCK FULLY ASSOCIATIVE WA WB LRU"
./csim 1 8192 64 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 12: BIG BLOCK FULLY ASSOCIATIVE WA WB FIFO"
./csim 1 8192 64 write-allocate write-back fifo < gcc.trace
echo "----------------------"




echo "Test 13: LITTLE BLOCK DIRECT MAPPED NWA WT LRU"
./csim 32768 1 16 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 14: LITTLE BLOCK DIRECT MAPPED NWA WT FIFO"
./csim 32768 1 16 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 15: LITTLE BLOCK DIRECT MAPPED WA WB LRU"
./csim 32768 1 16 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 16: LITTLE BLOCK DIRECT MAPPED WA WB FIFO"
./csim 32768 1 16 write-allocate write-back fifo < gcc.trace
echo "----------------------"




echo "Test 17: LITTLE BLOCK SET ASSOCIATIVE NWA WT LRU"
./csim 256 128 16 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 18: LITTLE BLOCK SET ASSOCIATIVE NWA WT FIFO"
./csim 256 128 16 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 19: LITTLE BLOCK SET ASSOCIATIVE WA WB LRU"
./csim 256 128 16 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 20: LITTLE BLOCK SET ASSOCIATIVE WA WB FIFO"
./csim 256 128 16 write-allocate write-back fifo < gcc.trace
echo "----------------------"



echo "Test 21: LITTLE BLOCK FULLY ASSOCIATIVE NWA WT LRU"
./csim 1 32768 16 no-write-allocate write-through lru < gcc.trace
echo "----------------------"

echo "Test 22: LITTLE BLOCK FULLY ASSOCIATIVE NWA WT FIFO"
./csim 1 32768 16 no-write-allocate write-through fifo < gcc.trace
echo "----------------------"

echo "Test 23: LITTLE BLOCK FULLY ASSOCIATIVE WA WB LRU"
./csim 1 32768 16 write-allocate write-back lru < gcc.trace
echo "----------------------"

echo "Test 24: LITTLE BLOCK FULLY ASSOCIATIVE WA WB FIFO"
./csim 1 32768 16 write-allocate write-back fifo < gcc.trace
echo "----------------------"