#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
 
#define MAX_MOV 31

// Vetores para armazenar a solução (posição inicial: solR1 e solC1; posição final: solR3 e solC3)
int solR1[MAX_MOV], solC1[MAX_MOV];
int solR3[MAX_MOV], solC3[MAX_MOV];
 
unsigned long nos = 0; // contador de nós (estados explorados do backtracking)
clock_t inicio;
bool achou = false;

// Protótipo da função backtracking
bool backtracking(char tabuleiro[][7], int pecas, int profundidade);

// Conta a quantidade de peças 'X'
int contaPecas(char tabuleiro[][7]){
    int count = 0;
    for(int i = 0; i < 7; i++)
        for(int k = 0; k < 7; k++)
            if(tabuleiro[i][k] == 'X') count++;
    return count;
}

// Verifica o movimento vertical. (l2 = linha intermediária, l3 = linha final do movimento)
bool movimentoValido(char tabuleiro[][7], int l2, int l3, int coluna){
    // Checa os limites do tabuleiro
    if(l2 < 0 || l2 >= 7 || l3 < 0 || l3 >= 7) return false;
    if(coluna < 0 || coluna >= 7) return false;
    // Checa se há uma peça adjacente a 'X' e se há um espaço vazio depois 'O'
    return (tabuleiro[l2][coluna] == 'X' && tabuleiro[l3][coluna] == 'O');
}

// Verifica o movimento horizontal. (c2 = coluna intermediária, c3 = coluna final do movimento)
bool movimentoValidoH(char tabuleiro[][7], int linha, int c2, int c3){
    // Checa os limites do tabuleiro
    if(c2 < 0 || c2 >= 7 || c3 < 0 || c3 >= 7) return false;
    if(linha < 0 || linha >= 7) return false;
    // Checa se há uma peça adjacente a 'X' e se há um espaço vazio depois 'O'
    return (tabuleiro[linha][c2] == 'X' && tabuleiro[linha][c3] == 'O');
}

// Tenta realizar os movimentos verticais
void movimentoVertical(char tabuleiro[][7], int linha, int coluna, int profundidade){
    if(achou) return;
 
    int dirs[] = {-1, 1};   // direções: cima = -1 ; baixo = 1
    for(int i = 0; i < 2; i++){
        int k1 = linha + dirs[i]; // peça intermediária
        int k2 = linha + 2 * dirs[i]; // peça final do movimento
 
        if(tabuleiro[linha][coluna] == 'X' && movimentoValido(tabuleiro, k1, k2, coluna)){
            // Faz o movimento
            tabuleiro[linha][coluna] = 'O';
            tabuleiro[k1][coluna]   = 'O';
            tabuleiro[k2][coluna]   = 'X';
            
            // Salva movimento feito
            solR1[profundidade] = linha; solC1[profundidade] = coluna;
            solR3[profundidade] = k2;   solC3[profundidade] = coluna;
 
            if(backtracking(tabuleiro, contaPecas(tabuleiro), profundidade + 1)){ achou = true; return; }
            
            // Desfaz movimento 
            tabuleiro[linha][coluna] = 'X';
            tabuleiro[k1][coluna]   = 'X';
            tabuleiro[k2][coluna]   = 'O';
        }
    }
}

// Tenta realizar os movimentos horizontais
void movimentoHorizontal(char tabuleiro[][7], int linha, int coluna, int profundidade){
    if(achou) return;
 
    int dirs[] = {-1, 1};
    for(int i = 0; i < 2; i++){
        int c1 = coluna + dirs[i];
        int c2 = coluna + 2 * dirs[i];
 
        if(tabuleiro[linha][coluna] == 'X' && movimentoValidoH(tabuleiro, linha, c1, c2)){
            // Faz o movimento
            tabuleiro[linha][coluna] = 'O';
            tabuleiro[linha][c1]    = 'O';
            tabuleiro[linha][c2]    = 'X';
            
            // Salva o movimento feito
            solR1[profundidade] = linha; solC1[profundidade] = coluna;
            solR3[profundidade] = linha; solC3[profundidade] = c2;
 
            if(backtracking(tabuleiro, contaPecas(tabuleiro), profundidade + 1)){ achou = true; return; }
            
            // Desfaz o movimento
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

// Refaz a solução e salva no arquivo restaum.out
void salvarSolucao(char tabInicial[][7]){
    FILE *f = fopen("restaum.out", "w");
 
    char tab[7][7];
    memcpy(tab, tabInicial, sizeof(tab));
 
    imprimeArquivo(tab, f);
    
    // looping para reproduzir os movimentos guardados anteriormente
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

void carregarTabuleiro(char tabuleiro[7][7]) {
    FILE *f = fopen("tabuleiro.txt", "r");

    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            fscanf(f, " %c", &tabuleiro[i][j]);
        }
    }

    fclose(f);
}

// função principal
bool backtracking(char tabuleiro[][7], int pecas, int profundidade){
    // caso base = pecas == 1 e a única peça estar no meio do tabuleiro
    if(pecas == 1) return (tabuleiro[3][3] == 'X');
    // limite da profundidade
    if(profundidade >= MAX_MOV) return false;
 
    nos++;
    // Mostra progresso a cada 200mil nós explorados
    if(nos % 200000 == 0){
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        printf("Progresso: %lu nos | profundidade=%d | pecas=%d | %.1fs\n", nos, profundidade, pecas, tempo);
        fflush(stdout);
    }
    
    // Tenta todos os movimentos possíveis do tabuleiro
    for(int i = 0; i < 7; i++){
        for(int k = 0; k < 7; k++){
            if(tabuleiro[i][k] == 'X'){
                movimentoVertical  (tabuleiro, i, k, profundidade); if(achou) return true;
                movimentoHorizontal(tabuleiro, i, k, profundidade); if(achou) return true;
            }
        }
    }
    // Caso nenhum caminho der solução
    return false;
}

int main()
{
    char tabuleiro[7][7];

    carregarTabuleiro(tabuleiro);
 
    char tabInicial[7][7];
    memcpy(tabInicial, tabuleiro, sizeof(tabuleiro));
 
    inicio = clock();
    printf("Buscando solucao...\n");
 
    backtracking(tabuleiro, contaPecas(tabuleiro), 0);
 
    salvarSolucao(tabInicial);
 
    return 0;
}
 
