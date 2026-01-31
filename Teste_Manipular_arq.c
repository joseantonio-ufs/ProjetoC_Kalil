// 🥉 Escolha uma edição aleatória das Olimpíadas e liste os atletas mais velhos por modalidade esportiva
//, além disso liste o total de Olimpíadas competidas pelos 3 mais velhos desta edição escolhida.

#include <stdio.h>

int main() {

// processo básico de abertura de arquivo criando um ponteiro do tipo FILE e passando como argumento o diretório do csv e ao lado o modo de uso
// r - read a - escreve (final) w - sobrescreve ------ r+ w+ a+ escrever e lem ao mesmo tempo
//(cuidado com as barras invertidas)
// devem ser duplas para não ver como caractere de escape
    FILE *arq = fopen("C:\\Users\\joser\\Downloads\\Projota\\results.csv", "r");

    if (arq == NULL) {
        puts("Error opening file"); // retorna um erro caso o ponteiro que receberia o arquivo seja NULL
        return 1;
    }


    char ch[200];
    while (fgets(ch, sizeof(ch), arq) != NULL) { /// caso o ponteiro não seja NULL entramos num loop em que supomos que cada linha tenha no 
        //máximo 200 caracteres e puxamos linha a linha do csv até o fget retornar um NULL (não pegou nada) e imprimimos tudo durante o loop
        printf("%s", ch);
    }

    fclose(arq); // fecha o arquivo (importante!)
    return 0;
}
