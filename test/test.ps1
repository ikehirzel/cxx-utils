clang++ obj.cpp -shared -o obj.dll
clang++ main.cpp impl.cpp -o main.exe -pedantic -I../include && ./main
