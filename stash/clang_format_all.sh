#!/bin/bash

grep -Ril --include=\*.{cpp,hpp} "" {ulib,benchmarks,unit-tests} | xargs -L 1 clang-format -i

clang-format -i main.cpp
