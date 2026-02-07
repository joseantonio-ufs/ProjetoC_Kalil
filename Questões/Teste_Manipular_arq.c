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
    char atletaNome[150];///// struct para salvar os campos interessantes a questão 
    int atletaId;
} Atleta;

////// alteração de parser para arquivos e definicção do parser como tipo da struct (objetivo de retornar um atleta por passagem no parser)
Atleta Parser(char str[]){

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
                posVirgulas[camposLidos++] = i;/// Loop de quebra. Se encontrar uma virgula (que esteja com aspa deligada), e perceber que o próximo espaço
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
    printf("Id: %d | Nome: %s | Ano: %d | Esporte: %s\n", comp.atletaId, comp.atletaNome, comp.ano, comp.esporte);
    return comp;
}

int main() {
    // Definindo um ano de leitura, futuramente será leitura pra o usuário
    int anoSorteado = 1912;
    
    // processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
    // r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo
    //(cuidado com as barras invertidas)
    // devem ser duplas para não ver como caractere de escape
    FILE *arq = fopen("C:\\Users\\joser\\Downloads\\Projota\\results.csv", "r"); // ponteiro para o arquivo
    
    if (arq == NULL) {
        puts("Error opening file"); // retorna um erro caso o ponteiro que receberia o arquivo seja NULL
        return 1;
    }


    
    char ch[350];
    while (fgets(ch, sizeof(ch), arq) != NULL) { /// caso o ponteiro não seja NULL entramos num loop em que supomos que cada linha tenha no 
        //máximo 200 caracteres e puxamos linha a linha do csv até o fget retornar um NULL (não pegou nada) e imprimimos tudo durante o loop
        Parser(ch);
    }

    fclose(arq); // fecha o arquivo (importante!)
    return 0;
}
