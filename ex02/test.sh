#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

echo -e "${YELLOW}Compiling ft_tail...${NC}"
make
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed. Please check your source code and Makefile.${NC}"
    exit 1
fi
echo -e "${GREEN}Compilation successful.${NC}"

if [ ! -x "./ft_tail" ]; then
    echo -e "${RED}Error: ft_tail executable not found or not executable after compilation.${NC}"
    echo "Please check your Makefile and ensure it produces an executable named 'ft_tail'."
    exit 1
fi

run_test() {
    test_name="$1"
    command="$2"
    expected_output="$3"

    echo -e "${YELLOW}Running test: $test_name${NC}"
    actual_output=$(eval "$command" 2>&1)

    expected_output=$(echo -n "$expected_output" | sed -z '$ s/\n$//')
    actual_output=$(echo -n "$actual_output" | sed -z '$ s/\n$//')
    
    if [ "$actual_output" = "$expected_output" ]; then
        echo -e "${GREEN}Test passed${NC}"
    else
        echo -e "${RED}Test failed${NC}"
        echo "Expected output:"
        echo "$expected_output"
        echo "Actual output:"
        echo "$actual_output"
    fi
    echo
}

echo "This is a test file." > test1.txt
echo -e "Line 1\nLine 2\nLine 3\nLine 4\nLine 5" > test2.txt
seq 1 100 > test3.txt

run_test "Basic usage" \
    "./ft_tail -c 10 test1.txt" \
    "test file."

run_test "Multiple files" \
    "./ft_tail -c 6 test1.txt test2.txt" \
    "==> test1.txt <==
file.
==> test2.txt <==
ine 5"

run_test "Stdin input" \
    "echo 'Hello, World!' | ./ft_tail -c 7" \
    "World!"

run_test "Large file" \
    "./ft_tail -c 15 test3.txt" \
    "98
99
100"

run_test "Entire file" \
    "./ft_tail -c 1000 test1.txt" \
    "This is a test file."

run_test "Zero bytes" \
    "./ft_tail -c 0 test1.txt" \
    ""

run_test "No -c option" \
    "./ft_tail test2.txt" \
    "Line 1
Line 2
Line 3
Line 4
Line 5"

run_test "Invalid file" \
    "./ft_tail -c 10 nonexistent.txt" \
    "ft_tail: nonexistent.txt: No such file or directory"

run_test "Invalid -c value" \
    "./ft_tail -c -5 test1.txt" \
    "ft_tail: invalid number of bytes: '-5'"

rm test1.txt test2.txt test3.txt

echo -e "${GREEN}All tests completed.${NC}"
