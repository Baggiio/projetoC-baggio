#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char linha[20];
    struct node *prox;
}node;  

typedef struct{
    node *inicio, *fim;
    int len;
}arr;

void putend(arr *a, char *linha){
    node *novo = (node*)malloc(sizeof(node));
    strcpy(novo->linha, linha);
    novo->prox = NULL;
    if(a->inicio == NULL){
        a->inicio = novo;
        a->fim = novo;
    }else{
        a->fim->prox = novo;
        a->fim = novo;
    }
    a->len++;
}

int cadastro(){
    char name[20], pass[20], cpf[20];
    printf("Nome:\n");
    scanf("%s", name);

    printf("CPF:\n");
    scanf("%s", &cpf);

    printf("Senha:\n");
    scanf("%s", &pass);
    
    char aux[20];
    snprintf(aux, sizeof(aux), "%s.txt", cpf);

    FILE *arq = fopen(aux, "w");

    fprintf(arq, "%s\n", name);
    fprintf(arq, "%s\n", cpf);
    fprintf(arq, "%s\n", pass);

    printf("Cadastro realizado com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    fclose(arq);
    return 0;
}

int valida(){
    char passs[20], cpf[20];
    printf("CPF:\n");
    scanf("%s", &cpf);
    printf("Senha:\n");
    scanf("%s", &passs);
    char aux[20];
    snprintf(aux, sizeof(aux), "%s.txt", cpf);

    FILE *arq = fopen(aux, "r");

    if(arq == NULL)
        return 1;
    char line[20];
    int i = 0;
    while(fgets(line, sizeof(line), arq)){
        if(i == 2){
            int comp = strcmp(line, passs);
            if(comp > 0){
                printf("Logado com sucesso!\n");
                return 0;
            }
        }
        i++;
    }
    return 1;
}

void transferencia(){
    char cpf[20], cpf2[20], valor[20], dia[8], mes[8], ano[16];
    printf("CPF:\n");
    scanf("%s", cpf);
    printf("CPF destinatário:\n");
    scanf("%s", cpf2);
    printf("Dia:\n");
    scanf("%s", dia);
    printf("Mês:\n");
    scanf("%s", mes);
    printf("Ano:\n");
    scanf("%s", ano);
    printf("Valor:\n");
    scanf("%s", valor);

    int val = valida();
    while(val != 0){
        printf("CPF ou senha incorretos!\nTente novamente!\n");
        val = valida();
    }

    if(val == 0){
        char aux[20], aux2[20];
        snprintf(aux, sizeof(aux), "%s.txt", cpf);
        snprintf(aux2, sizeof(aux2), "%s.txt", cpf2);

        FILE *arq = fopen(aux, "r");
        FILE *arq2 = fopen(aux2, "r");

        char name[8], name2[8];
        int i = 0;
        while(i < 2){
            if(i == 0){
                fgets(name, sizeof(name), arq);
                fgets(name2, sizeof(name2), arq2);
            }
            i++;
        }

        fclose(arq);
        fclose(arq2);

        arq = fopen(aux, "a");
        arq2 = fopen(aux2, "a");

        fprintf(arq, "-%s - to %s%s/%s/%s\n", valor, name2, dia, mes, ano);
        fprintf(arq2, "+%s - from %s%s/%s/%s\n", valor, name, dia, mes, ano);

        fclose(arq);
        fclose(arq2);
        
        printf("Transferencia realizada com sucesso!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
    else{
        printf("CPF ou senha invalidos!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
}

void despesa(){
    char cpf[20], valor[20], dia[8], mes[8], ano[16];
    printf("CPF:\n");
    scanf("%s", cpf);
    printf("Dia:\n");
    scanf("%s", dia);
    printf("Mês:\n");
    scanf("%s", mes);
    printf("Ano:\n");
    scanf("%s", ano);
    printf("Valor:\n");
    scanf("%s", valor);
    printf("Descrição:\n");
    char desc[20];
    scanf("%s", desc);
    printf("Categoria:\n");
    char cat[20];
    scanf("%s", cat);

    int val = valida();
    while(val != 0){
        printf("CPF ou senha incorretos!\nTente novamente!\n");
        val = valida();
    }

    if(val == 0){
        char aux[20];
        snprintf(aux, sizeof(aux), "%s.txt", cpf);

        FILE *arq = fopen(aux, "r");

        char name[8];
        int i = 0;
        while(i < 2){
            if(i == 0){
                fgets(name, sizeof(name), arq);
            }
            i++;
        }

        fclose(arq);

        arq = fopen(aux, "a");

        fprintf(arq, "-%s - %s - %s\n%s/%s/%s\n", valor, desc, cat, dia, mes, ano);

        fclose(arq);
        
        printf("Despesa realizada com sucesso!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
    else{
        printf("CPF ou senha invalidos!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
}

void relmen(){
    char cpf[20];
    printf("CPF:\n");
    scanf("%s", cpf);

    int val = valida();
    while(val != 0){
        printf("CPF ou senha incorretos!\nTente novamente!\n");
        val = valida();
    }
    if(val == 0){
        char aux[20];
        snprintf(aux, sizeof(aux), "%s.txt", cpf);

        FILE *arq = fopen(aux, "r");
        FILE *wr = fopen("relmen.txt", "w");

        char line[20];
        int i = 0;
        while(fgets(line, sizeof(line), arq)){
            if(i == 0){
                printf("Nome: %s", line);
                fprintf(wr, "Nome: %s", line);
            }
            else if(i > 2){
                if(i % 2 == 0){
                    char date[8];
                    
                }
                else{
                    printf("+%s", line);
                    fprintf(wr, "+%s", line);
                }
                printf("%s", line);
                fprintf(wr, "%s", line);
            }
            i++;
        }
        fclose(arq);
        fclose(wr);
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
    else{
        printf("CPF ou senha invalidos!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        getchar();
    }
}

int begin(){
    int i;
    printf("Possui cadastro?\n1 - Sim\n2 - Não\n");
    scanf("%d", &i);
    if(i == 1){
        int val = valida();
        if(val == 0){
            printf("Pressione qualquer tecla para continuar...\n");
            getchar();
            getchar();
            return 0;
        }else{
            printf("Senha ou CPF incorretos!\n");
            return 1;
        }
    }else{
        return cadastro();
    }
}

int menu(){
  int i;
  printf("Menu de opções:\n1 - Transferência\n2 - Poupança\n3 - Relatório Mensal\n4 - Relatório anual\n5 - Depósito\n6 - cadastrar despesa\n0 - Sair\nSelecione uma opção:\n");
  scanf("%d", &i);
  return i;
}

int main(void) {
    int cad = begin();
    if(cad == 0){
        while(cad == 0){
            int op = menu();
            switch(op){
                case 1:
                    printf("Transferência\n");
                    transferencia();
                    continue;
                case 2:
                    printf("Poupança\n");
                    break;
                case 3:
                    printf("Relatório Mensal\n");
                    relmen();
                    break;
                case 4:
                    printf("Relatório Anual\n");
                    break;
                case 5:
                    printf("Depósito\n");
                    break;
                case 6:
                    printf("Cadastrar despesa\n");
                    despesa();
                    break;
                case 0:
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida!\n");
                    break;
            }
        }   
    }else{
        main();
    }
    printf("%d", cad);
    return 0;
}