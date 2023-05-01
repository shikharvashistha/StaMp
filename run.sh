#!/bin/bash

cd library && make all && sudo ldconfig && cd ..

cd examples && make all && cd ..

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/library

cd examples

# Run the examples
make run-fib
make run-matrix
make run-vector
