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

Atleta Parser(char str[]){
//1912 Summer Olympics,"Singles, Men (Olympic)",,=17,,Jean-François Blanchy,1,FRA,Tennis,,
//1912 Summer Olympics,"Doubles, Men (Olympic)",Jean Montariol,DNS,,Jean-François Blanchy,1,FRA,Tennis,,

    int posVirgulas[10];
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
 
    Atleta comp;

    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano);
    
   

    if(posVirgulas[3] + 1 == posVirgulas[4]){strcpy(comp.medalha, "Vazio");}
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            comp.medalha[z] = str[i];
            comp.medalha[z+1] = '\0';
        }
    }

    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.atleta_id = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atleta_id = atoi(Id);
    }

    if(posVirgulas[6] + 1 == posVirgulas[7]){strcpy(comp.pais, "Vazio");}
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            comp.pais[z] = str[i];
            comp.pais[z+1] = '\0';
        }
    }
    
    //printf("Posição de eficiência do pais %s: %d. Valor da razão entre medalhas e atletas: %d  %s\n", comp.medalha, comp.atleta_id, comp.ano, comp.pais);
    return comp;
}

  //Criação de um array de structs do tipo Pais
  Pais  listaDePaises[10];
  int anoEncontrado = 0;
  


//implementação da função de comparação baseada no cálculo da razão, para ordenação dos países 
int comparaPaises(const void* a, const void* b){
  const Pais* p1 = (Pais*) a;
  const Pais* p2 = (Pais*) b;
  //Função ordenação em ordem decrescente
  return p2->numeroAtletas - p1->numeroAtletas;
}

  int comparaPaises2(const void* a, const void* b){
      const Pais* p1 = (Pais*) a;
      const Pais* p2 = (Pais*) b;

      float razao1 = 0.0, razao2 = 0.0;

      if(p1->numeroAtletas > 0){
        razao1 = (float)p1->numeroMedalhas/(float)p1->numeroAtletas;
      }

      if(p2->numeroAtletas>0){
        razao2 = (float)p2->numeroMedalhas/(float)p2->numeroAtletas;
      }

      //Função ordenação em ordem decrescente
      if(razao1>razao2){
        return -1;
      }else{
        return 1;
      }
}


void entradaDosPaises(Pais arrayPais[]){
    //Entrada de dados dos países escolhidos e inicialização dos campos da struct
  printf("Escolha o 10 paises que voce almeija obter a razao: ");
  for(int i = 0;i<10;i++){
    scanf(" %[^\n]", arrayPais[i].NOC);
    arrayPais[i].numeroMedalhas = 0;
    arrayPais[i].numeroAtletas = 0;
  }
}

void incrementadorMedalhasEAtletas(FILE *arquivo){
  //Criando variáveis que serão utilizadas futuramente na lógica do código
 
  int anoEscolhido;
  int analisandoAno;
  //Essa parte irá servir para garantir que não hajam atletas repetidos para um mesmo país 
  int IdAnterior = 0;
   //Entrada do ano de determinada Olimpíada
  printf("Digite o ano escolhido: ");
  scanf("%d", &anoEscolhido);
  
  //Implementação da lógica principal do código nesse bloco
  char linha[2000];
  fgets(linha, sizeof(linha), arquivo);
  while(fgets(linha, sizeof(linha), arquivo)!= NULL){
    //Pega o primeiro campo do arquivo, no caso, o ano
    sscanf(linha, "%d", &analisandoAno);
    

    Atleta AtletaGeral = Parser(linha);
    if(analisandoAno==anoEscolhido){
      anoEncontrado = 1;
      for(int i = 0; i<10; i++){
          if(strcmp(listaDePaises[i].NOC, AtletaGeral.pais) == 0){
            //Se o nome do país analisado for o mesmo da linha atual, verificar se tem medalha ou não, e se sim, incrementa o número de medalhas
            if(strcmp(AtletaGeral.medalha, "Vazio") != 0){
              listaDePaises[i].numeroMedalhas++;
            }
           
            
            if(AtletaGeral.atleta_id != IdAnterior){
              listaDePaises[i].numeroAtletas++;
              IdAnterior = AtletaGeral.atleta_id;
            }  
          }
        }
    }
  }
}

void capturaDeDados(Pais arrayPais[]){
   qsort(arrayPais, 10, sizeof(Pais), comparaPaises);
   qsort(arrayPais,10, sizeof(Pais), comparaPaises2);
  if(anoEncontrado==1){
  //Realizando o cálculo da razão, com a lista já ordenada, e pegando os 3 primeiros países, que são os que foram os que obtiveram as maiores razões
  for(int i = 0; i< 10; i++){
    float razao = (float)arrayPais[i].numeroMedalhas/(float)arrayPais[i].numeroAtletas;

    if(arrayPais[i].numeroAtletas == 0){
      printf("O pais %s nao participou dessa edicao dos jogos olimpicos\n", arrayPais[i].NOC);
  }else{
    if(razao!=0){
     printf("Posicao do pais %s em relacao a eficiencia : %d°. Numero de atletas: %d. Numero de medalhas: %d. Razao: %.3lf\n",arrayPais[i].NOC,i+1,arrayPais[i].numeroAtletas,arrayPais[i].numeroMedalhas,razao);
   }else{
    printf("Posicao do pais %s em relacao a eficiencia : %d°. Numero de atletas: %d. Numero de medalhas: %d. Razao: 0.0\n",arrayPais[i].NOC,i+1,arrayPais[i].numeroAtletas,arrayPais[i].numeroMedalhas);
   }
  }
}
}else{
  printf("Esse ano nao e olimpico!");
}
  }
 

void gestao_q2(){
      //Aqui ocorre a abertura do arquivo results.csv e o teste para confirmar que o arquivo abriu mesmo 
  FILE *results = fopen("C:/Users/phall/Desktop/Projeto/results.csv","r");

  if(results == NULL){
    puts("Error opening file");
  }

  //Chamando a função responsável do permitir a leitura dos países e atribuí-los aos campos da listaDePaises
  entradaDosPaises(listaDePaises);


  //Chamando a função responsável pela lógica principal do código de incrementação do número de medalhas e de atletas
  incrementadorMedalhasEAtletas(results);
  
  //Chamando a função responsável por capturar os dados e exibí-los no terminal
  capturaDeDados(listaDePaises);

//Sessão para encerramento do arquivo
fclose(results);
  
}
