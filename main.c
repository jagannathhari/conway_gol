#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include "./parser.h"
#include "./utils.h"
#define W 1000
#define H 1000

#define TITLE "Conway Game Of Life"

#define SIZE 20
#define FPS  10

#define MAX_FILEPATH_SIZE 2048

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


void fill_random(char **grid , int rows , int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            grid[i][j] = GetRandomValue('0','1');
        }
    }
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

void draw_cell(char cell_val, int x, int y,int size) {

    if (cell_val == '1') {
        DrawRectangle((x - 1) * size, (y - 1) * size, size, size, RED);
    } else {
        DrawRectangleLines((x - 1) * size, (y - 1) * size, size, size, BLACK);
    }
}

void update_grid(char **grid, char **state, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        memcpy(grid[i], state[i], cols);
    }
}

void fill_cell_on_click(char **grid, int button,int size) {
    if (IsMouseButtonDown(button)) {
        Vector2 pos = GetMousePosition();
        int x = pos.x / size + 1;
        int y = pos.y / size + 1;
        grid[y][x] = (button == MOUSE_BUTTON_LEFT) ? '1' : '0';
    }
}

int main(int argc , char **argv) {
    World world; 
    int rows = H / SIZE;
    int cols = W / SIZE;
    int size = SIZE;
    if(argc==2){
        create_world(argv[1],&world);
        rows = world.height;
        cols = world.width;
        size = H/rows;
    }else{
        world.height = rows;
        world.width = cols;
        world.grid = allocate_grid(rows + 2, cols + 2, '0');
    }

    

    bool start = false;

    char **state = allocate_grid(rows + 2, cols + 2, '0');

    InitWindow(W, H, TITLE);

    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {

        fill_cell_on_click(world.grid, MOUSE_BUTTON_LEFT,size);
        fill_cell_on_click(world.grid, MOUSE_BUTTON_RIGHT,size);

        if (IsKeyReleased(KEY_SPACE))
            start = !start;
        ClearBackground(WHITE);

        BeginDrawing();
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                if (start) {
                    int total_neh = get_total_neighour(world.grid, i, j);
                    update_state(state, world.grid[i][j], i, j, total_neh);
                }
                draw_cell(world.grid[i][j], j, i,size);
            }
        }
        EndDrawing();
        if (start)
            update_grid(world.grid, state, rows, cols);

        if (IsKeyReleased(KEY_C)) {
            clear_grid(world.grid, rows, cols, '0');
            clear_grid(state, rows, cols, '0');
        }else if(IsKeyDown(KEY_R)){
            fill_random(world.grid,rows,cols);
        }
    }
    CloseWindow();
    return 0;
}
