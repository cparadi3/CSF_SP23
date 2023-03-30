#!/bin/bash

echo "Test 1: "
./csim 1 1 4 no-write-allocate write-through lru < gcc.trace
echo "----------------------"
echo "Test 2:"
