//🥈 Considere 10 países a sua escolha. Para cada país, calcule a razão entre o número total de medalhas e o número total de atletas que representaram esse país em uma determinada olimpíada, identificando os países mais eficientes.
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "arquivo.h"   // ligando ao .h
#include <unistd.h>
#include <locale.h> // Necessário para forçar o ponto decimal
//Aqui, definimos a struct de Atleta, que contem campos específicos os quais serão usados posteriormente na implementação da lógica do código
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

static Atleta Parser(char str[]){


    //Aqui, definimos variáveis que serão utilizadas para a lógica de separação dos campos que serão utilizados
    int posVirgulas[10];
    int camposLidos = 0;
    int aspas = 0;

    //Verificação para saber se a vígula esta localizada em um campo delimitado por aspas, e se não estiver, incrementar posVirgulas[camposLidos++]  
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
  
    //Criação de uma variável do tipo atleta para a atribuição dos campos;
    Atleta comp;

    //Capturando o campo "ano", que esta localizado sempre na primeira parte das linhas do results.csv, por meio da copia dessa string para o campo "ano", e depois realizando a conversão de string para int
    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano);
    
   
   //Capturando o campo "medalha", que esta localizado entre as vírgulas 3 e 4 do results.csv, por meio da copia dessa string para o campo "medalha"
    if(posVirgulas[3] + 1 == posVirgulas[4]){strcpy(comp.medalha, "Vazio");}
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            comp.medalha[z] = str[i];
            comp.medalha[z+1] = '\0';
        }
    }

    //Capturando o campo "Id", que esta localizado entre as vírgulas 5 e 6 do results.csv, por meio da copia dessa string para o campo "Id", e depois convertendo esse campo para um inteiro
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.atleta_id = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atleta_id = atoi(Id);
    }

    //Capturando o campo "NOC", que esta localizado entre as vírgulas 6 e 7 do results.csv, por meio da copia dessa string para o campo "pais"
    if(posVirgulas[6] + 1 == posVirgulas[7]){strcpy(comp.pais, "Vazio");}
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            comp.pais[z] = str[i];
            comp.pais[z+1] = '\0';
        }
    }
    
    
    return comp;
}

  //Criação de um array de structs do tipo Pais
  Pais  listaDePaises[10];

  //Criando uma variável que será utilizada posteriormente para implementar a lógica de: Verificar se o ano escolhido foi um ano Olímpico
  int anoEncontrado = 0;
  


//implementação das funções de comparação baseadas no cálculo da razão, para ordenação dos países 
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

      if(p1->numeroAtletas == 0){
        razao1 = -1;
      }

      if(p2->numeroAtletas == 0){
        razao2 = -1;
      }


      //Função ordenação em ordem decrescente
      if(razao1>razao2){
        return -1;
      }else{
        return 1;
      }
}


static void escrever(){// função para criação de arquivo que será escrito os comandos do gnuplot e, criando o .gp
    FILE* f_script = fopen("script.gp", "w");
    if(f_script == NULL){
        puts("Erro ao gerar arquivo .gp");// verifica de se fato criou
        return;
    }
    fprintf(f_script, "set terminal png size 1200,800\n");
    fprintf(f_script, "set output 'Grafico_Q3.png'\n");
    fprintf(f_script, "set title \"Razão do número de medalhas\"\n");
    fprintf(f_script, "set ylabel \"Razão\"\n");
    fprintf(f_script, "set xlabel \"Países\"\n");// linhas de comando que o gnuplot usará sendo escritas no arquivo com fprintf
    fprintf(f_script, "set grid y\n");
    fprintf(f_script, "set style data histograms\n");
    fprintf(f_script, "set style fill solid 1.0 border -1\n");
    fprintf(f_script, "set boxwidth 0.7\n");
    fprintf(f_script, "set xtics rotate by -90 scale 0\n");
    fprintf(f_script, "set bmargin 10\n"); 
    fprintf(f_script, "set yrange [0:*]\n");
    fprintf(f_script, "plot 'dados_q3.dat' using 2:xtic(1) notitle linecolor rgb \"#2980b9\"\n");
    fclose(f_script); // fechamos o arquivo
}

static void gerar_grafico_gnuplot(Pais arrayPais[]) {
    // Força o uso de .  para decimais porque o gnuplot, pois ele só aceita número es inglês e não em português com ,
    setlocale(LC_NUMERIC, "C");

    escrever();
    FILE *dados = fopen("dados_q3.dat", "w");// abrimos o .dat que receberá os dados da lista salva e usará patra o gráfico
    if (dados == NULL) {
        printf("Não abriu arquivo! Sem gráfico nessa\n");// verifica se criou de fato
        return;
    }

    for (int i = 0; i < 10; i++) {
        float razao = 0.0;
        if (arrayPais[i].numeroAtletas > 0) {
            razao = (float)arrayPais[i].numeroMedalhas / (float)arrayPais[i].numeroAtletas; // verifica se o numero de atletas é maior que 0 para não realizar divisão por 0 e quebrar tudo
        }
        fprintf(dados, "\"%s\" %.3f\n", arrayPais[i].NOC, razao); // salva .dat os dados necessários
    }
    fclose(dados); // fechasse o arquivo
    
    printf("\nProcessando grafico...\n");
    
    
    int status = system("gnuplot script.gp"); // verifica que o gp esta na pasta e retorna 0 se esta

    if (status == 0) { 
         printf("Grafico gerado!\n");
         printf("Deseja abrir o grafico? Y/N\n");
         char c;
         scanf(" %c", &c);
         while(c != 'N' && c != 'n'){
            if(c == 'Y'  || c =='y'){
                printf("\nAbrindo...\n");// verifica se o usuário quer abrir e abre caso sim
                #ifdef _WIN32 
                    system("start Grafico_Q3.png");
                #elif __APPLE__
                    system("open Grafico_Q3.png");// diretivas para abrir a depender do os usando
                #else
                    // Linux / FreeBSD
                    system("xdg-open Grafico_Q3.png");
                #endif
                 remove("dados_q3.dat");
                 remove("script.gp"); // apagasse o que foi criado
                return; 
            }
            else{
                printf("Invalido\n");
                printf("Digite Novamente\n"); // continua o lop se o usuario digitar algo diferente de Y ou N
                scanf(" %c", &c); // Lê direto na variável c
            }
         }
    } 
    sleep(1); // delay para permitir o os apagar
    remove("dados_q3.dat"); // remove independente de gerar gráfico
    remove("script.gp");
}


//Criando a função que será utilizada para o usuário poder dizer quais países ele quer obter informações acerca da razão entre o número de medalhas e o de atletas
int entradaDosPaises(Pais arrayPais[]){
    //Entrada de dados dos países escolhidos e inicialização dos campos da struct
  printf("Escolha o 10 paises que voce almeija obter a razao: Exemplo de entrada:  FRA CHN CRC DEN CAN GBR GER GRE HKG HUN (coloque nesse formato, todos na mesma linha para facilitar) \n");
  int i = 0;
    // O loop continua até que tenhamos preenchido exatamente as 10 posições
    while (i < 10) {
        //fica lendo o que ficou no buffer
        if (scanf("%s", arrayPais[i].NOC) == 1) {
            arrayPais[i].numeroMedalhas = 0;
            arrayPais[i].numeroAtletas = 0;
            i++; 
        } else {
            // Se algo der muito errado ele finaliza 
            printf("\nInvalido\n");
            return 0;
        }
    }
    
    return 1; // Sucesso ao preencher os 10
}

//Criando a função que contem o lógica principal do programa, que irá servir para incrementar o contador de medalhas e de atletas para cada item da lista de Paises
int incrementadorMedalhasEAtletas(FILE *arquivo){
  //Criando variáveis que serão utilizadas futuramente na lógica do código
  int anoEscolhido;
  int analisandoAno;
  //Essa parte irá servir para garantir que não hajam atletas repetidos para um mesmo país 
  int IdAnterior = 0;
   //Entrada do ano de determinada Olimpíada
  printf("\nDigite o ano escolhido: ");
  scanf("%d", &anoEscolhido);
  printf("\n");
  
  //Implementação da lógica principal do código nesse bloco
  int ano_e_olimpico = 0;
  char linha[2000];
  fgets(linha, sizeof(linha), arquivo);
  while(fgets(linha, sizeof(linha), arquivo)!= NULL){
    //Pega o primeiro campo do arquivo, no caso, o ano
    sscanf(linha, "%d", &analisandoAno);
    
    //Criando um atleta geral, que servirá implementar a lógica do Parser, usado para ler cada linha do arquivo de maneira eficiente, capturando apenas os campos necessários
     Atleta AtletaGeral = Parser(linha);
    if(analisandoAno==anoEscolhido){
      ano_e_olimpico++;
      //Ele muda o valor de anoEncontrado para um, que será utilizado em uma condicional mais para frente do código para que ele possa imprimir todas as informações referentes às razões, visto que se tivesse continuado zero, o ano seria tido como um em que não houve Olimpiada
      anoEncontrado = 1;
      for(int i = 0; i<10; i++){
          if(strcmp(listaDePaises[i].NOC, AtletaGeral.pais) == 0){
            //Se o nome do país analisado for o mesmo da linha atual, verificar se tem medalha ou não, de acorodo com o nome que aparece la, e se sim, incrementa o número de medalhas
            if(strcmp(AtletaGeral.medalha, "Bronze") == 0 || strcmp(AtletaGeral.medalha, "Silver") == 0|| strcmp(AtletaGeral.medalha, "Gold") == 0){
              listaDePaises[i].numeroMedalhas++;
            }
           
            //Aqui ele usa a variável IdAnterior para analisar o Id atual do atleta. Aproveitando do fato, de no results.csv, ele esta agrupado por atletas, então o id se mantem o mesmo até mudar de atleta
            if(AtletaGeral.atleta_id != IdAnterior){
              listaDePaises[i].numeroAtletas++;
              IdAnterior = AtletaGeral.atleta_id;
            }  
          }
        }
    }
  }
  if(ano_e_olimpico == 0){
    printf("\nAno analisado nao e olimpico\n");
    return 0;
  }
  return 1;
}

//Implementando função que vai chamar as funções "comparaPaises" e "comparaPaises2" para realizar a ordenação da lista de paises com base na razao, e, após isso, vai realizar o exibição dos dados de cada pais escolhido individualmente
void capturaDeDados(Pais arrayPais[]){
   qsort(arrayPais, 10, sizeof(Pais), comparaPaises);
   qsort(arrayPais,10, sizeof(Pais), comparaPaises2);

//Usando a variável do anoEncontrado como uma condicional, onde, se ela tiver mudado de 0 para 1, isso implica que o ano escolhido é olímpico. Porém, se ela continuou 0, isso implica que o ano não é olímpico, e portanto, ele cai no else
if(anoEncontrado==1){
  //Realizando o cálculo da razão, com a lista já ordenada
  for(int i = 0; i< 10; i++){
    float razao = (float)arrayPais[i].numeroMedalhas/(float)arrayPais[i].numeroAtletas;

    //Verificando se o número de atletas do país analisado é 0, pois, se for, isso implica que o país não participou daquela edição das Olimpiadas, l
    if(arrayPais[i].numeroAtletas == 0){
      printf("Nao houveram dados gerados para %s (NOC inexistente ou pais ausente nessa edicao)\n", arrayPais[i].NOC);
  }else{
    //Implementação dessa condicional só para formatar o zero e deixar o código visualmente mais agradável e compreensivo
    if(razao!=0){
     printf("Posicao do pais %s em relacao a eficiencia : %d - Numero de atletas: %d. Numero de medalhas: %d. Razao: %.3lf\n",arrayPais[i].NOC,i+1,arrayPais[i].numeroAtletas,arrayPais[i].numeroMedalhas,razao);
   }else{
    printf("Posicao do pais %s em relacao a eficiencia : %d - Numero de atletas: %d. Numero de medalhas: %d. Razao: 0.0\n",arrayPais[i].NOC,i+1,arrayPais[i].numeroAtletas,arrayPais[i].numeroMedalhas);
   }
  }
}
}else{
  printf("Esse ano nao e olimpico!");
}
  }
 

void gestao_q2(FILE *results){
      //Aqui ocorre a abertura do arquivo results.csv e o teste para confirmar que o arquivo abriu mesmo 

  //Chamando a função responsável do permitir a leitura dos países e atribuí-los aos campos da listaDePaises
  int leu_certo = entradaDosPaises(listaDePaises);

  if (leu_certo == 0){
    printf("\nEncerrando questao");
    return;
  }

  //Chamando a função responsável pela lógica principal do código de incrementação do número de medalhas e de atletas
  int anoOlimpico = incrementadorMedalhasEAtletas(results);
  if (anoOlimpico == 0){
  return;}
  //Chamando a função responsável por capturar os dados e exibí-los no terminal
  capturaDeDados(listaDePaises);

  gerar_grafico_gnuplot(listaDePaises);
  
}
