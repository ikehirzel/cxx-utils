#!/bin/bash
g++ main.cpp impl.cpp  -o main -Wall -Wextra -std=c++11 -pedantic -I../include && ./main
