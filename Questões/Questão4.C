#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//criar um array giagntesco armazenando os dados dos atletas foi mais conveniente aqui
//se não aramzenar os dados no array seriam feitas muitas iterações desnecessárisa nos csvs
//essas iterações levariam muito tempo ou exigiriam ordenações e complexidades desnecessárias
#define MAX_ID 150000

//para facilitar retorno de 2 inteiros vou aramzenar o numero de medalhistas num array
//primeiro indice contém número de homens medalhistas segundo numero de mulheres

 char generos[MAX_ID]; 

//funcao que lê o csv para preencher o mapa de generos
void mapear_generos() {
    //zera o array para evitar lixo
    memset(generos, 0, sizeof(generos));

    FILE *arquivo = fopen("bios.csv", "r");
    //se o arquivo não for encontrado exibe isso no print
    if( arquivo == NULL) {
        printf("O arquivo não foi encontrado");
        exit(1);
    }

    char linha[4096];
    fgets(linha, 4096, arquivo); //pula o cabecalho

    while (fgets(linha, 4096, arquivo)) {
        //lógica de parser
        //pegar só a primeira letra da string, é o suficiente para diferenciar genero
        int id_temp = -1; 
        char genero_temp = 0;
        int coluna_Atual = 0; //parametrizando colunas para facilitar iteração
        int i = 0;
        int j = 0;
        int dentro_aspas = 0; //variável que define se é vírgula de separação de coluna ou não
        char buffer_campo[1024]; //guarda o texto da coluna

        //percorre a linha caractere por caractere
        while (linha[i] != '\0' && linha[i] != '\n') {
            
            char caractere = linha[i];

            //como existem virgulas dentro de algumas strings como em nomes e datas
            //é preciso fazer uma prevenção ara evitar cortar no meio da string
            if (caractere == '\"') {
                //como as aspas sempre vem em duplas
                //então começa fora, depois entra, depois sai de novo
                dentro_aspas = !dentro_aspas; //lógica de inversão de entrou ou saiu das aspas
            }
            //se for uma virgula fora de aspas é um separador coluna
            else if (caractere == ',' && !dentro_aspas) {
                buffer_campo[j] = '\0'; //fecha a string do campo

                //aqui começa a captura dos dados
                //no arquivo a coluna 2 é a de genero, mas como a contagem começa do 0
                //então a coluna 1 será a do GENERO
                if (coluna_Atual == 1) {
                    //como tem chance de vir com aspas
                    //verifica se veio normal (M) ou com aspas ("M")
                    if (buffer_campo[0] == 'M' || buffer_campo[0] == 'F') {
                        genero_temp = buffer_campo[0];
                    } 
                    //se vier com aspas pega o indice 1
                    else if (j > 1 && (buffer_campo[1] == 'M' || buffer_campo[1] == 'F')) {
                        genero_temp = buffer_campo[1];
                    }
                }

                //o ID ta na coluna 8 do csv, isso seria o 7 para o compilador
                else if (coluna_Atual == 7) {
                    //converte string para int
                    //se vier com aspas, pula o primeiro termo
                    if (buffer_campo[0] == '\"') {
                        id_temp = atoi(&buffer_campo[1]);
                    } else {
                        id_temp = atoi(buffer_campo);
                    }
                    //como já tem o genero e o id não precisa ver o resto da linha
                    //serve como otimização
                    break; 
                }
            
                j = 0; //reseta a iteração 
                coluna_Atual++; //pula para a proxima coluna
            }
            //guarda caractere normal no buffer
            else {
                if (j < 1023) {
                    buffer_campo[j] = caractere;
                    j++;
                }
            }
            i++; 
        }
        //depois do processo, salva no array
        if (id_temp >= 0 && id_temp < MAX_ID && genero_temp != 0) {
            generos[id_temp] = genero_temp;
        }
        //while acaba
    }
    fclose(arquivo);
}
