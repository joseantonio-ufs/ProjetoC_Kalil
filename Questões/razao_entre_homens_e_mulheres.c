#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Nota: pensei em abstrair o que há de enssencial entre os parsers nas funções e criar outra
//função parser para chamar ela nas demais funções ao usar, mas pesquisando imaginei que isso
//apesar de diminuir código escrito podria adicioanr complexidade de operações desnecessarias
//e poderia causar problemas de overhead ao repetir várias operações de salvar contexto e
//executar funções, além de que apesar das semlhanças o trabalho de criar um parser
//personalizavel em funções que fazem coisas diferentes também complcairia desnecessariamente
//também precisaria de estruturas de ponteiros e callbacks que podem ser evitados


//criar um array giagntesco armazenando os dados dos atletas foi mais conveniente aqui
//se não aramzenar os dados no array seriam feitas muitas iterações desnecessárisa nos csvs
//essas iterações levariam muito tempo ou exigiriam ordenações e complexidades desnecessárias
#define MAX_ID 150000


//para facilitar retorno de 2 inteiros vou aramzenar o numero de medalhistas num array
//primeiro indice contém número de homens medalhistas segundo numero de mulheres

 char generos[MAX_ID]; 

//funcao que lê o csv para preencher o mapa de generos
void mapear_generos(FILE *arquivoBios) {
    //zera o array para evitar lixo
    memset(generos, 0, sizeof(generos));

    //definir e resetar o arquivo apra iterar do zero
    FILE *arquivo = arquivoBios;
    //se o arquivo não for encontrado exibe isso no print
    if( arquivo == NULL) {
        printf("O arquivo não foi encontrado");
        exit(1);
    }
    rewind(arquivo);

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
    //fclose(arquivo);
}



//responsavel por iterar pelo csv e pelo array para contar os medalhistas
void contar_medalhistas(char *pais_alvo, int *resultado, FILE *arquivoResultados) {
    //o primeiro indice conta as medalhas para homens e o segundo para mulheres
    //define como 0 para limpar lixo ou resetar e garantir funcionamento do imcremento
    resultado[0] = 0;
    resultado[1] = 0;

    //definir e resetar o arquivo apra iterar do zero
    FILE *arquivo = arquivoResultados;
    if (arquivo == NULL) {
        printf("Erro ao abrir results.csv\n");
        exit(1);
    }
    rewind(arquivo);

    char linha[4096]; 
    fgets(linha, 4096, arquivo); //pula o cabeçalho

    while (fgets(linha, 4096, arquivo)) {
        
        //os dados serão guardados em variaveis temporarias (temp)
        //minemonico pois temp é autoexlicativo e está bem explicado no comentário
        int id_atleta = -1;
        char noc_temp[10] = "";     
        char medalha_temp[20] = ""; 

        //variaveis do parser
        int i = 0, j = 0, coluna_atual = 0; 
        int dentro_aspas = 0; //importante para lidar com as aspas que vem no arquivo
        char buffer[1024];

        while (linha[i] != '\0' && linha[i] != '\n') {
            char caractere = linha[i];
            
            //se for o \r do windows reinicia o loop direto
            //serve para evitar bugs limpando \r que possam vir no buffer
            if (caractere == '\r') {
                i++;
                continue;
            }

            //se for aspas aplica lógica de inversão de True/False para se estar dentro ou fora
            //de string
            if (caractere == '\"') {
                dentro_aspas = !dentro_aspas;
            }
            
            else if (caractere == ',' && !dentro_aspas) {
                buffer[j] = '\0'; //fecha string

                
                //coluna 4 é a medalha, se tiver vazia então não ganhou
                if (coluna_atual == 4) {
                    strncpy(medalha_temp, buffer, 19);
                }

                //coluna 6 é o id do atleta, deve ser salvo e reaproveitado para depois
                //conferir o genero, salvo no array gigante
                else if (coluna_atual == 6) {
                    if (buffer[0] == '\"') {
                        id_atleta = atoi(&buffer[1]);
                    } else {
                        id_atleta = atoi(buffer);
                    }
                }

                //o NOC é a coluna 7
                else if (coluna_atual == 7) {
                    //se tiver aspas, tira
                    if (buffer[0] == '\"') {
                        strncpy(noc_temp, &buffer[1], 9);
                    } else {
                        strncpy(noc_temp, buffer, 9);
                    }
                    
                    //strchr vai encontrar o primeiro caractere igual a "
                    //serve para limpar aspas acidentais que possam ser caoturadas
                    //é criado um ponteiro apontando para o endereço desse caractere
                    //depois o caractere é zerado, definido  como \0, fim da string
                    char *ponteiro = strchr(noc_temp, '\"');
                    if (ponteiro) {
                        *ponteiro = 0;
                    }
                    
                    //para não estender mais que o necessário é melhor usar um break
                    //serve como otimização
                    break; 
                }

                j = 0;
                coluna_atual++;
            }
            else {
                if (j < 1023) buffer[j++] = caractere;
            }
            i++;
        }



        //verificar se é o pais certo, se for diferente pula para o próximo da lista
        if (strcmp(noc_temp, pais_alvo) != 0) {
            continue;
        }

        //Verificar se ganhou medalha, os atletas sem medalha tem esse campo vazio(,,) no 
        //results.csv, tambem tem que verificar se a string tem tamanho menor que 2 ou é "NA"
        if (strlen(medalha_temp) < 2 || strcmp(medalha_temp, "NA") == 0) {
            continue;
        }

        //Verificar o genero usando o ID
        //essa parte vai aproveitar o array gigante que foi preenchido de forma auxiliar
        //para economizar iterações, baseado no id do atleta medalhista
        //confere o array de generos, cuja posiçção é igual ao ID para verificar o genero
        if (id_atleta >= 0 && id_atleta < MAX_ID) {
            char genero = generos[id_atleta];
        
            
            if (genero == 'M') {
                resultado[0]++;
            } else if (genero == 'F') {
                resultado[1]++;
            }
        }
    }
    //fclose(arquivo);
}



//função responsável por converter nome do país no código NOC
//isso garantemelhor precisão na busca para países que itveram mudanças durante a historia
char* converter_nome(char nome_pais[]) {
    //variável estática para que não seja destruida após a execução da função
    //armazena e retorna o código NOC desejado
    static char codigo_encontrado[5];

    //inicialmente o NOC é definido como NADA, se o pais for encontrado ele é redefinido
    //como a variável é static, é preciso atribuir o valor a ela fora de sua definição
    //para que possa ser resetada caso a função seja chamada mais de uma vez
    strcpy(codigo_encontrado, "NADA");

    FILE *f = fopen("noc_regions.csv", "r");
    //evitar bug caso não tenha arquivo
    if (f == NULL) {
        printf("csv não encontrado \n");
        return codigo_encontrado;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), f); //pula cabeçalho


    while (fgets(linha, sizeof(linha), f)) {
        //guardar em variáveis temporarias (temp) os valores
        char *ponteiro = linha;
        //importante limpar as variáveis temporárias com ="" para não bugar com lixo
        char noc_temp[10] = "";
        char regiao_temp[50] = "";
        char notes_temp[50] = "";
        int tamanho_coluna; //ajuda o parser aramzenando os passos em cada coluna

        //primeiro pegar o código NOC para converter o nome do pass em um código
        //o strcspn conta quantos caracteres tem até chegar na próxima vírgula
        //calcula o tamanho da coluna  
        tamanho_coluna = strcspn(ponteiro, ","); 
        
        //como o NOC vem antes do pais do csv, então caso ele exista é armazenado numa variável
        //temporaria para ser reaproveitado caso seja do pais alvo
        if (tamanho_coluna > 0) {
            strncpy(noc_temp, ponteiro, tamanho_coluna);
            noc_temp[tamanho_coluna] = '\0'; //fecha string
        }
        ponteiro += tamanho_coluna + 1; //avança proxima coluna, 1 passo apos a virgula 

        //procura virgula ou quebra de linha para separar, contando o numero de passos
        //\r deve ser considerado por ser usado no windows
        //define o tamanho da coluna
        tamanho_coluna = strcspn(ponteiro, ",\n\r");
        
        if (tamanho_coluna > 0) {
            strncpy(regiao_temp, ponteiro, tamanho_coluna);
            regiao_temp[tamanho_coluna] = '\0';
        }
        
        ponteiro += tamanho_coluna + 1;
        //avança a coluna de novo, como essa é a última coluna então a separação não
        //é mais por vírgula e passa a ser por enter, considerar \n e \r para windows tambem
        tamanho_coluna = strcspn(ponteiro, "\n\r"); 
        if (tamanho_coluna > 0) {
            strncpy(notes_temp, ponteiro, tamanho_coluna);
            notes_temp[tamanho_coluna] = '\0';
        }

        //importante que o usuario digite corretamente para funcionar
        //se o nome encontrado no csv for o desejado então a conversão foi definida
        if (strcmp(regiao_temp, nome_pais) == 0 || strcmp(notes_temp, nome_pais) == 0) {
            strcpy(codigo_encontrado, noc_temp);
            break; //se acahr nçao precisa repetir o loop
        }
    }

    fclose(f);
  
    return codigo_encontrado;
}

void gestao_q4(FILE *arquivo_resultados, FILE *arquivo_bios) {
    //defina endereçamento para abrir os csvs
     FILE *arquivoResultados = arquivo_resultados;
     FILE *arquivoBios = arquivo_bios;

    //preenche o array
    mapear_generos(arquivoBios);
    
    int acumuladora = 0; //acumuladora que controla o loop

    printf("Insira o nome de 4 paises e veja a comparacao de medalhas totais por genero\n");

    //no loop a comparação será feita entre os 4 países, repetindo o processo
    while (acumuladora < 4) {
        int placar[2]; 
        char NOC[5];
        char pais[50];

        printf("Insira o nome em ingles do pais desejado, com escrita compativel ao nome do time do pais nas olimpiadas ex: France \n");
        scanf(" %s", pais);
        //o fgets pega o \n, isso atrapalharia a comparação, então é preciso remover o \n
        //para isos usar o strcspn que percorre a string até achar o \n e transforma em \0
        pais[strcspn(pais, "\n")] = 0;

        strcpy(NOC, converter_nome(pais)); //converte o nome em código NOC

        //se o país for inválido, o NOC será "NADA", então nem chama a função
        if (strcmp(NOC, "NADA")) { //se for invalido retorna 0 e não cai na condição
            contar_medalhistas(NOC, placar, arquivoResultados); //usa o código NOC para identificar as medalhas
            printf("%s:\n %d medalhas para Homens, %d para Mulheres \n\n", pais, placar[0], placar[1]);
        } else {
            //se retornar 0, o NOC é igual a "NADA", então sai do if e cai no else
            printf("Pais invalido\n\n");
        }
        acumuladora++;
    }
}
