#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>

#define W 1000
#define H 1000

#define TITLE "Conway Game Of Life"

#define SIZE 30
#define GRID_EXT (GRID + 2)
#define FPS 10

int get_total_neighour(char **grid, int i, int j) {
    int sum = 0;

    sum += grid[i][j - 1] - '0'; // left
    sum += grid[i][j + 1] - '0'; // right

    sum += grid[i - 1][j] - '0'; // top
    sum += grid[i + 1][j] - '0'; // bottom

    sum += grid[i - 1][j + 1] - '0'; // top right
    sum += grid[i - 1][j - 1] - '0'; // top left

    sum += grid[i + 1][j + 1] - '0'; // bottom right
    sum += grid[i + 1][j - 1] - '0'; // bottom left

    return sum;
}

bool should_die(int total_neougher) {
    return (total_neougher < 2 || total_neougher > 3);
}

bool can_become_alive(int total_neougher) { return total_neougher == 3; }

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

void update_state(char **state, char cell_val, int row, int col,
                  int total_neighbour) {
    if (cell_val == '1') {
        if (should_die(total_neighbour)) {
            state[row][col] = '0';
        } else {
            state[row][col] = '1';
        }
    } else {
        if (can_become_alive(total_neighbour)) {
            state[row][col] = '1';
        }
    }
}

void draw_cell(char cell_val, int x, int y) {

    if (cell_val == '1') {
        DrawRectangle((x - 1) * SIZE, (y - 1) * SIZE, SIZE, SIZE, RED);
    } else {
        DrawRectangleLines((x - 1) * SIZE, (y - 1) * SIZE, SIZE, SIZE, BLACK);
    }
}

void update_grid(char **grid, char **state, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        memcpy(grid[i], state[i], cols);
    }
}

void fill_cell_on_click(char **grid) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Vector2 pos = GetMousePosition();
        int x = pos.x / SIZE + 1;
        int y = pos.y / SIZE + 1;
        grid[y][x] = (grid[y][x] == '1') ? '0' : '1';
    }
}
int main() {
    int rows = H / SIZE;
    int cols = W / SIZE;

    bool start = false;

    char **grid = allocate_grid(rows + 2, cols + 2, '0');
    char **state = allocate_grid(rows + 2, cols + 2, '0');

    InitWindow(W, H, TITLE);

    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {

        fill_cell_on_click(grid);
        if (IsKeyReleased(KEY_SPACE))
            start = !start;

        ClearBackground(WHITE);

        BeginDrawing();
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (start) {
                    int total_neh = get_total_neighour(grid, i, j);
                    update_state(state, grid[i][j], i, j, total_neh);
                }
                draw_cell(grid[i][j], j, i);
            }
        }
        EndDrawing();
        if (start)
            update_grid(grid, state, rows, cols);
    }
    CloseWindow();
    return 0;
}
