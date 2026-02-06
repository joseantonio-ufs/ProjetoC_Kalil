#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//Aqui, definimos a struc de Atleta, que contem campos específicos os quais serão usados posteriormente na implementação da lógica do código
typedef struct{
  int ano;
  char medalha[10];
  int atleta_id;
  char pais[20];

} Atleta;


//Aqui, definimos a struct de Pais com os dados que serão necessários, a fim de posteriormente criar um array de Structs Pais para implementação da lógica do código
typedef struct{
    int numeroMedalhas;
    int numeroAtletas;
    char NOC[30];
} Pais;


Atleta leitura(char str[]){
  //Criando um Array de Ponteiros a fim de armazenar cada campo do arquivo csv por linha lida
  char** lista = malloc(sizeof(char*)*11);
  
  for(int i = 0; i<4; i++){
    *(lista+i) = malloc(sizeof(char)*150);
  }

  //Definindo variáveis que serão usadas posteriormente para garantir a separação correta dos campos de cada linha
  int posicaoInicial = -1;
  int virgulas[11];
  int camposLidos = 0;
  int aspas = 0;

  //Esse laço for foi criado para armazenar o espaço de cada vírgula, visto que estamos trabalhando com a separação de espaços por vírgula
  for(int i = 0; i<strlen(str); i++){
    if(str[i] == '"'){
      aspas = !aspas;//Aqui, estamos tirando a excessão da vírgula para o caso dela estar dentro de um campo delimitado por aspas. Nesse caso, como a vírgula não significa o fim do campo, não contamos ela dentro do array
    }
    if((str[i] == ',' && str[i+1] != ' ') || str[i] == '\n'){
      if(!aspas){
        virgulas[camposLidos++] = i; //Aqui, ele primeiro executa a operação de atribuição, somente após isso é que ele incrementa camposLidos
      }
    }
  }

  camposLidos = 0;//É preciso zerar essa variável a fim de realizar uma novo leitura do array "virgulas", só que para as próximas linhas do arquivo

   //Nesse laço for, usamos a posição das virgulas para delimitar os campos e passá-los para o array lista
  for(int i = 0; i<11; i++){
    char campo[200];
    //Se a distância entre delimitadores for <=1, o campo está vazio no CSV
    if(virgulas[i] - posicaoInicial<=1){
      strcpy(lista[camposLidos++], "Vazio");
      posicaoInicial = virgulas[i];
    }
    else{
        //Nessa parte do código, ele cumpre o papel de copiar manualmente os caracteres entre as duas vírgulas
      for(int k = posicaoInicial+1, z = 0; k<virgulas[i]; k++, z++){
        campo[z] = str[k];
        campo[z+1] = '\0';
      }
      strcpy(lista[camposLidos++], campo);
      posicaoInicial = virgulas[i];
    }
  }


   //Aqui ele cria um Atleta Geral e atribui as informações do array lista para os campos específicos da struct desse atleta, fazendo as devidas conversões necessárias
  Atleta atleta;
  if(strcmp(lista[0], "Vazio")!=0){
    atleta.ano = atoi(lista[0]);
  }else{
    atleta.ano = 0;
  }
  if(strcmp(lista[4], "Vazio")!=0){
    strcpy(atleta.medalha,lista[4]);
  }else{
    strcpy(atleta.medalha, "Vazio");
  }
  if(strcmp(lista[6], "Vazio")!=0){
    atleta.atleta_id = atoi(lista[6]);
  }else{
    atleta.atleta_id = 0;
  }
  if(strcmp(lista[7], "Vazio")!=0){
    strcpy(atleta.pais, lista[7]);
  }
  else{}

  
  //Libera a memória do array de ponteiros
  for(int i = 0; i<=11; i++){
    free(lista[i]);
  }
  free(lista);

  return atleta;
}


int main(){}
