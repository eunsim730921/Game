#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define X_MAX 79
#define Y_MAX 24

void move_arrow_key(char chr, int *x, int *y, int x_b, int y_b);
void gotoxy(int x, int y);

int main(void)
{
    char key;
    int x = 10, y = 5;

    do
    {
        gotoxy(x, y);
        printf("A");
        key = getch();
        move_arrow_key(key, &x, &y, X_MAX, Y_MAX);
    } while (key != 27);

    return 0;
}