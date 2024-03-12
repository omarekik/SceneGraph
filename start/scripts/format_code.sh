#!/bin/bash

# Navigate to the project root directory before running the script
project_root="$(git rev-parse --show-toplevel)"
cd "$project_root" || exit

# clang format all source and header files
find ./start/ -iname *.h -o -iname *.cpp -o -iname *.hpp | xargs clang-format -i

# Check if any changes were made by clang-format
if git diff -- '*.h' '*.cpp' '*.hpp' --exit-code; then
  echo "Formatting is ok, no changes applied"
else
  echo "Formatting is wrong, changes have been applied. Re-stage modified files"
  exit 1
fi
