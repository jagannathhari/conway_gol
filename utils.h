#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_grid(char **grid, int rows, int cols, int fill);
char **allocate_grid(int rows, int cols, int fill);
#endif
