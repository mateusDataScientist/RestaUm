#include <stdio.h>
#include <stdlib.h>

int main
{
    FILE *file;

    file = fopen("tabuleiroCheio.txt", r);

    if (file == NULL){
        printf("Erro ao abrir arquivo");
        exit(1);
    }

    printf(file);

    fclose(file);
    return 0;
}