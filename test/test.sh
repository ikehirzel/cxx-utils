#!/bin/sh
g++ obj.cpp -shared -fPIC -o obj.so
g++ main.cpp -o main -I../include -ldl -o main && ./main
