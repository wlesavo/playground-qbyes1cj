#!/bin/sh

make -O3 -inline -omit-frame-pointer -unroll-loops -unsafe-math-optimizations -no-trapping-math && ./great_escape/great_escape
