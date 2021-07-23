#!/bin/bash
g++ main.cpp impl.cpp other.cpp -I../include -I../lib/cxx-data/include -o main -std=c++17 -pedantic && ./main
