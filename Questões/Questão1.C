// 🥇 Faça o ranking dos 10 atletas mais velhos de todos os tempos que participaram das olimpíadas, por gênero.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// mapa - > itero results e pego todos os atletas (mesmo repetidos), pego seu id, ano, nome e esporte. mapeio o bios por hashkey, associo ao indice e puxo a data de nascimento.  atribuo a idade ao atleta. meto um qsort e acabou;

// struct pra salvar o que importa de cada atleta
typedef struct {
char esporte[100];
char atletaNome[150];
int Id;
int anoOlimpiada;
int idade;
} Atleta;


// struct pra salvar ano de nascimento e id para cruzar dados com o arquivo bios e extrais informações necessárias
typedef struct {
char ano[10];
int atletaId;
} biosAtleta;


// Função pra quebrar cada linha do csv em seus método, baseado na posição das vírgulas
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

// Puxo o ano da Olimpíada direto no sscanf pois é o primeiro campo e sempre aparece, então não tem risco de erro
    sscanf(str,"%d",&comp.anoOlimpiada);

// a partir daqui as extrações são com base nas posições das vírgulas. note que se as virgulas estiverem lado a lacvo o campo é considerado vazio
    
// Se o campo de nome tiver vazio já copia como "Vazio". Se não, copia caractere a caractere com laço
    if(posVirgulas[4] + 1 == posVirgulas[5]){strcpy(comp.atletaNome, "Vazio");}
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

// Extraio o id do atleta. Se não tiver nada entre as vírgulas, vira -1.
    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.Id = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.Id = atoi(Id);
    }

    // a mesma ideia para extrais esportes
    if(posVirgulas[7] + 1 == posVirgulas[8]){strcpy(comp.esporte, "Vazio");}
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    return comp;
}



// Parser pra carregar as datas de nascimento dos atletas, dats encontradas com base nos ids por serem únicos
biosAtleta ParserBios(char str[]){

    int posVirgulas[15];
    int camposLidos = 0;
    int aspas = 0;

    // Mesma estratégia das vírgulas do parser anterior
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
 
    biosAtleta comp; // criamos o objeto que salva os dados de bios atleta

    // Pego o ID pra servir de chave pro cruzamento.
    char Id[10];
    if(posVirgulas[6] + 1 == posVirgulas[7]){comp.atletaId = -1;}
    else{
        for (int i = posVirgulas[6] + 1, z = 0; i < posVirgulas[7]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.atletaId = atoi(Id);
    }


    // Aqui pego o ano de nascimento com base na condição de haver 4 digitos seguidos. isso ocorre porque o campo nascimento pode aparecer
    // de 4 formas diferentes, incluindo sem o ano e só o local
    char nascimento[150];
    if(posVirgulas[3] + 1 == posVirgulas[4]){strcpy(comp.ano,"Vazio");}
    else{
        for (int i = posVirgulas[3] + 1, z = 0; i < posVirgulas[4]; i++, z++){
            nascimento[z] = str[i];
            nascimento[z+1] = '\0';
        }
        // Se não achar o ano, coloco 3000 pra o cara ser cotado com ano negatico no cáuculo da idade. assim, ficando em último na ordenação
        comp.ano = 3000;
        for(int s = 0; s < strlen(nascimento) - 3; s++){
            if(isdigit(nascimento[s])){
                if(isdigit(nascimento[s+1])){
                    if(isdigit(nascimento[s+2])){
                        if(isdigit(nascimento[s+3])){
                            char anoReal[5];
                            anoReal[0] = nascimento[s];
                            anoReal[1] = nascimento[s+1];
                            anoReal[2] = nascimento[s+2];// necessario que 4 caracteres seguidos sejam digitos
                            anoReal[3] = nascimento[s+3];
                            anoReal[4] = '\0';
                            // Achei o ano, converto e saio do loop.
                            comp.ano = atoi(anoReal);
                        }
                    }
                }
            }
        }
    }
    return comp;
}
