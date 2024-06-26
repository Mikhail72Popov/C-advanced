/* 
Задача 1. Сколько раз встречается максимум
На стандартном потоке ввода задается натуральное число N (N > 0), после которого следует
последовательность из N целых чисел.
На стандартный поток вывода напечатайте, сколько раз в этой последовательности встречается максимум.
Указание: использовать массивы запрещается.
Данные на входе: Натуральное число N (N > 0), после которого следует последовательность из N целых
чисел.
Данные на выходе: Одно целое число.
Пример No1
Данные на входе: 6 1 2 3 2 0 3
Данные на выходе:2
Пример No2
Данные на входе: 3 5 2 -1
Данные на выходе:1 
*/

#include <stdio.h>

int main() {
    int N;
    scanf("%d", &N);

    int max, count;
    scanf("%d", &max);
    count = 1;

    for (int i = 1; i < N; i++) {
        int num;
        scanf("%d", &num);

        if (num > max) {
            max = num;
            count = 1;
        } else if (num == max) {
            count++;
        }
    }

    printf("%d\n", count);

    return 0;
}



