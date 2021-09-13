echo "##################################################################`nTesting Data`n##################################################################`n`n"
clang++ data.cpp -o data_test.exe -Wall -Wextra -pedantic -I../include && ./data_test
echo "`n##################################################################`nTesting Validation`n##################################################################`n`n"
clang++ validation.cpp -o validation_test.exe -Wall -Wextra -pedantic -I../include && ./validation_test
echo "`n##################################################################`nTesting JSON`n##################################################################`n`n"
clang++ json.cpp -o json_test.exe -Wall -Wextra -pedantic -I../include && ./json_test
