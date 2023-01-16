#include <stdio.h>
#include <string.h>
#include <time.h>

#define N 9

int k = 0;
char map[N][9], ch_hor[N][N + 1], ch_ver[N][N + 1];
char ch_hor_tmp[N][N + 1], ch_ver_tmp[N][N + 1];
int int_hor[N][N] = {0}, int_ver[N][N] = {0};
int n, c_pr = 0;

void str_to_int(const char str[], int res[N]) {
    int counter = 0;
    int iter = 0;
    for (int i = 0; i < N; i++) {
        if (str[i] == '*') counter++;
        else if (counter != 0) {
            res[iter] = counter;
            counter = 0;
            iter++;
        }
    }
}

int check_map() {
//    map всегда один
    for (int i = 0; i < n; i++)
    {
        int tmp_arr[N] = {0};
        str_to_int(map[i], tmp_arr);
        for (int j = 0; j < n; j++)
        {
            if (tmp_arr[j] != int_hor[i][j]) return 0;
        }

    }
    for (int i = 0; i < n; i++)
    {
        char tmp_str[9] = {"00000000"};
        int tmp_int[N] = {0};
        for (int j = 0; j < n; j++){
            tmp_str[j] = map[j][i];
        }
        str_to_int(tmp_str, tmp_int);
        for (int j = 0; j < n; j++)
        {
            if (tmp_int[j] != int_ver[i][j]) return 0;
        }
    }

    return 1;
}

void cut1(char str[]) {
    int ind, i = -1;
    while(str[++i] != '0' && i < N);
    if (i == N) str[7] = '0';
    else {
        ind = strstr(str, "0") - str;
        str[ind - 1] = '0';
    }
}

void add(char str[], char sym) {
    if (strstr(str, "0") != NULL) str[strstr(str, "0") - str] = sym;
    else {
        int res[N];
        str_to_int(str, res);
        if (res[0] == 0) {
            str[0] = '*';
            return;
        }
        str[7] = '0';
    }
}

void put_map() {
    FILE *output = fopen("../output.txt", "a");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char a = map[i][j];
            if (a == '*' || a == '_') {
                fputc(map[i][j], output);
            } else fputc('_', output);
        }
        fputc('\n', output);
    }
    fputc('\n', output);
    fclose(output);
}

void print_arr(char arr[][9]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%c", arr[i][j]);
        printf("\n");
    }
}

void set_arr(char arr[][9], char sym) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) arr[i][j] = sym;
    }
}

int islands_str_cmp(char str1[], char str2[]) {
    //str2 - начало str1
//    int flag_res = 1;
    int arr1[N] = {0}, arr2[N] = {0};
    str_to_int(str1, arr1);
    str_to_int(str2, arr2);
//    if (arr2[0] == 0) return 0;
    for (int i = 0; i < n; i++) {
        if (arr1[i] < arr2[i] && arr2[i]/*arr1[i] != arr2[i]*/) return 0/*return 0*/;
    }
    return 1;
}

int islands_str_cmp_strogo(char str1[], char str2[]) {
    //str2 - начало str1
    int iter1 = 0, iter2 = 0;
    int arr1[N] = { 0 }, arr2[N] = { 0 };
    str_to_int(str1, arr1);
    str_to_int(str2, arr2);
    /*if (arr2[0] == 0) return 0;*/
    for (int i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) return 0;
    }
    return 1;
}


void recursion(int i, int j) {
    k++;
//    printf("%d %d\n", i, j);
//    print_arr(map);
    if (i == n) {
//        printf("---------------------------\n");
//        printf("%d - %d kogda i > n\n",i,j);
//        print_arr(map);
//        printf("\n");
//        print_arr(ch_hor_tmp);
//        printf("\n");
//        print_arr(ch_ver_tmp);
//        printf("\n");
//        printf("---------------------------\n");
        if (check_map()) {
            put_map();
            c_pr = 1;
        }
    } else if (j == n) {
        if (islands_str_cmp_strogo(ch_hor[i], ch_hor_tmp[i])) recursion(i + 1, 0);
    } else {
        map[i][j] = '*';
        add(ch_hor_tmp[i], '*');
        add(ch_ver_tmp[j], '*');
//        print_arr(ch_ver_tmp);
//        printf("\n");
//        print_arr(ch_hor_tmp);
//        printf("\n");
        if (islands_str_cmp(ch_hor[i], ch_hor_tmp[i])
            && islands_str_cmp(ch_ver[j], ch_ver_tmp[j])) {
            recursion(i, j + 1);
        }
//        printf("%d\n", islands_str_cmp(ch_hor[i], ch_hor_tmp[i])
//                     && islands_str_cmp(ch_ver[i], ch_ver_tmp[i]));
        cut1(ch_hor_tmp[i]);
        cut1(ch_ver_tmp[j]);
        map[i][j] = '_';

        int flag_i, flag_j;
        flag_i = (j > 0) && map[i][j - 1] == '*' ;
        flag_j = (i > 0) && map[i - 1][j] == '*';

        if (flag_i && j != N) add(ch_hor_tmp[i], '_');
        if (flag_j && i != N) add(ch_ver_tmp[j], '_');

        if (islands_str_cmp(ch_hor[i], ch_hor_tmp[i])
            && islands_str_cmp(ch_ver[j], ch_ver_tmp[j])) {
            recursion(i, j + 1);
        }
        if (flag_i) cut1(ch_hor_tmp[i]);
        if (flag_j) cut1(ch_ver_tmp[j]);
//        print_arr(map);
    }
}


int main() {

    double time_spent = 0.0;
    clock_t begin = clock();

    FILE *input;
    FILE *output = fopen("../output.txt", "w");
    fclose(output);
    input = fopen("../input.txt", "r");
    set_arr(ch_hor, '0');
    set_arr(ch_ver, '0');
    set_arr(ch_hor_tmp, '0');
    set_arr(ch_ver_tmp, '0');
    set_arr(map, '0');
    int tmp = -1, c;
    fscanf(input, "%d", &n);

    //horizont
    for (int i = 0; i < n; i++) {
        c = 0;
        do {
            fscanf(input, "%d", &tmp);
//            printf("%d ", tmp);
            int_hor[i][c] = tmp;
            for (int j = 0; j < tmp; j++) add(ch_hor[i], '*');
            if (tmp != 0) add(ch_hor[i], '_');
            c++;
        } while (tmp != 0);
//        printf("\n");
    }


    //vertical
    for (int i = 0; i < n; i++) {
        c = 0;
        do {
            fscanf(input, "%d", &tmp);
//            printf("%d ", tmp);
            int_ver[i][c] = tmp;
            for (int j = 0; j < tmp; j++) add(ch_ver[i], '*');
            if (tmp != 0) add(ch_ver[i], '_');
            c++;
        } while (tmp != 0);
//        printf("\n");
    }


    recursion(0, 0);
    fclose(input);
    if (!c_pr) {
        FILE *output = fopen("../output.txt", "w");
        fputs("no map", output);
        fclose(output);
    }

    time_spent += (double)(clock() - begin) / CLOCKS_PER_SEC;

    printf("%d time:%f", k, time_spent);
    return 0;
}
