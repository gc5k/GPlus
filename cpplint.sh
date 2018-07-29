#!/bin/bash
#
# Check whether the source files follow Google C++ Coding Style.

if [[ $# -gt 0 ]]; then
  files=$@
else
  files=$(find . -path ./third_party -prune -printf '' -o \
    -name "*.h" -print -o -name "*.cpp" -print)
fi

for file in ${files}; do
  python cpplint.py --root=.. ${file}
done
