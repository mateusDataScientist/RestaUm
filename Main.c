#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
 
#define MAX_MOV 31
 
int solR1[MAX_MOV], solC1[MAX_MOV];
int solR3[MAX_MOV], solC3[MAX_MOV];
 
unsigned long nos = 0;
clock_t inicio;
bool achou = false;
 
bool backtracking(char tabuleiro[][7], int pecas, int prof);

int contaPecas(char tabuleiro[][7]){
    int count = 0;
    for(int i = 0; i < 7; i++)
        for(int k = 0; k < 7; k++)
            if(tabuleiro[i][k] == 'X') count++;
    return count;
}
 
bool movimentoValido(char tabuleiro[][7], int l2, int l3, int coluna){
    if(l2 < 0 || l2 >= 7 || l3 < 0 || l3 >= 7) return false;
    if(coluna < 0 || coluna >= 7) return false;
    return (tabuleiro[l2][coluna] == 'X' && tabuleiro[l3][coluna] == 'O');
}
 
bool movimentoValidoH(char tabuleiro[][7], int linha, int c2, int c3){
    if(c2 < 0 || c2 >= 7 || c3 < 0 || c3 >= 7) return false;
    if(linha < 0 || linha >= 7) return false;
    return (tabuleiro[linha][c2] == 'X' && tabuleiro[linha][c3] == 'O');
}
 
void movimentoVertical(char tabuleiro[][7], int linha, int coluna, int prof){
    if(achou) return;
 
    int dirs[] = {-1, 1};
    for(int i = 0; i < 2; i++){
        int k1 = linha + dirs[i];
        int k2 = linha + 2 * dirs[i];
 
        if(tabuleiro[linha][coluna] == 'X' && movimentoValido(tabuleiro, k1, k2, coluna)){
            tabuleiro[linha][coluna] = 'O';
            tabuleiro[k1][coluna]   = 'O';
            tabuleiro[k2][coluna]   = 'X';
 
            solR1[prof] = linha; solC1[prof] = coluna;
            solR3[prof] = k2;   solC3[prof] = coluna;
 
            if(backtracking(tabuleiro, contaPecas(tabuleiro), prof + 1)){ achou = true; return; }
 
            tabuleiro[linha][coluna] = 'X';
            tabuleiro[k1][coluna]   = 'X';
            tabuleiro[k2][coluna]   = 'O';
        }
    }
}
 
void movimentoHorizontal(char tabuleiro[][7], int linha, int coluna, int prof){
    if(achou) return;
 
    int dirs[] = {-1, 1};
    for(int i = 0; i < 2; i++){
        int c1 = coluna + dirs[i];
        int c2 = coluna + 2 * dirs[i];
 
        if(tabuleiro[linha][coluna] == 'X' && movimentoValidoH(tabuleiro, linha, c1, c2)){
            tabuleiro[linha][coluna] = 'O';
            tabuleiro[linha][c1]    = 'O';
            tabuleiro[linha][c2]    = 'X';
 
            solR1[prof] = linha; solC1[prof] = coluna;
            solR3[prof] = linha; solC3[prof] = c2;
 
            if(backtracking(tabuleiro, contaPecas(tabuleiro), prof + 1)){ achou = true; return; }
 
            tabuleiro[linha][coluna] = 'X';
            tabuleiro[linha][c1]    = 'X';
            tabuleiro[linha][c2]    = 'O';
        }
    }
}
 
void imprimeTabuleiro(char tabuleiro[][7]){
    for(int i = 0; i < 7; i++){
        for(int k = 0; k < 7; k++)
            printf("%c ", tabuleiro[i][k]);
        printf("\n");
    }
    printf("\n");
}
 
void imprimeArquivo(char tabuleiro[][7], FILE *f){
    fprintf(f, "#########\n");
    for(int i = 0; i < 7; i++){
        fprintf(f, "#");
        for(int k = 0; k < 7; k++){
            if     (tabuleiro[i][k] == 'X') fputc('o', f);
            else if(tabuleiro[i][k] == 'O') fputc(' ', f);
            else                             fputc('#', f);
        }
        fprintf(f, "#\n");
    }
    fprintf(f, "#########\n\n");
}
 
void salvarSolucao(char tabInicial[][7]){
    FILE *f = fopen("restaum.out", "w");
 
    char tab[7][7];
    memcpy(tab, tabInicial, sizeof(tab));
 
    imprimeArquivo(tab, f);
 
    for(int i = 0; i < MAX_MOV; i++){
        int r1 = solR1[i], c1 = solC1[i];
        int r3 = solR3[i], c3 = solC3[i];
        int r2 = (r1 + r3) / 2;
        int c2 = (c1 + c3) / 2;
 
        tab[r1][c1] = 'O';
        tab[r2][c2] = 'O';
        tab[r3][c3] = 'X';
 
        imprimeArquivo(tab, f);
    }
 
    fclose(f);
    printf("Solução encontrada! Peças restantes: %d\n", contaPecas(tab));
}

bool backtracking(char tabuleiro[][7], int pecas, int prof){
    if(pecas == 1) return (tabuleiro[3][3] == 'X');
    if(prof >= MAX_MOV) return false;
 
    nos++;
    if(nos % 200000 == 0){
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        printf("Progresso: %lu nos | prof=%d | pecas=%d | %.1fs\n", nos, prof, pecas, tempo);
        fflush(stdout);
    }
 
    for(int i = 0; i < 7; i++){
        for(int k = 0; k < 7; k++){
            if(tabuleiro[i][k] == 'X'){
                movimentoVertical  (tabuleiro, i, k, prof); if(achou) return true;
                movimentoHorizontal(tabuleiro, i, k, prof); if(achou) return true;
            }
        }
    }
    return false;
}

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
 
    char tabInicial[7][7];
    memcpy(tabInicial, tabuleiro, sizeof(tabuleiro));
 
    inicio = clock();
    printf("Buscando solucao...\n");
 
    backtracking(tabuleiro, contaPecas(tabuleiro), 0);
 
    salvarSolucao(tabInicial);
 
    return 0;
}
 
