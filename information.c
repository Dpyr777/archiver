#include <stdio.h>
#include "types.h"
#include "math_func.h"

void set_information(symbol* symbols, int* k, unsigned int* kk, int *fsize_2)
{
    int count = 1;
    for (int i = 0; i < MAX_SYMBOLS; i++)
    {
        if (symbols[i].code[0] != '\0') {
            if (i != 10) {
            printf("N= %d\tASCII= %d\tSymb=%c\tFreq= %f\tNum=%f\tCode= %s\t\n",count, symbols[i].ch, symbols[i].ch, symbols[i].freq, symbols[i].freq*(*kk), symbols[i].code);
            } else {
            printf("N= %d\tASCII= %d\tSymb=\\n\tFreq= %f\tNum=%f\tCode= %s\t\n",count, symbols[i].ch, symbols[i].freq, symbols[i].freq*(*kk), symbols[i].code);
            }
            ++count;
        }
    }
    printf("\nLetters = %d\n", *kk);
    printf("Entropy = %f\n", entropy_count(symbols)); // Энтропия H = -(sum_all(Pk * log(Pk)) Pk -> вероятность
    printf("Bit cost = %f\n", bcost(*fsize_2, *kk)); // Битовые затраты R = sum_all(Pk * Rk) Rk -> число бит в коде символа
}
