# printf "##################################################################\nTesting Data\n##################################################################\n\n"
# g++ data.cpp impl.cpp -o data_test -Wall -Wextra -std=c++17 -pedantic -I../include && ./data_test
printf "\n##################################################################\nTesting Validation\n##################################################################\n\n"
g++ validation.cpp -o validation_test -Wall -Wextra -std=c++17 -pedantic -I../include && ./validation_test
# printf "\n##################################################################\nTesting JSON\n##################################################################\n\n"
# g++ json.cpp impl.cpp -o json_test -Wall -Wextra -std=c++17 -pedantic -I../include && ./json_test
