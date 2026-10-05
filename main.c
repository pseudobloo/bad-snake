#include "raylib.h"
#include "stdlib.h"
#include "time.h"
#include <stdio.h>

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NONE
} Direction;

typedef enum {
    GAME_OVER,
    GAME_PLAY,
    GAME_PAUSE
} GameStates;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Rectangle body[30];
    Direction oldDirection;
    Direction queuedDirection;
    Direction currentDirection;
    Position pos;
    int speed;
    int size;
    float timer;
    bool needApple;
} Snake;

void drawBoard(int w, int h);
void drawSnake(Snake *s);
void moveSnake(Snake *s, float *t, GameStates *gs);
void setQueuedDirection(Snake *s);
void drawApple(Rectangle *a);
void addToSnake(Snake *s);

int main(void) {
    
    int sw = 800, sh = 800;
    int size = 50;
    float timer = 90;
    float increment = 6;
    GameStates gs = GAME_PAUSE;
    srand(time(NULL));
    Rectangle apple;
    char cscore[3];
    int score = 0;

    Snake s = {
        .body[0] = {350, 350, size, size},
        .pos = {7, 7},
        .size = 1,
        .speed = 400,
        .timer = timer,
        .oldDirection = NONE,
        .queuedDirection = NONE,
        .currentDirection = NONE,
        .needApple = true
    };

    InitWindow(sw, sh, "Snake");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    
    
    while(!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ESCAPE) && gs == GAME_PLAY) {
            gs = GAME_PAUSE;
        }

        else if (IsKeyPressed(KEY_ESCAPE) && gs == GAME_PAUSE) {
            gs = GAME_PLAY;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        drawBoard(sw, sh);

        switch (gs) {
            case GAME_PLAY:
                setQueuedDirection(&s);

                if(s.needApple) {
                    apple = (Rectangle) {rand() % 16 * 50, rand() % 16 * 50, size, size};
                    s.needApple = false;
                }
                
                drawApple(&apple);
                drawSnake(&s);
                
                sprintf(cscore, "%d", score);
                DrawText(cscore, 10, 10, 30, BLACK);
                
                s.timer -= increment;
                    if (s.timer <= 0) moveSnake(&s, &timer, &gs);
                
                if (apple.x == s.body[0].x && apple.y == s.body[0].y) {
                    s.needApple = true;
                    score++;
                    s.size++;
                    addToSnake(&s);

                    if (rand() % 4 == 0) increment += 2;
                    
                    

                }
                
                break;

            case GAME_PAUSE:
                DrawRectangle(0, 0, 800, 800, BLUE);
                DrawText("Snake!", 288, 290, 40, WHITE);
                DrawText("Press enter/esc to play.", 200, 370, 40, WHITE);
                if (IsKeyPressed(KEY_ENTER)) gs = GAME_PLAY;
                break;
            
            case GAME_OVER:
                DrawRectangle(0, 0, 800, 800, RED);
                char winner[60];
                sprintf(winner,"Your score is: %d." , score);
                DrawText("Game Over!", 288, 290, 40, WHITE);
                DrawText(winner, 288, 330, 40, WHITE);
                DrawText("Press enter to play again.", 200, 370, 40, WHITE);
                for (int i = 1; i < s.size; i++) {
                    s.body[i] = (Rectangle) {};
                }
                s.size = 1;
                s.body[0] = (Rectangle) {350, 350, 50, 50};
                s.timer = timer;
                apple = (Rectangle) {0,0,0,0};
                increment = 6;
                s.needApple = true;
                s.currentDirection = NONE;
                s.oldDirection = NONE;
                s.queuedDirection = NONE;
                if (IsKeyPressed(KEY_ENTER)) {
                    gs = GAME_PLAY;
                    score = 0;
                }
                

                break;
            
            default:
                break;
        
        
        
        }
        


        EndDrawing();
    }
    
    CloseWindow();
    
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

void setQueuedDirection(Snake *s) {

    s->oldDirection = s->currentDirection;

    if(IsKeyDown(KEY_W) && (s->oldDirection != DOWN || s->size == 1) && ((int)s->body[0].x % 50 == 0)) {
        s->queuedDirection = UP;
    }

    if (IsKeyDown(KEY_S) && (s->oldDirection != UP || s->size == 1) && ((int)s->body[0].x % 50 == 0)) {
        s->queuedDirection = DOWN;
    }

    if (IsKeyDown(KEY_A) && (s->oldDirection !=RIGHT || s->size == 1) && ((int)s->body[0].y % 50 == 0)) {
        s->queuedDirection = LEFT;
    }

    if (IsKeyDown(KEY_D) && (s->oldDirection !=LEFT || s->size == 1) && ((int)s->body[0].y % 50 == 0)) {
        s->queuedDirection = RIGHT;
    }

    s->currentDirection = s->queuedDirection;
}

void drawSnake(Snake *s) {

    for (int i = 0; i < s->size; i++) {
        DrawRectangleRec(s->body[i], GREEN);
    }

}

void moveSnake(Snake *s, float *t, GameStates *gs) {
    int step = 50;
    Rectangle old[30];
    
    for (int i = 1; i < s->size; i++) {
        old[i] = s->body[i - 1];
    }

    for (int i = 1; i < s->size; i++) {
        s->body[i] = old[i];
    }

     

    switch (s->currentDirection) {
        case UP:
            s->body[0].y -= step;
            break;
        case DOWN:
            s->body[0].y += step;
            break;
        case LEFT:
            s->body[0].x -= step;
            break;
        case RIGHT:
            s->body[0].x += step;
            break;
        case NONE:
            break;
    }  

    if (s->body[0].y > 800 || s->body[0].y < 0 || s->body[0].x > 800 || s->body[0].x < 0) {
        *gs = GAME_OVER;
    }

    for (int i = 1; i < s->size; i++) {
        if (CheckCollisionRecs(s->body[0], old[i])) *gs = GAME_OVER;
    }

    s->timer = *t;
}

void drawApple(Rectangle *a) {
    DrawRectangleRec(*a, BLUE);
}

void addToSnake(Snake *s) {
    Rectangle old = s->body[s->size -1];
    
    s->body[s->size] = old;


}