// 🥉 Escolha uma edição aleatória das Olimpíadas e liste os atletas mais velhos por modalidade esportiva
//, além disso liste o total de Olimpíadas competidas pelos 3 mais velhos desta edição escolhida.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// Criação de uma struct Atleta com campos referentes a uma determinada edição olímpica
typedef struct {
    int ano;                    // ano da edição olímpica
    char esporte[100];          // modalidade esportiva
    char atletaNome[150];       // nome do atleta
    int atletaId;               // id único do atleta
    int idade;                  // idade calculada posteriormente com o cruzamento com o bios.csv
} Atleta;

// Struct do tipo biosAtleta para auxiliar a cruzar dados biográficos do atleta
typedef struct {
int ano;                // ano de nascimento
int atletaId;           // id do atleta
} biosAtleta;


typedef struct{
    Atleta* array;
    int contador;
} GuardaListaAtleta; // struct para armazenar o array e contarv quantos elemnetos estão armazenados nele
// isso permite modificar oa array no próprio endereço, já que caso passassemos o ponteiro do array como parâmetro, ele seria copiado por valor

typedef struct{
    biosAtleta* array;// a mesma ideia do struct acima
    int contador;
} GuardaListaBios;



// Parser responsável por extrair dados do arquivo results.csv
// Esse parser é responsável por realizar a leitura de cada linha do results.csv, de forma manual, utilizando das posições das vírgulas para evitar erros com campos entre aspas
Atleta Parser(char str[]){

  // note que se a posição de uma virgula mais 1 for igual a da próxima, então o campo é vazio

     // Vetor para armazenar as posições das vírgulas relevantes, mapeamos para saber onde começa e onde termina um campo
    int posVirgulas[10];
    int camposLidos = 0; // itera a posição das vírgulas
    int aspas = 0; // espécie de interruptor que impede salvar posição de vírgulas dentro de campos. por exemplo 

    // Percorre a string identificando vírgulas válidas, ou seja, aquelas que estão fora de aspas
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"') // aqui evita virgulas inválidas
        aspas = !aspas;
        if ((str[i] == ',' && str[i+1] != ' ')){ // salva apenas vírgulas válida
            if(aspas){}
            else{
            posVirgulas[camposLidos++] = i;
            }
        }
    }
 
    Atleta comp; // cra-se o objeto atleta, com todas as informações necessárias, que será retornado pela função

    // Extrai o ano da edição olímpica, correspondentes aos primeiros 4 caracteres de cada linha do results.csv
    char ano[5];
    for (int i = 0, z = 0; i < 4; i++, z++){
            ano[z] = str[i];
            ano[z+1] = '\0';
        }
    comp.ano = atoi(ano); // após ler a string ano, convertemos para inteiro e passamos para o campo da struct
    
   
   //Extrai o nome do atleta, que esta localizado entre as vírgulas 4 e 5 da linha. Caso o campo esteja vazio, ele é marcado como "Vazio"
    if(posVirgulas[4] + 1 == posVirgulas[5]){strcpy(comp.atletaNome, "Vazio");}
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';  // após ler a string nome, salvamos na própria struct
        }
    }

   //Extrai o id do atleta, que está localizado entre as vírgulas 5 e 6 de cada linha. Caso o campo esteja vazio, então o id é definido como -1 
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.atletaId = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);  // após ler a string ID, convertemos para inteiro e passamos para o campo da struct
    }

    //Extrai o esporte associado ao atleta, que esta localizado entre as vírgulas 7 e 8 do arquivo. Ele também realiza a verificação se o campo realmente possui conteúdo
    if(posVirgulas[8] - posVirgulas[7] <= 1){strcpy(comp.esporte, "Vazio");}
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0'; // após ler a string esporte, salvamos na própria struct
        }
    }

    return comp; // retorna o atleca já configurado
}



//Implementação do Parser responsável pelo arquivo bios.csv
//Esse Parser é responsável por realizar a leitura separada de dados biográficos de cada atleta, a fim de adquirir dados para o cálculo da idade
biosAtleta ParserBios(char str[]){

    // ideia semelhante do parser Atleta, só aumentamos o array para poder ler até 15 vírgulas
    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    //Aqui, estamos identificando as vírgulas e contabilizando somente aquelas que estão fora de campos com aspas  
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
 
    biosAtleta comp;// o mapeamento de ID e idade

    //Esse campo extrai o id do atleta, que esta localizado entre as linhas 6 e 7, e, após essa extração, converte ele de string para inteiro
    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){comp.atletaId = -1;}
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }

//Esse campo extrai o ano de nascimento do atleta, que esta localizado entre a linha 3 e 4 do bios.csv
// Caso não encontre um ano válido, ele assume valor alto para evitar cálculo errado. Assim caso não possua idade ele será jogado para o final quando ordenarmos o array
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){comp.ano = 3000;}
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        comp.ano = 3000;
        // Esse campo realiza uma busca manual pelo ano, buscando as primeiras quatro vezes seguidas que aparecem números
        //visto que essa é a única parte da linha capaz de representar o ano e que pode aparecer de 4 formar diferentes, incluindo aparecer um texto sem ano nenhum
        // literalmente sem ano, não é nem vazio, tem alguma coisa que definitivamente não é o ano
        for(int s = 0; s < strlen(nascimento) - 3; s++){
            if(isdigit(nascimento[s])){
                if(isdigit(nascimento[s+1])){
                    if(isdigit(nascimento[s+2])){
                        if(isdigit(nascimento[s+3])){
                            char anoReal[5];
                            anoReal[0] = nascimento[s];
                            anoReal[1] = nascimento[s+1];
                            anoReal[2] = nascimento[s+2];
                            anoReal[3] = nascimento[s+3];// encontrou 4 digitos seguidos, salva e converte para salvar no campo do struct
                            anoReal[4] = '\0';
                            comp.ano = atoi(anoReal);
                        }
                    }
                }
            }
        }
    }

    return comp; // retornar o mapa, com id e ano de nascimento
}


//Implementação de uma função comparadora para a ordenação dos nomes em ordem alfabética, usado no qsort futuramente
int compara(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    return strcmp(ia->atletaNome, ib->atletaNome);
}

//Implementação de uma função comparadora para a ordenação dos atletas em ordem alfabética por esporte e por idade, deixando quem tem mesmo esporte junto
// e o mais velho do esporte no topo
int compara2(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    if (strcmp(ia->esporte, ib->esporte) == 0)
    return ib->idade - ia->idade;

    return strcmp(ia->esporte, ib->esporte);
}

// ordena deixamdo o mais velho topo, faz mais sentido posteriormente na função peneira()
int compara3(const void* a, const void* b){
    const Atleta* ia = (Atleta*) a;
    const Atleta* ib = (Atleta*) b;
    return ib->idade - ia->idade;
}

// ordena um conjunto de inteiros
int compararInteiros(const void *a, const void *b) {
    return (*(int*)a - *(int*)b); // Para ordem crescente
    // return (*(int*)b - *(int*)a); // Para ordem decrescente (b - a)
}


void maisVelhos(Atleta array[], int zonaSeg, FILE* arq){// função que recebe o array, por valor mesmo, já completamente ordenado, e trata de buscar quanstas vezes os 
  // tres atletas mais velhos participaram de olimpiadas

    for(int j = 0; j < 3; j++){// iterasse uma vez pra cada atleta

        int listAnos[100]; // array que armazena o máxiimo de anos que o atleta da iteração particicpou
        int i = 0; // posição do array anos
        rewind(arq); // necessário para retornar o ponteiro do arfquivo para o inicio novamente


        // espécie de mini parser embutido, percorre o results para contar o total de vezes que o id do atleta aperece em linhas
        // seja repetido ou não

        char str[3000]; // tamanho máximo da linha lida por arquivo
        fgets(str, sizeof(str), arq);// ignora o cabeçalho do csv
        while (fgets(str, sizeof(str), arq) != NULL){ // lê até o fim do arquivo

            int posVirgulas[10];
            int camposLidos = 0;// estrutura de mapeamneto já abordada
            int aspas = 0;

            // mapeia as vírgulas válidas do results
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

            // retira o ID e compara, caso seja igual ao do atleta da iteração então salva na lista de anos
            char Id[10];
            if(posVirgulas[5] + 1 == posVirgulas[6]){strcpy(Id, "0");}
            else{
                for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
                    Id[z] = str[i];
                    Id[z+1] = '\0';
                }
            int idReal = atoi(Id);
            if(array[j].atletaId == idReal)
            sscanf(str, "%d", &listAnos[i++]);
        }
        }

        qsort(listAnos, i, sizeof(int), compararInteiros); // ordena a lista de anos usando o comparador de inteiros

        //UNICIDADE
    
        // utilizando a lista de inteiros ordenada, apagamos somente duplicatas. importante paraa não contabilizar várias participações de atletas num memso ano
            int posSegura = 1;
            for(int k = 1; k < i; k++){
                if(listAnos[k] == listAnos[k-1]){}
                else{
                listAnos[posSegura] = listAnos[k];
                posSegura++;
                }
            }
            // posSegura indica exatamente a quantidade de elementos diferentes

        printf("\nAtleta: %s  Idade na edição: %d  Olimpíadas participadas na história: %d", array[j].atletaNome, array[j].idade, posSegura); 
        // imprimimos na tela a idade do atleta no ano e o número de edições participadas na história
    }
}


GuardaListaAtleta CriarSelecao(int anoBusca, FILE* arq){// função do tipo do struct que salva o array de atletas e o contador do número de elementos dele
    GuardaListaAtleta lista; // objeto da struct que guardará a lista e o contador

        int cont = 0; // contador interno que será copiado por valor para o da struct

        int capacidade = 5; // capacidade inicialdo array
        Atleta* competidores =  malloc(sizeof(Atleta)*capacidade); // alocação do array de Atletas que sera copiado para o da struct
        
        char ch[3000]; // bffer máximo para ler uma linha 
        fgets(ch, sizeof(ch), arq); // ignora cabeçalho
        while (fgets(ch, sizeof(ch), arq) != NULL){ /// caso o ponteiro não seja NULL entramos num loop de ler e tratar cada linha até o fim do arquivo

            if ((cont + 1) == capacidade){
                capacidade *= capacidade;
                Atleta* temp = realloc(competidores, sizeof(Atleta) * capacidade);//realoca array criado para caso atinja o tamanho máximo, copiando o conteúdo do anterior e aumentando o tamanho
                competidores = temp; // espécie de arraylist encontrado no java
            }

            int anoObservado;
            sscanf(ch, "%d", &anoObservado); // retira o ano de forma formatada, pois ele é o primeiro campo das linhas e sempre aparece

            if(anoObservado == anoBusca){// aqui observamos se o ano da linha correspondete bate com o ano escolhido pelo usuário (observado), caso bat, então adicionamos a lista

                Atleta temp = Parser(ch); // cria um objeto atleta e salva nele o competidor "comp" criado e retornado pelo parser
                if(strcmp(temp.esporte, "Vazio") == 0){} 
                else{
                competidores[(cont)++] = temp; // aqui salvamos esse objeto temp (temporário) no array caso ele possua o campo esporte
                }
            }
        }
        lista.array = competidores; // passamos a refrencia do array com petidores para o array da struct, haja vista que foi criado por alocação dinâmica e não desaperece sem free
        lista.contador = cont;
        
    return lista; // retornamos o struct
}

GuardaListaBios informacoesAtletas(FILE* bios){// função do tipo do struct que salva o array de biosAtletas e o contador do número de elementos dele
    GuardaListaBios listagem; // objeto da struct que guardará a lista e o contador

    int num = 0; // contador interno que será copiado por valor para o da struct

    int capacidade2 = 5; // capacidade inicialdo array
    biosAtleta* lista = malloc(sizeof(biosAtleta)*capacidade2); // alocação do array de Atletas que sera copiado para o da struct
        
    
    char newPala[3000];
    fgets(newPala, sizeof(newPala), bios);
    while (fgets(newPala, sizeof(newPala), bios) != NULL){ // caso o ponteiro não seja NULL entramos num loop de ler e tratar cada linha até o fim 
        

        if ((num + 1) == capacidade2){
            capacidade2 *= capacidade2;
            biosAtleta* temp = realloc(lista, sizeof(biosAtleta) * capacidade2);//realoca array criado para caso atinja o tamanho máximo, copiando o conteúdo do anterior e aumentando o tamanho
            lista = temp;// passando a referencia
        }

        lista[(num)++] = ParserBios(newPala); // aqui adicionamos todo e qualquer bios, já que bios não possui o campo ano e precisamos cruzar dados com  Atletas usando i ID

    }
    listagem.array = lista;
    listagem.contador = num; // passagem de valor do contador e retorno do struct que guarda o mapeamento do bios
    return listagem;
}

void exibir(GuardaListaAtleta lista, FILE* arq){/// Função que exibe os dados processado e guardados nos arrays
    for (int i = 0; i < lista.contador; i++) {
            if (lista.array[i].idade < 0){
                printf("\n%d - Esporte: %s  Nome: %s  Idade: Sem idade  Ano: %d  Id:  %d\n", i, lista.array[i].esporte, lista.array[i].atletaNome, lista.array[i].ano, lista.array[i].atletaId);
            }
            else
            printf("\n%d - Esporte: %s  Nome: %s  Idade: %d  Ano: %d  Id:  %d\n", i+1, lista.array[i].esporte, lista.array[i].atletaNome, lista.array[i].idade, lista.array[i].ano, lista.array[i].atletaId);
        }

        maisVelhos(lista.array, lista.contador, arq); // exibe os mais velhos da ediçaõ e diz quanstas participaram
}

void peneira(GuardaListaAtleta* lista, GuardaListaBios* lista2, int AnoEscolha){ // Funcão primncipal de processamento de dados

    qsort(lista->array, lista->contador, sizeof(Atleta), compara); // pega o array original com todos os atletas dos results, mesmo as duplicatas, e deixa todos com mesmo nome junto
    // isso permite isolar duplicatas, tendo em vista que todos de mesmo e mesmo id tambem estarão juntos

    /// UNICIDADE
    int zonaSeg = 1;
    for(int k = 1; k < lista->contador; k++){
        if (strcmp(lista->array[k].esporte, lista->array[k-1].esporte) == 0 && lista->array[k].atletaId - lista->array[k-1].atletaId == 0){}
        else{
            lista->array[zonaSeg] = lista->array[k];
            zonaSeg++;//// retira atletas de mesmo id e mesmo esporte, caso tenham esportes diferentes e mesmo id mantém
        }// aqui retiramos apenas duplicatas de mesmo id e esporte. note que assim evitamos casos de atletas que participaram de dois esportes distintos em um mesmo ano
    }

    Atleta* temp = realloc(lista->array, sizeof(Atleta) * zonaSeg); // apos retirar as duplicatas realocamos o array para ter o tamanho exato de at6letas válidos
    lista->array = temp;
    

    for(int k = 0; k < lista2->contador; k++){
        int quebraloop = 1;
        int itera = zonaSeg - 1;
        while(quebraloop){
            if(lista2->array[k].atletaId == lista->array[itera].atletaId)
            lista->array[itera].idade = AnoEscolha - lista2->array[k].ano;// cruzamos dados com o array de csv para assim associar a cada id o ano de nascimento, e, consequentemente
          // gavar no campo idade a idade do atleta
            itera--;
          // for com while. perocrresse bios uma vez e tenta achar o atleta no array de atletas válidos
            if (itera == -1)
            quebraloop = 0;
        }
    }


    qsort(lista->array, zonaSeg, sizeof(Atleta), compara2); // ordena pela idade e esporte, assim organizando o array em blocos de atletas do mesmo esporte, do mais velho para o mais novo


    int segundaZonaSeg = 1;
    for(int k = 1; k < zonaSeg; k++){
        if (strcmp(lista->array[k].esporte, lista->array[k-1].esporte) == 0){}
        else{
            lista->array[segundaZonaSeg] = lista->array[k];// apenas deixa o mais velho de cada esporte
            segundaZonaSeg++;
        }
    }
    Atleta* temp2 = realloc(lista->array, sizeof(Atleta) * segundaZonaSeg);// realocamos para deixar o novo array válido
    lista->array = temp2;
    lista->contador = segundaZonaSeg;
    
    qsort(lista->array, segundaZonaSeg, sizeof(Atleta), compara3);// agora só com o mais velho de cada esporte deixamos em ordem descrescente para deixar organizado na saída
}


void gestao(FILE* arq, FILE* bios){ // função de getsão princiapal. comom o código foi quebrado em blocos de funções, essa aqui organiza tudo e chama

    int AnoEscolha;
    printf("Escolha o ano para busca: ");// deixamos o usuário escolher o ano
    scanf("%d", &AnoEscolha);
    
    GuardaListaAtleta lista = CriarSelecao(AnoEscolha, arq); // ciramos a truct que guarda o ponteiro do array e guardamos tudo dentro dela
    if (lista.contador == 0){
        printf("\n\nNenhum dado encontrado desta edição -> %d\n\n\n", AnoEscolha);
        return;
    }// se o contado retornar 0 quer dizer que nenhum jiogado foi encontrado no ano, logo o ano não é olímpico

    GuardaListaBios listagem = informacoesAtletas(bios); // guardamos a lista bios
    peneira(&lista, &listagem, AnoEscolha); // passamos tudo para peneira e processamos os dados

    exibir(lista, arq); // por fim exibimos o que foi encontrado
    
    fclose(arq); // fecha os arquivos 
    fclose(bios);
}


int main(){
    FILE *arq = fopen("results.csv", "r");// abre os arquivos
    FILE *bios = fopen("bios.csv", "r");
    if (arq == NULL || bios == NULL) {
            puts("Error ao abrir arquivos"); // retorna um erro caso o ponteiro que receberia o arquivo seja NULL
            return 1;
    }
    gestao(arq, bios); // chama a função de gestão passando os ponteiros
    return 0;
}
