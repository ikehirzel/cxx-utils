clang++ obj.cpp -shared -o obj.dll
clang++ main.cpp impl.cpp -o main.exe -Wall -Wextra -std=c++11 -pedantic -I../include && ./main
