// 🥉 Escolha uma edição aleatória das Olimpíadas e liste os atletas mais velhos por modalidade esportiva
//, além disso liste o total de Olimpíadas competidas pelos 3 mais velhos desta edição escolhida.

#include <stdio.h>
#include <string.h>//////////////////////// depois de quebrar a cabeça com 3 versões diferentes de parses essa saiu, quebrando cada linha do csv em 11 campos armazenados em arrays de strings armazenados dinâmicamente
// breve comentarei este e farei a resolução da questão
#include <stdlib.h>


typedef struct {
int ano;
char tipo[10];
char esporte[20];
char evento[20];
char atletaNome[40];
int atletaId[20];
char noc[10];
char time[40];
int posicao;
int empate;
char medalha[8];
} Atleta;



void seila(char str[]){

    char** array = malloc(sizeof(char*)*11);
    for(int i = 0; i < 11; i ++){
        array[i] =  malloc(sizeof(char)*150);
    }
    

    int posIni = -1;
    int p[11];
    int camposLidos = 0;

    for(int i = 0; i < strlen(str); i++){
        if ((str[i] == ',' && str[i+1] != ' ') || str[i] == '\n'){
            p[camposLidos++] = i;
        }
    }
    // for(int i = 0; i < 11; i++){
    //     printf("%d ", p[i]);
    // } 

    camposLidos = 0;

    for(int i = 0; i <= 10; i++){
        char campo[150];
        if (p[i] - posIni <= 1){
            strcpy(array[camposLidos++], "Vazio");
            posIni = p[i];
        }
        else{
            for(int k = posIni+1, z = 0; k < p[i]; k++, z++){
                campo[z] = str[k];
                campo[z+1] = '\0';
            }
            strcpy(array[camposLidos++], campo);
            posIni = p[i];
        }
    } 

    printf("\n");
    for(int i = 0; i <= 10; i++){
        printf("%s ", array[i]);
    } 
    free(array); 
}

int main() {

    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
    // r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo
    //(cuidado com as barras invertidas)
    // devem ser duplas para não ver como caractere de escape
        FILE *arq = fopen("C:\\Users\\joser\\Downloads\\Projota\\results.csv", "r");
    
        if (arq == NULL) {
            puts("Error opening file"); // retorna um erro caso o ponteiro que receberia o arquivo seja NULL
            return 1;
        }
    
        
        char ch[350];
        while (fgets(ch, sizeof(ch), arq) != NULL) { /// caso o ponteiro não seja NULL entramos num loop em que supomos que cada linha tenha no 
            //máximo 200 caracteres e puxamos linha a linha do csv até o fget retornar um NULL (não pegou nada) e imprimimos tudo durante o loop
            seila(ch);
        }
    
        fclose(arq); // fecha o arquivo (importante!)
        return 0;
    }
