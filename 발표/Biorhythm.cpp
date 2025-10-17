#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_SKYBLUE "\x1b[36m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

#define PHYSICAL_CYCLE 23
#define EMOTIONAL_CYCLE 28
#define INTELLECTUAL_CYCLE 33

bool is_leap(int year);
int get_days_in_month(int year, int month);
long get_total_days(int year, int month, int day);
void print_biorhythm_table(long birth_total_days, int sy, int sm, int sd, int ey, int em, int ed);
int get_valid_integer(const char* prompt);
void get_valid_date(const char* main_prompt, int* p_year, int* p_month, int* p_day);
const char* get_color_for_status(char status);

int main(void) {
    char restart_choice = 'y';

    do {
        printf("\n--- 바이오리듬 계산 프로그램 ---\n");
        printf("생년월일과 확인하고 싶은 기간을 입력하세요.\n");

        int birth_year, birth_month, birth_day;
        int start_year, start_month, start_day;
        int end_year, end_month, end_day;

        get_valid_date("\n1. 생년월일을 입력하세요", &birth_year, &birth_month, &birth_day);
        get_valid_date("\n2. 조회를 시작할 날짜를 입력하세요", &start_year, &start_month, &start_day);
        get_valid_date("\n3. 조회를 종료할 날짜를 입력하세요", &end_year, &end_month, &end_day);

        long birth_total_days = get_total_days(birth_year, birth_month, birth_day);
        long start_total_days = get_total_days(start_year, start_month, start_day);
        long end_total_days = get_total_days(end_year, end_month, end_day);

        if (start_total_days < birth_total_days) {
            printf("\n오류: 조회 시작일이 생년월일보다 빠를 수 없습니다.\n");
        } else if (end_total_days < start_total_days) {
            printf("\n오류: 조회 종료일이 시작일보다 빠를 수 없습니다.\n");
        } else {
            print_biorhythm_table(birth_total_days, start_year, start_month, start_day, end_year, end_month, end_day);
        }

        char buffer[10];
        printf("\n다른 기간도 조회하시겠습니까? (y/n): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            restart_choice = tolower(buffer[0]);
        } else {
            restart_choice = 'n';
        }

    } while (restart_choice == 'y');

    printf("\n프로그램을 종료합니다.\n");
    return 0;
}

const char* get_color_for_status(char status) {
    switch (status) {
        case '+': return COLOR_GREEN;
        case '-': return COLOR_SKYBLUE;
        case 'D': return COLOR_RED;
        default:  return COLOR_RESET;
    }
}

void print_biorhythm_table(long birth_total_days, int sy, int sm, int sd, int ey, int em, int ed) {
    printf("\n--- 조회 기간: %d년 %d월 %d일 ~ %d년 %d월 %d일 ---\n", sy, sm, sd, ey, em, ed);
    printf("+------------+-----------------+-----------------+-----------------+\n");
    printf("|    날짜    |   신체 (23일)   |   감정 (28일)   |   지성 (33일)   |\n");
    printf("+------------+-----------------+-----------------+-----------------+\n");

    int current_year = sy;
    int current_month = sm;
    int current_day = sd;

    while (true) {
        long current_total_days = get_total_days(current_year, current_month, current_day);
        long day_diff = current_total_days - birth_total_days;

        double p_val = sin(2.0 * M_PI * day_diff / PHYSICAL_CYCLE) * 100;
        char p_stat = (p_val > 20) ? '+' : ((p_val < -20) ? '-' : 'D');

        double e_val = sin(2.0 * M_PI * day_diff / EMOTIONAL_CYCLE) * 100;
        char e_stat = (e_val > 20) ? '+' : ((e_val < -20) ? '-' : 'D');

        double i_val = sin(2.0 * M_PI * day_diff / INTELLECTUAL_CYCLE) * 100;
        char i_stat = (i_val > 20) ? '+' : ((i_val < -20) ? '-' : 'D');

        printf("| %04d-%02d-%02d |  %s%6.1f%% (%c)%s  |  %s%6.1f%% (%c)%s  |  %s%6.1f%% (%c)%s  |\n",
               current_year, current_month, current_day,
               get_color_for_status(p_stat), p_val, p_stat, COLOR_RESET,
               get_color_for_status(e_stat), e_val, e_stat, COLOR_RESET,
               get_color_for_status(i_stat), i_val, i_stat, COLOR_RESET);

        if (current_year == ey && current_month == em && current_day == ed) {
            break;
        }

        current_day++;
        if (current_day > get_days_in_month(current_year, current_month)) {
            current_day = 1;
            current_month++;
            if (current_month > 12) {
                current_month = 1;
                current_year++;
            }
        }
    }
    printf("+------------+-----------------+-----------------+-----------------+\n");
    printf("| ※ 상태: %s고조기(+)%s, %s저조기(-)%s, %s위험기(D)%s                  |\n",
           COLOR_GREEN, COLOR_RESET, COLOR_SKYBLUE, COLOR_RESET, COLOR_RED, COLOR_RESET);
    printf("+-----------------------------------------------------------------+\n");
}

void get_valid_date(const char* main_prompt, int* p_year, int* p_month, int* p_day) {
    printf("%s\n", main_prompt);
    while (true) {
        *p_year = get_valid_integer(" > 년도: ");
        *p_month = get_valid_integer(" > 월: ");
        *p_day = get_valid_integer(" > 일: ");

        if (*p_month < 1 || *p_month > 12) {
            printf(">> 오류: 월은 1월에서 12월 사이여야 합니다. 다시 입력해주세요.\n\n");
            continue;
        }
        int max_days = get_days_in_month(*p_year, *p_month);
        if (*p_day < 1 || *p_day > max_days) {
            printf(">> 오류: %d년 %d월에는 %d일이 없습니다. (1 ~ %d일 사이로 입력해주세요)\n\n", *p_year, *p_month, *p_day, max_days);
            continue;
        }
        break;
    }
}

bool is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_days_in_month(int year, int month) {
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap(year)) {
        days[2] = 29;
    }
    return days[month];
}

long get_total_days(int year, int month, int day) {
    long total = (year - 1) * 365L + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    for (int i = 1; i < month; i++) {
        total += get_days_in_month(year, i);
    }
    total += day;
    return total;
}

int get_valid_integer(const char* prompt) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) exit(1);
        buffer[strcspn(buffer, "\n")] = 0;
        char *endptr;
        long value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf(">> 오류: 숫자만 입력해주세요.\n");
        } else {
            return (int)value;
        }
    }
}
