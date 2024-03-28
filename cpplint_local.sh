#!/bin/sh

CPPLINT_PATH="cpplint.py"
# Run cpplint.py on all .cpp and .hpp files.
python3 $CPPLINT_PATH \
    --verbose=3 \
    --exclude=Team20/Code20/cmake-build-debug \
    --exclude=Team20/Code20/cmake \
    --exclude=Team20/Code20/.idea \
    --exclude=Team20/Code20/external/ \
    --exclude=Team20/Code20/lib/ \
    --exclude=Team20/Code20/src/autotester \
    --exclude=Team20/Code20/src/autotester_gui \
    --recursive \
    --filter=-legal/copyright,-whitespace/tab,-readability/braces,-whitespace/ending_newline,-readability/namespace,-build/header_guard,-build/storage_class,-build/c++11,-whitespace/blank_line,-readability/todo \
    --extensions=c,h,cpp ./
