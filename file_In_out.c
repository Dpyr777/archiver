#include <stdio.h>
#include "types.h"

void readingFromFile(FILE* readFile, symbol* symbols, int* kolvo, unsigned int* kk, int* k)
{
    // считываем по байту данные(символы) до конца файла
    int ch; 
    while((ch = fgetc(readFile)) != EOF){
        ++kolvo[ch]; // считаем сколько раз встретился каждый символ
        if (kolvo[ch] == 1){
            ++(*k); // подсчет уникальных символов
        }
        ++(*kk); // подсчет всех символов в файле
    }

    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (kolvo[i] > 0){
            symbols[i].ch = i;
            symbols[i].freq = (float)kolvo[i] / *kk; // подсчет вероятности, частоты 
        }
        symbols[i].code[0] = '\0'; // записываем нулейвой символ конца строки

    }
}

