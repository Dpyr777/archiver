#ifndef DECOMPRESS_H
#define DECOMPRESS_H
void decompress(FILE* readFile, FILE* decompressedFile, CodeSym* codeTable, int uniqueCharCount, unsigned int charLen);

#endif
