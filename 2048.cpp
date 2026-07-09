#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const int TAM = 4;
const int META = 2048;
const string ARQUIVO_RECORDES = "recordes.txt";

struct Perfil
{
    string nome;
    int recordeClassico;
    int recordeDueto;
};

// --------------------------------------------------
// Funcoes auxiliares de tela
// --------------------------------------------------

void limparTela() //limpa a tela do console
{
     system("cls");
}

void pausar()   //impede que o programa mude de tela até o usuario apertar enter
{
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --------------------------------------------------
// Perfil do jogador e arquivo de recordes
// --------------------------------------------------

Perfil criarPerfil()  //utiliza o struct pra criar um perfil do jogador, com nome e recordes zerados
{
    Perfil jogador;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Digite seu nome: ";
    getline(cin, jogador.nome);

    if(jogador.nome == "")
    {
        jogador.nome = "Sem nome";
    }

    jogador.recordeClassico = 0;
    jogador.recordeDueto = 0;

    return jogador;
}

void carregarPerfil(Perfil &jogador)  //procurar o perfil do jogador no arquivo de recordes e carregar os recordes salvos
{
    ifstream arquivo(ARQUIVO_RECORDES);

    if(!arquivo)
    {
        return;
    }

    Perfil perfilLido;

    while(getline(arquivo, perfilLido.nome))
    {
        arquivo >> perfilLido.recordeClassico;
        arquivo >> perfilLido.recordeDueto;

        arquivo.ignore(numeric_limits<streamsize>::max(), '\n');

        if(perfilLido.nome == jogador.nome)
        {
            jogador.recordeClassico = perfilLido.recordeClassico;
            jogador.recordeDueto = perfilLido.recordeDueto;

            arquivo.close();
            return;
        }
    }

    arquivo.close();
}

void salvarPerfil(const Perfil &jogador) //salva no arquivo os dados do jogador atualizados, seja um novo recorde ou um novo jogador
{
    vector<Perfil> listaDePerfis;

    ifstream arquivoEntrada(ARQUIVO_RECORDES);

    if(arquivoEntrada)
    {
        Perfil perfilLido;

        while(getline(arquivoEntrada, perfilLido.nome))
        {
            arquivoEntrada >> perfilLido.recordeClassico;
            arquivoEntrada >> perfilLido.recordeDueto;

            arquivoEntrada.ignore(numeric_limits<streamsize>::max(), '\n');

            listaDePerfis.push_back(perfilLido);
        }

        arquivoEntrada.close();
    }

    bool perfilEncontrado = false;

    for(int i = 0; i < listaDePerfis.size(); i++)
    {
        if(listaDePerfis[i].nome == jogador.nome)
        {
            listaDePerfis[i] = jogador;
            perfilEncontrado = true;
        }
    }

    if(!perfilEncontrado)
    {
        listaDePerfis.push_back(jogador);
    }

    ofstream arquivoSaida(ARQUIVO_RECORDES);

    for(int i = 0; i < listaDePerfis.size(); i++)
    {
        arquivoSaida << listaDePerfis[i].nome << endl;
        arquivoSaida << listaDePerfis[i].recordeClassico << endl;
        arquivoSaida << listaDePerfis[i].recordeDueto << endl;
    }

    arquivoSaida.close();
}

Perfil prepararPerfil() //cria o jogador que vai jogar a partida e carrega os recordes salvos, caso existam
{
    Perfil jogador = criarPerfil();

    carregarPerfil(jogador);

    return jogador;
}

void atualizarRecorde(int pontuacao, int &recorde) //atualiza o recorde do jogador caso a pontuacao da partida seja maior que o recorde
{
    if(pontuacao > recorde)
    {
        recorde = pontuacao;
        cout << "\nNovo recorde!\n";
    }
}

vector<Perfil> carregarTodosPerfis() //usado na função recordes, le todos os jogadores salvos no arquivo e os coloca em uma lista de perfis
{
    vector<Perfil> listaDePerfis;

    ifstream arquivo(ARQUIVO_RECORDES);

    if(!arquivo)
    {
        return listaDePerfis;
    }

    Perfil perfilLido;

    while(getline(arquivo, perfilLido.nome))
    {
        arquivo >> perfilLido.recordeClassico;
        arquivo >> perfilLido.recordeDueto;

        arquivo.ignore(numeric_limits<streamsize>::max(), '\n');

        listaDePerfis.push_back(perfilLido);
    }

    arquivo.close();

    return listaDePerfis;
}

// --------------------------------------------------
// Tabuleiro
// --------------------------------------------------

void inicializarTabuleiro(int tabuleiro[TAM][TAM])  //inicializa o tabuleiro com todas as posições vazias 
{
    for(int linha = 0; linha < TAM; linha++)
    {
        for(int coluna = 0; coluna < TAM; coluna++)
        {
            tabuleiro[linha][coluna] = 0;
        }
    }
}

void copiarTabuleiro(const int origem[TAM][TAM], int destino[TAM][TAM]) //copia todos os valores de uma matri para a outra
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
    cout << left << setw(30) << "TABULEIRO A";
    cout << "TABULEIRO B" << right << endl;

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

    switch(comando)
    {
        case 'A':
            return moverEsquerda(tabuleiro, pontos);

        case 'D':
            return moverDireita(tabuleiro, pontos);

        case 'W':
            return moverCima(tabuleiro, pontos);

        case 'S':
            return moverBaixo(tabuleiro, pontos);

        default:
            return false;
    }
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

void mostrarMenuPrincipal()
{
    cout << "=============================\n";
    cout << "            2048\n";
    cout << "=============================\n";
    cout << "1 - Novo Jogo\n";
    cout << "2 - Recordes\n";
    cout << "3 - Regras\n";
    cout << "4 - Sair\n";
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

void mostrarRecordes()
{
    limparTela();

    vector<Perfil> listaDePerfis = carregarTodosPerfis();

    cout << "=============================\n";
    cout << "           RECORDES\n";
    cout << "=============================\n\n";

    if(listaDePerfis.size() == 0)
    {
        cout << "Nenhum recorde salvo ainda.\n";
    }
    else
    {
        for(int i = 0; i < listaDePerfis.size(); i++)
        {
            cout << "Jogador: " << listaDePerfis[i].nome << endl;
            cout << "Recorde Classico: " << listaDePerfis[i].recordeClassico << endl;
            cout << "Recorde Dueto:    " << listaDePerfis[i].recordeDueto << endl;
            cout << "-----------------------------\n";
        }
    }

    pausar();
}

void mostrarRegras()
{
    limparTela();

    cout << "=============================\n";
    cout << "            REGRAS\n";
    cout << "=============================\n\n";

    cout << "OBJETIVO GERAL\n";
    cout << "O objetivo do jogo 2048 e combinar pecas de mesmo valor ate formar uma peca 2048.\n\n";

    cout << "TABULEIRO\n";
    cout << "- O jogo utiliza um tabuleiro 4x4.\n";
    cout << "- Cada posicao pode estar vazia ou conter uma peca numerica.\n";
    cout << "- As pecas seguem potencias de 2: 2, 4, 8, 16, 32, 64, etc.\n\n";

    cout << "INICIO DA PARTIDA\n";
    cout << "- Ao iniciar uma partida, o tabuleiro comeca vazio.\n";
    cout << "- Duas pecas sao geradas aleatoriamente.\n";
    cout << "- Cada nova peca pode ser 2 ou 4.\n";
    cout << "- A chance de aparecer 2 e maior que a chance de aparecer 4.\n\n";

    cout << "COMANDOS\n";
    cout << "W - mover para cima\n";
    cout << "A - mover para esquerda\n";
    cout << "S - mover para baixo\n";
    cout << "D - mover para direita\n";
    cout << "Q - voltar ao menu durante a partida\n\n";

    cout << "MOVIMENTACAO\n";
    cout << "- Todas as pecas deslizam na direcao escolhida.\n";
    cout << "- Pecas iguais que se encontram se fundem.\n";
    cout << "- Exemplo: 2 + 2 gera 4.\n";
    cout << "- Exemplo: 4 + 4 gera 8.\n";
    cout << "- Uma peca criada por fusao nao pode se fundir novamente no mesmo movimento.\n\n";

    cout << "PONTUACAO\n";
    cout << "- A pontuacao aumenta de acordo com as fusoes.\n";
    cout << "- Se duas pecas 2 formam uma peca 4, o jogador ganha 4 pontos.\n";
    cout << "- Se duas pecas 8 formam uma peca 16, o jogador ganha 16 pontos.\n\n";

    cout << "MODO CLASSICO\n";
    cout << "- O jogador controla apenas um tabuleiro.\n";
    cout << "- O objetivo e criar uma peca 2048.\n";
    cout << "- A vitoria ocorre quando qualquer posicao do tabuleiro possui 2048.\n";
    cout << "- A derrota ocorre quando o tabuleiro esta cheio e sem movimentos possiveis.\n\n";

    cout << "MODO DUETO\n";
    cout << "- O jogador controla dois tabuleiros ao mesmo tempo.\n";
    cout << "- Um unico comando e aplicado aos dois tabuleiros.\n";
    cout << "- O movimento so acontece se os dois tabuleiros mudarem.\n";
    cout << "- Se apenas um tabuleiro puder se mover, a jogada e cancelada.\n";
    cout << "- Quando a jogada e valida, uma nova peca surge em cada tabuleiro.\n";
    cout << "- A pontuacao e conjunta, somando os pontos dos dois tabuleiros.\n";
    cout << "- A vitoria ocorre apenas quando os dois tabuleiros possuem uma peca 2048.\n";
    cout << "- A derrota ocorre quando pelo menos um dos tabuleiros trava.\n\n";

    cout << "RECORDES\n";
    cout << "- Antes de iniciar uma partida, o jogador informa seu nome.\n";
    cout << "- O jogo procura se ja existe um perfil salvo com esse nome.\n";
    cout << "- Cada jogador possui recorde separado para o modo classico e para o modo dueto.\n";
    cout << "- Os recordes ficam armazenados no arquivo recordes.txt.\n";

    pausar();
}

// --------------------------------------------------
// Modo Classico
// --------------------------------------------------

void jogarClassico(Perfil &jogador)
{
    int tabuleiro[TAM][TAM];
    int pontuacao = 0;

    iniciarPartida(tabuleiro);

    while(true)
    {
        limparTela();

        cout << "========== MODO CLASSICO ==========\n";
        cout << "Jogador:   " << jogador.nome << endl;
        cout << "Pontuacao: " << pontuacao << endl;
        cout << "Recorde:   " << jogador.recordeClassico << endl;

        mostrarTabuleiro(tabuleiro);

        if(possuiValor(tabuleiro, META))
        {
            cout << "Parabens! Voce venceu!\n";
            atualizarRecorde(pontuacao, jogador.recordeClassico);
            salvarPerfil(jogador);
            pausar();
            break;
        }

        if(estaTravado(tabuleiro))
        {
            cout << "Game Over!\n";
            atualizarRecorde(pontuacao, jogador.recordeClassico);
            salvarPerfil(jogador);
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
            atualizarRecorde(pontuacao, jogador.recordeClassico);
            salvarPerfil(jogador);
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

void jogarDueto(Perfil &jogador)
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
        cout << "Jogador:            " << jogador.nome << endl;
        cout << "Pontuacao conjunta: " << pontuacao << endl;
        cout << "Recorde dueto:      " << jogador.recordeDueto << endl << endl;

        mostrarDoisTabuleiros(tabuleiroA, tabuleiroB);

        if(possuiValor(tabuleiroA, META) && possuiValor(tabuleiroB, META))
        {
            cout << "Parabens! Os dois tabuleiros chegaram em 2048!\n";
            atualizarRecorde(pontuacao, jogador.recordeDueto);
            salvarPerfil(jogador);
            pausar();
            break;
        }

        if(estaTravado(tabuleiroA) || estaTravado(tabuleiroB))
        {
            cout << "Game Over! Pelo menos um dos tabuleiros travou.\n";
            atualizarRecorde(pontuacao, jogador.recordeDueto);
            salvarPerfil(jogador);
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
            atualizarRecorde(pontuacao, jogador.recordeDueto);
            salvarPerfil(jogador);
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
// Funcoes chamadas pelo switch case
// --------------------------------------------------

void menuNovoJogo()
{
    bool voltar = false;

    while(!voltar)
    {
        limparTela();

        mostrarMenuNovoJogo();

        int escolhaModo;
        cin >> escolhaModo;

        switch(escolhaModo)
        {
            case 1:
            {
                Perfil jogador = prepararPerfil();
                jogarClassico(jogador);
                salvarPerfil(jogador);
                break;
            }

            case 2:
            {
                Perfil jogador = prepararPerfil();
                jogarDueto(jogador);
                salvarPerfil(jogador);
                break;
            }

            case 3:
            {
                voltar = true;
                break;
            }

            default:
            {
                cout << "\nOpcao invalida.\n";
                pausar();
                break;
            }
        }
    }
}

void sairDoJogo(bool &sair)
{
    sair = true;
}

// --------------------------------------------------
// Main
// --------------------------------------------------

int main()
{
    srand(time(NULL));

    bool sair = false;

    while(!sair)
    {
        limparTela();

        mostrarMenuPrincipal();

        int opcao;
        cin >> opcao;

        switch(opcao)
        {
            case 1:
            {
                menuNovoJogo();
                break;
            }

            case 2:
            {
                mostrarRecordes();
                break;
            }

            case 3:
            {
                mostrarRegras();
                break;
            }

            case 4:
            {
                sairDoJogo(sair);
                break;
            }

            default:
            {
                cout << "\nOpcao invalida.\n";
                pausar();
                break;
            }
        }
    }

    cout << "\nJogo encerrado.\n";

    return 0;
}