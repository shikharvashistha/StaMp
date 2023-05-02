# Parallel Programming using Pthreads

## 1. How to run the code

- Go to the libray folder and run the following command to compile the library

```make all```

- Run the following command to link the library in the same folder

```sudo ldconfig```

- Go to the examples folder and run the following command to compile the examples

```make all```

- Go to the working directory and run the following command to set the library path

```export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/library```

- To clean the library and examples folder run the following command

```make clean```

- How to run object files

```cd examples```
```make run-fib```
```make run-matrix```
```make run-vector```

<!--## 2. Shortcut to run the code

- Run ```chmod +x run.sh clean.sh``` to make the file executable

- Run ```run.sh``` file to perform all the operations(Install, compile, link, run)

- Run ```clean.sh``` file to clean the library and examples folder

#!/bin/bash

cd library && make clean && cd ..

cd examples && make clean && cd ..


#!/bin/bash

cd library && make all && sudo ldconfig && cd ..

cd examples && make all && cd ..

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/library

cd examples

# Run the examples
make run-fib
make run-matrix
make run-vector

-->