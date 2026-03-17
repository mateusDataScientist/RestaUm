#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void movimentoVertical(char tabuleiro[][7], int linha, int coluna);
void imprimeTabuleiro(char tabuleiro[][7]);
bool movimentoValido(char tabuleiro[][7], int linha2, int linha3, int coluna);
int contaPecas(char tabuleiro[][7]);

int main()
{
    
    char tabuleiro[7][7] = {
    {'#','#','X','X','X','#','#'},
    {'#','#','X','X','X','#','#'},
    {'X','X','X','X','X','X','X'},
    {'X','X','X','O','X','X','X'},
    {'X','X','X','X','X','X','X'},
    {'#','#','X','X','X','#','#'},
    {'#','#','X','X','X','#','#'}
    };

    movimentoVertical(tabuleiro, 1, 3);
    imprimeTabuleiro(tabuleiro);

    return 0;
}

int contaPecas(char tabuleiro[][7]){
    int count;
    for(int i=0; i<7; i++){
        for(int k=0; k<7; k++){
            if(tabuleiro[i][k]=='X') count++;
        }
    }
    return count;
}

void movimentoVertical(char tabuleiro[][7], int linha, int coluna){
    int movimentosVerticais[] = {-1, 1};
    for (int i = 0; i < 2; i++){
        int k1 = linha + movimentosVerticais[i];
        int k2 = linha + 2*movimentosVerticais[i];

        if (movimentoValido(tabuleiro, k1, k2, coluna)){
            tabuleiro[linha][coluna] = 'O';
            tabuleiro[k1][coluna] = 'O';
            tabuleiro[k2][coluna] = 'X';

            imprimeTabuleiro(tabuleiro);

            movimentoVertical(tabuleiro, k2, coluna);

            tabuleiro[linha][coluna] = 'X';
            tabuleiro[k1][coluna] = 'X';
            tabuleiro[k2][coluna] = 'O';
            imprimeTabuleiro(tabuleiro);
        }

        


    }
}


void imprimeTabuleiro(char tabuleiro[][7]){
    for (int i=0;i<7;i++){
        for (int k=0;k<7;k++){
            printf("%c ", tabuleiro[i][k]);
        }
        printf("\n");
    }
    printf("\n");
}

bool movimentoValido(char tabuleiro[][7], int linha2, int linha3, int coluna){
    if(linha2 < 0 || linha2 > 7) return false;
    if(tabuleiro[linha2][coluna] == 'X' && tabuleiro[linha3][coluna] == 'O') return true;
    return false;
}