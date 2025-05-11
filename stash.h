/*
 * MIT License
 * 
 * Copyright (c) 2025 Le Juez Victor
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STASH_H
#define STASH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* === Allocators === */

#ifndef STASH_MALLOC
#   define STASH_MALLOC(sz) malloc(sz)
#endif

#ifndef STASH_REALLOC
#   define STASH_REALLOC(nb, sz) realloc(nb, sz)
#endif

#ifndef STASH_FREE
#   define STASH_FREE(mem) free(mem)
#endif

/* === Common Things === */

enum {
    STASH_ERROR_KEY_NOT_FOUND    = -3,
    STASH_ERROR_OUT_OF_BOUNDS    = -2,
    STASH_ERROR_OUT_OF_MEMORY    = -1,
    STASH_SUCCESS                = 0,
    STASH_EMPTY                  = 1,
    STASH_KEY_EXISTS             = 2
};

typedef struct {
    void* curr;
    void* prev;
    void* next;
} stash_it;

/* === Containers Types === */

typedef struct {
    void* data;             // Pointer to array elements
    size_t count;           // Number of elements currently in the array
    size_t capacity;        // Total array capacity (allocated space)
    size_t elem_size;        // Size of an element (in bytes)
} stash_arr;

typedef struct {
    uint32_t key;           // ID Key
    void* value;            // Pointer to the value
    bool occupied;          // Indicates if the entry is occupied
} stash_umap_entry;

typedef struct {
    stash_arr buckets;        // Bucket array
    size_t count;           // Number of elements in the table
    size_t value_size;       // Size of stored values
} stash_umap;

typedef struct {
    stash_arr elements;       // Store the objects directly (contiguous)
    stash_arr valid_flags;     // Store if an ID is valid (Boolean array)
    stash_arr free_ids;        // List of liberated IDs
    uint32_t next_id;        // Next ID to be assigned
    size_t elem_size;        // Size of an element
} stash_reg;


#ifdef __cplusplus
extern "C" {
#endif

/* === Array Functions === */

stash_arr stash_arr_create(size_t capacity, size_t elem_size);
void stash_arr_destroy(stash_arr* array);
stash_arr stash_arr_copy(const stash_arr* src);
bool stash_arr_is_valid(const stash_arr* array);
bool stash_arr_is_empty(const stash_arr* array);
int stash_arr_reserve(stash_arr* array, size_t newCapacity);
int stash_arr_shrink_to_fit(stash_arr* array);
void stash_arr_clear(stash_arr* array);
int stash_arr_resize(stash_arr* array, size_t size, const void* element);
void stash_arr_fill(stash_arr* array, const void* data);
int stash_arr_insert(stash_arr* array, size_t index, const void* elements, size_t count);
stash_it stash_arr_begin(stash_arr* array);
void stash_arr_previous(stash_arr* array, stash_it* it);
void stash_arr_next(stash_arr* array, stash_it* it);
stash_it stash_arr_end(stash_arr* array);
void* stash_arr_back(stash_arr* array);
void* stash_arr_front(stash_arr* array);
void* stash_arr_at(stash_arr* array, size_t index);
int stash_arr_push_back(stash_arr* array, const void* element);
int stash_arr_push_front(stash_arr* array, const void* element);
int stash_arr_push_at(stash_arr* array, size_t index, const void* element);
int stash_arr_pop_back(stash_arr* array, void* element);
int stash_arr_pop_front(stash_arr* array, void* element);
int stash_arr_pop_at(stash_arr* array, size_t index, void* element);
bool stash_arr_compare(const stash_arr* a, const stash_arr* b);

/* === Table Container === */

stash_umap stash_umap_create(size_t initialCapacity, size_t value_size);
int stash_umap_reserve(stash_umap* table, size_t newCapacity);
void stash_umap_destroy(stash_umap* table);
bool stash_umap_is_valid(const stash_umap* table);
bool stash_umap_is_empty(const stash_umap* table);
stash_it stash_umap_begin(stash_umap* array);
void stash_umap_previous(stash_umap* array, stash_it* it);
void stash_umap_next(stash_umap* array, stash_it* it);
stash_it stash_umap_end(stash_umap* array);
int stash_umap_insert(stash_umap* table, uint32_t key, const void* value);
int stash_umap_remove(stash_umap* table, uint32_t key, void* element);
int stash_umap_get(const stash_umap* table, uint32_t key, void* element);
bool stash_umap_contains(const stash_umap* table, uint32_t key);
void stash_umap_clear(stash_umap* table);
size_t stash_umap_count(const stash_umap* table);

/* === Registry Container === */

stash_reg stash_reg_create(size_t capacity, size_t elem_size);
void stash_reg_destroy(stash_reg* reg);
bool stash_reg_is_valid(const stash_reg* reg);
bool stash_reg_exists(const stash_reg* reg, uint32_t id);
stash_it stash_reg_begin(stash_reg* reg);
void stash_reg_previous(stash_reg* reg, stash_it* it);
void stash_reg_next(stash_reg* reg, stash_it* it);
stash_it stash_reg_end(stash_reg* reg);
uint32_t stash_reg_push(stash_reg* reg, const void* element);
bool stash_reg_pop(stash_reg* reg, uint32_t id, void* element);
void* stash_reg_get(stash_reg* reg, uint32_t id);
uint32_t stash_reg_get_alloc_count(const stash_reg* reg);

#ifdef __cplusplus
}
#endif

#endif // STASH_H

#ifdef STASH_IMPL

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* === Utils Functions === */

static inline int64_t u_stash_next_po2_u64(int64_t x)
{
    if (x == 0) return 1;
    if ((x & (x - 1)) == 0) return x << 1; // (x * 2)
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32; //< Only for 64 bits
    x++;
    return x;
}

/* === Public Array Implementation === */

stash_arr stash_arr_create(size_t capacity, size_t elem_size)
{
    stash_arr array = { 0 };

    if (capacity == 0 || elem_size == 0) {
        return array;
    }

    void* data = STASH_MALLOC(capacity * elem_size);
    if (!data) return array;

    array.data = data;
    array.capacity = capacity;
    array.elem_size = elem_size;

    return array;
}

void stash_arr_destroy(stash_arr* array)
{
    if (array->data) {
        STASH_FREE(array->data);
        array->data = NULL;
    }
    array->count = 0;
    array->capacity = 0;
    array->elem_size = 0;
}

stash_arr stash_arr_copy(const stash_arr* src)
{
    stash_arr array = { 0 };

    size_t size = src->count * src->elem_size;
    if (size == 0) return array;

    array.data = STASH_MALLOC(size);
    if (array.data == NULL) return array;

    memcpy(array.data, src->data, size);

    array.count = src->count;
    array.capacity = src->count;
    array.elem_size = src->elem_size;

    return array;
}

bool stash_arr_is_valid(const stash_arr* array)
{
    return array->data != NULL
        && array->capacity > 0
        && array->elem_size > 0;
}

bool stash_arr_is_empty(const stash_arr* array)
{
    return array->count == 0;
}

int stash_arr_reserve(stash_arr* array, size_t newCapacity)
{
    if (array->capacity >= newCapacity) {
        return STASH_SUCCESS;
    }

    void* new_data = STASH_REALLOC(array->data, newCapacity * array->elem_size);
    if (!new_data) return STASH_ERROR_OUT_OF_MEMORY;

    array->data = new_data;
    array->capacity = newCapacity;

    return STASH_SUCCESS;
}

int stash_arr_shrink_to_fit(stash_arr* array)
{
    if (array->count == array->capacity) {
        return 1;
    }

    if (array->count == 0) {
        return STASH_EMPTY;
    }

    void* new_data = STASH_REALLOC(array->data, array->count * array->elem_size);
    if (!new_data) return STASH_ERROR_OUT_OF_MEMORY;

    array->data = new_data;
    array->capacity = array->count;

    return STASH_SUCCESS;
}

void stash_arr_clear(stash_arr* array)
{
    array->count = 0;
}

int stash_arr_resize(stash_arr* array, size_t size, const void* element)
{
    if (size <= array->count) {
        array->count = size;
        return STASH_SUCCESS;
    }

    if (size >= array->capacity) {
        int ret = stash_arr_reserve(array, size);
        if (ret < 0) return ret;
    }

    if (element != NULL) {
        for (size_t i = array->count; i < size; i++) {
            memcpy((char*)array->data + i * array->elem_size, element, array->elem_size);
        }
    }
    else {
        memset((char*)array->data + array->count * array->elem_size, 0, (size - array->count) * array->elem_size);
    }

    array->count = size;

    return STASH_SUCCESS;
}

void stash_arr_fill(stash_arr* array, const void* data)
{
    const void* end = (char*)array->data + array->capacity * array->elem_size;
    for (char* ptr = (char*)array->data; (void*)ptr < end; ptr += array->elem_size) {
        memcpy(ptr, data, array->elem_size);
    }
    array->count = array->capacity;
}

int stash_arr_insert(stash_arr* array, size_t index, const void* elements, size_t count)
{
    if (index > array->count) {
        return STASH_ERROR_OUT_OF_BOUNDS;
    }

    size_t newSize = array->count + count;

    if (newSize > array->capacity) {
        newSize = u_stash_next_po2_u64(newSize);
        int ret = stash_arr_reserve(array, newSize);
        if (ret < 0) return ret;
    }

    // Moving items to make room for new items
    void* destination = (char*)array->data + (index + count) * array->elem_size;
    void* source = (char*)array->data + index * array->elem_size;
    size_t bytesToMove = (array->count - index) * array->elem_size;
    memmove(destination, source, bytesToMove);

    // Inserting new elements
    void* target = (char*)array->data + index * array->elem_size;
    memcpy(target, elements, count * array->elem_size);

    // Updating array count
    array->count = newSize;

    return STASH_SUCCESS;
}

stash_it stash_arr_begin(stash_arr* array)
{
    stash_it it = { 0 };

    if (array == NULL || array->count == 0) {
        return it;
    }

    it.curr = array->data;

    if (array->count > 1) {
        it.next = (char*)it.curr + array->elem_size;
    }

    return it;
}

void stash_arr_previous(stash_arr* array, stash_it* it)
{
    if (array == NULL || it == NULL) {
        return;
    }

    if (it->prev == NULL) {
        it->next = (array->count > 0) ? array->data : NULL;
        it->curr = NULL;
    }

    void* next = it->curr;
    it->curr = it->prev;
    it->next = next;

    it->prev = (char*)it->curr - array->elem_size;
    if (it->prev < array->data) {
        it->prev = NULL;
    }
}

void stash_arr_next(stash_arr* array, stash_it* it)
{
    if (array == NULL || it == NULL) {
        return;
    }

    if (it->next == NULL) {
        it->prev = it->curr;
        it->curr = NULL;
    }

    void* prev = it->curr;
    it->curr = it->next;
    it->prev = prev;

    it->next = (char*)it->curr + array->elem_size;
    if ((char*)it->next >= (char*)array->data + array->count * array->elem_size) {
        it->next = NULL;
    }
}

stash_it stash_arr_end(stash_arr* array)
{
    stash_it it = { 0 };

    if (array == NULL || array->count == 0) {
        return it;
    }

    it.curr = (char*)array->data + (array->count - 1) * array->elem_size;

    if (array->count == 1) {
        return it;
    }

    it.prev = (char*)it.curr - array->elem_size;

    return it;
}

void* stash_arr_back(stash_arr* array)
{
    return (char*)array->data + (array->count - 1) * array->elem_size;
}

void* stash_arr_front(stash_arr* array)
{
    return array->data;
}

void* stash_arr_at(stash_arr* array, size_t index)
{
    if (index >= array->count) return NULL;
    return (char*)array->data + index * array->elem_size;
}

int stash_arr_push_back(stash_arr* array, const void* element)
{
    if (array->count >= array->capacity) {
        size_t newSize = u_stash_next_po2_u64(array->count + 1);
        int ret = stash_arr_reserve(array, newSize);
        if (ret < 0) return ret;
    }

    void* target = (char*)array->data + array->count * array->elem_size;
    if (element) memcpy(target, element, array->elem_size);
    else memset(target, 0, array->elem_size);
    array->count++;

    return STASH_SUCCESS;
}

int stash_arr_push_front(stash_arr* array, const void* element)
{
    if (array->count >= array->capacity) {
        size_t newSize = u_stash_next_po2_u64(array->count + 1);
        int ret = stash_arr_reserve(array, newSize);
        if (ret < 0) return ret;
    }

    // Move all existing items to the right to make room
    void* destination = (char*)array->data + array->elem_size;
    void* source = array->data;
    size_t bytesToMove = array->count * array->elem_size;
    memmove(destination, source, bytesToMove);

    // Copy new item to start or fill with zeroes
    if (element) memcpy(array->data, element, array->elem_size);
    else memset(array->data, 0, array->elem_size);

    // Increment count
    array->count++;

    return STASH_SUCCESS;
}

int stash_arr_push_at(stash_arr* array, size_t index, const void* element)
{
    if (index >= array->count) {
        return STASH_ERROR_OUT_OF_BOUNDS;
    }

    if (array->count >= array->capacity) {
        size_t newSize = u_stash_next_po2_u64(array->count + 1);
        int ret = stash_arr_reserve(array, newSize);
        if (ret < 0) return ret;
    }

    // Move existing items from index to make room
    void* destination = (char*)array->data + index * array->elem_size;
    void* source = (char*)array->data + index * array->elem_size;
    size_t bytesToMove = (array->count - index) * array->elem_size;
    memmove(destination, source, bytesToMove);

    // Copy new item to destination or fill with zeroes
    if (element) memcpy(source, element, array->elem_size);
    else memset(source, 0, array->elem_size);

    // Increment count
    array->count++;

    return STASH_SUCCESS;
}

int stash_arr_pop_back(stash_arr* array, void* element)
{
    if (array->count == 0) {
        return STASH_EMPTY;
    }

    array->count--;
    if (element != NULL) {
        void* source = (char*)array->data + array->count * array->elem_size;
        memcpy(element, source, array->elem_size);
    }

    return STASH_SUCCESS;
}

int stash_arr_pop_front(stash_arr* array, void* element)
{
    if (array->count == 0) {
        return STASH_EMPTY;
    }

    if (element != NULL) {
        memcpy(element, array->data, array->elem_size);
    }

    // Move all remaining items to the left
    void* source = (char*)array->data + array->elem_size;
    void* destination = array->data;
    size_t bytesToMove = (array->count - 1) * array->elem_size;
    memmove(destination, source, bytesToMove);

    // Reduce array count
    array->count--;

    return STASH_SUCCESS;
}

int stash_arr_pop_at(stash_arr* array, size_t index, void* element)
{
    if (index >= array->count) {
        return STASH_ERROR_OUT_OF_BOUNDS;
    }

    if (element != NULL) {
        void* source = (char*)array->data + index * array->elem_size;
        memcpy(element, source, array->elem_size);
    }

    // Move the remaining items to the left to fill the hole
    void* destination = (char*)array->data + index * array->elem_size;
    void* sourceStart = (char*)array->data + (index + 1) * array->elem_size;
    size_t bytesToMove = (array->count - index - 1) * array->elem_size;

    memmove(destination, sourceStart, bytesToMove);

    // Reduce array count
    array->count--;

    return STASH_SUCCESS;
}

bool stash_arr_compare(const stash_arr* a, const stash_arr* b)
{
    if (a->count != b->count || a->elem_size != b->elem_size) {
        return false;
    }

    return !memcmp(a->data, b->data, a->count * a->elem_size);
}

/* === Private Table Implementation === */

static inline size_t u_stash_umap_hash_u32(uint32_t key, size_t capacity)
{
    // Murmur3 algorithm

    key ^= key >> 16;
    key *= 0x85ebca6b;
    key ^= key >> 13;
    key *= 0xc2b2ae35;
    key ^= key >> 16;

    return key % capacity;
}

static int64_t u_stash_find_entry_index(const stash_umap* table, uint32_t key)
{
    if (!stash_umap_is_valid(table) || table->buckets.count == 0) {
        return -1;
    }

    size_t capacity = table->buckets.count;
    size_t index = u_stash_umap_hash_u32(key, capacity);
    size_t original_index = index;

    // Linear probing collision resolution
    do {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, index);
        if (entry && entry->occupied && entry->key == key) {
            return (int64_t)index;
        }

        if (entry && !entry->occupied) {
            // Free slot found, key does not exist
            return -1;
        }

        // Move to the next bucket (linear polling)
        index = (index + 1) % capacity;
    } while (index != original_index);

    // Full table and key not found
    return -1;
}

static int u_stash_find_free_slot(const stash_umap* table, uint32_t key)
{
    if (!stash_umap_is_valid(table) || table->buckets.count == 0) {
        return -1;
    }

    size_t capacity = table->buckets.count;
    size_t index = u_stash_umap_hash_u32(key, capacity);
    size_t original_index = index;

    // Searching for the free location using linear probing
    do {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, index);
        if (entry) {
            if (!entry->occupied) {
                return (int)index;
            }
            if (entry->key == key) {
                // The key already exists
                return -2;
            }
        }

        // Move to the next bucket
        index = (index + 1) % capacity;
    } while (index != original_index);

    // Table pleine
    return -1;
}

/* === Public Table Implementation === */

stash_umap stash_umap_create(size_t initialCapacity, size_t value_size)
{
    stash_umap table;
    table.count = 0;
    table.value_size = value_size;

    size_t actual_capacity = initialCapacity > 0 ? initialCapacity : 16;
    table.buckets = stash_arr_create(actual_capacity, sizeof(stash_umap_entry));

    if (stash_arr_is_valid(&table.buckets)) {
        stash_umap_entry default_entry = { 0 };
        stash_arr_resize(&table.buckets, initialCapacity, &default_entry);
    }

    return table;
}

int stash_umap_reserve(stash_umap* table, size_t newCapacity)
{
    if (!stash_umap_is_valid(table) || newCapacity < table->count) {
        return STASH_ERROR_OUT_OF_MEMORY;
    }
    stash_arr_reserve(&table->buckets, newCapacity);
    return STASH_SUCCESS;
}

void stash_umap_destroy(stash_umap* table)
{
    if (!stash_umap_is_valid(table)) {
        return;
    }

    for (size_t i = 0; i < table->buckets.count; i++) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at(&table->buckets, i);
        if (entry && entry->occupied && entry->value) {
            STASH_FREE(entry->value);
        }
    }

    stash_arr_destroy(&table->buckets);

    table->count = 0;
    table->value_size = 0;
}

bool stash_umap_is_valid(const stash_umap* table)
{
    return table && stash_arr_is_valid(&table->buckets);
}

bool stash_umap_is_empty(const stash_umap* table)
{
    return !stash_umap_is_valid(table) || table->count == 0;
}

stash_it stash_umap_begin(stash_umap* table)
{
    stash_it it = { 0 };

    if (!stash_umap_is_valid(table) || table->count == 0) {
        return it;
    }

    // Find the first occupied location
    for (size_t i = 0; i < table->buckets.count; i++) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, i);
        if (entry && entry->occupied) {
            it.curr = entry->value;

            // Store the current index for navigation
            it.prev = (void*)(uintptr_t)i;

            // Find the next occupied element
            size_t next_idx = i + 1;
            while (next_idx < table->buckets.count) {
                stash_umap_entry* nextEntry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, next_idx);
                if (nextEntry && nextEntry->occupied) {
                    it.next = nextEntry->value;
                    break;
                }
                next_idx++;
            }

            // If we haven't found a next one, there isn't one.
            if (next_idx >= table->buckets.count) {
                it.next = NULL;
            }

            break;
        }
    }

    return it;
}

void stash_umap_previous(stash_umap* table, stash_it* it)
{
    if (!stash_umap_is_valid(table) || !it || table->count == 0) {
        return;
    }

    // If we are already at the beginning or before
    if (it->curr == NULL) {
        // If next is set, we are just before the start
        if (it->next != NULL) {
            // We stay before the start
            return;
        }

        // Otherwise we go to the end
        *it = stash_umap_end(table);
        return;
    }

    // Retrieve the current index
    size_t curr_idx = (size_t)(uintptr_t)it->prev;

    // Find the previous occupied element
    size_t prev_idx = curr_idx;
    if (prev_idx > 0) {
        prev_idx--;

        while (prev_idx > 0) {
            stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, prev_idx);
            if (entry && entry->occupied) {
                break;
            }
            prev_idx--;
        }

        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, prev_idx);
        if (entry && entry->occupied) {
            // Save the current pointer
            void* oldCurr = it->curr;

            // Update the iterator
            it->curr = entry->value;
            it->next = oldCurr;
            it->prev = (void*)(uintptr_t)prev_idx;
        }
        else {
            // We are before the beginning
            it->next = it->curr;
            it->curr = NULL;
            it->prev = NULL;
        }
    }
    else {
        // We are before the beginning
        it->next = it->curr;
        it->curr = NULL;
        it->prev = NULL;
    }
}

void stash_umap_next(stash_umap* table, stash_it* it)
{
    if (!stash_umap_is_valid(table) || !it || table->count == 0) {
        return;
    }

    // If we are already at the end or after
    if (it->curr == NULL) {
        // If prev is set, we are just after the end
        if (it->prev != NULL) {
            // We stay after the end
            return;
        }

        // Otherwise, we go to the beginning
        *it = stash_umap_begin(table);
        return;
    }

    // If next is NULL, we are at the end
    if (it->next == NULL) {
        // Save current pointer to prev (to allow going back)
        void* oldCurr = it->curr;
        it->prev = oldCurr;
        it->curr = NULL;
        return;
    }

    // Retrieve the current index
    size_t curr_idx = (size_t)(uintptr_t)it->prev;

    // Find the next occupied element
    size_t next_idx = curr_idx + 1;
    while (next_idx < table->buckets.count) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, next_idx);
        if (entry && entry->occupied) {
            break;
        }
        next_idx++;
    }

    if (next_idx < table->buckets.count) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, next_idx);

        // Update the iterator
        it->curr = entry->value;
        it->prev = (void*)(uintptr_t)next_idx;

        // Find the next occupied element
        size_t next_next_idx = next_idx + 1;
        while (next_next_idx < table->buckets.count) {
            stash_umap_entry* nextEntry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, next_next_idx);
            if (nextEntry && nextEntry->occupied) {
                it->next = nextEntry->value;
                break;
            }
            next_next_idx++;
        }

        // If we haven't found a next one, there isn't one
        if (next_next_idx >= table->buckets.count) {
            it->next = NULL;
        }
    }
    else {
        // We are at the end
        void* oldCurr = it->curr;
        it->prev = oldCurr;
        it->curr = NULL;
        it->next = NULL;
    }
}

stash_it stash_umap_end(stash_umap* table)
{
    stash_it it = { 0 };

    if (!stash_umap_is_valid(table) || table->count == 0) {
        return it;
    }

    // Find the last occupied location
    int64_t i = table->buckets.count - 1;
    while (i >= 0) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, (size_t)i);
        if (entry && entry->occupied) {
            it.curr = entry->value;
            it.prev = (void*)(uintptr_t)i;

            // For consistency with the rest of the API
            // next is NULL for the last element
            it.next = NULL;
            break;
        }
        i--;
    }

    return it;
}

int stash_umap_insert(stash_umap* table, uint32_t key, const void* value)
{
    if (!stash_umap_is_valid(table) || !value) {
        return STASH_ERROR_OUT_OF_MEMORY;
    }

    // Check if the table needs to be resized
    if (table->buckets.count + 1 >= table->buckets.capacity) {
        size_t newSize = u_stash_next_po2_u64(table->buckets.count + 1);
        stash_arr_reserve(&table->buckets, newSize);
    }

    // Find a free location
    int slot_index = u_stash_find_free_slot(table, key);
    if (slot_index == -2) {
        return STASH_KEY_EXISTS; // The key already exists
    }
    else if (slot_index == -1) {
        return STASH_ERROR_OUT_OF_MEMORY; // Table is full
    }

    // Allocate memory for the value
    void* value_copy = STASH_MALLOC(table->value_size);
    if (!value_copy) {
        return STASH_ERROR_OUT_OF_MEMORY;
    }

    // Copy value
    memcpy(value_copy, value, table->value_size);

    // Get the entry and update it
    stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at(&table->buckets, slot_index);
    entry->key = key;
    entry->occupied = true;
    entry->value = value_copy;

    table->count++;

    return STASH_SUCCESS;
}

int stash_umap_remove(stash_umap* table, uint32_t key, void* element)
{
    if (!stash_umap_is_valid(table)) {
        return STASH_ERROR_KEY_NOT_FOUND;
    }

    int64_t index = u_stash_find_entry_index(table, key);
    if (index < 0) {
        return STASH_ERROR_KEY_NOT_FOUND;
    }

    stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at(&table->buckets, (uint32_t)index);

    // Copy the value if requested
    if (element != NULL) {
        memcpy(element, entry->value, table->value_size);
    }

    // Free the value memory
    STASH_FREE(entry->value);

    // Mark the entry as free
    entry->occupied = false;
    entry->value = NULL;

    table->count--;

    return STASH_SUCCESS;
}

int stash_umap_get(const stash_umap* table, uint32_t key, void* element)
{
    if (!stash_umap_is_valid(table) || !element) {
        return STASH_ERROR_KEY_NOT_FOUND;
    }

    int64_t index = u_stash_find_entry_index(table, key);
    if (index < 0) {
        return STASH_ERROR_KEY_NOT_FOUND;
    }

    stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at((stash_arr*)&table->buckets, (uint32_t)index);
    memcpy(element, entry->value, table->value_size);

    return STASH_SUCCESS;
}

bool stash_umap_contains(const stash_umap* table, uint32_t key)
{
    if (!stash_umap_is_valid(table)) return false;
    return u_stash_find_entry_index(table, key) >= 0;
}

void stash_umap_clear(stash_umap* table)
{
    if (!stash_umap_is_valid(table)) {
        return;
    }

    for (size_t i = 0; i < table->buckets.count; i++) {
        stash_umap_entry* entry = (stash_umap_entry*)stash_arr_at(&table->buckets, i);
        if (entry && entry->occupied) {
            if (entry->value) {
                STASH_FREE(entry->value);
                entry->value = NULL;
            }
            entry->occupied = false;
        }
    }

    table->count = 0;
}

size_t stash_umap_count(const stash_umap* table)
{
    return stash_umap_is_valid(table) ? table->count : 0;
}

/* === Public Registry Implementation === */

stash_reg stash_reg_create(size_t capacity, size_t elem_size)
{
    stash_reg reg = { 0 };
    reg.free_ids = stash_arr_create(capacity, sizeof(uint32_t));
    reg.valid_flags = stash_arr_create(capacity, sizeof(bool));
    reg.elements = stash_arr_create(capacity, elem_size);
    reg.elem_size = elem_size;
    reg.next_id = 1;
    return reg;
}

void stash_reg_destroy(stash_reg* reg)
{
    stash_arr_destroy(&reg->valid_flags);
    stash_arr_destroy(&reg->elements);
    stash_arr_destroy(&reg->free_ids);
}

bool stash_reg_is_valid(const stash_reg* reg)
{
    return stash_arr_is_valid(&reg->valid_flags)
        && stash_arr_is_valid(&reg->elements)
        && stash_arr_is_valid(&reg->free_ids);
}

bool stash_reg_exists(const stash_reg* reg, uint32_t id)
{
    if (id == 0 || id >= reg->next_id) return false;
    return ((bool*)reg->valid_flags.data)[id - 1];
}

stash_it stash_reg_begin(stash_reg* reg)
{
    stash_it it = { 0 };

    if (reg == NULL || reg->elements.count == 0) {
        return it;
    }

    // Find the first valid element
    for (uint32_t id = 1; id < reg->next_id; id++) {
        if (((bool*)reg->valid_flags.data)[id - 1]) {
            it.curr = stash_arr_at(&reg->elements, id - 1);

            // Store the current ID in prev for later use
            // This is a trick to find out what ID is associated with this element
            it.prev = (void*)(uintptr_t)id;

            // Find the next valid element for next
            uint32_t next_id = id + 1;
            while (next_id < reg->next_id && !((bool*)reg->valid_flags.data)[next_id - 1]) {
                next_id++;
            }

            if (next_id < reg->next_id) {
                it.next = stash_arr_at(&reg->elements, next_id - 1);
            }
            else {
                it.next = NULL;
            }

            break;
        }
    }

    return it;
}

void stash_reg_previous(stash_reg* reg, stash_it* it)
{
    if (reg == NULL || it == NULL || reg->elements.count == 0) {
        return;
    }

    // If we are already at the beginning or before
    if (it->curr == NULL) {
        // If next is set, we are just before the start
        if (it->next != NULL) {
            // We are before the beginning, so we stay here
            return;
        }

        // Otherwise we go to the end
        *it = stash_reg_end(reg);
        return;
    }

    // Retrieve current ID
    uint32_t curr_id = (uint32_t)(uintptr_t)it->prev;

    // Find previous valid ID
    uint32_t prev_id = curr_id - 1;
    while (prev_id >= 1 && !((bool*)reg->valid_flags.data)[prev_id - 1]) {
        prev_id--;
    }

    if (prev_id >= 1) {
        // Save the current pointer
        void* oldCurr = it->curr;

        // Update the iterator
        it->curr = stash_arr_at(&reg->elements, prev_id - 1);
        it->next = oldCurr;
        it->prev = (void*)(uintptr_t)prev_id;
    }
    else {
        // We are before the beginning
        it->next = it->curr;
        it->curr = NULL;
        it->prev = NULL;
    }
}

void stash_reg_next(stash_reg* reg, stash_it* it)
{
    if (reg == NULL || it == NULL || reg->elements.count == 0) {
        return;
    }

    // If we are already at the end or after
    if (it->curr == NULL) {
        // If prev is set, we are just after the end
        if (it->prev != NULL) {
            // We're past the end, so we're staying here.
            return;
        }

        // Otherwise, we go to the beginning
        *it = stash_reg_begin(reg);
        return;
    }

    // If next is NULL, we are at the end
    if (it->next == NULL) {
        // Save current pointer to prev
        it->prev = it->curr;
        it->curr = NULL;
        return;
    }

    // Retrieve current ID
    uint32_t curr_id = (uint32_t)(uintptr_t)it->prev;

    // Find the next valid ID
    uint32_t next_id = curr_id + 1;
    while (next_id < reg->next_id && !((bool*)reg->valid_flags.data)[next_id - 1]) {
        next_id++;
    }

    // Save the old current pointer
    void* oldCurr = it->curr;

    // Update the iterator
    it->curr = it->next;
    it->prev = (void*)(uintptr_t)next_id;

    if (next_id < reg->next_id) {
        // Find the next valid element after next_id
        uint32_t next_next_id = next_id + 1;
        while (next_next_id < reg->next_id && !((bool*)reg->valid_flags.data)[next_next_id - 1]) {
            next_next_id++;
        }

        if (next_next_id < reg->next_id) {
            it->next = stash_arr_at(&reg->elements, next_next_id - 1);
        }
        else {
            it->next = NULL;
        }
    }
    else {
        it->next = NULL;
    }
}

stash_it stash_reg_end(stash_reg* reg)
{
    stash_it it = { 0 };

    if (reg == NULL || reg->elements.count == 0) {
        return it;
    }

    // Find the last valid element
    for (uint32_t id = reg->next_id - 1; id >= 1; id--) {
        if (((bool*)reg->valid_flags.data)[id - 1]) {
            it.curr = stash_arr_at(&reg->elements, id - 1);

            // Store current ID in prev
            it.prev = (void*)(uintptr_t)id;

            // Find the previous valid element for prev
            uint32_t prev_id = id - 1;
            while (prev_id >= 1 && !((bool*)reg->valid_flags.data)[prev_id - 1]) {
                prev_id--;
            }

            if (prev_id >= 1) {
                it.prev = stash_arr_at(&reg->elements, prev_id - 1);
                // Reuse prev to store the ID
                it.prev = (void*)(uintptr_t)id;
            }

            break;
        }
    }

    return it;
}

uint32_t stash_reg_push(stash_reg* reg, const void* element)
{
    uint32_t id = 0;

    if (reg->free_ids.count > 0) {
        stash_arr_pop_back(&reg->free_ids, &id);
    }
    else {
        stash_arr_push_back(&reg->elements, NULL);
        stash_arr_push_back(&reg->valid_flags, &(bool){true});
        id = reg->next_id++;
    }

    void* elem = stash_arr_at(&reg->elements, id - 1);

    if (element) memcpy(elem, element, reg->elem_size);
    else memset(elem, 0, reg->elem_size);

    ((bool*)reg->valid_flags.data)[id - 1] = true;

    return id;
}

bool stash_reg_pop(stash_reg* reg, uint32_t id, void* element)
{
    if (!stash_reg_exists(reg, id)) {
        return false;
    }

    if (element != NULL) {
        void* elem = stash_arr_at(&reg->elements, id - 1);
        memcpy(element, elem, reg->elem_size);
    }

    stash_arr_push_back(&reg->free_ids, &id);
    ((bool*)reg->valid_flags.data)[id - 1] = false;

    return true;
}

void* stash_reg_get(stash_reg* reg, uint32_t id)
{
    if (!stash_reg_exists(reg, id)) return NULL;
    return stash_arr_at(&reg->elements, id - 1);
}

uint32_t stash_reg_get_alloc_count(const stash_reg* reg)
{
    return (uint32_t)reg->elements.count;
}

#ifdef __cplusplus
}
#endif

#endif // STASH_IMPL
