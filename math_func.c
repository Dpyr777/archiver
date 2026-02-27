#include <math.h>
#include "types.h"
// Энтропия показывает сколько бит информации несет каждый символ, идеальная кодировка
// H = Сумма всех множителей (Pk * log(Pk);  Pk - вероятность появления символа
//double entropy_count(symbol* symbols, int* k)
double entropy_count(symbol* symbols)
{
    double H = 0;
    for (int i = 0; i < MAX_SYMBOLS; ++i){
        if (symbols[i].code[0] != '\0'){
            H += symbols[i].freq * log2(symbols[i].freq);
        }
    } 
    H = -H;
    return H;
    
}
// Битовые затраты. Сколько тратится в стреднем бит на кодировку одного символа
double bcost(int fsize_2, int file_length)
{
    double R = (double)fsize_2 / file_length; 

    return R;
}
