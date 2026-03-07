#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "decompress.h"

/* 
compress file contains {metadata}{data}
{[uniqueCharCount(1 int)] "[1Symbol(1 char),1Code(1 int), 1SizeCode(unsigned char)] [2Symbol,2Code,2SizeCode]... ",[charLen(1 unsigned int)]} {data}
 */

void readMetadata(FILE* compressedFile, CodeSym* codeTable, int uniqueCharCount, unsigned int* charLen){
    // считываем символ и код симовола, затем записываем в массив CodeSym codeTable;
    for (int i = 0; i < uniqueCharCount; ++i){
        fread(&codeTable[i].symbol, 1, 1, compressedFile);
        fread(&codeTable[i].code, sizeof(int), 1, compressedFile);
        fread(&codeTable[i].len, sizeof(unsigned char), 1, compressedFile);
    }
    fread(charLen, sizeof(int), 1, compressedFile); 
}
int compareCode(const void *a, const void *b){
    CodeSym* cd1 = (CodeSym*)a;
    CodeSym* cd2 = (CodeSym*)b;
    if (cd1->code == cd2->code){
        return cd1->len - cd2->len;
    }
    return cd1->code - cd2->code;
}

int main(int argc, char** argv){

    if (argc != 3) {
        printf("Error with arguments. Two arguments are needed\n");
        return 1;
    }
    FILE* compressedFile,* decompressedFile;
    compressedFile = fopen(argv[1], "rb");
    decompressedFile = fopen(argv[2], "wb");
    if (compressedFile == NULL || decompressedFile == NULL){
        printf("File not open");
        return 1;
    }
    int uniqueCharCount = 0;
    unsigned int charLen = 0; 
    
    fread(&uniqueCharCount, sizeof(int), 1, compressedFile);
    CodeSym* codeTable = (CodeSym*)malloc(sizeof(CodeSym) * uniqueCharCount);

    readMetadata(compressedFile, codeTable, uniqueCharCount, &charLen); 
#if 0
    for (int i = 0; i < uniqueCharCount; ++i){
        printf("symbol = %c\n", codeTable[i].symbol);
        printf("code = %d\n", codeTable[i].code);
        printf("len = %d\n", codeTable[i].len);
    }
#endif
    qsort(codeTable, uniqueCharCount, sizeof(CodeSym), compareCode);
    decompress(compressedFile, decompressedFile, codeTable, uniqueCharCount, charLen);
    fclose(compressedFile);
    fclose(decompressedFile);

    free(codeTable);






    return 0;
}
