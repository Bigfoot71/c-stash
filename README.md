# c-stash - Lightweight C Containers

**Stash** is a lightweight C library that provides basic data structures such as **dynamic arrays**, **hash maps** with `uint32_t` keys, and a **registry** with unique IDs. It allows for generic manipulation of these structures using `void*` pointers.

## Features

* **`stash_arr`**: Generic dynamic array.

  * Insert, remove, and resize elements.
  * Supports iteration and manipulation of elements.

* **`stash_umap`**: Hash map with `uint32_t` keys.

  * Insert, retrieve, and remove elements.
  * Check for element presence and count elements.

* **`stash_reg`**: Element registry with unique IDs.

  * Manage IDs and store elements.
  * Retrieve, remove, or add elements by their ID.

## Allocation and Deallocation

Stash allows you to customize **memory allocators** by defining the following macros before including the header file:

```c
#define STASH_MALLOC(sz)   malloc(sz)
#define STASH_REALLOC(nb, sz) realloc(nb, sz)
#define STASH_FREE(mem)    free(mem)
```

If these are not defined, the standard `malloc()`, `realloc()`, and `free()` functions will be used.

## Usage Examples

### Creating a Dynamic Array

```c
stash_arr arr = stash_arr_create(10, sizeof(int));
int value = 42;
stash_arr_push_back(&arr, &value);
```

### Creating a Hash Map

```c
stash_umap map = stash_umap_create(10, sizeof(int));
int value = 42;
stash_umap_insert(&map, 1234, &value);
```

### Creating an Element Registry

```c
stash_reg reg = stash_reg_create(10, sizeof(int));
int value = 42;
uint32_t id = stash_reg_push(&reg, &value);
```

## API

The library exposes functions to manage each container (array, hashmap, registry) and perform operations like insertion, removal, iteration, and memory management.

* `stash_arr_create()` : Creates a dynamic array.
* `stash_umap_create()` : Creates a hash map.
* `stash_reg_create()` : Creates an element registry.

See the `stash.h` header file for the full list of functions.

## Compilation

To compile and use this library, simply include the `stash.h` header file in your C project and define `STASH_IMPL` **at least once** before including the header to enable the implementation.

## License

This project is distributed under the MIT License. See the `LICENSE` file for more information.
