# DataStructrues
Implementations of common data-structures :
- bit-array
- vector
- stack
- queue
- linked-list
- doubly linked-list
- sorted-list
- hash table
- binary sorted tree (iterative solution)
- binary sorted tree (recursive solution)
- heap
- priority queue (based on doubly linked list)
- priority queue (based on heap)


Data structures are generic, and compatible with all primitive data-types.<br>
Further information can be found in each container API see `./include`

> All code is written in ANSI C89 



## Usage
In order to use one of the containers download both files:
`./src/ds_name.c` and `./include/ds_name.h`.

Compile with the following line to create an object file

* Production mode
```bash
    gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -c src/ds_name.c
```

* Debug mode
```bash
    gcc -ansi -pedantic-errors -Wall -Wextra -g -D_DEBUG -c src/ds_name.c
```



## Tests
Tests were built for each container. See ./test directory for more information.
