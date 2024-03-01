
#ifndef PARSER_H 
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0
#define FILE_OPEN_ERROR 1
#define MEMORY_ALLOCATION_ERROR 2
#define SPACE 32
#define CAPACITY 1024


#define eat_space(src) eat_similar_char(src, SPACE)
#define eat_char(src) (src->pos++)
#define eat_n_char(src, n) (src->pos += n)


typedef struct {
    int *array;
    int pos;
    int capacity;
} DynamicArray;

typedef struct {
    unsigned char *buffer;
    long size;
    long pos;
} Source;

typedef struct {
    int width;
    int height;
    DynamicArray live_cells;
} World;

void handle_error(int error_code);
void init_array(DynamicArray *array);
void resize_array(DynamicArray *array);
void push_array(DynamicArray *array, int value);
void free_array(DynamicArray *array);
static void eat_line(Source *src);
static void eat_similar_char(Source *src, unsigned char c);
static int get_number(Source *src);
static int parse_number_assignment(Source *src);
static void get_live_cells(Source *src, DynamicArray *array, int width);
static void parse_world_info(Source *src, World *world);
void create_world(const char *c, World *world);

#endif 
