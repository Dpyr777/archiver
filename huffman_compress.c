#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "types.h"
#include "arch_logic.h"
#include "information.h"
#include "file_In_out.h"
#include "math_func.h"
#include "compress.h"


int main(int argc, char* argv[])
{

    if (argc != 3) {
        printf("Error with arguments. Two arguments are needed\n");
        return 1;
    }
    FILE* fp, * fp2, * fp3; //указатели на файлы
    fp = fopen(argv[1], "rb"); //открываем конкретный файл
    fp2 = fopen("temp.txt", "wb");//открываем файл для записи бинарного кода
    fp3 = fopen(argv[2], "wb");//открываем файл для записи сжатого файла

    int chh;  // в эту переменную читается информация из файла
    int k = 0; //счётчик количества различных букв, уникальных символов
    unsigned int kk = 0; // счётчик количества всех знаков в файле
                
    int kolvo[MAX_SYMBOLS] = { 0 };//инициализируем массив количества уникальных символов
    symbol symbols[MAX_SYMBOLS] = { 0 }; //инициализируем массив записей 
    symbol* psym[MAX_SYMBOLS]; //инициализируем массив указателей на записи
    int fsize2 = 0;//счётчик количества символов из 0 и 1 в промежуточном файле temp
                   
    //Обработка ошибок чтения файла
    if (fp == NULL || fp2 == NULL || fp3 == NULL)
    {
        puts("File not open");
        return 0;
    }
    
    // Читаем из файла по байтно, считаем количество символов и частоту;
    readingFromFile(fp, symbols, kolvo, &kk, &k);  //Эту функцию опишите самостоятельно
    // обработка крайних случаев файл пуст(нет не одного символа)
    if (k == 0){
        puts("The compression file is empty!!!");
        return 0;
    }

    //В массив указателей psym заносим адреса записей
    // Тут будет ваш код (Пока не обязательно)

    int index = 0;
    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (kolvo[i] > 0){
            psym[index] = &symbols[i]; // располагаем указатели по порядку;
            ++index;
        }
    }

    //Сортировка по убыванию по частоте 
    // Тут будет ваш код
    // сортируем sort_insert указатели не трогая массив symbols
    for (int i = 0; i < k - 1; ++i){
        int index = i;
        while (index >= 0 && psym[index]->freq < psym[index + 1]->freq){
            symbol* temp = psym[index];
            psym[index] = psym[index + 1];
            psym[index + 1] = temp;
            --index;
        }
    }
#if 0
    for (int i = 0; i < k; ++i) {
        printf("freq = %f\n", psym[i]->freq);
    }
#endif


    symbol* root; 
    //обработка крайних случаев
    if (k == 1){
        symbols[0].code[0] = '0';
        symbols[1].code[1] = '\0';

    } else {
        root = makeTree(psym, k);//вызов функции создания дерева Хафмана
        makeCodes(root);//вызов функции получения кода
    }

    rewind(fp);//возвращаем указатель в файле в начало файла
                       
    //в цикле читаем исходный файл, и записываем полученные в функциях коды в промежуточный файл
    while ((chh = fgetc(fp)) != EOF){
        fputs(symbols[chh].code, fp2);
    }
    fsize2 = ftell(fp2);

    fclose(fp); // закрываем текст для сжатия
    fclose(fp2); // закрываем временный файл с кодом из 0 и 1;
                        
    fp2 = fopen("temp.txt", "rb");
    if (fp2 == NULL){
        printf("File temp.txt not open\n");
        return 1;
    }

    compress(fp2, fp3, symbols, k, kk);
    set_information(symbols, &k, &kk, &fsize2); 
   
    fclose(fp2); // закрываем временный файл с кодом из 0 и 1;
    fclose(fp3); // закрываем сжатый файл архив;
    return 0;
}
