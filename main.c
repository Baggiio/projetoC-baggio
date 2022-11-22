#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

typedef struct {
  char nome[10], senha[10], cpf[10];
} Login;

typedef struct {
  int mes, dia, ano;
  float valor;
  char descr[100], cat[100];
} transf;

void cadastro() {
  char cpf[100], cpfs[100];

  printf("Digite seu CPF: ");
  scanf("%s", cpf);

  strcpy(cpfs, cpf);
  strcat(cpfs, "s.bin");
  strcat(cpf, ".bin");

  FILE *arq = fopen(cpf, "wb");
  FILE *arq2 = fopen(cpfs, "wb");

  Login user;
  strcpy(user.cpf, cpf);

  printf("Digite seu nome: ");
  scanf("%s", user.nome);

  printf("Digite sua senha: ");
  scanf("%s", user.senha);

  if (arq == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  fwrite(&user, sizeof(Login), 1, arq);
  fwrite("\n", sizeof(char),1, arq);

  fclose(arq);
  fclose(arq2);
  printf("Cadastro realizado com sucesso!\n");
  printf("Pressione qualquer tecla para continuar...\n");
  getchar();
  getchar();
}

int valida(char cpfx[10]) {
  char password[100], cpf[10];

  strcpy(cpf, cpfx);
  printf("Insira sua senha:\n");
  scanf("%s", password);

  strcat(cpf, ".bin");

  FILE *arq = fopen(cpf, "rb");
  if (arq == NULL) {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  Login user;
  fread(&user, sizeof(Login), 1, arq);

  if (strcmp(user.senha, password) == 0) {
    printf("Usuário validado com sucesso!\n\n");
    return 0;
  } else {
    printf("CPF ou Senha incorretos! Tente novamente.\n");
    return 1;
  }

  return 1;
}

void transferencia() {
  transf t;
  char cpf2[10], cpf[10];

  printf("Insira o seu CPF (apenas números):\n");
  scanf("%s", cpf);
  printf("Insira a data da transferência (xx/yy/zzzz): ");
  scanf("%d/%d/%d", &t.dia, &t.mes, &t.ano);
  printf("Insira o valor em reais (XX.YY): ");
  scanf("%f", &t.valor);
  printf("Insira o CPF do destinarário: ");
  scanf("%s", cpf2);

  char cpf2s[10], cpfdest[10];
  strcpy(cpf2s, cpf2);
  strcpy(cpfdest, cpf2);
  strcat(cpf2, ".bin");

  FILE *try = fopen(cpf2, "rb");
  while (try == NULL) {
    printf("CPF não encontrado, digite novamente: ");
    scanf("%s", cpf2);
    try = fopen(cpf2, "rb");
  }
  fclose(try);

  printf("Digite a descrição da transferencia: ");
  scanf("%s", t.descr);
  printf("Digite a categoria da transferencia: ");
  scanf("%s", t.cat);

  printf("Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
  printf("Valor: R$ %.2f\n", t.valor);
  printf("CPF do destinatário: %s\n", cpfdest);
  printf("\nValide seus dados para transferir:\n");

  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }

  if (val == 0) {
    strcat(cpf2s, "s.bin");
    FILE *arq = fopen(cpf2s, "ab");

    fwrite(&t, sizeof(transf), 1, arq);
    fwrite("\n", sizeof(char),1, arq);
    fclose(arq);

    strcat(cpf, "s.bin");

    FILE *arq2 = fopen(cpf, "ab");
    t.valor = t.valor * -1;

    fwrite(&t, sizeof(transf), 1, arq2);
    fwrite("\n", sizeof(char),1, arq);
    fclose(arq2);

    printf("Transferencia realizada com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

void despesa() {
  transf t;
  char  cpf[10];

  printf("Insira o seu CPF (apenas números):\n");
  scanf("%s", cpf);
  printf("Insira a data (xx/yy/zzzz): ");
  scanf("%d/%d/%d", &t.dia, &t.mes, &t.ano);
  printf("Insira o valor em reais (XX.YY): ");
  scanf("%f", &t.valor);

  printf("Digite a descrição: ");
  scanf("%s", t.descr);
  printf("Digite a categoria: ");
  scanf("%s", t.cat);

  printf("Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
  printf("Valor: R$ %.2f\n", t.valor);
  printf("Valide seus dados para cadastrar:\n");

  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }

  if (val == 0) {
    strcat(cpf, "s.bin");

    FILE *arq2 = fopen(cpf, "ab");
    t.valor = t.valor * -1;
    fwrite(&t, sizeof(transf), 1, arq2);
    fwrite("\n", sizeof(char),1, arq2);
    fclose(arq2);

    printf("Despesa cadastrada com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

void relanu(){
  char cpf[10], cpfdir[10];
  int anoinx;
  printf("Insira o seu CPF (apenas números):\n");
  scanf("%s", cpf);

  printf("Digite o ano que deseja consultar (xxxx):\n");
  scanf("%d", &anoinx);

  const int anoin = anoinx;
  strcpy(cpfdir, cpf);

  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }
  
  if (val == 0) {
    strcat(cpf, "s.bin");


    transf t;
    char filename[100];
    strcpy(filename, "Relatorio Anual - ");
    strcat(filename, cpfdir);
    strcat(filename, ".txt");

    FILE *arq = fopen(cpf, "rb");
    FILE *wr = fopen(filename, "wb");
    while (1) {

      fread(&t, 1+sizeof(transf), 1, arq);
      if (feof(arq)) {
        break;
      }

      if (t.ano == anoin) {

        fprintf(wr, "Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
        fprintf(wr, "Valor: R$ %.2f\n", t.valor);
        fprintf(wr, "Descrição: %s\n", t.descr);
        fprintf(wr, "Categoria: %s\n", t.cat);
        fprintf(wr, "\n");

      }
    }
    printf("Relatório Anual gerado com sucesso!\n");
    fclose(arq);
    fclose(wr);
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

void ifexist(char cat[100], char diretorio[100]) {
  char path[100];
  strcpy(path, diretorio);
  strcat(cat, ".txt");
  strcat(path, cat);
  strcpy(cat, path);
  FILE *arq = fopen(cat, "r");
  if (arq == NULL) {
    FILE *arq = fopen(cat, "w");
    fclose(arq);  
  } else {
    fclose(arq);
  }
}

void relmen() {
  char cpf[10], cpfdir[10];
  int mesinx, anoinx;
  printf("Insira o seu CPF (apenas números):\n");
  scanf("%s", cpf);
  printf("Digite o mês e ano que deseja consultar (xx/yyyy):\n");
  scanf("%d/%d", &mesinx, &anoinx);

  const int mesin = mesinx;
  const int anoin = anoinx;
  strcpy(cpfdir, cpf);

  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }
  
  if (val == 0) {
    strcat(cpf, "s.bin");

    FILE *arq = fopen(cpf, "rb");

    // fseek(arq, sizeof(Login)+1, SEEK_SET);

    transf t;

    char directory[50];
    strcpy(directory, "Relatorio Mensal - ");

    strcat(directory, cpfdir);

    char dirwin[50];
    strcpy(dirwin, directory);

    strcat(directory, "\\");

    DIR *dir = opendir(directory);
    if (dir)
    {
      struct dirent *dire;
      while ((dire = readdir(dir)) != NULL)
      {
        if (strcmp(dire->d_name, ".") == 0 || strcmp(dire->d_name, "..") == 0)
        {
          continue;
        } else {
          char rem[100];
          strcpy(rem, directory);
          strcat(rem, dire->d_name);
          remove(rem);
        }
      }
      closedir(dir);
    }
    else if (ENOENT == errno)
    {
      mkdir(directory);
      closedir(dir);
    }
    else
    {
      printf("Erro ao criar diretório!\n");
      closedir(dir);
    }

    while (1) {
      fread(&t, 1+sizeof(transf), 1, arq);
      if (feof(arq))
      {
        break;
      }

      if (t.mes == mesin && t.ano == anoin) {

        char aux[100];

        strcpy(aux, t.cat);
        ifexist(aux, directory);
        FILE *wr = fopen(aux, "a");

        fprintf(wr, "Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
        fprintf(wr, "Valor: R$ %.2f\n", t.valor);
        fprintf(wr, "Descrição: %s\n", t.descr);
        fprintf(wr, "Categoria: %s\n", t.cat);
        fprintf(wr, "\n");
        fclose(wr);
      }
    }
    printf("Relatório Mensal gerado com sucesso!\n");
    fclose(arq);

    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

void saldo() {
  char cpf[10];
  printf("CPF:\n");
  scanf("%s", cpf);
  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }
  if (val == 0) {
    strcat(cpf, "s.bin");
    FILE *arq = fopen(cpf, "rb");
    transf s;
    double money = 0;
    while (1) {
      int count = fread(&s, 1 + sizeof(transf), 1, arq);
      if (feof(arq))
      {
        break;
      }
      money += s.valor;
    }
    printf("Saldo atual: R$ %.2lf\n", money);
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

void deposito(){
  char cpf[10];
  printf("Insira o seu CPF:\n");
  scanf("%s", cpf);

  int val = valida(cpf);
  while (val != 0) {
    val = valida(cpf);
  }
  
  
  if (val == 0) {
    strcat(cpf, "s.bin");

    FILE *arq = fopen(cpf, "ab");
    transf t;
    printf("Insira a data do depósito (xx/yy/zzzz): ");
    scanf("%d/%d/%d", &t.dia, &t.mes, &t.ano);
    printf("Insira o valor em reais (XX.YY): ");
    scanf("%f", &t.valor);
    printf("Insira a descrição:\n");
    scanf("%s", t.descr);
    char cat[100];
    strcpy(cat, "Deposito");
    strcpy(t.cat, cat);
    fwrite(&t, sizeof(transf), 1, arq);
    fwrite("\n", 1, 1, arq);
    fclose(arq);
    printf("Deposito realizado com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

  } else {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
  }
}

int begin() {
  int i;
  printf("Possui cadastro?\n1 - Sim\n2 - Não\n");
  scanf("%d", &i);
  if (i == 1) {
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();
    return 0;
  } else {
    cadastro();
    return 0;
  }
}

int menu() {
  int i;
  printf("Menu de opções:\n1 - Consultar Saldo\n2 - Transferência\n3 - Poupança\n4 - Relatório "
         "Mensal\n5 - Relatório Anual\n6 - Depósito\n7 - Cadastrar Despesa\n0 "
         "- Sair\nSelecione uma opção:\n");
  scanf("%d", &i);
  return i;
}

int main(void) {
  int cad = begin();
  if (cad == 0) {
    while (cad == 0) {
      int op = menu();
      switch (op) {
      case 1:
        printf("Consultar Saldo:\n");
        saldo();
        break;
      case 2:
        printf("\nTransferência:\n");
        transferencia();
        continue;
      case 3:
        printf("\nPoupança:\n");
        break;
      case 4:
        printf("\nRelatório Mensal:\n");
        relmen();
        break;
      case 5:
        printf("\nRelatório Anual:\n");
        relanu();
        continue;;
      case 6:
        printf("\nDepósito:\n");
        deposito();
        continue;
      case 7:
        printf("\nCadastrar Despesa:\n");
        despesa();
        break;
      case 0:
        printf("Saindo...\n");
        exit(1);
      default:
        printf("Opção inválida!\n");
        break;
      }
    }
  } else {
    main();
  }
  printf("%d", cad);
  return 0;
}