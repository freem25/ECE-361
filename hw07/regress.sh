
#!/bin/sh

# Homework 7 - Bracket Checker Regression Tests
#
# Runs each input file through the balance program.
# Compares actual output and exit status against expected results.
# Prints PASS or FAIL for each test and a final summary.

set -u

# Counters for successful and failed tests.
tests_run=0
tests_failed=0

# Create a temporary file to store actual program output.
actual_output=$(mktemp) || exit 1

# Remove the temporary file when the script exits.
trap 'rm -f "$actual_output"' 0
trap 'exit 1' 1 2 3 15

# Run every test input in the inputs directory.
for input in inputs/*.txt; do

    # Extract the test name without its directory or extension.
    name=${input##*/}
    name=${name%.txt}

    # Locate the expected output and exit status.
    expected_output="expected/$name.out"
    expected_status="expected/$name.status"

    tests_run=$((tests_run + 1))

    # Verify that both expected-result files exist.
    if [ ! -f "$expected_output" ] ||
       [ ! -f "$expected_status" ]; then
        echo "FAIL: $name (missing expected result)"
        tests_failed=$((tests_failed + 1))
        continue
    fi

    # Run the bracket checker and capture its output.
    ./balance < "$input" > "$actual_output"
    actual_status=$?

    # Read the expected exit status.
    expected_code=$(cat "$expected_status")

    # Compare the output and exit status.
    if diff -u "$expected_output" "$actual_output" > /dev/null &&
       [ "$actual_status" -eq "$expected_code" ]; then

        echo "PASS: $name"

    else
        echo "FAIL: $name"

        # Display output differences if they exist.
        diff -u "$expected_output" "$actual_output" || true

        # Display the expected and actual exit statuses.
        echo "Expected exit status: $expected_code"
        echo "Actual exit status:   $actual_status"

        tests_failed=$((tests_failed + 1))
    fi
done

# Print the final test summary.
echo ""
echo "Regression tests run: $tests_run"
echo "Regression tests passed: $((tests_run - tests_failed))"
echo "Regression tests failed: $tests_failed"

# Return a nonzero exit status if any test failed.
if [ "$tests_failed" -ne 0 ]; then
    exit 1
fi

exit 0
