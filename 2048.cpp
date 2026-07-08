#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>
#include <limits>
#include <string>

using namespace std;

const int TAM = 4;
const int META = 2048;
const string ARQUIVO_RECORDES = "recordes.txt";

// --------------------------------------------------
// Funcoes auxiliares de tela
// --------------------------------------------------

void limparTela()
{
    cout << string(40, '\n');
}

void pausar()
{
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --------------------------------------------------
// Recordes
// --------------------------------------------------

void carregarRecordes(int &recordeClassico, int &recordeDueto)
{
    ifstream arquivo(ARQUIVO_RECORDES);

    if(!arquivo)
    {
        recordeClassico = 0;
        recordeDueto = 0;
        return;
    }

    arquivo >> recordeClassico >> recordeDueto;

    if(arquivo.fail())
    {
        recordeClassico = 0;
        recordeDueto = 0;
    }

    arquivo.close();
}

void salvarRecordes(int recordeClassico, int recordeDueto)
{
    ofstream arquivo(ARQUIVO_RECORDES);

    arquivo << recordeClassico << endl;
    arquivo << recordeDueto << endl;

    arquivo.close();
}

void atualizarRecorde(int pontuacao, int &recorde)
{
    if(pontuacao > recorde)
    {
        recorde = pontuacao;
        cout << "\nNovo recorde!\n";
    }
}

// --------------------------------------------------
// Tabuleiro
// --------------------------------------------------

void inicializarTabuleiro(int tabuleiro[TAM][TAM])
{
    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            tabuleiro[linha][coluna] = 0;
        }
    }
}

void copiarTabuleiro(const int origem[TAM][TAM], int destino[TAM][TAM])
{
    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            destino[linha][coluna] = origem[linha][coluna];
        }
    }
}

void mostrarCelula(int valor)
{
    if(valor == 0)
    {
        cout << setw(6) << ".";
    }
    else
    {
        cout << setw(6) << valor;
    }
}

void mostrarTabuleiro(const int tabuleiro[TAM][TAM])
{
    cout << endl;

    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            mostrarCelula(tabuleiro[linha][coluna]);
        }

        cout << endl;
    }

    cout << endl;
}

void mostrarDoisTabuleiros(const int tabuleiroA[TAM][TAM],
                           const int tabuleiroB[TAM][TAM])
{
    cout << setw(30) << left << "TABULEIRO A";
    cout << "TABULEIRO B" << endl;

    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            mostrarCelula(tabuleiroA[linha][coluna]);
        }

        cout << "      ";

        for(int coluna = 0; coluna < TAM; coluna++)
        {
            mostrarCelula(tabuleiroB[linha][coluna]);
        }

        cout << endl;
    }

    cout << endl;
}

// --------------------------------------------------
// Pecas aleatorias
// --------------------------------------------------

int contarVazios(const int tabuleiro[TAM][TAM])
{
    int quantidade = 0;

    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            if(tabuleiro[linha][coluna] == 0)
            {
                quantidade++;
            }
        }
    }

    return quantidade;
}

void gerarPeca(int tabuleiro[TAM][TAM])
{
    int vazios = contarVazios(tabuleiro);

    if(vazios == 0)
    {
        return;
    }

    int posicao = rand() % vazios;
    int contador = 0;

    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            if(tabuleiro[linha][coluna] == 0)
            {
                if(contador == posicao)
                {
                    if(rand() % 10 < 9)
                    {
                        tabuleiro[linha][coluna] = 2;
                    }
                    else
                    {
                        tabuleiro[linha][coluna] = 4;
                    }

                    return;
                }

                contador++;
            }
        }
    }
}

void iniciarPartida(int tabuleiro[TAM][TAM])
{
    inicializarTabuleiro(tabuleiro);

    gerarPeca(tabuleiro);
    gerarPeca(tabuleiro);
}

// --------------------------------------------------
// Movimentacao de linhas
// --------------------------------------------------

void compactarLinha(int linha[TAM])
{
    int auxiliar[TAM] = {0};
    int indice = 0;

    for(int i = 0; i < TAM; i++)
    {
        if(linha[i] != 0)
        {
            auxiliar[indice] = linha[i];
            indice++;
        }
    }

    for(int i = 0; i < TAM; i++)
    {
        linha[i] = auxiliar[i];
    }
}

int combinarLinha(int linha[TAM])
{
    int pontosGanhos = 0;

    for(int i = 0; i < TAM - 1; i++)
    {
        if(linha[i] != 0 && linha[i] == linha[i + 1])
        {
            linha[i] = linha[i] * 2;
            linha[i + 1] = 0;

            pontosGanhos += linha[i];
        }
    }

    return pontosGanhos;
}

void inverterLinha(int linha[TAM])
{
    for(int i = 0; i < TAM / 2; i++)
    {
        int auxiliar = linha[i];

        linha[i] = linha[TAM - 1 - i];
        linha[TAM - 1 - i] = auxiliar;
    }
}

// --------------------------------------------------
// Movimentos horizontais
// --------------------------------------------------

bool moverEsquerda(int tabuleiro[TAM][TAM], int &pontos)
{
    bool mudou = false;

    for(int linha = 0; linha < TAM; linha++)
    {
        int antes[TAM];

        for(int coluna = 0; coluna < TAM; coluna++)
        {
            antes[coluna] = tabuleiro[linha][coluna];
        }

        compactarLinha(tabuleiro[linha]);
        pontos += combinarLinha(tabuleiro[linha]);
        compactarLinha(tabuleiro[linha]);

        for(int coluna = 0; coluna < TAM; coluna++)
        {
            if(antes[coluna] != tabuleiro[linha][coluna])
            {
                mudou = true;
            }
        }
    }

    return mudou;
}

bool moverDireita(int tabuleiro[TAM][TAM], int &pontos)
{
    bool mudou = false;

    for(int linha = 0; linha < TAM; linha++)
    {
        int antes[TAM];

        for(int coluna = 0; coluna < TAM; coluna++)
        {
            antes[coluna] = tabuleiro[linha][coluna];
        }

        inverterLinha(tabuleiro[linha]);

        compactarLinha(tabuleiro[linha]);
        pontos += combinarLinha(tabuleiro[linha]);
        compactarLinha(tabuleiro[linha]);

        inverterLinha(tabuleiro[linha]);

        for(int coluna = 0; coluna < TAM; coluna++)
        {
            if(antes[coluna] != tabuleiro[linha][coluna])
            {
                mudou = true;
            }
        }
    }

    return mudou;
}

// --------------------------------------------------
// Movimentos verticais
// --------------------------------------------------

void copiarColuna(const int tabuleiro[TAM][TAM], int coluna, int vetor[TAM])
{
    for(int linha = 0; linha < TAM; linha++)
    {
        vetor[linha] = tabuleiro[linha][coluna];
    }
}

void salvarColuna(int tabuleiro[TAM][TAM], int coluna, int vetor[TAM])
{
    for(int linha = 0; linha < TAM; linha++)
    {
        tabuleiro[linha][coluna] = vetor[linha];
    }
}

bool moverCima(int tabuleiro[TAM][TAM], int &pontos)
{
    bool mudou = false;

    for(int coluna = 0; coluna < TAM; coluna++)
    {
        int antes[TAM];
        int vetor[TAM];

        copiarColuna(tabuleiro, coluna, antes);
        copiarColuna(tabuleiro, coluna, vetor);

        compactarLinha(vetor);
        pontos += combinarLinha(vetor);
        compactarLinha(vetor);

        salvarColuna(tabuleiro, coluna, vetor);

        for(int linha = 0; linha < TAM; linha++)
        {
            if(antes[linha] != vetor[linha])
            {
                mudou = true;
            }
        }
    }

    return mudou;
}

bool moverBaixo(int tabuleiro[TAM][TAM], int &pontos)
{
    bool mudou = false;

    for(int coluna = 0; coluna < TAM; coluna++)
    {
        int antes[TAM];
        int vetor[TAM];

        copiarColuna(tabuleiro, coluna, antes);
        copiarColuna(tabuleiro, coluna, vetor);

        inverterLinha(vetor);

        compactarLinha(vetor);
        pontos += combinarLinha(vetor);
        compactarLinha(vetor);

        inverterLinha(vetor);

        salvarColuna(tabuleiro, coluna, vetor);

        for(int linha = 0; linha < TAM; linha++)
        {
            if(antes[linha] != vetor[linha])
            {
                mudou = true;
            }
        }
    }

    return mudou;
}

bool executarMovimento(int tabuleiro[TAM][TAM], char comando, int &pontos)
{
    comando = toupper(comando);

    if(comando == 'A')
    {
        return moverEsquerda(tabuleiro, pontos);
    }
    else if(comando == 'D')
    {
        return moverDireita(tabuleiro, pontos);
    }
    else if(comando == 'W')
    {
        return moverCima(tabuleiro, pontos);
    }
    else if(comando == 'S')
    {
        return moverBaixo(tabuleiro, pontos);
    }

    return false;
}

// --------------------------------------------------
// Vitoria e derrota
// --------------------------------------------------

bool possuiValor(const int tabuleiro[TAM][TAM], int valor)
{
    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            if(tabuleiro[linha][coluna] == valor)
            {
                return true;
            }
        }
    }

    return false;
}

bool existeFusaoPossivel(const int tabuleiro[TAM][TAM])
{
    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM - 1; coluna++)
        {
            if(tabuleiro[linha][coluna] == tabuleiro[linha][coluna + 1])
            {
                return true;
            }
        }
    }

    for(int coluna = 0; coluna < TAM; coluna++)
    {
        for(int linha = 0; linha < TAM - 1; linha++)
        {
            if(tabuleiro[linha][coluna] == tabuleiro[linha + 1][coluna])
            {
                return true;
            }
        }
    }

    return false;
}

bool estaTravado(const int tabuleiro[TAM][TAM])
{
    if(contarVazios(tabuleiro) > 0)
    {
        return false;
    }

    if(existeFusaoPossivel(tabuleiro))
    {
        return false;
    }

    return true;
}

// --------------------------------------------------
// Menus
// --------------------------------------------------

int lerOpcao()
{
    int opcao;

    while(!(cin >> opcao))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Digite uma opcao valida: ";
    }

    return opcao;
}

void mostrarMenuPrincipal()
{
    cout << "=============================\n";
    cout << "            2048\n";
    cout << "=============================\n";
    cout << "1 - Novo Jogo\n";
    cout << "2 - Recordes\n";
    cout << "3 - Sair\n";
    cout << "Escolha: ";
}

void mostrarMenuNovoJogo()
{
    cout << "=============================\n";
    cout << "          NOVO JOGO\n";
    cout << "=============================\n";
    cout << "1 - Modo Classico\n";
    cout << "2 - Modo Dueto\n";
    cout << "3 - Voltar\n";
    cout << "Escolha: ";
}

void mostrarRecordes(int recordeClassico, int recordeDueto)
{
    limparTela();

    cout << "=============================\n";
    cout << "           RECORDES\n";
    cout << "=============================\n";
    cout << "Modo Classico: " << recordeClassico << endl;
    cout << "Modo Dueto:    " << recordeDueto << endl;

    pausar();
}

// --------------------------------------------------
// Modo Classico
// --------------------------------------------------

void jogarClassico(int &recordeClassico)
{
    int tabuleiro[TAM][TAM];
    int pontuacao = 0;

    iniciarPartida(tabuleiro);

    while(true)
    {
        limparTela();

        cout << "========== MODO CLASSICO ==========\n";
        cout << "Pontuacao: " << pontuacao << endl;
        cout << "Recorde:   " << recordeClassico << endl;

        mostrarTabuleiro(tabuleiro);

        if(possuiValor(tabuleiro, META))
        {
            cout << "Parabens! Voce venceu!\n";
            atualizarRecorde(pontuacao, recordeClassico);
            pausar();
            break;
        }

        if(estaTravado(tabuleiro))
        {
            cout << "Game Over!\n";
            atualizarRecorde(pontuacao, recordeClassico);
            pausar();
            break;
        }

        cout << "W - Cima\n";
        cout << "A - Esquerda\n";
        cout << "S - Baixo\n";
        cout << "D - Direita\n";
        cout << "Q - Voltar ao menu\n";
        cout << "Comando: ";

        char comando;
        cin >> comando;
        comando = toupper(comando);

        if(comando == 'Q')
        {
            atualizarRecorde(pontuacao, recordeClassico);
            break;
        }

        if(comando != 'W' && comando != 'A' && comando != 'S' && comando != 'D')
        {
            cout << "\nComando invalido.\n";
            pausar();
            continue;
        }

        int pontosMovimento = 0;
        bool mudou = executarMovimento(tabuleiro, comando, pontosMovimento);

        if(mudou)
        {
            pontuacao += pontosMovimento;
            gerarPeca(tabuleiro);
        }
        else
        {
            cout << "\nMovimento nao alterou o tabuleiro.\n";
            pausar();
        }
    }
}

// --------------------------------------------------
// Modo Dueto
// --------------------------------------------------

void jogarDueto(int &recordeDueto)
{
    int tabuleiroA[TAM][TAM];
    int tabuleiroB[TAM][TAM];

    int pontuacao = 0;

    iniciarPartida(tabuleiroA);
    iniciarPartida(tabuleiroB);

    while(true)
    {
        limparTela();

        cout << "============ MODO DUETO ============\n";
        cout << "Pontuacao conjunta: " << pontuacao << endl;
        cout << "Recorde dueto:      " << recordeDueto << endl << endl;

        mostrarDoisTabuleiros(tabuleiroA, tabuleiroB);

        if(possuiValor(tabuleiroA, META) && possuiValor(tabuleiroB, META))
        {
            cout << "Parabens! Os dois tabuleiros chegaram em 2048!\n";
            atualizarRecorde(pontuacao, recordeDueto);
            pausar();
            break;
        }

        if(estaTravado(tabuleiroA) || estaTravado(tabuleiroB))
        {
            cout << "Game Over! Pelo menos um dos tabuleiros travou.\n";
            atualizarRecorde(pontuacao, recordeDueto);
            pausar();
            break;
        }

        cout << "W - Cima\n";
        cout << "A - Esquerda\n";
        cout << "S - Baixo\n";
        cout << "D - Direita\n";
        cout << "Q - Voltar ao menu\n";
        cout << "Comando unico para os dois tabuleiros: ";

        char comando;
        cin >> comando;
        comando = toupper(comando);

        if(comando == 'Q')
        {
            atualizarRecorde(pontuacao, recordeDueto);
            break;
        }

        if(comando != 'W' && comando != 'A' && comando != 'S' && comando != 'D')
        {
            cout << "\nComando invalido.\n";
            pausar();
            continue;
        }

        int copiaA[TAM][TAM];
        int copiaB[TAM][TAM];

        copiarTabuleiro(tabuleiroA, copiaA);
        copiarTabuleiro(tabuleiroB, copiaB);

        int pontosA = 0;
        int pontosB = 0;

        bool mudouA = executarMovimento(copiaA, comando, pontosA);
        bool mudouB = executarMovimento(copiaB, comando, pontosB);

        if(mudouA && mudouB)
        {
            copiarTabuleiro(copiaA, tabuleiroA);
            copiarTabuleiro(copiaB, tabuleiroB);

            pontuacao += pontosA + pontosB;

            gerarPeca(tabuleiroA);
            gerarPeca(tabuleiroB);
        }
        else
        {
            cout << "\nMovimento cancelado.\n";
            cout << "No modo dueto, os dois tabuleiros precisam mudar ao mesmo tempo.\n";
            pausar();
        }
    }
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main()
{
    srand(time(NULL));

    int recordeClassico;
    int recordeDueto;

    carregarRecordes(recordeClassico, recordeDueto);

    bool sair = false;

    while(!sair)
    {
        limparTela();

        mostrarMenuPrincipal();

        int opcao = lerOpcao();

        if(opcao == 1)
        {
            bool voltar = false;

            while(!voltar)
            {
                limparTela();

                mostrarMenuNovoJogo();

                int escolhaModo = lerOpcao();

                if(escolhaModo == 1)
                {
                    jogarClassico(recordeClassico);
                    salvarRecordes(recordeClassico, recordeDueto);
                }
                else if(escolhaModo == 2)
                {
                    jogarDueto(recordeDueto);
                    salvarRecordes(recordeClassico, recordeDueto);
                }
                else if(escolhaModo == 3)
                {
                    voltar = true;
                }
                else
                {
                    cout << "\nOpcao invalida.\n";
                    pausar();
                }
            }
        }
        else if(opcao == 2)
        {
            mostrarRecordes(recordeClassico, recordeDueto);
        }
        else if(opcao == 3)
        {
            salvarRecordes(recordeClassico, recordeDueto);
            sair = true;
        }
        else
        {
            cout << "\nOpcao invalida.\n";
            pausar();
        }
    }

    cout << "\nJogo encerrado.\n";

    return 0;
}