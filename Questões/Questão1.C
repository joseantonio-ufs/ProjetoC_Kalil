#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// itero results e pego todos os atletas (mesmo repetidos), pego seu id, ano, nome e esporte. mapeio o bios por hashkey, associo ao indice e puxo a 
// data de nascimento.  atribuo a idade ao atleta. meto um qsort e acabou;

typedef struct {
char esporte[100];
char atletaNome[150];
int Id;
int anoOlimpiada;
int idade;
} Atleta;


typedef struct {
char ano[10];
int atletaId;
} biosAtleta;


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

    sscanf(str,"%d",&comp.anoOlimpiada);

    if(posVirgulas[4] + 1 == posVirgulas[5]){strcpy(comp.atletaNome, "Vazio");}
    else{
        for (int i = posVirgulas[4] + 1, z = 0; i < posVirgulas[5]; i++, z++){
            comp.atletaNome[z] = str[i];
            comp.atletaNome[z+1] = '\0';
        }
    }

    char Id[10];
    if(posVirgulas[5] + 1 == posVirgulas[6]){comp.Id = -1;}
    else{
        for (int i = posVirgulas[5] + 1, z = 0; i < posVirgulas[6]; i++, z++){
            Id[z] = str[i];
            Id[z+1] = '\0';
        }
        comp.Id = atoi(Id);
    }

    if(posVirgulas[7] + 1 == posVirgulas[8]){strcpy(comp.esporte, "Vazio");}
    else{
        for (int i = posVirgulas[7] + 1, z = 0; i < posVirgulas[8]; i++, z++){
            comp.esporte[z] = str[i];
            comp.esporte[z+1] = '\0';
        }
    }

    return comp;
}
