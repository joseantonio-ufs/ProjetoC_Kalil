// 🥇 Faça o ranking dos 10 atletas mais velhos de todos os tempos que participaram das olimpíadas, por gênero.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// itero results e pego todos os atletas (mesmo repetidos), pego seu id, ano, nome e esporte. mapeio o bios por hashkey, associo ao indice e puxo a 
// data de nascimento.  atribuo a idade ao atleta. meto um qsort e acabou;

typedef struct {
    char esporte[100];        // esporte da participação
    char atletaNome[150];     // nome do atleta
    int Id;                   // id do atleta
    int anoOlimpiada;         // ano da olimpíada
    int idade;                // idade calculada
    char genero[15];          // gênero do atleta (associado via bios)
} Atleta;

typedef struct {
    char ano[10];             // ano de nascimento
    int atletaId;             // id do atleta
    char genero[10];          // gênero vindo do bios.csv
} biosAtleta;

Atleta Parser(char str[]){
    // linhas típicas do results.csv

    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;

    // identifica apenas vírgulas que realmente separam colunas
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){}
            else{
                posVirgulas[camposLidos++] = i;
            }
        }
    }
 
    Atleta comp;

    // extrai o ano da olimpíada (sempre no início da linha)
    sscanf(str,"%d",&comp.anoOlimpiada);
    
    // extrai o nome do atleta
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
    }
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    // extrai o id do atleta
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.Id = -1;
    }
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.Id = atoi(Id);
    }

    // extrai o esporte da participação
    if(posVirgulas[7] + 1 == posVirgulas[8]){
        strcpy(comp.esporte, "Vazio");
    }
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    // retorna a participação já parseada
    return comp;
}

biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    // mesma lógica do parser do results
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
            aspas = !aspas;

        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){}
            else{
                posVirgulas[camposLidos++] = i;
            }
        }
    }
 
    biosAtleta comp;
    
    // extrai o gênero do atleta
    char sexo[15];
    if(posVirgulas[0] + 1 == posVirgulas[1]){
        strcpy(comp.genero, "Assexual");
    }
    else{
        for (int i = posVirgulas[0] + 1, z = 0; i < posVirgulas[1]; i++, z++){
            sexo[z] = str[i];
            sexo[z+1] = '\0';
        }
        strcpy(comp.genero, sexo);
    }

    // extrai o id do atleta
    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){
        comp.atletaId = -1;
    }
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

    // campo textual de nascimento
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        strcpy(comp.ano,"Vazio");
    }
    else{
        // copia o campo inteiro de nascimento
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        // busca manual por um ano (4 dígitos)
        strcpy(comp.ano,"Vazio");
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
                s = TotalIteraçoes;
            }
        }
    }

    // retorna os dados do bios
    return comp;
}

// ordenação por idade (decrescente)
int compara1(const void* a, const void* b){
    const Atleta* ia = (Atleta*)a;
    const Atleta* ib = (Atleta*)b;
    return ib->idade - ia->idade;
}

// ordenação por id para agrupar atletas iguais
int compara2(const void* a, const void* b){
    const Atleta* ia = (Atleta*)a;
    const Atleta* ib = (Atleta*)b;

    if(ia->Id - ib->Id == 0){
        return ib->idade - ia->idade;
    }
    return ia->Id - ib->Id;
}

int main(){

    // abertura dos arquivos csv
    FILE *arq = fopen("results.csv", "r");
    FILE *bios = fopen("bios.csv", "r");

    // valida abertura
    if (arq == NULL || bios == NULL) {
        puts("Error opening file"); 
        return 1;
    }
    
    int cont = 0;
    int capacidade = 5;

    // vetor dinâmico de participações
    Atleta* competidores =  malloc(sizeof(Atleta)*capacidade);
    
    char ch[3000];
    fgets(ch, sizeof(ch), arq); // ignora cabeçalho

    // leitura completa do results.csv
    while (fgets(ch, sizeof(ch), arq) != NULL){

        // realocação dinâmica
        if ((cont + 1) == capacidade){
            capacidade *= capacidade;
            Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);
            competidores = temp;
        }

        // parse da linha e armazenamento
        competidores[cont++] = Parser(ch);
    }

    int capacidade2 = 3;

    // vetor bios indexado por atletaId
    biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2);

    char newPala[3000];
    fgets(newPala, sizeof(newPala), bios); // ignora cabeçalho

    // leitura do bios.csv
    while(fgets(newPala, sizeof(newPala), bios) != NULL){ 

        biosAtleta temp = ParserBios(newPala);

        // garante índice direto pelo atletaId
        while(temp.atletaId > capacidade2 - 1){
            capacidade2 *= capacidade2;
            biosAtleta* temp2 = realloc(lista, sizeof(biosAtleta) * capacidade2);
            lista = temp2;
        }

        // associação direta id -> bios
        lista[temp.atletaId] = temp;
    }

    // cálculo da idade e cópia do gênero
    for(int k = 0, y = 0; k < cont; k++, y++){
        if(strcmp(lista[competidores[y].Id].ano, "Vazio") == 0){
            competidores[y].idade = 0;
            strcpy(competidores[y].genero,lista[competidores[y].Id].genero);
        }
        else{
            competidores[y].idade =
                competidores[y].anoOlimpiada -
                atoi(lista[competidores[y].Id].ano);
            strcpy(competidores[y].genero,lista[competidores[y].Id].genero);
        }
    }

    // ordena por id para remoção de duplicados
    qsort(competidores, cont, sizeof(Atleta), compara2);

    int ZonaSeg = 1;

    // remove atletas repetidos mantendo o mais velho
    for(int k = 1; k < cont; k++){
        if (competidores[k].Id - competidores[k-1].Id == 0){}
        else{
            competidores[ZonaSeg] = competidores[k];
            ZonaSeg++;
        }
    }

    // ajusta o tamanho final do vetor
    competidores = realloc(competidores, sizeof(Atleta) * ZonaSeg);

    // ordena definitivamente por idade
    qsort(competidores, ZonaSeg, sizeof(Atleta), compara1);

    // vetores finais por gênero
    Atleta homens[10];
    Atleta mulheres[10];

    // seleção dos top 10 por gênero
    for(int i = 0, h = 0, m = 0; i < ZonaSeg; i++){
        if(h < 10 && strcmp(competidores[i].genero, "Male") == 0){
            homens[h++] = competidores[i];
        }
        if(m < 10 && strcmp(competidores[i].genero, "Female") == 0){
            mulheres[m++] = competidores[i];
        }
        if(h == 10 && m == 10){
            i = ZonaSeg;
        }
    }

    // impressão dos homens
    printf("\n->Homens\n");
    for (int i = 0; i < 10; i++) {
        printf("Atleta: %s  Esporte: %s  Idade: %d  Sexo: Masculinho  Ano participado: %d\n",
               homens[i].atletaNome,
               homens[i].esporte,
               homens[i].idade,
               homens[i].anoOlimpiada);
    }
    
    // impressão das mulheres
    printf("\n->Mulheres\n");
    for (int i = 0; i < 10; i++) {
        printf("Atleta: %s  Esporte: %s  Idade: %d  Sexo: Feminino  Ano participado: %d\n",
               mulheres[i].atletaNome,
               mulheres[i].esporte,
               mulheres[i].idade,
               mulheres[i].anoOlimpiada);
    }

    // debug simples de leitura
    printf("\n%d", cont);

    fclose(arq);   // fecha results.csv
    fclose(bios);  // fecha bios.csv

    printf("\n\nPresione ENTER para fechar...");
    getchar(); // pausa o console
    return 0;
}
