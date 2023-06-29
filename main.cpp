#include <iostream>
#include <random>

using namespace std;

struct No {
    char tabuleiro[3][3] = {{'*', '*', '*'}, // Define uma estrutura de dados para representar o tabuleiro do jogo da velha
                            {'*', '*', '*'},
                            {'*', '*', '*'}};
    int valor; // Valor associado ao nó
};

// Função para exibir o tabuleiro na tela
void mostraMatriz(char tabuleiro[3][3]) {
    system("cls"); // Limpa a tela

    cout << "    1   2   3  " << endl;
    cout << "  +---+---+---+" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < 3; j++) {
            cout << tabuleiro[i][j] << " | ";
        }
        cout << endl;
        cout << "  +---+---+---+" << endl;
    }
}

// Função para verificar se um jogador venceu o jogo
bool verificarVitoria(char tabuleiro[3][3], char jogador) {
    // Verificar linhas
    for (int i = 0; i < 3; i++) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador && tabuleiro[i][2] == jogador)
            return true;
    }
    // Verificar colunas
    for (int j = 0; j < 3; j++) {
        if (tabuleiro[0][j] == jogador && tabuleiro[1][j] == jogador && tabuleiro[2][j] == jogador)
            return true;
    }
    // Verificar diagonais
    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][2] == jogador)
        return true;

    if (tabuleiro[0][2] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][0] == jogador)
        return true;

    return false;
}

// Função para verificar se houve empate
bool verificarEmpate(char tabuleiro[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == '*') {
                return false; // Ainda existem posições vazias, não é empate
            }
        }
    }
    return true; // Todas as posições estão preenchidas, é empate
}

// Função para avaliar o valor de um nó
int avaliarNo(char tabuleiro[3][3]) {
    if (verificarVitoria(tabuleiro, 'O'))
        return 1;
    else if (verificarVitoria(tabuleiro, 'X'))
        return -1;
    else
        return 0;
}

// Funções auxiliares para calcular o máximo e o mínimo entre dois valores
int maxValor(int melhorValor, int valor) {
    if (valor > melhorValor) {
        return valor;
    } else {
        return melhorValor;
    }
}

int minValor(int melhorValor, int valor) {
    if (valor < melhorValor) {
        return valor;
    } else {
        return melhorValor;
    }
}

// Função Minimax para determinar o próximo movimento do ‘bot’
int minimax(No *no, bool maximizando) {
    if (verificarVitoria(no->tabuleiro, 'O'))
        return 1;
    else if (verificarVitoria(no->tabuleiro, 'X'))
        return -1;
    else if (verificarEmpate(no->tabuleiro))
        return 0;

    int melhorValor;

    if (maximizando) {
        melhorValor = -1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (no->tabuleiro[i][j] == '*') {
                    no->tabuleiro[i][j] = 'O';
                    int valor = minimax(no, false);
                    no->tabuleiro[i][j] = '*';
                    melhorValor = maxValor(melhorValor, valor);
                }
            }
        }
    } else {
        melhorValor = 1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (no->tabuleiro[i][j] == '*') {
                    no->tabuleiro[i][j] = 'X';
                    int valor = minimax(no, true);
                    no->tabuleiro[i][j] = '*';
                    melhorValor = minValor(melhorValor, valor);
                }
            }
        }
    }

    return melhorValor;
}

// Função para receber a jogada do jogador humano
void jogadaHumana(No &no) {
    int i, j;
    do {
        cout << "Onde você quer jogar? [linha] [coluna] ";
        cin >> i >> j;
        i--; // Ajuste para começar do índice 0
        j--; // Ajuste para começar do índice 0
    } while (i < 0 || i > 2 || j < 0 || j > 2 || no.tabuleiro[i][j] != '*');

    no.tabuleiro[i][j] = 'X';
}

// Função para o ‘bot’ fazer a sua jogada
void jogadaBot(No &no) {
    int melhorValor = -1;
    int melhorLinha = -1;
    int melhorColuna = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (no.tabuleiro[i][j] == '*') {
                no.tabuleiro[i][j] = 'O';
                no.valor = minimax(&no, false);
                no.tabuleiro[i][j] = '*';

                if (no.valor > melhorValor) {
                    melhorValor = no.valor;
                    melhorLinha = i;
                    melhorColuna = j;
                }
            }
        }
    }

    no.tabuleiro[melhorLinha][melhorColuna] = 'O';
}

int main() {
    srand(time(nullptr));
    No no;

    bool vitoria = false;
    int inicio = rand() % 2; // Define aleatoriamente quem começa o jogo
    while (!vitoria) {
        cout << "Jogo da velha" << endl;
        mostraMatriz(no.tabuleiro);

        if (inicio == 0) {
            jogadaHumana(no);
            inicio = 1;
        } else if (inicio == 1) {
            jogadaBot(no);
            inicio = 0;
        }

        if (verificarVitoria(no.tabuleiro, 'X')) {
            mostraMatriz(no.tabuleiro);
            cout << "Jogador Ganhou" << endl;
            vitoria = true;
        } else if (verificarVitoria(no.tabuleiro, 'O')) {
            mostraMatriz(no.tabuleiro);
            cout << "Bot Ganhou" << endl;
            vitoria = true;
        } else if (verificarEmpate(no.tabuleiro)) {
            mostraMatriz(no.tabuleiro);
            cout << "Deu velha." << endl;
            vitoria = true;
        }
    }
    system("pause"); // Pausa o programa antes de encerrar
    return 0;
}
