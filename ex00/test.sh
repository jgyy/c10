#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

make

run_test() {
    test_name="$1"
    command="$2"
    expected_output="$3"
    
    echo -n "Testing $test_name: "
    output=$(eval "$command" 2>&1)
    
    if [ "$output" = "$expected_output" ]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "Expected: $expected_output"
        echo "Got: $output"
    fi
    echo
}

echo "Hello, World!" > test1.txt
echo -n "No newline" > test2.txt
echo -e "Line 1\nLine 2\nLine 3" > test3.txt

run_test "No arguments" "./ft_display_file" "File name missing."
run_test "Too many arguments" "./ft_display_file file1 file2" "Too many arguments."
run_test "Non-existent file" "./ft_display_file nonexistent.txt" "Cannot read file."
run_test "Regular file" "./ft_display_file test1.txt" "Hello, World!"
run_test "File without newline" "./ft_display_file test2.txt" "No newline"
run_test "Multi-line file" "./ft_display_file test3.txt" "Line 1
Line 2
Line 3"

dd if=/dev/urandom of=large_file.txt bs=1M count=5 2>/dev/null
run_test "Large file (5MB)" "./ft_display_file large_file.txt | wc -c" "5242880"

rm test1.txt test2.txt test3.txt large_file.txt

make fclean
