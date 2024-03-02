#include "./parser.h"
#include "./utils.h"

void handle_error(int error_code) {
    switch (error_code) {
        case FILE_OPEN_ERROR:
            fprintf(stderr, "Error opening file: %s\n", strerror(errno));
            break;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(stderr, "Error allocating memory\n");
            exit(error_code);
            break;
        default:
            fprintf(stderr, "Unknown error\n");
    }
}

void init_array(DynamicArray *array){
        array->array = malloc(sizeof(int)*CAPACITY);
        if(array->array==NULL){
            handle_error(MEMORY_ALLOCATION_ERROR);
        }
        array->pos = 0;
        array->capacity = CAPACITY;
}
void resize_array(DynamicArray *array){
        array->capacity *= 2;
        array->array = realloc(array->array,sizeof(int)*array->capacity);
        if(array->array==NULL){
            handle_error(MEMORY_ALLOCATION_ERROR);
        }

}
void push_array(DynamicArray *array ,int value){
    if(array->pos>=array->capacity){
        resize_array(array);
    }
    array->array[array->pos++] = value;
}
void free_array(DynamicArray *array){
    free(array->array);
    array->array = NULL;
    array->pos = 0;
    array->capacity = 0;
}

static void eat_line(Source *src) {
    while (src->pos < src->size && src->buffer[src->pos++] != '\n') {
    }
}

static void eat_similar_char(Source *src, unsigned char c) {
    while (src->pos < src->size && src->buffer[src->pos] == c) {
        src->pos++;
    }
}

static int get_number(Source *src) {
    int num = 0;
    char c;
    while ((c = src->buffer[src->pos]) != '\0' && isdigit(c)) {
        num = num * 10 + (c - '0');
        src->pos++;
    }
    return num;
}

static int parse_number_assignment(Source *src) {
    int x;
    eat_char(src);       // eat x
    eat_space(src);      // eat continous spaces
    eat_char(src);       // eat =
    eat_space(src);      // eat continous spaces
    x = get_number(src); // get the value
    eat_space(src);      // eat continous spaces
    return x;
}

static void get_live_cells(Source *src, char **grid, int width) {
    int x = 0;
    int y = 0;
    int temp=0;
    while (src->pos < src->size && src->buffer[src->pos] != '!') {
        char c = src->buffer[src->pos];
        if(c=='\n' || c =='\r'){
            src->pos++;
            continue;
        } 
        if (isdigit(c)) {
            temp = get_number(src);
            c = src->buffer[src->pos];
            if (c == '$') {
                y += temp;
                x = 0;
            } else {
                if (c == 'o' || c == 'O') {
                    while (temp--) {
                        grid[y+1][x+1] = '1';
                        x++;
                    }
                } else {
                    x += temp;
                }
            }
        } else if (c == '$') {
            y++;
            x = 0;
        } else {
            if (c == 'o' || c == 'O') {
                grid[y+1][x+1] = '1';
            }
            x++;
        }
        src->pos++;
    }
}
static void parse_world_info(Source *src,World *world) {
    int width = parse_number_assignment(src);
    
    eat_char(src);  // eat comma;
    eat_space(src); // eat continous spaces

    int height = parse_number_assignment(src);
    world->width = width;
    world->height= height;
    world->grid= allocate_grid(world->height+2,world->width+2,'0');
    // TODO parse survival rule

    eat_line(src);
    get_live_cells(src,world->grid,width);
}

void create_world(const char *c,World *world){
    FILE *f = fopen(c,"r");
    if (!f) {
        handle_error(FILE_OPEN_ERROR);
    }
    Source src;
    int x_pos = 0;
    int y_pos = 0;

    fseek(f, 0, SEEK_END);
    src.size = ftell(f);
    fseek(f, 0, SEEK_SET);
    src.buffer = malloc(sizeof(unsigned char) * src.size);
    src.pos = 0;
    fread(src.buffer, sizeof(unsigned char), src.size, f);
    while (src.pos < src.size) {
        switch (src.buffer[src.pos]) {
        case '#':
            eat_line(&src);
            break;
        case 'x':
        case 'X':
            parse_world_info(&src,world);
            break;
        default:
            src.pos++;
        }
    }
    fflush(stdout);
    fclose(f);
}

