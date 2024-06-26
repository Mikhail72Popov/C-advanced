/* 
Поиск последовательности бит
На вход программе подается беззнаковое 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31).
Требуется взять K подряд идущих битов числа N так, чтобы полученное число было максимальным.
Программа должна вывести полученное число.
Данные на входе:Два целых числа: 32-битное целое число N и натуральное число K (1 ≤ K ≤ 31).
Данные на выходе:Одно целое число

Пример No1
Данные на входе:21
Примечание: 2 = 0000 00102, максимальное число 1
Данные на выходе:1

Пример No2
Данные на входе:12 3
Примечание: 12 = 0000 11002, максимальное число 1102 = 6
Данные на выходе:6 
*/

#include <stdio.h>
#include <stdint.h>
/* 
int main(){
    uint32_t N;
    int K;
    scanf("%u %u", &N, &K);

    // 000101000 - N K = 4
    // 0001 01000
    // 0 0010 1000
    // 00 0101 000
    // 000 1010 00
    uint32_t mask = ~0;
    mask <<= (32 - K);
    // 111100000

    uint32_t max = 0;
    // K = 31 2 варианта
    for (int i = 0; i <= (32 - K); ++i)
    {
        uint32_t v = mask & N >> (32 - K - i);
        if (v > max)
        {
            max = v;
        }
    mask >>= 1;
    }

    printf("%u\n", max);

    return 0;
} 
*/


uint32_t worker(uint32_t ,int );
uint32_t slice_bits(uint32_t, int
start, int length);
int main ()
{
    int k;
    uint32_t n, res;
    scanf("%u %d", &n, &k);
    res = worker (n, k);
    printf("%u\n", res);
    return 0;
}
uint32_t worker (uint32_t n, int k){
    uint32_t max = 0;// minimal value for unsigned types
    for (int i = 0; i<=32-k ; i++)
    {
        uint32_t cur = slice_bits (n, i, k);
        if(cur > max)
        {
            max = cur;
        }
    }
    return max;
}
uint32_t slice_bits(uint32_t n, int start, int length){
    return(n & (~(uint32_t)0 >> (32-length))<<start ) >>
    start;
}