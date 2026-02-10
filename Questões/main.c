#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

// Bloco que ajeita os caracteres nao lidos direito no Windows
#ifdef _WIN32
    #include <windows.h>
#endif

#include "arquivo.h"

// Funcao para limpar a tela em qualquer sistema
void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // --- CONSERTAR OS CARACTERES ---
    setlocale(LC_ALL, ""); // Configura para o padrao do sistema
    #ifdef _WIN32
        SetConsoleOutputCP(65001); // Forcando UTF-8 no Windows (Corrigindo 'Kindelan')
    #endif
    // ------------------------------------------

    //Abrindo os arquivos results.csv e o bios.csv
    FILE *arq = fopen("results.csv", "r");
    FILE *bios = fopen("bios.csv", "r");

    //Verificando o caso dos arquivos nao conseguirem ser lidos
    if (arq == NULL || bios == NULL) {
        printf("ERRO: Nao foi possivel abrir os arquivos CSV.\n");
        printf("Pressione Enter para sair...");
        getchar();
        return 1;
    }

    //Criando as variaveis que serao usadas para a implementacao da logica do main
    int opcao;
    char continuar = 'Y';

    do {
        limpar_tela();
        //Criando o hud de apresentacao das opcoes, onde o usuario sera instruido sobre qual opcao ele deve digitar para compilar a questao desejada
        printf("====================================================================\n");
        printf("                    SISTEMA DE ANALISE OLIMPICA                     \n");
        printf("====================================================================\n\n");
        
        printf("  [1] Ranking: 10 Atletas Mais Velhos (Geral)\n");
        printf("  [2] Eficiencia: Razao Medalhas vs Atletas (Top 10 Paises)\n");
        printf("  [3] Modalidade: Mais Velhos por Esporte em uma Edicao\n");
        printf("  [4] Genero: Comparativo de Medalhas (4 Paises)\n");
        printf("  [0] Sair\n");
        
        printf("\n--------------------------------------------------------------------\n");
        printf("Digite sua opcao: ");
        //Entrada para o usuario digitar qual questao ele deseja obter o resultado
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            opcao = -1;
        }
        
        printf("\n");
         
        //Implementacao do switch para analisar o que o codigo deve fazer a depender da opcao que o usuario escolher
        //Os rewinds servem para garantir que o arquivo sera sempre lido do inicio a cada iteracao da questao 
        switch (opcao) {
            case 1:
                rewind(arq); rewind(bios);
                gestao_q1(arq, bios);
                break;
            case 2:
                rewind(arq); 
                gestao_q2(arq);
                break;
            case 3:
                rewind(arq); rewind(bios);
                gestao_q3(arq, bios);
                break;
            case 4:
                rewind(arq); rewind(bios);
                gestao_q4(arq, bios);
                break;
            case 0:
                continuar = 'N';
                break;
            default:
                printf("Opcao invalida!\n");
        }

        //Apos rodar a questao, caso o numero escolhido pelo usuario nao seja 0, ele podera escolher se almeija ver os resultados de outra questao. Caso ele digite "Y" ou "y", ele podera escolher qual questao ele quer. Caso "N" ou "n", ai o programa para
        if (opcao != 0) {
            printf("\n--------------------------------------------------------------------\n");
            printf("Deseja realizar outra consulta? [Y/N]: ");
            scanf(" %c", &continuar);
            continuar = toupper(continuar);
        }

    } while (continuar == 'Y');

    //Fechando ambos os arquivos da bios e do results
    fclose(arq);
    fclose(bios);
    
    return 0;
}
