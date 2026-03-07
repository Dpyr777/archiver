#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"types.h"
#include"compress.h"

void writeMetadata(FILE* archiveFile, symbol* symbols, int uniqueCharCount, unsigned int charLen){
    fwrite(&uniqueCharCount, sizeof(int), 1, archiveFile);

    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (symbols[i].code[0] != '\0'){
            fwrite(&symbols[i].ch, 1, 1, archiveFile);
            char* endptr;
            int code = strtol(symbols[i].code, &endptr, 2); 
            assert (*endptr == '\0' && "The line is not fully read");
            fwrite(&code, sizeof(int), 1, archiveFile);
            unsigned char len = strlen(symbols[i].code);
            fwrite(&len, sizeof(unsigned char), 1, archiveFile);
        }
    } 
    fwrite(&charLen, sizeof(unsigned int), 1, archiveFile);
}

void compress(FILE* readFile, FILE* archiveFile, symbol *symbols, int uniqueCharCount, unsigned int charLen){
    int yesOrNo = 'y'; 
    while (1){
        printf("\nCompression with metadata ?\n(y or n): ");
        yesOrNo = getchar();
        while(getchar() != '\n');
        if (yesOrNo == 'y' ||  yesOrNo == 'n'){
            break;
        }
    }
    if (yesOrNo == 'y'){
        writeMetadata(archiveFile, symbols, uniqueCharCount, charLen);  
    }
    union code code1; // байт и битовые поля;
    code1.sym_to_write = 0;
    int ch;
    int count = 0; // счетчик битов для записи байта;
    while ((ch = fgetc(readFile)) != EOF){// считываем байты из temp.txt каждый байт это бит
        ++count;
        if (ch == '1') { // записываем 1 в нужный бит пока не наберется байт;
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
            fputc(code1.sym_to_write, archiveFile); // записываем сжатые данные
            count = 0; // сбрасываем счечик битов
            code1.sym_to_write = 0; // обнуляем биты
        }
    }
    if (count != 0) {
        fputc(code1.sym_to_write, archiveFile); // дозаписываем неполный байт 
    }
}

