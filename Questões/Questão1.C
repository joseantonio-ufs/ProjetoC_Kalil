// 🥇 Faça o ranking dos 10 atletas mais velhos de todos os tempos que participaram das olimpíadas, por gênero.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// itero results e pego todos os atletas (mesmo repetidos), pego seu id, ano, nome e esporte.
// mapeio o bios por hashkey, associo ao indice e puxo a data de nascimento.
// atribuo a idade ao atleta. meto um qsort e acabou;

// Estrutura que representa uma participação do atleta em uma olimpíada
// Cada linha do results.csv vira exatamente um Atleta
typedef struct {
    char esporte[100];      // nome do esporte associado àquela participação
    char atletaNome[150];   // nome completo do atleta
    int Id;                 // id único do atleta (athlete_id do csv)
    int anoOlimpiada;       // ano da olimpíada daquela participação
    int idade;              // idade calculada do atleta naquele ano
    char genero;            // NOVO: gênero extraído diretamente da linha do results
} Atleta;

// Estrutura mínima do bios.csv
// Funciona como uma tabela de acesso rápido pelo atletaId
typedef struct {
    char ano[10];           // ano de nascimento (string para tratar "Vazio")
    int atletaId;           // chave do atleta
} biosAtleta;


// Parser de uma linha do results.csv
Atleta Parser(char str[]){
    // Exemplos de linhas:
    // 1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,
    // 1912 Summer Olympics,"Doubles, Men (Olympic)",Jean Montariol,DNS,,Jean-François Blanchy,1,FRA,Tennis,,

    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;

    // Varre a linha inteira identificando apenas as vírgulas que realmente separam colunas
    // Vírgulas dentro de campos entre aspas são ignoradas
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){
                // vírgula interna a campo textual, ignora
            } else {
                posVirgulas[camposLidos++] = i;
            }
        }
    }
 
    Atleta comp;

    // O ano da olimpíada sempre aparece no início da linha
    sscanf(str,"%d",&comp.anoOlimpiada);
    
    // Nome do atleta (campo entre posVirgulas[4] e posVirgulas[5])
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
    } else {
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    // NOVO: extração direta do gênero
    // O gênero aparece logo após o primeiro campo textual
    // Caso esteja vazio, marca como 'N'
    if(posVirgulas[0] + 1 == posVirgulas[1]){
        comp.genero = 'N';
    } else {
        comp.genero = str[posVirgulas[0]+1];
    }

    // ID do atleta (athlete_id)
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

    // Esporte associado à participação
    if(posVirgulas[7] + 1 == posVirgulas[8]){
        strcpy(comp.esporte, "Vazio");
    } else {
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
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

    // Identificação das vírgulas válidas (fora de aspas)
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){
                // ignora vírgulas internas
            } else {
                posVirgulas[camposLidos++] = i;
            }
        }
    }
 
    biosAtleta comp;

    // ID do atleta no bios.csv
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

    // Campo de nascimento vem como texto livre
    // Exemplo: "1 April 1969 in Meulan, Yvelines (FRA)"
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        strcpy(comp.ano,"Vazio");
    } else {
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        // Inicialmente marca como Vazio
        strcpy(comp.ano,"Vazio");

        // Busca manualmente qualquer sequência de 4 dígitos
        // Assume que a primeira encontrada é o ano de nascimento
        char anoCopiado[5];
        int TotalIteraçoes = strlen(nascimento);

        for(int s = 0; s < TotalIteraçoes - 3; s++){
            if(isdigit(nascimento[s]) &&
               isdigit(nascimento[s+1]) &&
               isdigit(nascimento[s+2]) &&
               isdigit(nascimento[s+3])){

                anoCopiado[0] = nascimento[s];
                anoCopiado[1] = nascimento[s+1];
                anoCopiado[2] = nascimento[s+2];
                anoCopiado[3] = nascimento[s+3];
                anoCopiado[4] = '\0';

                strcpy(comp.ano,anoCopiado);
                s = TotalIteraçoes; // força saída do loop
            }
        }
    }
    return comp;
}


// Função de comparação para o qsort
// Ordena do mais velho para o mais novo
int compara(const void* a, const void* b){
    const Atleta* ia = (Atleta*)a;
    const Atleta* ib = (Atleta*)b;
    return ib->idade - ia->idade;
}

int main(){

    FILE *arq = fopen("results.csv", "r");
    FILE *bios = fopen("bios.csv", "r");

    // Verificação básica de abertura de arquivos
    if (arq == NULL || bios == NULL) {
        puts("Error opening file");
        return 1;
    }
    
    int cont = 0;
    int capacidade = 5;

    // Vetor dinâmico que armazena TODAS as participações (inclusive repetidas)
    Atleta* competidores =  malloc(sizeof(Atleta)*capacidade);
    
    char ch[3000];
    fgets(ch, sizeof(ch), arq); // descarta cabeçalho

    // Leitura completa do results.csv
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

    // NOVO: vetor bios funciona como um hashmap direto
    // índice == atletaId
    biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2);

    char newPala[3000];
    fgets(newPala, sizeof(newPala), bios); // descarta cabeçalho

    while(fgets(newPala, sizeof(newPala), bios) != NULL){ 

        if ((num + 1) == capacidade2){
            capacidade2 *= capacidade2;
            biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);
            lista = temp;
        }

        biosAtleta temp = ParserBios(newPala);

        // NOVO: garante que o vetor comporte diretamente o atletaId como índice
        while(temp.atletaId > capacidade2 - 1){
            capacidade2 *= capacidade2;
            biosAtleta* temp2 = realloc(lista, sizeof(biosAtleta) * capacidade2);
            lista = temp2;
        }

        // NOVO: acesso direto por índice (hash simples)
        lista[temp.atletaId] = temp;
    }

    // Cálculo da idade do atleta na respectiva olimpíada
    for(int k = 0, y = 0; k < cont; k++, y++){
        if(strcmp(lista[competidores[y].Id].ano, "Vazio") == 0){
            competidores[y].idade = 0;
        } else {
            competidores[y].idade =
                competidores[y].anoOlimpiada -
                atoi(lista[competidores[y].Id].ano);
        }
    }

    // Ordenação global por idade decrescente
    qsort(competidores, cont, sizeof(Atleta), compara);

    // Impressão dos mais velhos (top 20 para inspeção)
    for (int i = 0; i < 20; i++) {
        printf("%d - Esporte: %s  Nome: %s  idade: %d\n",
               i,
               competidores[i].esporte,
               competidores[i].atletaNome,
               competidores[i].idade);
    }

    printf("\n%d", cont);// teste para saber se leu todo csv

    fclose(arq);
    fclose(bios);
    return 0;
}
