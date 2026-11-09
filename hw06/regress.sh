
#!/bin/sh

# ECE 361 - Homework 6
# Compare replay output against expected traces.

set -eu

# Create a temporary directory for actual outputs.
tmpdir=$(mktemp -d)

# Remove temporary files when the script exits.
trap 'rm -rf "$tmpdir"' EXIT

# Test both scenario files.
for name in hold_values toggle_sequence
do
    echo "Testing scenario: $name"

    # Run the scenario and save its actual output.
    if ! ./replay "scenarios/$name.txt" > "$tmpdir/$name.txt"; then
        echo "FAIL: $name (replay failed)"
        exit 1
    fi

    # Compare actual and expected output.
    if diff -u "expected/$name.txt" "$tmpdir/$name.txt"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name (output differs)"
        exit 1
    fi
done

echo "All regression tests passed."
