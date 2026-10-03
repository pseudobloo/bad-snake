#include "raylib.h"

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE
} Direction;

typedef struct {
    Rectangle body[10];
    Direction oldDirection;
    Direction queuedDirection;
    Direction currentDirection;
    int speed;
    int size;
} Snake;

void drawBoard(int w, int h);
void drawSnake(Snake *s);
void moveSnake(Snake *s);
void setCurrentKey(Snake *s);

int main(void) {
    
    int sw = 800, sh = 800;
    int size = 50;


    Snake s = {
        .body[0] = {350, 350, size, size},
        .size = 1,
        .speed = 200,
        .oldDirection = NONE,
        .queuedDirection = NONE,
        .currentDirection = NONE
    };

    InitWindow(sw, sh, "Snake");
    SetTargetFPS(60);
    
    while(!WindowShouldClose()) {

        setCurrentKey(&s);
        moveSnake(&s);

        BeginDrawing();

        ClearBackground(WHITE);

        drawBoard(sw, sh);
        drawSnake(&s);


        EndDrawing();
    }
    
    
    return 0;
}


void drawBoard(int w, int h) {
    
    int size = 50;
    bool offset = false;

    for (int cw = 0; cw < w; cw += 100) {
        for (int ch = 0; ch < h; ch += 50) {
            DrawRectangle(offset ? cw + 50 : cw, ch, size, size, BROWN);
            offset = !offset;
        }
    }
}

void setCurrentKey(Snake *s) {

    if(IsKeyDown(KEY_W)) {
        s->currentDirection = UP;
    }

    if (IsKeyDown(KEY_S)) {
        s->currentDirection = DOWN;
    }

    if (IsKeyDown(KEY_A)) {
        s->currentDirection = LEFT;
    }

    if (IsKeyDown(KEY_D)) {
        s->currentDirection = RIGHT;
    }

}

void drawSnake(Snake *s) {

    for (int i = 0; i < s->size; i++) {
        DrawRectangleRec(s->body[i], GREEN);
    }

}

void moveSnake(Snake *s) {
    float delta = GetFrameTime();

    switch (s->currentDirection) {
        case UP:
            s->body[0].y -= s->speed * delta;
            break;
        case DOWN:
            s->body[0].y += s->speed * delta;
            break;
        case LEFT:
            s->body[0].x -= s->speed * delta;
            break;
        case RIGHT:
            s->body[0].x += s->speed * delta;
            break;
        default:
            break;
        
    }
}