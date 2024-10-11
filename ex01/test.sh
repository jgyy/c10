#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m'

run_make() {
    echo -e "${YELLOW}Running make clean...${NC}"
    make clean
    echo -e "${YELLOW}Running make...${NC}"
    make
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error: make command failed. Please check your Makefile and source code.${NC}"
        exit 1
    fi
    echo -e "${GREEN}Compilation successful.${NC}"
    echo ""
}

run_make

if [ ! -f "./ft_cat" ]; then
    echo -e "${RED}Error: ft_cat executable not found after compilation.${NC}"
    echo "Please check your Makefile and ensure it creates an executable named 'ft_cat'."
    exit 1
fi

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local expected_exit_code="$4"

    echo "Running test: $test_name"

    output=$(eval "$command" 2>&1)
    exit_code=$?

    if [ "$output" = "$expected_output" ]; then
        echo -e "${GREEN}Output matches expected${NC}"
    else
        echo -e "${RED}Output doesn't match expected${NC}"
        echo "Expected: $expected_output"
        echo "Got: $output"
    fi

    if [ $exit_code -eq $expected_exit_code ]; then
        echo -e "${GREEN}Exit code matches expected${NC}"
    else
        echo -e "${RED}Exit code doesn't match expected${NC}"
        echo "Expected: $expected_exit_code"
        echo "Got: $exit_code"
    fi

    if [ "$(printf '%s\n' "$output")" = "$(printf '%s\n' "$expected_output")" ]; then
        echo -e "${GREEN}Output matches expected${NC}"
    else
        echo -e "${RED}Output doesn't match expected${NC}"
        echo "Expected: $expected_output"
        echo "Got: $output"
    fi

    echo ""
}

echo -n "Hello, World!" > test1.txt
echo -n "This is a test file." > test2.txt
echo "" > empty.txt

run_test "Single file" "./ft_cat test1.txt" "Hello, World!" 0
run_test "Multiple files" "./ft_cat test1.txt test2.txt" "Hello, World!This is a test file." 0
run_test "Empty file" "./ft_cat empty.txt" "" 0
run_test "Non-existent file" "./ft_cat nonexistent.txt" "ft_cat: nonexistent.txt: No such file or directory" 1
run_test "Standard input" "echo 'Test input' | ./ft_cat" "Test input" 0
run_test "Dash argument" "echo 'Dash input' | ./ft_cat -" "Dash input" 0
run_test "Mixed files and stdin" "printf 'Stdin' | ./ft_cat test1.txt - test2.txt" "Hello, World!StdinThis is a test file." 0

rm test1.txt test2.txt empty.txt

echo "All tests completed."

echo -e "${YELLOW}Running make fclean...${NC}"
make fclean
echo -e "${GREEN}Cleanup completed.${NC}"
