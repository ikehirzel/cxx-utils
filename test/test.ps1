clang++ obj.cpp -shared -o obj.dll
clang++ main.cpp -o main.exe -I../include && ./main
