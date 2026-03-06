#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "decompress.h"

_Bool findCode(CodeSym* codeTable, int uniqueCharCount, int code, unsigned char codeLen, char *ch){
    int left = 0;
    int right = uniqueCharCount - 1;
    while(left <= right){
        int mid = left + ((right - left) >> 1);
        if (code == codeTable[mid].code && codeTable[mid].len == codeLen){
            printf("findCode\n");
            *ch = codeTable[mid].symbol;
            printf("ch = %c\n", *ch);
            return 1;
        }
        if (code > codeTable[mid].code){
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return 0;
}

void decompress(FILE* compressedFile, FILE* decompressedFile, CodeSym* codeTable, int uniqueCharCount, unsigned int charLen){
    char* buffer = (char*) malloc(charLen + 1);
    unsigned int indexBuffer = 0;
#if 0
    if (sizeBuff > 50 * 1024 * 1024){
        sizeBuff = 50 * 1024 * 1024; // 50MB
    }
#endif
    if (buffer == NULL){
        printf("Alloc failed\n");
        exit(1);
    }
    // рассчитываю минимальную длину кода символа в файле.
    unsigned char minCode = codeTable[0].len;
    for (int i = 1; i < uniqueCharCount; ++i){ 
        if (minCode > codeTable[i].len){
            minCode = codeTable[i].len;
        }
    }
    char ch;
    int byte;
    unsigned int code = 0;
    unsigned char codeLen = 0;
    unsigned char mask = 0;
    while(indexBuffer < charLen){
        if (mask == 0) {
            mask = 0b10000000; // начинаю со старшего бита;
            byte = getc(compressedFile);
            if (byte == EOF){
                break;
            }
        }
        while(mask > 0 && codeLen < minCode){
            code <<= 1;
            code += (mask & byte) ? 1 : 0; // если бит равен 1 добавляем единицу к коду; 
            mask >>= 1;
            ++codeLen;
        }
        if (findCode(codeTable, uniqueCharCount, code, codeLen, &ch)){
            codeLen = 0;
            code = 0;
            buffer[indexBuffer] = ch;
            ++indexBuffer;
        } 

        while(mask > 0 && codeLen >= minCode){
            code <<= 1;
            code += (mask & byte) ? 1 : 0; // если бит равен 1 добавляем единицу к коду; 
            mask >>= 1;
            ++codeLen;
            if (findCode(codeTable, uniqueCharCount, code, codeLen, &ch)){
                codeLen = 0;
                code = 0;
                buffer[indexBuffer] = ch;
                ++indexBuffer;
                break;
            } 
        }
    }
    for (int i = 0; i < 10; ++i){
        printf("buffer[%d] = %c\n", i, buffer[i]);
    
    }
    fwrite(buffer, sizeof(char), charLen, decompressedFile);
    //buffer[indexBuffer] = '\0';
    free(buffer);

}
