#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int order;
    char shape[3];
    int number;
} trump;

void make_card(trump m_card[]);
void display_card(trump m_card[]);
void shuffle_card(trump m_card[]);

int main(void)
{
    trump card[52];
    make_card(card);
    shuffle_card(card);
    display_card(card);
    return 0;
}
void make_card(trump m_card[])
{
    char shape[4][3] = {"♠", "◆", "♥", "♣"};
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = i * 13; j < i * 13 + 13; j++)
        {
            m_card[j].order = i;
            strcpy(m_card[j].shape, shape[i]);
            m_card[j].number = j % 13 + 1;
            switch (m_card[j].number)
            {
                case 1: m_card[j].number = 'A'; break;
                case 11: m_card[j].number = 'J'; break;
                case 12: m_card[j].number = 'Q'; break;
                case 13: m_card[j].number = 'K'; break;
            }
        }
    }
}
void shuffle_card(trump m_card[])
{
    int i, rnd;
    trump temp;
    srand(time(NULL));
    for(i = 0; i < 52; i++)
    {
        rnd = rand() % 52;
        temp = m_card[rnd];
        m_card[rnd] = m_card[i];
        m_card[i] = temp;
    }
}
void display_card(trump m_card[])
{
    int i;
    for(i = 0; i < 52; i++)
    {
        if(i % 13 == 0) printf("\n");
        if(m_card[i].number < 10)
            printf("%s%d ", m_card[i].shape, m_card[i].number);
        else
            printf("%s%c ", m_card[i].shape, m_card[i].number);
    }
    printf("\n");
}