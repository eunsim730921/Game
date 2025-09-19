#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

int calc_frequency(int octave, int inx);
void practice_piano(void);

int main(void)
{
    printf("1부터 8까지 숫자 키를 누르면\n");
    printf("각 음의 소리가 출력됩니다.\n\n");
    printf("1:도 2:레 3:미 4:파 5:솔 6:라 7:시 8:도\n");
    printf("프로그램 종료는 q 또는 ESC키\n");

    practice_piano();

    return 0;
}

void practice_piano(void)
{
    int index[8] = {0, 2, 4, 5, 7, 9, 11, 12};
    char ch;

    while (1)
    {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'q' || ch == 'Q' || ch == 27)
                break;

            if (ch >= '1' && ch <= '8')
            {
                int keyNum = ch - '1';
                int freq = calc_frequency(4, index[keyNum]);
                Beep(freq, 300);
            }
        }
        Sleep(10);
    }
}

int calc_frequency(int octave, int inx)
{
    int noteNumber = (octave * 12) + inx;
    int A4Number = (4 * 12) + 9;
    int n = noteNumber - A4Number;
    double freq = 440.0 * pow(2.0, n / 12.0);
    return (int)(freq + 0.5);
}