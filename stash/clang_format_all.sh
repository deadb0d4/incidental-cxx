#!/bin/bash

grep -Ril --include=\*.{cpp,hpp} "" . | xargs -L 1 clang-format -i
