// 🥉 Escolha uma edição aleatória das Olimpíadas e liste os atletas mais velhos por modalidade esportiva
//, além disso liste o total de Olimpíadas competidas pelos 3 mais velhos desta edição escolhida.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

typedef struct {
int ano;
char esporte[100];
char atletaNome[150];
int atletaId;
} Atleta;
// struct para armazenar apenas os campos relevantes do atleta no arquivo results.csv

typedef struct {
int ano;
int atletaId;
} biosAtleta;
// struct auxiliar para armazenar dados biográficos (ano de nascimento e id do atleta)

Atleta Parser(char str[]){
    //1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,
    //1912 Summer Olympics,"Doubles, Men (Olympic)",Jean Montariol,DNS,,Jean-François Blanchy,1,FRA,Tennis,,

    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;
    // controle das aspas para ignorar vírgulas que fazem parte do campo textual

    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
        aspas = !aspas;
        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){}
            else{
            posVirgulas[camposLidos++] = i;
            // salva a posição das vírgulas válidas para separar os campos do CSV
            }
        }
    }
 
    Atleta comp;
    // estrutura que irá receber os dados extraídos da linha

    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano);
    // os quatro primeiros caracteres da linha sempre representam o ano da edição
    
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
        // tratamento para campo de nome vazio
    }
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
        // copia o nome do atleta entre as posições das vírgulas
    }

    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.atletaId = -1;
        // id inválido ou ausente
    }
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
        // conversão do id textual para inteiro
    }

    if(posVirgulas[8] - posVirgulas[7] <= 1){
        strcpy(comp.esporte, "Vazio");
        // tratamento para esporte não informado
    }
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
        // extração do nome do esporte
    }

    return comp;
}

// Roles,Sex,Full name,Used name,Born,Died,NOC,athlete_id,Measurements,Affiliations,Nick/petnames,Title(s),Other names,Nationality,Original name,Name order
// arquivo bios.csv possui mais campos e estrutura diferente do results.csv

biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;
    // mesmo esquema de separação por vírgulas, agora considerando mais campos

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
    // struct que irá armazenar o id e o ano de nascimento

    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){
        comp.atletaId = -1;
        // id ausente no arquivo bios
    }
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
        // conversão do id do atleta
    }

    //"1 April 1969 in Meulan, Yvelines (FRA)"
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        comp.ano = 3000;
        // valor alto usado como fallback quando não há data
    }
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        comp.ano = 3000;
        // valor padrão antes da tentativa de extração real

        for(int s = 0; s < strlen(nascimento) - 3; s++){
            if(isdigit(nascimento[s])){
                if(isdigit(nascimento[s+1])){
                    if(isdigit(nascimento[s+2])){
                        if(isdigit(nascimento[s+3])){
                            char anoReal[5];
                            anoReal[0] = nascimento[s];
                            anoReal[1] = nascimento[s+1];
                            anoReal[2] = nascimento[s+2];
                            anoReal[3] = nascimento[s+3];
                            anoReal[4] = '\0';
                            comp.ano = atoi(anoReal);
                        }
                    }
                }
            }
        }
        // NOVO: extração manual do ano de nascimento percorrendo a string
    }

    return comp;
}

int main(){

    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
    // r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo
    //(cuidado com as barras invertidas)
    // devem ser duplas para não ver como caractere de escape

        FILE *arq = fopen("results.csv", "r");
        FILE *bios = fopen("bios.csv", "r");
        // ponteiros para os dois arquivos utilizados no programa

        if (arq == NULL || bios == NULL) {
            puts("Error opening file");
            // verificação básica de erro na abertura dos arquivos
            return 1;
        }
        
        int AnoEscolha = 2022;
        // ano da edição escolhida para filtragem dos atletas  // NOVO

        int* cont = malloc(sizeof(int));
        *cont = 0;
        // contador dinâmico de atletas encontrados

        int capacidade2 = 5;
        biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2);
        // vetor dinâmico para armazenar dados biográficos

        int capacidade = 5;
        Atleta* competidores =  malloc(sizeof(Atleta)*capacidade);
        // vetor dinâmico para armazenar atletas da edição escolhida
        
        char ch[3000];
        fgets(ch, sizeof(ch), arq);
        // descarte do cabeçalho do arquivo results.csv

        while (fgets(ch, sizeof(ch), arq) != NULL){
            // leitura linha a linha do arquivo results.csv

            if ((*cont + 1) == capacidade){
                capacidade *= capacidade;
                Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);
                // realocação dinâmica conforme crescimento do vetor  // NOVO
                competidores = temp;
            }

            int anoObservado;
            sscanf(ch, "%d", &anoObservado);
            // leitura direta do ano da linha

            if(anoObservado == AnoEscolha){
                competidores[(*cont)++] = Parser(ch);
                // apenas atletas da edição escolhida são armazenados
            }
        }

        if (*cont == 0){
            printf("\n\nNenhum dado encontrado desta edição -> %d\n\n\n", AnoEscolha);
            return 1;
            // tratamento para edição sem atletas encontrados
        }

        for (int i = 0; i < *cont; i++){
            printf("Id: %d | Nome: %s | Ano: %d | Esporte: %s\n",
                   competidores[i].atletaId,
                   competidores[i].atletaNome,
                   competidores[i].ano,
                   competidores[i].esporte);
            // impressão dos atletas filtrados
        }

        int* num = malloc(sizeof(int));
        *num = 0;
        // contador dinâmico para o arquivo bios.csv
        
        char newPala[3000];
        fgets(newPala, sizeof(newPala), bios);
        // descarte do cabeçalho do arquivo bios.csv

        while (fgets(newPala, sizeof(newPala), bios) != NULL){
            // leitura linha a linha do arquivo bios.csv

            if ((*num + 1) == capacidade2){
                capacidade2 *= capacidade2;
                biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);
                // realocação dinâmica da lista de bios  // NOVO
                lista = temp;
            }

            lista[(*num)++] = ParserBios(newPala);
            // armazenamento dos dados biográficos
        }

        for (int i = 0; i < *num; i++) {
            printf("Id: %d | Ano: %d \n", lista[i].atletaId, lista[i].ano);
            // impressão dos dados processados
        }
}
