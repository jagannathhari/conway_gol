#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) ((a) < (b)) ? (a) : (b)

void clear_grid(char **grid, int rows, int cols, int fill);
char **allocate_grid(int rows, int cols, int fill);
#endif
