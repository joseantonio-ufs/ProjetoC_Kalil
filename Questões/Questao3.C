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
int idade; /// add campo idade  // NOVO: campo necessário após cruzamento com bios
} Atleta;
// struct principal para armazenar dados do atleta já filtrados por edição

typedef struct {
int ano;
int atletaId;
} biosAtleta;
// struct auxiliar para armazenar ano de nascimento e id do atleta (arquivo bios)

Atleta Parser(char str[]){
    //1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,
    //1912 Summer Olympics,"Doubles, Men (Olympic)",Jean Montariol,DNS,,Jean-François Blanchy,1,FRA,Tennis,,

    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;
    // controle de aspas para não contar vírgulas internas aos campos

    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
        aspas = !aspas;
        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){}
            else{
            posVirgulas[camposLidos++] = i;
            // guarda a posição das vírgulas válidas para separação dos campos
            }
        }
    }
 
    Atleta comp;
    // atleta que será preenchido a partir da linha do CSV

    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano);
    // os quatro primeiros caracteres da linha sempre representam o ano da edição
    
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
        // tratamento para nome ausente
    }
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
        // cópia do nome do atleta
    }

    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.atletaId = -1;
        // id inválido ou inexistente
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
        // esporte não informado
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


//Roles,Sex,Full name,Used name,Born,Died,NOC,athlete_id,...
// arquivo bios.csv possui estrutura diferente e mais campos

biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;
    // mesmo esquema de separação por vírgulas, agora para o arquivo bios

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
    // struct que armazenará id e ano de nascimento

    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){
        comp.atletaId = -1;
        // id ausente
    }
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
        // leitura do id do atleta
    }

    //"1 April 1969 in Meulan, Yvelines (FRA)"
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        comp.ano = 3000;
        // fallback para data inexistente
    }
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        comp.ano = 3000;
        // valor padrão antes da extração real

        for(int s = 0; s < strlen(nascimento) - 3; s++){
            if(isdigit(nascimento[s]) && isdigit(nascimento[s+1]) &&
               isdigit(nascimento[s+2]) && isdigit(nascimento[s+3])){
                char anoReal[5];
                anoReal[0] = nascimento[s];
                anoReal[1] = nascimento[s+1];
                anoReal[2] = nascimento[s+2];
                anoReal[3] = nascimento[s+3];
                anoReal[4] = '\0';
                comp.ano = atoi(anoReal);
            }
        }
        // NOVO: extração manual do ano de nascimento percorrendo a string
    }

    return comp;
}



int compara(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    return strcmp(ia->atletaNome, ib->atletaNome);
}
// NOVO: função de comparação usada no qsort para ordenação por nome



int main(){

    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
    // r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo

        FILE *arq = fopen("results.csv", "r");
        FILE *bios = fopen("bios.csv", "r");
        // ponteiros para os dois arquivos utilizados

        if (arq == NULL || bios == NULL) {
            puts("Error opening file"); 
            return 1;
        }

        int AnoEscolha = 2022;
        // edição escolhida para filtragem dos atletas

        int* cont = malloc(sizeof(int));
        *cont = 0;
        // contador dinâmico de atletas da edição escolhida

        int capacidade2 = 5;
        biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2);
        // vetor dinâmico para dados biográficos

        int capacidade = 5;
        Atleta* competidores =  malloc(sizeof(Atleta)*capacidade);
        // vetor dinâmico para atletas da edição
        
        char ch[3000];
        fgets(ch, sizeof(ch), arq);
        // descarte do cabeçalho do results.csv

        while (fgets(ch, sizeof(ch), arq) != NULL){

            if ((*cont + 1) == capacidade){
                capacidade *= capacidade;
                Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);///////////////////////// LENTO AINDA
                competidores = temp;
            }

            int anoObservado;
            sscanf(ch, "%d", &anoObservado);
            // leitura direta do ano da linha

            if(anoObservado == AnoEscolha){
                competidores[(*cont)++] = Parser(ch);
                // guarda apenas atletas da edição escolhida
            }
        }

        if (*cont == 0){
            printf("\n\nNenhum dado encontrado desta edição -> %d\n\n\n", AnoEscolha);
            return 1;
        }

        int* num = malloc(sizeof(int));
        *num = 0;
        // contador para o arquivo bios
        
        char newPala[3000];
        fgets(newPala, sizeof(newPala), bios);
        // descarte do cabeçalho do bios.csv

        while (fgets(newPala, sizeof(newPala), bios) != NULL){

            if ((*num + 1) == capacidade2){
                capacidade2 *= capacidade2;
                biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);///////////////////////// LENTO AINDA
                lista = temp;
            }

            lista[(*num)++] = ParserBios(newPala);
            // armazenamento dos dados biográficos
        }

        qsort(competidores, *cont, sizeof(Atleta), compara); 
        // NOVO: ordenação para facilitar remoção de duplicados

        /// UNICIDADE
        int zonaSeg = 1;
        for(int k = 1; k < *cont; k++){
            if (strcmp(competidores[k].esporte, competidores[k-1].esporte) == 0 &&
                competidores[k].atletaId - competidores[k-1].atletaId == 0){}
            else{
                competidores[zonaSeg] = competidores[k];
                zonaSeg++;//// retira atletas de mesmo id e mesmo esporte
            }
        }

        Atleta* temp = realloc(competidores, sizeof(Atleta) * zonaSeg);
        competidores = temp;
        // NOVO: compactação do vetor após remoção de duplicados

        for(int k = 0; k < *num; k++){
            int quebraloop = 1;
            int itera = zonaSeg - 1;
            // varredura reversa para cruzamento entre competidores e bios

            while(quebraloop){
                if(lista[k].atletaId == competidores[itera].atletaId)
                competidores[itera].idade = AnoEscolha - lista[k].ano;
                // NOVO: cálculo da idade a partir do ano de nascimento

                itera--;

                if (itera == -1)
                quebraloop = 0;
            }
        }

        for (int i = 0; i < zonaSeg; i++){
            printf("Id: %d | Nome: %s | Ano: %d | Esporte: %s  Idade: %d\n",
                   competidores[i].atletaId,
                   competidores[i].atletaNome,
                   competidores[i].ano,
                   competidores[i].esporte,
                   competidores[i].idade);
            // impressão final dos atletas com idade calculada
        }
}
