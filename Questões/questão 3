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
char atletaNome[150]; ///// struct para salvar os campos interessantes a questão
int atletaId;
} Atleta;
////// alteração de parser para arquivos diferentres e criação de struct para atletas

typedef struct {
int ano;
int atletaId;
} biosAtleta; ////// struct auxiliar para armazenar dados biográficos do atleta (ano de nascimento e id)



////// alteração de parser para arquivos e definicção do parser como tipo da struct (objetivo de retornar um atleta por passagem no parser)
Atleta Parser(char str[]){
    //1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,
    //1912 Summer Olympics,"Doubles, Men (Olympic)",Jean Montariol,DNS,,Jean-François Blanchy,1,FRA,Tennis,,

    // Aqui define onde as vírgulas aparecem pra separar as colunas do CSV
    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0; // especie de interruptor feito para evitar vigulas dentro do próprio campo (apatrentemente o csv só tem virgulas dentro dos campos entre aspas)

    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"')
        aspas = !aspas;
        if ((str[i] == ',' && str[i+1] != ' ')){
            if(aspas){}
            else{
            posVirgulas[camposLidos++] = i; /// Loop de quebra. Se encontrar uma virgula (que esteja com aspa deligada), e perceber que o próximo espaço
            // não é vazio, então incrementa
            }
        }
    }
 
    Atleta comp; // criamos o atleta

    // Pegando os 4 primeiros caracteres da linha que são sempre o ano e passando pro atleta
    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano);
    
    // Verificando se o campo do nome tá vazio ou se tem conteúdo entre as vírgulas 4 e 5
    if(posVirgulas[4] + 1 == posVirgulas[5]){strcpy(comp.atletaNome, "Vazio");}
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    // Mesmo esquema aqui, mas pegando o id do atleta e convertendo pra inteiro
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.atletaId = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

    // Finalizando com o nome do esporte
    if(posVirgulas[8] - posVirgulas[7] <= 1){strcpy(comp.esporte, "Vazio");}
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    // note que sempre há um tratamento para caso o campo seja vazio

   //---------------------------------------------------------------------------------------

    // Esse printf é só pra testar se o parser tá pegando
    printf("ID: %-6d | Nome: %-30s | Ano: %d | Esporte: %-20s\n", comp.atletaId, comp.atletaNome, comp.ano, comp.esporte);
    return comp;
}



////// parser específico para o arquivo bios.csv, retornando dados biográficos do atleta
biosAtleta ParserBios(char str[]){

    // Aqui define onde as vírgulas aparecem pra separar as colunas do CSV (arquivo bios tem mais campos)
    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

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
 
    biosAtleta comp; // criamos a struct de bios

    // Pegando o id do atleta e convertendo pra inteiro
    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){comp.atletaId = -1;}
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

    //"1 April 1969 in Meulan, Yvelines (FRA)"
    // tratamento do campo de nascimento para extrair apenas o ano
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){comp.ano = 3000;}
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }
        if(nascimento[0] == '"')
        sscanf(nascimento, "\"%*d %*s %d", &comp.ano);
        else
        sscanf(nascimento, "%*d %*s %d", &comp.ano);
    }

    // printf apenas para depuração do parser de bios
    printf("Id: %d | Ano: %d \n", comp.atletaId, comp.ano, comp.ano);
    return comp;
}



int main() {

    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
    // r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo
    //(cuidado com as barras invertidas)
    // devem ser duplas para não ver como caractere de escape
        FILE *arq = fopen("results.csv", "r"); // ponteiro para o arquivo principal
        FILE *bios = fopen("bios.csv", "r");   // ponteiro para o arquivo de bios

        if (arq == NULL) {
            puts("Error opening file"); // retorna um erro caso o ponteiro que receberia o arquivo seja NULL
            return 1;
        }
    
        char ch[3000]; ////////// aumentando tamanho do char para linhas grandes
        while (fgets(ch, sizeof(ch), arq) != NULL) { /// caso o ponteiro não seja NULL entramos num loop em que supomos que cada linha tenha no 
            //máximo o tamanho do buffer e puxamos linha a linha do csv até o fget retornar um NULL (não pegou nada)
            Parser(ch);
        }
    
        char newPala[3000];
        fgets(newPala, sizeof(newPala), bios); // descartando cabeçalho do arquivo bios
        while (fgets(newPala, sizeof(newPala), bios) != NULL){ /// loop de leitura linha a linha do arquivo bios
            ParserBios(newPala);
        }

        fclose(arq); // fecha o arquivo (importante!)
        return 0;
}
