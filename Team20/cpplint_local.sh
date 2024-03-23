#!/bin/sh

CPPLINT_PATH="../cpplint.py"
# Run cpplint.py on all .cpp and .hpp files.
python $CPPLINT_PATH \
    --verbose=3 \
    --exclude=Code20/cmake-build-debug \
    --exclude=Code20/cmake \
    --exclude=Code20/.idea \
    --exclude=Code20/external/ \
    --exclude=Code20/lib/ \
    --exclude=Code20/src/autotester \
    --exclude=Code20/src/autotester_gui \
    --recursive \
    --filter=-legal/copyright,-whitespace/tab,-readability/braces,-whitespace/ending_newline,-readability/namespace,-build/header_guard,-build/storage_class,-build/c++11,-whitespace/blank_line,-readability/todo \
    --extensions=c,h,cpp ./