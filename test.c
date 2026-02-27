#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#define MAX_SYMBOLS 256

void reading_from_file(FILE* fp, symbol *symbols, int *kolvo, int *kk, int *k)
{
    int ch;
    while((ch = fgetc(fp)) != EOF){
        ++kolvo[ch];
        if (kolvo[ch] == 1){
            ++*k;
        }
        ++*kk;
    }
    for (int i = 0; i < 256; ++i){
        if (kolvo[i] > 0) {
            symbols[i].ch = i;
            symbols[i].freq = (float)kolvo[i] / *kk;
            symbols[i].code[0] = '\0';
        }

    }
    
    for (int i = 0; i < 256; ++i){
        if (kolvo[i] != 0){
            printf("(%d) %c = %d\n", i, i, kolvo[i]); 
        }
    }
    printf("unique = %d; all symbols = %d\n", *k, *kk);


}
symbol*  makeTree(symbol *psym[], int k){

    symbol * parent = (symbol*)malloc(sizeof(symbol));
    parent->code[0] = '\0';
    parent->freq = psym[k - 1]->freq + psym[k - 2]->freq;
    parent->left = psym[k - 2];
    parent->right = psym[k - 1];
    psym[k - 2] = parent;
    if (k == 2){
        return parent;
    }
    for (int i = k - 3; i >= 0; --i){
        if (parent->freq > psym[i]->freq) {
            psym[i + 1] = psym[i];
            psym[i] = parent;
        } else {
            break;
        }
    }

    return makeTree(psym, k - 1);
}
void makeCodes(symbol* root){
    if (root->left){
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        //printf("code current = %s\n", root->left->code); 
        makeCodes(root->left);
    }

    if (root->right){
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        makeCodes(root->right);
    }

}
void writing_to_file(FILE* fp2, FILE* fp3, symbol* symbols, int* kolvo, int* kk, int* k, int* fsize2){
    union code code1;
    code1.sym_to_write = 0;
    int ch;
    int count = 0;
    while((ch = fgetc(fp2)) != EOF){
       ++count;
       if (ch == '1'){
           switch (count){
               case 1:
                   code1.byte.b8 = 1;
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
       if(count == 8) {
           fputc(code1.sym_to_write, fp3);
           code1.sym_to_write = 0;
           count = 0;
       }
    }
   if (count != 0) {
       fputc(code1.sym_to_write, fp3);
   }
}

int main(int argc, char **argv) {

    FILE *fp, *fp2, *fp3;
    fp = fopen("hello.txt", "rb"); //открываем конкретный файл
    fp2 = fopen("temp1.txt", "wb");//открываем файл для записи бинарного кода
    fp3 = fopen(argv[2], "wb");//открываем файл для записи сжатого файла
                                                //
    int ch;  // в эту переменную читается информация из файла
    int k = 0; //счётчик количества различных букв, уникальных символов
    int kk = 0; // счётчик количества всех знаков в файле
                
    int kolvo[MAX_SYMBOLS] = { 0 };//инициализируем массив количества уникальных символов
    symbol symbols[MAX_SYMBOLS] = { 0 }; //инициализируем массив записей 
    symbol* psym[MAX_SYMBOLS]; //инициализируем массив указателей на записи
                               
    int fsize2 = 0;//счётчик количества символов из 0 и 1 в промежуточном файле temp
                                 
                   
    reading_from_file(fp, symbols, kolvo, &kk, &k);

    int index = 0;
    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (kolvo[i] > 0){
            psym[index] = &symbols[i];
            ++index;
        }
    }
    

    //Сортировка по убыванию по частоте 
    // Тут будет ваш код
    for (int i = 0; i < k - 1; ++i){
        int index = i;
        while (index >= 0 && psym[index]->freq < psym[index + 1]->freq){
            symbol* temp = psym[index];
            psym[index] = psym[index + 1];
            psym[index + 1] = temp;
            --index;
        }
    }
    for (int i = 0; i < k; ++i){
        printf("freq = %f\n", psym[i]->freq);
    }
    symbol* root;//вызов функции создания дерева Хафмана
    if (k > 1){
        root = makeTree(psym, k);//вызов функции создания дерева Хафмана
    } else {
        root = psym[0];
    }
    makeCodes(root);

    rewind(fp);//возвращаем указатель в файле в начало файла
                       //
    //в цикле читаем исходный файл, и записываем полученные в функциях коды в промежуточный файл
    while ((ch = fgetc(fp)) != EOF){
        fputs(symbols[ch].code, fp2);
    }
    fsize2 = ftell(fp2); // position cursor if cursor = EOF size byte file temp.txt

    printf("fsize2 = %d\n", fsize2); 
    fclose(fp);
    fclose(fp2);
    fp2 = fopen("temp.txt", "rb");

    //writing_to_file(fp2, fp3, symbols, kolvo, &kk, &k, &fsize2);  //Эту функцию опишите самостоятельно
    //set_information(symbols, &k, &kk, &fsize2); 



    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (kolvo[i] > 0){
            printf("symbols %c code = %s \n", symbols[i].ch, symbols[i].code);
        }
    }



    while(0 && root){
        printf("root->freq = %f\n", root->freq);
        root = root->left;
    }
    union code code1;
    code1.sym_to_write = 0;
    code1.byte.b8 = 1;
    code1.byte.b7 = 1;
    printf("code1 byte.b8 = 1 and code1.byte.b7 = 1 sym_to_write = %d\n", code1.sym_to_write);

    return 0;
}

