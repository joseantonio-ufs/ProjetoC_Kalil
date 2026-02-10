// 🥇 Faça o ranking dos 10 atletas mais velhos de todos os tempos que participaram das olimpíadas, por gênero.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "arquivo.h"   // header com protótipos e possíveis definições externas

// itero results e pego todos os atletas (mesmo repetidos), pego seu id, ano, nome e esporte. mapeio o bios por hashkey, associo ao indice e puxo a 
// data de nascimento.  atribuo a idade ao atleta. meto um qsort e acabou;

// Criação de uma struct Atleta com campos referentes a uma determinada edição olímpica
typedef struct {
    char esporte[100]; // ano da edição olímpica
    char atletaNome[150];  // nome do atleta
    int Id; // id único do atleta
    int anoOlimpiada; // ano da edição olímpica 
    int idade; // idade calculada posteriormente com o cruzamento com o bios.csv
    char genero[30]; // genero atribuido, baseado no segundo campo do nbios.csv
} Atleta;

// Struct do tipo biosAtleta para auxiliar a cruzar dados biográficos do atleta
typedef struct {
    char ano[10];  // ano de nascimento
    int atletaId; // id do atleta
    char genero[30];// genero que será pasado para o atleta
} biosAtleta;


// parser do results.csv 
static Atleta Parser(char str[]){ // static para isolar só para essa questão
    // faz a leitura manual da linha, ignorando vírgulas dentro de aspas
    // note que se a posição de uma virgula mais 1 for igual a da próxima, então o campo é vazio

    int posVirgulas[10];  // Vetor para armazenar as posições das vírgulas relevantes, mapeamos para saber onde começa e onde termina um campo
    int num_Virgulas_lidas = 0;   // usado para validar linhas incompletas
    int camposLidos = 0; // itera a posição das vírgulas
    int aspas = 0; // espécie de interruptor que impede salvar posição de vírgulas dentro de campos. por exemplo 

    // identifica apenas vírgulas válidas como separador de coluna
    for(int i = 0; i < strlen(str); i++){
        if (str[i] == '"') // aqui evita virgulas inválidas
            aspas = !aspas;

        if ((str[i] == ',' && str[i+1] != ' ')){ // salva apenas vírgulas válida
            if(aspas){}
            else{
                posVirgulas[camposLidos++] = i;
                num_Virgulas_lidas++;
            }
        }
    }
 
    Atleta comp; // cria-se o objeto atleta, com todas as informações necessárias, que será retornado pela função


    // lê o ano da olimpíada no início da string
    sscanf(str,"%d",&comp.anoOlimpiada);

    // valida se a linha tem colunas suficientes
    if (num_Virgulas_lidas < 10){
        strcpy(comp.atletaNome, "NoN"); // marca linha inválida
    }
    else{
        // extrai nome do atleta
        if(posVirgulas[4] + 1 == posVirgulas[5]){
            strcpy(comp.atletaNome, "Vazio");
        }
        else{
            for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
                comp.atletaNome[z] = str[i];
                comp.atletaNome[z+1] = '\0'; // salva nome válido
            }
        }
    }

    // extrai o id do atleta
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){
        comp.Id = -1; // caso não exista é atribuido um id negativo
    }
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0'; // salva o id em uma várialvel
        }
        comp.Id = atoi(Id); // converte id e salva nos campos
    }

    // extrai o esporte
    if(posVirgulas[7] + 1 == posVirgulas[8]){
        strcpy(comp.esporte, "Vazio");
    }
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0'; // salva o esporte no campo da struct
        }
    }

    return comp; // retorna o atleca já configurado
}


//Implementação do Parser responsável pelo arquivo bios.csv
//Esse Parser é responsável por realizar a leitura separada de dados biográficos de cada atleta
// a fim de adquirir dados para o cálculo da idade e retirar o gê+nero
static biosAtleta ParserBios(char str[]){ // static para isolar só para essa questão

     // ideia semelhante do parser Atleta, só aumentamos o array para poder ler até 15 vírgulas
    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    // mesma lógica de separação por vírgulas fora de aspas
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
 
    biosAtleta comp; // o mapeamento de ID, nascimento e sexo
    
    // extrai o gênero
    char sexo[15];
    if(posVirgulas[0] + 1 == posVirgulas[1]){
        strcpy(comp.genero, "Sem genero encontrado");
    }
    else{
        for (int i = posVirgulas[0] + 1, z = 0; i < posVirgulas[1]; i++, z++){
            sexo[z] = str[i];
            sexo[z+1] = '\0';
        }
        strcpy(comp.genero, sexo); // copia o gênero para o campo correto da struct
    }

    // extrai o id do atleta
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

    // extrai o ano de nascimento a partir do campo textual
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){
        strcpy(comp.ano,"Vazio");
    }
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }

        // busca manual por 4 dígitos consecutivos
        strcpy(comp.ano,"Vazio");
        char anoCopiado[5];
        int TotalIteraçoes = strlen(nascimento);

        // Esse campo realiza uma busca manual pelo ano, buscando as primeiras quatro vezes seguidas que aparecem números
        //visto que essa é a única parte da linha capaz de representar o ano e que pode aparecer de 4 formar diferentes, incluindo aparecer um texto sem ano nenhum
        // literalmente sem ano, não é nem vazio, tem alguma coisa que definitivamente não é o ano
        for(int s = 0; s < TotalIteraçoes - 3; s++){
            if(isdigit(nascimento[s]) &&
               isdigit(nascimento[s+1]) &&
               isdigit(nascimento[s+2]) &&
               isdigit(nascimento[s+3])){
                anoCopiado[0] = nascimento[s];
                anoCopiado[1] = nascimento[s+1];
                anoCopiado[2] = nascimento[s+2];
                anoCopiado[3] = nascimento[s+3];
                anoCopiado[4] = '\0';
                strcpy(comp.ano,anoCopiado); // encontrou 4 digitos seguidos, salva e converte para salvar no campo do struct
                s = TotalIteraçoes;
            }
        }
    }
    return comp;  // retornar o mapa, com id e ano de nascimento
}


// cria dinamicamente a lista de atletas a partir do results.csv
int gerarLista_Atletas(Atleta** competidores, FILE* arq){
    int cont = 0;
    int capacidade = 5;

    // alocação inicial
    *competidores = malloc(sizeof(Atleta)*capacidade);
    
    char ch[3000];
    fgets(ch, sizeof(ch), arq); // ignora cabeçalho

    // leitura linha a linha
    while (fgets(ch, sizeof(ch), arq) != NULL){

        // expansão dinâmica do vetor, semelhante arraylist
        if ((cont + 1) == capacidade){
            capacidade *= capacidade;
            Atleta* temp = realloc(*competidores, sizeof(Atleta) * capacidade);
            *competidores = temp;
        }

        // filtra linhas inválidas
        Atleta verificador = Parser(ch);
        if (strcmp(verificador.atletaNome, "NoN") != 0)
            (*competidores)[cont++] = Parser(ch);
    }
    return cont; // retorna o tamanho da lista de atletas
}


// carrega o bios.csv indexando diretamente pelo atletaId (hashtable)
int dados_Bios(biosAtleta** lista, FILE* bios){
    int capacidade2 = 3;
    int num = 0;

    // alocação inicial
    *lista = malloc(sizeof(biosAtleta)*capacidade2);

    char newPala[3000];
    fgets(newPala, sizeof(newPala), bios); // ignora cabeçalho

    while(fgets(newPala, sizeof(newPala), bios) != NULL){ // lê todo bios

        biosAtleta temp = ParserBios(newPala); // cria um objeto temporario de bios

        // garante índice válido para acesso direto, expandindo array caso não tenha espaço
        while(temp.atletaId > capacidade2 - 1){
            capacidade2 *= capacidade2;
            biosAtleta* temp2 = realloc(*lista, sizeof(biosAtleta) * capacidade2);
            *lista = temp2;
        }

        // associação direta com id e indice. o id do atleta vai ser sua posição no array
        // garante busca direta para cruzamento de dados O(1)
        (*lista)[temp.atletaId] = temp;
    }
    return num; // retorna o tamanho do bios lido
}


// cruza results com bios e calcula idade e gênero, cruzamento feito usando ids mapeados
void cruzar_Dados(int cont, Atleta* competidores, biosAtleta* lista){
    for(int k = 0, y = 0; k < cont; k++, y++){
        if(strcmp(lista[competidores[y].Id].ano, "Vazio") == 0){
            competidores[y].idade = 0;
            strcpy(competidores[y].genero, lista[competidores[y].Id].genero);
        }
        else{
            competidores[y].idade =
                competidores[y].anoOlimpiada -
                atoi(lista[competidores[y].Id].ano);
            strcpy(competidores[y].genero, lista[competidores[y].Id].genero);
        }
    }
}


// comparadores usados no qsort
static int compara1(const void* a, const void* b){// ordena por idade, da maior pra menor
    const Atleta* ia = (Atleta*)a;
    const Atleta* ib = (Atleta*)b;
    return ib->idade - ia->idade;
}

static int compara2(const void* a, const void* b){// ordena pelo id e depois pela idade
    const Atleta* ia = (Atleta*)a;
    const Atleta* ib = (Atleta*)b;
    if(ia->Id - ib->Id == 0){
        return ib->idade - ia->idade;// assim deixa blocos de pessoas do mesmo id e com a idade de sua edição, assim deixando a edição em que foi mais
        // velho no topo
    }
    return ia->Id - ib->Id;
}


// ordena, remove duplicados e retorna novo tamanho
int Ordena_Limpa_Ordena(Atleta* competidores,int cont){ 

    // agrupa atletas iguais por id
    qsort(competidores, cont, sizeof(Atleta), compara2);

    int ZonaSeg = 1;

    // remove duplicados com id igual, mantendo o apenas o velho
    for(int k = 1; k < cont; k++){
        if (competidores[k].Id - competidores[k-1].Id == 0){}
        else{
            competidores[ZonaSeg] = competidores[k];
            ZonaSeg++;
        }
    }

    // ajusta tamanho final
    competidores = realloc(competidores, sizeof(Atleta) * ZonaSeg);

    // ordena definitivamente por idade, sempre com Ids diferentes, garantindo sempre os mais velhos definitivos
    qsort(competidores, ZonaSeg, sizeof(Atleta), compara1);

    return ZonaSeg; // retorna o tamanho do atual array que foi realocado
}

Atleta homens[10]; // array para armazenar 10 homens
Atleta mulheres[10];// criadas globalmente para servir pra a plotagem de gráfico
// separa e imprime os 10 mais velhos por gênero
void separador(Atleta* competidores, int ZonaSeg){
     // array para as 10 mulheres

    // peneira de separação
    for(int i = 0, h = 0, m = 0; i < ZonaSeg; i++){
        if(h < 10 && strcmp(competidores[i].genero, "Male") == 0){
            homens[h++] = competidores[i];
        }
        if(m < 10 && strcmp(competidores[i].genero, "Female") == 0){
            mulheres[m++] = competidores[i];
        }
        if(h == 10 && m == 10){
            i = ZonaSeg;
        }
    }

    if(homens[8].atletaNome[0] == '"'){
    char corretiva[100];
    for (int i = 1; i < strlen(homens[8].atletaNome) - 1; i++){ // corrige o decimo nome com aspas para dar problema na plotagem gráfica
        corretiva[i-1] = homens[8].atletaNome[i];
        corretiva[i] = '\0';
    }
    strcpy(homens[8].atletaNome, corretiva);
    }else{
    char corretiva[100];
    for (int i = 1; i < strlen(homens[9].atletaNome) - 1; i++){ // corrige o decimo nome com aspas para dar problema na plotagem gráfica
        corretiva[i-1] = homens[9].atletaNome[i];
        corretiva[i] = '\0';
    }
    strcpy(homens[8].atletaNome, corretiva);
    }

    // impressão final
    printf("\n->Homens\n");
    for (int i = 0; i < 10; i++) {
        printf("Atleta: %s | Esporte: %s | Idade: %d | Sexo: Masculinho | Ano participado: %d\n",
               homens[i].atletaNome, homens[i].esporte,
               homens[i].idade, homens[i].anoOlimpiada);
    }
    
    printf("\n->Mulheres\n");
    for (int i = 0; i < 10; i++) {
        printf("Atleta: %s | Esporte: %s | Idade: %d | Sexo: Feminino | Ano participado: %d\n",
               mulheres[i].atletaNome, mulheres[i].esporte,
               mulheres[i].idade, mulheres[i].anoOlimpiada);
    }
}


// libera toda a memória alocada anteriormente
void libera_Sistema(int pos, int num, Atleta* competidores, biosAtleta* lista){
    if (competidores != NULL) {
        free(competidores);
    }
    if (lista != NULL) {
        free(lista);
    }
}


static void escrever(){
    FILE* f_script = fopen("script.gp", "w");//Responsável por criar e abrir o arquivo de script
    if(f_script == NULL){
        puts("Erro ao gerar arquivo .gp");
        return;
    }

    //Comandos responsáveis por configurar o Gnuplot para gerar o gráfico

    fprintf(f_script, "set terminal png size 1200,800\n"); //Define formato PNG e tamanho
    fprintf(f_script, "set output 'Grafico_Q3.png'\n"); //Nome do arquivo de saída
    fprintf(f_script, "set title \"Atletas maisn velhos de todo os tempos\"\n"); 
    fprintf(f_script, "set ylabel \"Idade\"\n"); //Rótulo do eixo Y
    fprintf(f_script, "set xlabel \"Nomes\"\n"); //Rótulo de eixo X
    fprintf(f_script, "set grid y\n"); //Adiciona linhas de grade no eixo Y
    fprintf(f_script, "set style data histograms\n"); //Define que o estilo é histograma
    fprintf(f_script, "set style fill solid 1.0 border -1\n"); //Preenchimento sólido das barras
    fprintf(f_script, "set boxwidth 0.7\n"); //Largura das barras
    fprintf(f_script, "set xtics rotate by -90 scale 0\n"); //Rotaciona os nomes no eixo X em 90 graus
    fprintf(f_script, "set bmargin 10\n");  //Aumenta a margem inferior para caber os nomes
    fprintf(f_script, "set yrange [0:*]\n"); //Garante que o eixo Y comece no zero
   
    //Aqui, ele utiliza a coluna 2 para a altura e a coluna 1 para os nomes
    fprintf(f_script, "plot 'dados_q3.dat' using 2:xtic(1) notitle linecolor rgb \"#2980b9\"\n");
    fclose(f_script); //Fecha o arquivo de script
}

static void gerar_grafico_gnuplot(Atleta* lista) {
    escrever(); //Chama a função que cria o arquivo
    FILE *dados = fopen("dados_q3.dat", "w"); //Cria o arquivo que conterá os números
    if (dados == NULL) {
        printf("Não abriu arquivo! Sem gráfico nessa\n");
        return;
    }
   
    //Gravando os dados da struct no arquivo de texto
    for (int i = 0; i < 10; i++) {
            //Aqui, o fprintf coloca o nome entre as aspas para evitar erros com relação a nomes compostos
            fprintf(dados, "\"%s\" %d\n", homens[i].atletaNome, homens[i].idade);
            fprintf(dados, "\"%s\" %d\n", mulheres[i].atletaNome, mulheres[i].idade);
    }
    fclose(dados); 

    printf("\nProcessando grafico...\n");
    
    //Tentando executar o Gnuplot no sistema operacional
    int status = system("gnuplot script.gp");

    if (status == 0) {
         //Gnuplot rodou sem erros
         printf("Grafico gerado!\n");
         printf("Deseja abrir o grafico? Y/N\n");
         char c;
         scanf(" %c", &c);
         while(c != 'N'){
            if(c == 'Y'  || c =='y'){
                printf("\nAbrindo...");
                sleep(2);//Da uma pausa de 2 segundos antes de abrir o gráfico
                // Bloco de portabilidade: identifica o comando correto para cada SO
                #ifdef _WIN32
                system("start Grafico_Q3.png"); //Windows
                c = 'N';    // Windows
                #elif __APPLE__
                    system("open Grafico_Q3.png"); //macOS
                    c = 'N';     // macOS
                #elif __linux__
                    system("xdg-open Grafico_Q3.png"); //Linux 
                    c = 'N'; // Linux (Ubuntu, Debian, Fedora...)
                #elif __FreeBSD__
                    system("xdg-open Grafico_Q3.png");
                    c = 'N'; // FreeBSD
                #else
                    printf("Sistema nao identificado. Abra 'Grafico_Q3.png' manualmente.\n");
                    c = 'N';
                #endif
                remove("dados_q3.dat");
                remove("script.gp"); //apaga os arquivos depois de usados
            }
            else{
                printf("Inválido\n");
                printf("Digite Novamente\n");
                char d;
                scanf(" %c", &d);
                c = d;
            }
         }
    }
    // Limpeza: Deleta os arquivos auxiliares para não sujar a pasta do projeto
    sleep(1);
    remove("dados_q3.dat");
    remove("script.gp");// apaga mesmo que não gere om gráfico
}

// função de da questão
void gestao_q1(FILE* arq, FILE* bios){
    Atleta* catalogo;

    // carrega participações
    int posicao = gerarLista_Atletas(&catalogo, arq);

    // carrega bios
    biosAtleta* dados;
    int liberarBios = dados_Bios(&dados, bios);

    // cruza dados
    cruzar_Dados(posicao, catalogo, dados);

    // ordena e remove duplicados
    posicao = Ordena_Limpa_Ordena(catalogo, posicao);

    // imprime resultado final
    separador(catalogo, posicao);
    gerar_grafico_gnuplot(catalogo);
    // libera memória
    libera_Sistema(posicao, liberarBios, catalogo, dados);
}
