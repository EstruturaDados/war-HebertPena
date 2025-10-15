// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 3
#define MAX_NOME 50

// Cores para os exércitos (facilita a leitura do código)
#define AZUL    "Azul"
#define VERMELHO "Vermelho"
#define VERDE   "Verde"
#define AMARELO "Amarelo"

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor_exercito[20];
    int num_tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa, const char* corJogador);
void simularAtaque(Territorio *mapa, int idOrigem, int idDestino, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int idMissao, const char* corJogador);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");

    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro crítico: Falha ao alocar memória para o mapa.\n");
        return 1; // Encerra o programa com código de erro
    }

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa);

    // - Define a cor do jogador e sorteia sua missão secreta.
    const char* corJogador = AZUL;
    int missaoJogador = sortearMissao();
    int escolha;
    int vitoria = 0; // Flag para controlar se o jogador venceu

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    do {
        // Limpa a tela para uma nova rodada (funciona em Windows e Linux/macOS)
        system("cls || clear");

        // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        exibirMapa(mapa);
        exibirMissao(missaoJogador);
        exibirMenuPrincipal();

        // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        printf("Sua escolha: ");
        scanf("%d", &escolha);
        limparBufferEntrada(); // Essencial para limpar o buffer após scanf

        switch (escolha) {
            case 1:
                // - Opção 1: Inicia a fase de ataque.
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                // - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
                if (verificarVitoria(mapa, missaoJogador, corJogador)) {
                    printf("\n*** PARABENS! Voce cumpriu sua missao e venceu o jogo! ***\n");
                    vitoria = 1; // Sinaliza que o jogador venceu
                } else {
                    printf("\nVoce ainda nao cumpriu sua missao. Continue lutando!\n");
                }
                break;
            case 0:
                // - Opção 0: Encerra o jogo.
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        if (escolha != 0 && !vitoria) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Aguarda o jogador pressionar Enter
        }

    } while (escolha != 0 && !vitoria);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Aloca dinamicamente a memória para o vetor de territórios.
 * @return Ponteiro para a memória alocada ou NULL em caso de falha.
 */
Territorio* alocarMapa() {
    // Usa calloc para alocar e inicializar a memória com zeros.
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

/**
 * @brief Preenche os dados iniciais de cada território no mapa.
 * @param mapa Ponteiro para o mapa do mundo a ser modificado.
 */
void inicializarTerritorios(Territorio *mapa) {
    // Territórios do jogador (Azul)
    strcpy(mapa[0].nome, "Brasil");      strcpy(mapa[0].cor_exercito, AZUL);    mapa[0].num_tropas = 5;
    strcpy(mapa[1].nome, "Argentina");   strcpy(mapa[1].cor_exercito, AZUL);    mapa[1].num_tropas = 3;

    // Territórios do inimigo 1 (Vermelho)
    strcpy(mapa[2].nome, "Peru");        strcpy(mapa[2].cor_exercito, VERMELHO); mapa[2].num_tropas = 4;
    strcpy(mapa[3].nome, "Colombia");    strcpy(mapa[3].cor_exercito, VERMELHO); mapa[3].num_tropas = 3;
    strcpy(mapa[4].nome, "Venezuela");   strcpy(mapa[4].cor_exercito, VERMELHO); mapa[4].num_tropas = 2;

    // Territórios do inimigo 2 (Verde)
    strcpy(mapa[5].nome, "Mexico");      strcpy(mapa[5].cor_exercito, VERDE);   mapa[5].num_tropas = 5;
    strcpy(mapa[6].nome, "Canada");      strcpy(mapa[6].cor_exercito, VERDE);   mapa[6].num_tropas = 3;

    // Territórios do inimigo 3 (Amarelo)
    strcpy(mapa[7].nome, "Alasca");      strcpy(mapa[7].cor_exercito, AMARELO); mapa[7].num_tropas = 2;
    strcpy(mapa[8].nome, "Groenlandia"); strcpy(mapa[8].cor_exercito, AMARELO); mapa[8].num_tropas = 3;
    strcpy(mapa[9].nome, "Islandia");    strcpy(mapa[9].cor_exercito, AMARELO); mapa[9].num_tropas = 4;
}

/**
 * @brief Libera a memória previamente alocada para o mapa.
 * @param mapa Ponteiro para o mapa a ser liberado.
 */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

/**
 * @brief Imprime na tela o menu de ações disponíveis para o jogador.
 */
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Vitoria\n");
    printf("0. Sair do Jogo\n");
    printf("---------------------\n");
}

/**
 * @brief Mostra o estado atual de todos os territórios no mapa.
 * @param mapa Ponteiro constante para o mapa (apenas leitura).
 */
void exibirMapa(const Territorio *mapa) {
    printf("======================== MAPA DO MUNDO ========================\n");
    printf("| ID  | Territorio      | Dono      | Tropas |\n");
    printf("|-----|-----------------|-----------|--------|\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-3d | %-15s | %-9s | %-6d |\n", i, mapa[i].nome, mapa[i].cor_exercito, mapa[i].num_tropas);
    }
    printf("==============================================================\n");
}

/**
 * @brief Exibe a descrição da missão atual do jogador.
 * @param idMissao O ID da missão sorteada.
 */
void exibirMissao(int idMissao) {
    printf("\n--- SUA MISSAO SECRETA ---\n");
    switch (idMissao) {
        case 0:
            printf("Destruir completamente o exercito VERMELHO.\n");
            break;
        case 1:
            printf("Conquistar 6 territorios no total.\n");
            break;
        case 2:
            printf("Destruir completamente o exercito VERDE.\n");
            break;
        default:
            printf("Missao desconhecida.\n");
            break;
    }
    printf("---------------------------\n");
}

/**
 * @brief Gerencia a interface para a ação de ataque.
 * @param mapa Ponteiro para o mapa a ser modificado.
 * @param corJogador String constante com a cor do jogador.
 */
void faseDeAtaque(Territorio *mapa, const char* corJogador) {
    int idOrigem, idDestino;
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do territorio de origem (seu): ");
    scanf("%d", &idOrigem);
    printf("Digite o ID do territorio de destino (inimigo): ");
    scanf("%d", &idDestino);
    limparBufferEntrada(); // Limpa o buffer após leituras

    simularAtaque(mapa, idOrigem, idDestino, corJogador);
}

/**
 * @brief Executa a lógica de uma batalha entre dois territórios.
 * @param mapa Ponteiro para o mapa a ser modificado.
 * @param idOrigem ID do território atacante.
 * @param idDestino ID do território defensor.
 * @param corJogador Cor do exército do jogador.
 */
void simularAtaque(Territorio *mapa, int idOrigem, int idDestino, const char* corJogador) {
    // Validações básicas
    if (idOrigem < 0 || idOrigem >= NUM_TERRITORIOS || idDestino < 0 || idDestino >= NUM_TERRITORIOS) {
        printf("Erro: ID de territorio invalido.\n");
        return;
    }
    if (strcmp(mapa[idOrigem].cor_exercito, corJogador) != 0) {
        printf("Erro: Voce so pode atacar a partir de um territorio seu.\n");
        return;
    }
    if (strcmp(mapa[idDestino].cor_exercito, corJogador) == 0) {
        printf("Erro: Voce nao pode atacar um territorio que ja e seu.\n");
        return;
    }
    if (mapa[idOrigem].num_tropas <= 1) {
        printf("Erro: O territorio de origem precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    printf("\n--- BATALHA: %s ataca %s ---\n", mapa[idOrigem].nome, mapa[idDestino].nome);

    // Lógica simplificada de dados (1 dado para cada)
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("Dado de Ataque: %d\n", dadoAtaque);
    printf("Dado de Defesa: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n");
        mapa[idDestino].num_tropas--;
    } else {
        printf("VITORIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
        mapa[idOrigem].num_tropas--;
    }

    // Verifica se o território foi conquistado
    if (mapa[idDestino].num_tropas == 0) {
        printf("\n*** TERRITORIO %s CONQUISTADO! ***\n", mapa[idDestino].nome);
        strcpy(mapa[idDestino].cor_exercito, corJogador);
        mapa[idDestino].num_tropas = 1; // Move 1 tropa para o novo território
        mapa[idOrigem].num_tropas--;    // Remove a tropa que se moveu
    }
}

/**
 * @brief Sorteia e retorna um ID de missão aleatório.
 * @return Um inteiro entre 0 e (NUM_MISSOES - 1).
 */
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

/**
 * @brief Verifica se o jogador cumpriu sua missão.
 * @param mapa Ponteiro constante para o mapa (apenas leitura).
 * @param idMissao O ID da missão a ser verificada.
 * @param corJogador A cor do exército do jogador.
 * @return 1 (verdadeiro) se a missão foi cumprida, 0 (falso) caso contrário.
 */
int verificarVitoria(const Territorio *mapa, int idMissao, const char* corJogador) {
    int i;
    switch (idMissao) {
        case 0: // Destruir o exército VERMELHO
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor_exercito, VERMELHO) == 0) {
                    return 0; // Se encontrar qualquer território vermelho, a missão não foi cumprida
                }
            }
            return 1; // Se o loop terminar, não há mais territórios vermelhos

        case 1: // Conquistar 6 territórios
            int territoriosConquistados = 0;
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor_exercito, corJogador) == 0) {
                    territoriosConquistados++;
                }
            }
            return (territoriosConquistados >= 6); // Retorna verdadeiro se tiver 6 ou mais

        case 2: // Destruir o exército VERDE
            for (i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor_exercito, VERDE) == 0) {
                    return 0; // Missão incompleta
                }
            }
            return 1; // Missão cumprida

        default:
            return 0; // Missão desconhecida não pode ser cumprida
    }
}

/**
 * @brief Função utilitária para limpar o buffer de entrada do teclado (stdin).
 * Previne problemas com leituras consecutivas de scanf e getchar.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
