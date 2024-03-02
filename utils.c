#include "./utils.h"
void clear_grid(char **grid, int rows, int cols, int fill) {
    if (grid == NULL)
        return;
    for (int i = 0; i < rows; i++) {
        memset(grid[i], fill, cols);
    }
}

char **allocate_grid(int rows, int cols, int fill) {
    char **grid = malloc(sizeof(char *) * rows);
    if (grid == NULL)
        return NULL;
    for (int i = 0; i < rows; ++i) {
        grid[i] = malloc(sizeof(char) * cols);
        if (grid[i] == NULL)
            return NULL;
        memset(grid[i], fill, cols);
    }
    return grid;
}

