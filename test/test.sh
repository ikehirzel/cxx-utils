#!/bin/bash
g++ obj.cpp -shared -fPIC -o obj.so
g++ main.cpp impl.cpp -o main -Wall -Wextra -std=c++11 -pedantic -I../include -ldl -o main && ./main
