#include "types.h"
#include <stdlib.h>
#include <string.h>

symbol* makeTree(symbol* psym[], int k)     //рeкурсивная функция создания дерева Хафмана. Передаем по указателю значение нового пришедшего элемента. В данном случае, самого нижнего. Также передаем общее количество символов k. Возвращаем - указатель на новую структуру типа symbol
{
    // возможно утечка, нужно осободить free();
    symbol* parent = (symbol*)malloc(sizeof(symbol)); // создаем родительский узел для хранения узлов или листьев;
    parent->code[0] = '\0'; //устанавливаем символ конца строки
    // левый и правый лист
    parent->left = psym[k - 2];
    parent->right = psym[k - 1];
    parent->freq = psym[k - 1]->freq + psym[k - 2]->freq; // узел хранит сумму чистот его ветвей
    psym[k - 2] = parent; // вставляем parent в массив на вместо правого листа; 
    if (k == 2){
        return parent; // Возращаем root(корень) Дерева Хафмана;
    }
    // сортируем один элемент. ищeм подходящее место смещая все остальные элементы в право 
    for (int i = k - 3; i >= 0; --i){
        if (parent->freq > psym[i]->freq){
            psym[i + 1] = psym[i];
            psym[i] = parent;
        } else {
            break; 
        }
    }
        
    return makeTree(psym, k - 1); // рекурсия, уменьшаем длину массива на один;
}


void makeCodes(symbol* root)//Рекурсивная функция кодирования. На вход приниает указатель на полученный узел дерева
{
    if (root->left){
        strcpy(root->left->code, root->code); // копируем строку(код) из корня(узла) в лист;
        strcat(root->left->code, "0"); // конкатинация двух строк (левого узла и "0")  
        makeCodes(root->left); // обход сначла левого поддерева
    }
    // аналогично обход правого поддерева
    if (root->right) {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        makeCodes(root->right);
    }

}

