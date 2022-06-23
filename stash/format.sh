#!/bin/bash

grep -Ril --include=\*.{cc,cpp,hpp} "" {ulib,benchmarks,unit-tests,examples} | xargs -L 1 clang-format -i

clang-format -i main.cc

black -q .
