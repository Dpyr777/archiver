#include <stdio.h>
#include "types.h"

void reading_from_file(FILE* fp, symbol* symbols, int* kolvo, int* kk, int* k)
{
    // считываем по байту данные(символы) до конца файла
    int ch; 
    while((ch = fgetc(fp)) != EOF){
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

//void writing_to_file(FILE* fp2 , FILE* fp3, symbol* symbols, int* kolvo, int* kk, int* k, int *fsize2)
// читаю из файлы биты и сразу записываю в сжатом виде в архивный файл;
void writing_to_file(FILE* fp2 , FILE* fp3, int *fsize2)
{
    union code code1;
    code1.sym_to_write = 0;
    int ch;
    int count = 0; // счетчик битов в байте начиная со старшего бита;
    while ((ch = fgetc(fp2)) != EOF){ // считываем байты из temp.txt каждый байт это бит;
        ++count;
        if (ch == '1') { // записываем бит в нужное место пока не наберется байт; 
            switch (count){
                case 1:
                    code1.byte.b8 = 1; // старший бит;
                    break;
                case 2:
                    code1.byte.b7 = 1;
                    break;
                case 3:
                    code1.byte.b6 = 1;
                    break;
                case 4:
                    code1.byte.b5 = 1;
                    break;
                case 5:
                    code1.byte.b4 = 1;
                    break;
                case 6:
                    code1.byte.b3 = 1;
                    break;
                case 7:
                    code1.byte.b2 = 1;
                    break;
                case 8:
                    code1.byte.b1 = 1;
                    break;
            }
        }
        if (count == 8) {
            fputc(code1.sym_to_write, fp3); // записываем сжатые данные
            count = 0; // сбрасываем счечик битов
            code1.sym_to_write = 0; // обнуляем биты
        }
    }
    if (count != 0) {
        fputc(code1.sym_to_write, fp3); // дозаписываем неполный байт(для декодирования нужно знать точное количество битов) 
    }

}

