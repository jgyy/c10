#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

run_test() {
    local test_name="$1"
    local command="$2"
    local max_lines="${3:-100}"
    local is_hexdump_command="${4:-false}"

    echo "Running test: $test_name"

    if [ "$is_hexdump_command" = "true" ]; then
        if diff <(./ft_hexdump $command | head -n $max_lines) <(hexdump -C $command | head -n $max_lines) > /dev/null; then
            echo -e "${GREEN}Test passed${NC}"
        else
            echo -e "${RED}Test failed${NC}"
            echo "Expected (first $max_lines lines):"
            hexdump -C $command | head -n $max_lines
            echo "Got (first $max_lines lines):"
            ./ft_hexdump $command | head -n $max_lines
        fi
    else
        if diff <($command | ./ft_hexdump | head -n $max_lines) <($command | hexdump -C | head -n $max_lines) > /dev/null; then
            echo -e "${GREEN}Test passed${NC}"
        else
            echo -e "${RED}Test failed${NC}"
            echo "Expected (first $max_lines lines):"
            $command | hexdump -C | head -n $max_lines
            echo "Got (first $max_lines lines):"
            $command | ./ft_hexdump | head -n $max_lines
        fi
    fi
    echo
}

make

run_test "Empty input" "echo -n"

run_test "Single line of text" "echo 'Hello, World!'"

run_test "Multiple lines of text" "echo -e 'Line 1\nLine 2\nLine 3'"

echo -n "This is a test of binary data." > test_binary_data
run_test "Binary data" "cat test_binary_data" 10
rm test_binary_data

yes "Pattern" | head -c 1M > test_large_file
run_test "Large file (1MB, first 10 lines)" "cat test_large_file" 10
rm test_large_file

echo -n "abcdefghijklmnop" > test_repeat_file
run_test "File with repeated content" "cat test_repeat_file"
rm test_repeat_file

for i in {0..255}; do printf "\\$(printf '%03o' $i)"; done > test_ascii_file
run_test "File with all ASCII characters" "cat test_ascii_file" 20
rm test_ascii_file

echo "File 1 content" > test_file1
echo "File 2 content" > test_file2
run_test "Multiple files" "test_file1 test_file2" 100 true
rm test_file1 test_file2

run_test "Non-existent file" "./ft_hexdump non_existent_file" 5

echo "Existing file content" > test_existing_file
run_test "Mix of existent and non-existent files" "test_existing_file non_existent_file" 10 true
rm test_existing_file

echo "All tests completed."
