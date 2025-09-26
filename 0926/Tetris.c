#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 22
#define SCREEN_START_X 20
#define SCREEN_START_Y 2
#define NUM_SHAPES 4

int playfield[BOARD_HEIGHT][BOARD_WIDTH];
int score = 0;

int current_block[4][4];
int block_x, block_y;

int shape_T[4][4] = { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} };
int shape_I[4][4] = { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} };
int shape_O[4][4] = { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} };
int shape_L[4][4] = { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} };

int (*tetrominoes[NUM_SHAPES])[4] = { shape_T, shape_I, shape_O, shape_L };

void gotoxy(int x, int y);
void init_game(void);
void draw_playfield(void);
void draw_current_block(void);
int check_collision(int bx, int by, int block[4][4]);
void lock_block(void);
void clear_lines(void);
void create_new_block(void);
void rotate_block(void);

void gotoxy(int x, int y) {
    COORD Pos = { (short)(x - 1), (short)(y - 1) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void init_game(void) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (j == 0 || j == BOARD_WIDTH - 1 || i == BOARD_HEIGHT - 1) {
                playfield[i][j] = 1;
            } else {
                playfield[i][j] = 0;
            }
        }
    }
    create_new_block();
}

void create_new_block(void) {
    block_x = BOARD_WIDTH / 2 - 2;
    block_y = 0;
    
    int shape_index = rand() % NUM_SHAPES;
    
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            current_block[i][j] = tetrominoes[shape_index][i][j];
        }
    }
}

void draw_playfield(void) {
    system("cls");
    gotoxy(SCREEN_START_X - 4, SCREEN_START_Y - 1);
    printf("Score: %d", score);

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            int screen_x = SCREEN_START_X + j * 2;
            int screen_y = SCREEN_START_Y + i;
            gotoxy(screen_x, screen_y);
            
            if (playfield[i][j] == 1) {
                printf("■");
            } else {
                printf("  ");
            }
        }
    }
    draw_current_block();
}

void draw_current_block(void) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (current_block[i][j] == 1) {
                gotoxy(SCREEN_START_X + (block_x + j) * 2, SCREEN_START_Y + block_y + i);
                printf("□");
            }
        }
    }
}

int check_collision(int bx, int by, int block[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[i][j] == 1) {
                if (by + i < 0) continue;
                if (playfield[by + i][bx + j] == 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void lock_block(void) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (current_block[i][j] == 1) {
                if (block_y + i >= 0) {
                    playfield[block_y + i][block_x + j] = 1;
                }
            }
        }
    }
}

void clear_lines(void) {
    for (int i = BOARD_HEIGHT - 2; i >= 0; i--) {
        int is_line_full = 1;
        for (int j = 1; j < BOARD_WIDTH - 1; j++) {
            if (playfield[i][j] == 0) {
                is_line_full = 0;
                break;
            }
        }

        if (is_line_full) {
            score += 100;
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < BOARD_WIDTH - 1; j++) {
                    playfield[k][j] = playfield[k - 1][j];
                }
            }
            for (int j = 1; j < BOARD_WIDTH - 1; j++) {
                playfield[0][j] = 0;
            }
            i++;
        }
    }
}

void rotate_block(void) {
    int temp[4][4] = {0};
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            temp[j][3-i] = current_block[i][j];
        }
    }
    if(!check_collision(block_x, block_y, temp)){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                current_block[i][j] = temp[i][j];
            }
        }
    }
}

int main(void) {
    SetConsoleOutputCP(65001);

    int game_over = 0;
    int fall_timer = 0;
    
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    init_game();

    while (!game_over) {
        if (_kbhit()) {
            char key = _getch();
            if (key == -32 || key == 224) {
                key = _getch();
            }
            switch (key) {
                case 72:
                    rotate_block();
                    break;
                case 75:
                    if (!check_collision(block_x - 1, block_y, current_block)) {
                        block_x--;
                    }
                    break;
                case 77:
                    if (!check_collision(block_x + 1, block_y, current_block)) {
                        block_x++;
                    }
                    break;
                case 80:
                    if (!check_collision(block_x, block_y + 1, current_block)) {
                        block_y++;
                    }
                    break;
            }
        }

        fall_timer++;
        if (fall_timer > 20) {
            fall_timer = 0;
            if (!check_collision(block_x, block_y + 1, current_block)) {
                block_y++;
            } else {
                lock_block();
                clear_lines();
                create_new_block();
                if(check_collision(block_x, block_y, current_block)){
                    game_over = 1;
                }
            }
        }

        draw_playfield();
        Sleep(20);
    }

    gotoxy(SCREEN_START_X + BOARD_WIDTH, SCREEN_START_Y + BOARD_HEIGHT / 2);
    printf("GAME OVER");
    getchar();

    return 0;
}