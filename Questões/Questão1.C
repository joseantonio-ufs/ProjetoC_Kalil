// 🥇 Faça o ranking dos 10 atletas mais velhos de todos os tempos que participaram das olimpíadas, por gênero.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// A ideia geral é:
// percorrer o arquivo results.csv coletando participações (mesmo repetidas),
// associar cada atleta ao seu id, buscar o ano de nascimento no bios.csv,
// calcular a idade em cada olimpíada e depois ordenar.

// Estrutura que representa uma participação do atleta em uma olimpíada
typedef struct {
    char esporte[100];
    char atletaNome[150];
    int Id;
    int anoOlimpiada;
    int idade;
} Atleta;

// Estrutura reduzida apenas com as informações necessárias do bios.csv
typedef struct {
    char ano[10];
    int atletaId;
} biosAtleta;


// Parser de uma linha do results.csv
Atleta Parser(char str[]){
    // Exemplo de linha:
    // 1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,

    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;

    // Varre a linha identificando apenas as vírgulas que realmente separam colunas,
    // ignorando aquelas que aparecem dentro de campos entre aspas
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if (str[i] == ',' && !aspas){
            posVirgulas[camposLidos++] = i;
        }
    }
 
    Atleta comp;

    // O ano da olimpíada aparece sempre no início da linha
    sscanf(str, "%d", &comp.anoOlimpiada);
    
    // Extrai o nome do atleta entre as vírgulas correspondentes
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
    } else {
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    // Extrai o ID do atleta e converte para inteiro
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.Id = -1;
    } else {
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.Id = atoi(Id);
    }

    // Extrai o nome do esporte associado àquela participação
    if(posVirgulas[7] + 1 == posVirgulas[8]){
        strcpy(comp.esporte, "Vazio");
    } else {
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            // Copia o campo esporte caractere por caractere
            // e mantém a string sempre terminada corretamente
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    return comp;
}


// Parser de uma linha do bios.csv
biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    // Mesmo processo de identificação das vírgulas válidas
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if (str[i] == ',' && !aspas){
            posVirgulas[camposLidos++] = i;
        }
    }
 
    biosAtleta comp;

    // Captura o ID do atleta presente no bios.csv
    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){
        comp.atletaId = -1;
    } else {
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

    // Campo de nascimento vem como texto livre, por exemplo:
    // "1 April 1969 in Meulan, Yvelines (FRA)"
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        strcpy(comp.ano, "Vazio");
    } else {
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        // Procura manualmente por uma sequência de 4 dígitos,
        // assumindo que ela representa o ano de nascimento
        strcpy(comp.ano, "Vazio");
        char anoCopiado[5];
        int TotalIteracoes = strlen(nascimento);

        for(int s = 0; s < TotalIteracoes - 3; s++){
            if(isdigit(nascimento[s]) &&
               isdigit(nascimento[s+1]) &&
               isdigit(nascimento[s+2]) &&
               isdigit(nascimento[s+3])){

                anoCopiado[0] = nascimento[s];
                anoCopiado[1] = nascimento[s+1];
                anoCopiado[2] = nascimento[s+2];
                anoCopiado[3] = nascimento[s+3];
                anoCopiado[4] = '\0';

                strcpy(comp.ano, anoCopiado);
                break;
            }
        }
    }

    return comp;
}


int main(){

    FILE *arq = fopen("results.csv", "r");
    FILE *bios = fopen("bios.csv", "r");

    // Verificação básica de abertura dos arquivos
    if (arq == NULL || bios == NULL) {
        puts("Error opening file");
        return 1;
    }
    
    int cont = 0;
    int capacidade = 5;

    // Vetor dinâmico para armazenar todas as participações
    Atleta* competidores = malloc(sizeof(Atleta) * capacidade);
    
    char ch[3000];
    fgets(ch, sizeof(ch), arq); // descarta o cabeçalho

    // Leitura do results.csv com alocando espaço quando necessário (semelhante arraylist do Java)
    while (fgets(ch, sizeof(ch), arq) != NULL){

        if ((cont + 1) == capacidade){
            capacidade *= capacidade;
            Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);
            competidores = temp;
        }

        competidores[cont++] = Parser(ch);
    }

    int capacidade2 = 3;
    int num = 0;

    // Vetor dinâmico para armazenar dados do bios.csv
    biosAtleta* lista = malloc(sizeof(biosAtleta) * capacidade2);

    char newPala[500];
    fgets(newPala, sizeof(newPala), bios); // descarta o cabeçalho

    while(fgets(newPala, sizeof(newPala), bios) != NULL){ 

        if ((num + 1) == capacidade2){
            capacidade2 *= capacidade2;
            biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);
            lista = temp;
        }

        lista[num++] = ParserBios(newPala);
    }

    // Cálculo da idade do atleta na olimpíada correspondente
    for(int k = 0; k < cont; k++){
        if(strcmp(lista[competidores[k].Id].ano, "Vazio") == 0){
            competidores[k].idade = 0;
        } else {
            competidores[k].idade =
                competidores[k].anoOlimpiada -
                atoi(lista[competidores[k].Id].ano);
        }
    }
    
    return 0;
}
