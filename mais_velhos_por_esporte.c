// 🥉 Escolha uma edição aleatória das Olimpíadas e liste os atletas mais velhos por modalidade esportiva
//, além disso liste o total de Olimpíadas competidas pelos 3 mais velhos desta edição escolhida.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// Estrutura que representa um atleta já associado a uma edição olímpica
typedef struct {
    int ano;                    // ano da edição olímpica
    char esporte[100];          // modalidade esportiva
    char atletaNome[150];       // nome do atleta
    int atletaId;               // id único do atleta
    int idade;                  // idade calculada posteriormente
} Atleta;

// Estrutura auxiliar para cruzar dados biográficos do atleta
typedef struct {
    int ano;                    // ano de nascimento
    int atletaId;               // id do atleta
} biosAtleta;


// Parser responsável por extrair dados do arquivo results.csv
// Faz leitura manual usando posições de vírgula para evitar erros com campos entre aspas
Atleta Parser(char str[]){

    // Vetor para armazenar as posições das vírgulas relevantes
    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;

    // Percorre a string identificando vírgulas válidas (fora de aspas)
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

    // Extrai o ano da edição olímpica (primeiros 4 caracteres da linha)
    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
        ano[z] = str[i];
        ano[z+1] = '\0';
    }
    comp.ano = atoi(ano);
    
    // Extrai o nome do atleta
    // Caso o campo esteja vazio, marca como "Vazio"
    if(posVirgulas[4] + 1 == posVirgulas[5]){
        strcpy(comp.atletaNome, "Vazio");
    }
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    // Extrai o id do atleta
    // Se o campo estiver vazio, define id como -1
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.atletaId = -1;
    }
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

    // Extrai o esporte associado ao atleta
    // Verifica se o campo realmente possui conteúdo
    if(posVirgulas[8] - posVirgulas[7] <= 1){
        strcpy(comp.esporte, "Vazio");
    }
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    return comp;
}


// Parser responsável pelo arquivo bios.csv
// NOVO: leitura separada de dados biográficos para cálculo de idade
biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    // Identifica as vírgulas fora de campos com aspas
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

    // Extrai o id do atleta
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

    // Extrai o ano de nascimento a partir do campo textual
    // Caso não encontre um ano válido, assume valor alto para evitar cálculo errado
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        comp.ano = 3000;
    }
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        comp.ano = 3000;
        // Procura manualmente uma sequência de 4 dígitos representando o ano
        for(int s = 0; s < strlen(nascimento) - 3; s++){
            if(isdigit(nascimento[s]) &&
               isdigit(nascimento[s+1]) &&
               isdigit(nascimento[s+2]) &&
               isdigit(nascimento[s+3])){
                
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

    return comp;
}


// Função auxiliar para ordenação crescente de inteiros
int compararInteiros(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}


// NOVO: calcula quantas olimpíadas os atletas mais velhos participaram
void maisVelhos(Atleta array[], int zonaSeg, FILE* arq){
    
    // Analisa apenas os 3 atletas mais velhos
    for(int j = 0; j < 3; j++){

        int listAnos[500];
        int i = 0;
        rewind(arq); // volta para o início do arquivo results.csv

        char str[3000];
        fgets(str, sizeof(str), arq); // ignora cabeçalho
        while (fgets(str, sizeof(str), arq) != NULL){ 

            int posVirgulas[10];
            int camposLidos = 0;
            int aspas = 0;

            // Identifica as vírgulas válidas da linha
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

            // Extrai o id do atleta da linha
            char Id[10];
            if(posVirgulas[5] + 1 == posVirgulas[6]){
                strcpy(Id, "0");
            }
            else{
                for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
                    Id[z] = str[i];
                    Id[z+1] = '\0';
                }
            }

            int idReal = atoi(Id);

            // Se o id bate com o atleta analisado, guarda o ano da edição
            if(array[j].atletaId == idReal)
                sscanf(str, "%d", &listAnos[i++]);
        }

        // Ordena os anos das participações
        qsort(listAnos, i, sizeof(int), compararInteiros);

        // Remove anos duplicados para contar participações únicas
        int posSegura = 1;
        for(int k = 1; k < i; k++){
            if(listAnos[k] == listAnos[k-1]){}
            else{
                listAnos[posSegura++] = listAnos[k];
            }
        }

        printf("\nAtleta: %s  Idade: %d  Olimpíadas participadas: %d",
               array[j].atletaNome, array[j].idade, posSegura);
    }
}


// Funções de comparação usadas pelo qsort
int compara(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    return strcmp(ia->atletaNome, ib->atletaNome);
}

// Ordena por esporte e, dentro dele, pela maior idade
int compara2(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    if (strcmp(ia->esporte, ib->esporte) == 0)
        return ib->idade - ia->idade;

    return strcmp(ia->esporte, ib->esporte);
}

// Ordena apenas pela idade decrescente
int compara3(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    return ib->idade - ia->idade;
}


int main(){

    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE
    // passando como argumento o nome do arquivo e o modo de abertura
    // r - leitura, w - sobrescrita, a - escrita ao final
    // r+ w+ a+ permitem leitura e escrita
    //(cuidado com as barras invertidas)
    // devem ser duplas para não serem interpretadas como caractere de escape

        FILE *arq = fopen("results.csv", "r");
        FILE *bios = fopen("bios.csv", "r");
        // abertura dos dois arquivos CSV utilizados no programa

        if (arq == NULL || bios == NULL) {
            puts("Error opening file"); 
            // validação básica para garantir que os arquivos foram abertos corretamente
            return 1;
        }
        

        int AnoEscolha = 2022;
        // edição das Olimpíadas usada como filtro principal do programa

        // if (AnoEscolha%2 != 0 || AnoEscolha > 2022 || AnoEscolha < 1896){
        //     printf("Ano Inválido");
        //     return 0;
        // }
        // validação comentada para restringir anos inválidos ou fora do intervalo olímpico


        int* cont = malloc(sizeof(int));
        *cont = 0;
        // contador dinâmico que armazena a quantidade de atletas da edição escolhida

        int capacidade2 = 5;
        biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2);
        // vetor dinâmico para armazenar os dados do arquivo bios.csv

        int capacidade = 5;
        Atleta* competidores =  malloc(sizeof(Atleta)*capacidade);
        // vetor dinâmico para armazenar atletas filtrados do results.csv
        
        char ch[3000];
        fgets(ch, sizeof(ch), arq);
        // leitura e descarte do cabeçalho do arquivo results.csv

        while (fgets(ch, sizeof(ch), arq) != NULL){
            // leitura linha a linha do arquivo results.csv até atingir EOF

            if ((*cont + 1) == capacidade){
                capacidade *= capacidade;
                // crescimento exponencial do vetor para evitar reallocs constantes
                Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);///////////////////////// LENTO AINDA
                competidores = temp;
            }

            int anoObservado;
            sscanf(ch, "%d", &anoObservado);
            // leitura direta do ano da edição presente na linha do CSV

            if(anoObservado == AnoEscolha){
                competidores[(*cont)++] = Parser(ch);
                // armazena apenas os atletas que pertencem à edição escolhida
            }
        }


        if (*cont == 0){
            printf("\n\nNenhum dado encontrado desta edição -> %d\n\n\n", AnoEscolha);
            // tratamento para caso não existam atletas na edição escolhida
            return 1;
        }


        int* num = malloc(sizeof(int));
        *num = 0;
        // contador dinâmico para o número de registros do arquivo bios.csv
        
        char newPala[3000];
        fgets(newPala, sizeof(newPala), bios);
        // leitura e descarte do cabeçalho do arquivo bios.csv

        while (fgets(newPala, sizeof(newPala), bios) != NULL){
            // leitura linha a linha do arquivo bios.csv

            if ((*num + 1) == capacidade2){
                capacidade2 *= capacidade2;
                // crescimento do vetor biográfico conforme necessário
                biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);///////////////////////// LENTO AINDA
                lista = temp;
            }

            lista[(*num)++] = ParserBios(newPala);
            // armazena id do atleta e ano de nascimento extraído do bios.csv
        }


        qsort(competidores, *cont, sizeof(Atleta), compara);
        // ordenação inicial por nome para facilitar remoção de duplicados

        /// UNICIDADE
        int zonaSeg = 1;
        // índice de escrita para compactar atletas únicos

        for(int k = 1; k < *cont; k++){
            if (strcmp(competidores[k].esporte, competidores[k-1].esporte) == 0 &&
                competidores[k].atletaId - competidores[k-1].atletaId == 0){}
            else{
                competidores[zonaSeg] = competidores[k];
                zonaSeg++;
                // mantém apenas atletas únicos por id + esporte
            }
        }

        Atleta* temp = realloc(competidores, sizeof(Atleta) * zonaSeg);
        competidores = temp;
        // redimensiona o vetor após a remoção de duplicados
        

        for(int k = 0; k < *num; k++){
            int quebraloop = 1;
            int itera = zonaSeg - 1;
            // varredura reversa para cruzamento entre atletas e dados biográficos

            while(quebraloop){
                if(lista[k].atletaId == competidores[itera].atletaId)
                competidores[itera].idade = AnoEscolha - lista[k].ano;
                // cálculo da idade com base no ano da edição e ano de nascimento

                itera--;

                if (itera == -1)
                quebraloop = 0;
                // controle manual de término do loop
            }
        }


        qsort(competidores, zonaSeg, sizeof(Atleta), compara2);
        // ordena por esporte e, dentro dele, por idade decrescente


        int segundaZonaSeg =1;
        // índice para manter apenas o atleta mais velho de cada esporte

        for(int k = 1; k < zonaSeg; k++){
            if (strcmp(competidores[k].esporte, competidores[k-1].esporte) == 0){}
            else{
                competidores[segundaZonaSeg] = competidores[k];
                // mantém apenas o mais velho de cada modalidade
                segundaZonaSeg++;
            }
        }

        Atleta* temp2 = realloc(competidores, sizeof(Atleta) * segundaZonaSeg);
        competidores = temp2;
        // compactação final do vetor por esporte
        
        qsort(competidores, segundaZonaSeg, sizeof(Atleta), compara3);
        // ordenação final apenas por idade (do mais velho para o mais novo)
        

        for (int i = 0; i < segundaZonaSeg; i++) {
            if (competidores[i].idade < 0){
                printf("\n%d - Esporte: %s  Nome: %s  Idade: Sem idade  Ano: %d  Id:  %d\n",
                       i,
                       competidores[i].esporte,
                       competidores[i].atletaNome,
                       competidores[i].ano,
                       competidores[i].atletaId);
                // tratamento para atletas sem idade válida
            }
            else
            printf("\n%d - Esporte: %s  Nome: %s  Idade: %d  Ano: %d  Id:  %d\n",
                   i+1,
                   competidores[i].esporte,
                   competidores[i].atletaNome,
                   competidores[i].idade,
                   competidores[i].ano,
                   competidores[i].atletaId);
            // impressão dos atletas mais velhos por modalidade
        }

        maisVelhos(competidores, segundaZonaSeg, arq);
        // cálculo do número de Olimpíadas disputadas pelos 3 atletas mais velhos

        printf("\n\n%d\n\n", *cont);
        // exibe o total bruto de atletas encontrados na edição escolhida
    
        fclose(arq);
        fclose(bios);
        // fechamento obrigatório dos arquivos

        return 0;
}

