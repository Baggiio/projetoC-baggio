#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

typedef struct
{
  char nome[10], senha[10], cpf[10];
} Login;

typedef struct
{
  int mes, dia, ano;
  float valor;
  char descr[100], cat[100];
} transf;

// Função para criar o arquivo de login do usuário e salvar os dados usando a struct Login nos aruivos "%cpf%.bin"
void cadastro()
{
  char cpf[100], cpfs[100];

  printf("Digite seu CPF: ");
  fgets(cpf, 100, stdin);
  sscanf(cpf, "%s", cpf);

  strcpy(cpfs, cpf);
  strcat(cpfs, "s.bin");
  strcat(cpf, ".bin");

  FILE *arq = fopen(cpf, "wb");
  FILE *arq2 = fopen(cpfs, "wb");

  Login user;
  strcpy(user.cpf, cpf);

  printf("Digite seu nome: ");
  fgets(user.nome, 100, stdin);
  sscanf(user.nome, "%[^\n]", user.nome);

  printf("Digite sua senha: ");
  fgets(user.senha, 100, stdin);
  sscanf(user.senha, "%[^\n]", user.senha);

  if (arq == NULL)
  {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  fwrite(&user, sizeof(Login), 1, arq);
  fwrite("\n", sizeof(char), 1, arq);

  fclose(arq);
  fclose(arq2);
  printf("Cadastro realizado com sucesso!\n");
  printf("\n");
}

// Função para verificar se o usuário já está cadastrado no sistema, chamada diversas vezes no código
int valida(char cpfx[10])
{
  char password[100], cpf[10];

  strcpy(cpf, cpfx);
  printf("Insira sua senha:\n");
  fgets(password, 100, stdin);
  sscanf(password, "%[^\n]s", password);

  strcat(cpf, ".bin");

  FILE *arq = fopen(cpf, "rb");
  if (arq == NULL)
  {
    printf("Erro ao abrir o arquivo");
    exit(1);
  }

  Login user;
  fread(&user, sizeof(Login), 1, arq);

  if (strcmp(user.senha, password) == 0)
  {
    printf("Usuário validado com sucesso!\n\n");
    return 0;
  }
  else
  {
    printf("CPF ou Senha incorretos! Tente novamente.\n");
    return 1;
  }

  return 1;
}

//calcula o saldo do usuário a partir do arquivo de transações "%cpf%s.bin"
double saldo(char cpf[10])
{
  char cpfs[10];
  strcpy(cpfs, cpf);
  strcat(cpfs, "s.bin");
  FILE *arq = fopen(cpfs, "rb");
  transf s;
  double money = 0;
  while (1)
  {
    int count = fread(&s, 1 + sizeof(transf), 1, arq);
    if (feof(arq))
    {
      break;
    }
    money += s.valor;
  }
  return money;
}

// escreve as transações no arquivo "%cpf%s.bin" de quem recebe e quem envia
void transferencia()
{
  transf t;
  char cpf2[10], cpf[10];
  char line[100];

  printf("Insira o seu CPF (apenas números):\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);

  printf("Insira a data da transferência (xx/yy/zzzz): ");
  fgets(line, 100, stdin);
  sscanf(line, "%d/%d/%d", &t.dia, &t.mes, &t.ano);

  printf("Insira o valor em reais (XX.YY): ");
  fgets(line, 100, stdin);
  sscanf(line, "%f", &t.valor);
  printf("Insira o CPF do destinarário: ");
  fgets(cpf2, 10, stdin);
  sscanf(cpf2, "%s", cpf2);

  char cpf2s[10], cpfdest[10];
  strcpy(cpf2s, cpf2);
  strcpy(cpfdest, cpf2);
  strcat(cpf2, ".bin");

  FILE *try = fopen(cpf2, "rb");
  while (try == NULL)
  {
    printf("CPF não encontrado, digite novamente: ");
    fgets(cpf2, 10, stdin);
    sscanf(cpf2, "%s", cpf2);
    try = fopen(cpf2, "rb");
  }
  fclose(try);

  printf("Digite a descrição da transferencia: ");
  fgets(t.descr, 100, stdin);
  sscanf(t.descr, "%[^\n]", t.descr);
  strcat(t.descr, " (Transferência)");

  printf("Digite a categoria da transferencia  (sem espaços): ");
  fgets(t.cat, 100, stdin);
  sscanf(t.cat, "%[^\n]", t.cat);

  printf("Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
  printf("Valor: R$ %.2f\n", t.valor);
  printf("CPF do destinatário: %s\n", cpfdest);
  printf("\nValide seus dados para transferir:\n");

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }

  if (val == 0)
  {
    strcat(cpf2s, "s.bin");
    FILE *arq = fopen(cpf2s, "ab");

    fwrite(&t, sizeof(transf), 1, arq);
    fwrite("\n", sizeof(char), 1, arq);
    fclose(arq);

    strcat(cpf, "s.bin");

    FILE *arq2 = fopen(cpf, "ab");
    t.valor = t.valor * -1;

    fwrite(&t, sizeof(transf), 1, arq2);
    fwrite("\n", sizeof(char), 1, arq);
    fclose(arq2);

    printf("Transferencia realizada com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
  else
  {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

typedef struct
{
  int d, m, y;
} Date;

// função para verificar se é ano bissexto
int countLeapYears(Date d)
{
  int years = d.y;

  if (d.m <= 2)
    years--;

  return years / 4 - years / 100 + years / 400;
}

// função para calcular o número de dias entre duas datas
int getDifference(Date dt1, Date dt2)
{

  const int monthDays[12] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};

  long int n1 = dt1.y * 365 + dt1.d;
  for (int i = 0; i < dt1.m - 1; i++)
  {
    n1 += monthDays[i];
  }
  n1 += countLeapYears(dt1);

  long int n2 = dt2.y * 365 + dt2.d;
  for (int i = 0; i < dt2.m - 1; i++)
  {
    n2 += monthDays[i];
  }
  n2 += countLeapYears(dt2);

  return (n2 - n1);
}

// função para calcular o saldo do usuário a partir de um intervalo de datas, poupança rendimento de 0.5% ao mês
void poupanca()
{
  transf t;
  Date dt1, dt2;
  char cpf[10];
  char line[100];
  int dia_apl, mes_apl, ano_apl;
  float valor_apl;
  printf("Insira o seu CPF (apenas números):\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);

  if (saldo(cpf) <= 0) {
    printf("Saldo insuficiente para aplicação!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    return;
  }
  else {

    printf("Insira a data de aplicação (xx/yy/zzzz): ");
    fgets(line, 100, stdin);
    sscanf(line, "%d/%d/%d", &dia_apl, &mes_apl, &ano_apl);
    printf("Insira a data de retirada (xx/yy/zzzz): ");
    fgets(line, 100, stdin);
    sscanf(line, "%d/%d/%d", &t.dia, &t.mes, &t.ano);
    printf("Insira o valor que deseja aplicar na poupança, em reais (XX.YY): ");
    fgets(line, 100, stdin);
    sscanf(line, "%f", &valor_apl);

    char cat[100];
    strcpy(cat, "Poupança");
    strcpy(t.cat, cat);

    char desc[100];
    strcpy(desc, "Rendimento da poupança");
    strcpy(t.descr, desc);

    int val = valida(cpf);
    while (val != 0)
    {
      val = valida(cpf);
    }

    if (val == 0)
    {
      double money = saldo(cpf);
      if (valor_apl > money) {
        printf("Saldo insuficiente para aplicação!\n");
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
        return;
      }
      else {
        strcat(cpf, "s.bin");

        dt1.d = dia_apl;
        dt1.m = mes_apl;
        dt1.y = ano_apl;
        dt2.d = t.dia;
        dt2.m = t.mes;
        dt2.y = t.ano;

        int dias = getDifference(dt1, dt2);
        int meses = dias / 30;
        float rendimento = valor_apl * 0.005 * meses;

        t.valor = rendimento;

        FILE *arq = fopen(cpf, "ab");

        fwrite(&t, sizeof(transf), 1, arq);
        fwrite("\n", sizeof(char), 1, arq);
        fclose(arq);

        printf("Aplicação realizada com sucesso!\n");
        printf("A sua aplicação rendeu R$ %.2f em %d meses.\n", rendimento, meses);
        printf("Pressione qualquer tecla para continuar...\n");
        getchar();
      }
    }
    else
    {
      printf("CPF ou senha invalidos!\n");
      printf("Pressione qualquer tecla para continuar...\n");
      getchar();
    }
  }
}

// cadastra uma despesa no arquivo de despesas do usuário "%cpf%s.bin"
void despesa()
{
  transf t;
  char cpf[10];
  char line[100];
  printf("Insira o seu CPF (apenas números):\n");
  fgets(line, 100, stdin);
  sscanf(line, "%[^\n]", cpf);
  printf("Insira a data (xx/yy/zzzz): ");
  fgets(line, 100, stdin);
  sscanf(line, "%d/%d/%d", &t.dia, &t.mes, &t.ano);
  printf("Insira o valor em reais (XX.YY): ");
  fgets(line, 100, stdin);
  sscanf(line, "%f", &t.valor);

  printf("Digite a descrição: ");
  fgets(t.descr, 100, stdin);
  sscanf(t.descr, "%[^\n]s", t.descr);
  printf("Digite a categoria (sem espaços): ");
  fgets(t.cat, 100, stdin);
  sscanf(t.cat, "%[^\n]s", t.cat);
  strcat(t.descr, " (Despesa)");

  printf("Data: %d/%d/%d\n", t.dia, t.mes, t.ano);
  printf("Valor: R$ %.2f\n", t.valor);
  printf("Valide seus dados para cadastrar:\n");

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }

  if (val == 0)
  {
    strcat(cpf, "s.bin");

    FILE *arq2 = fopen(cpf, "ab");
    t.valor = t.valor * -1;
    fwrite(&t, sizeof(transf), 1, arq2);
    fwrite("\n", sizeof(char), 1, arq2);
    fclose(arq2);

    printf("Despesa cadastrada com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
  else
  {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

// le as movimentações do no ano escolhido e salva em uma pasta com arquivos nomeados de acordo com a categoria das movimentações do usuário "%cpf%s.bin"
void relanu()
{
  char cpf[10], cpfdir[10];
  int anoinx;
  char line[100];
  printf("Insira o seu CPF (apenas números):\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);

  printf("Digite o ano que deseja consultar (xxxx):\n");
  fgets(line, 10, stdin);
  sscanf(line, "%d", &anoinx);

  const int anoin = anoinx;
  strcpy(cpfdir, cpf);

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }

  if (val == 0)
  {
    strcat(cpf, "s.bin");

    transf t;
    char filename[100];
    strcpy(filename, "Relatorio Anual - ");
    strcat(filename, cpfdir);
    strcat(filename, ".txt");

    FILE *arq = fopen(cpf, "rb");
    FILE *wr = fopen(filename, "wb");
    while (1)
    {

      fread(&t, 1 + sizeof(transf), 1, arq);
      if (feof(arq))
      {
        break;
      }

      if (t.ano == anoin)
      {

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
  }
  else
  {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

// testa se o arquivo existe
void ifexist(char cat[100], char diretorio[100])
{
  char path[100];
  strcpy(path, diretorio);
  strcat(cat, ".txt");
  strcat(path, cat);
  strcpy(cat, path);
  FILE *arq = fopen(cat, "r");
  if (arq == NULL)
  {
    FILE *arq = fopen(cat, "w");
    fclose(arq);
  }
  else
  {
    fclose(arq);
  }
}

// le as movimentações do mês escolhido e salva em uma pasta com arquivos nomeados de acordo com a categoria das movimentações do usuário "%cpf%s.bin"
void relmen()
{
  char cpf[10], cpfdir[10];
  int mesinx, anoinx;
  char line[100];
  printf("Insira o seu CPF (apenas números):\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);
  printf("Digite o mês e ano que deseja consultar (xx/yyyy):\n");
  fgets(line, 10, stdin);
  sscanf(line, "%d/%d", &mesinx, &anoinx);

  const int mesin = mesinx;
  const int anoin = anoinx;
  strcpy(cpfdir, cpf);

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }

  if (val == 0)
  {
    strcat(cpf, "s.bin");

    FILE *arq = fopen(cpf, "rb");

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
        }
        else
        {
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

    while (1)
    {
      fread(&t, 1 + sizeof(transf), 1, arq);
      if (feof(arq))
      {
        break;
      }

      if (t.mes == mesin && t.ano == anoin)
      {

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
  }
  else
  {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

// le as movimentações exibe o saldo para o usuario
void exibe_saldo()
{
  char cpf[10];
  printf("Insira o seu CPF:\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }
  if (val == 0)
  {
    double money = saldo(cpf);
    printf("Saldo atual: R$ %.2lf\n", money);
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

// adiciona uma despesa
void deposito()
{
  char cpf[10];
  printf("Insira o seu CPF:\n");
  fgets(cpf, 10, stdin);
  sscanf(cpf, "%s", cpf);

  int val = valida(cpf);
  while (val != 0)
  {
    val = valida(cpf);
  }

  if (val == 0)
  {
    strcat(cpf, "s.bin");

    FILE *arq = fopen(cpf, "ab");
    transf t;
    printf("Insira a data do depósito (xx/yy/zzzz): ");
    char line[100];
    fgets(line, 100, stdin);
    sscanf(line, "%d/%d/%d", &t.dia, &t.mes, &t.ano);
    printf("Insira o valor em reais (XX.YY): ");
    fgets(line, 100, stdin);
    sscanf(line, "%f", &t.valor);
    printf("Insira a descrição:\n");
    fgets(t.descr, 100, stdin);
    sscanf(t.descr, "%[^\n]", t.descr);
    char cat[100];
    strcpy(cat, "Deposito");
    strcpy(t.cat, cat);
    fwrite(&t, sizeof(transf), 1, arq);
    fwrite("\n", 1, 1, arq);
    fclose(arq);
    printf("\nDeposito realizado com sucesso!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
  else
  {
    printf("CPF ou senha invalidos!\n");
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
  }
}

// função que inicializa o código para fazer o login
int begin()
{
  char aux[100];
  int i;
  printf("Possui cadastro?\n1 - Sim\n2 - Não\n");
  fgets(aux, 10, stdin);
  sscanf(aux, "%d", &i);
  if (i == 1)
  {
    return 0;
  }
  else
  {
    cadastro();
    return 0;
  }
}

// função que imprime o menu
int menu()
{
  char aux[100];
  int i;
  printf("Menu de opções:\n1 - Saldo\n2 - Transferência\n3 - Poupança\n4 - Relatório Mensal\n5 - Relatório Anual\n6 - Depósito\n7 - Cadastrar Despesa\n8 - Novo cadastro\n0 - Sair\nSelecione uma opção:\n");
  fgets(aux, 10, stdin);
  sscanf(aux, "%d", &i);
  return i;
}

// main que chama as funções com base no menu
int main(void)
{
  int cad = begin();
  if (cad == 0)
  {
    while (cad == 0)
    {
      int op = menu();
      switch (op)
      {
      case 1:
        printf("Consultar Saldo:\n");
        exibe_saldo();
        continue;
      case 2:
        printf("\nTransferência:\n");
        transferencia();
        continue;
      case 3:
        printf("\nPoupança:\n");
        poupanca();
        continue;
      case 4:
        printf("\nRelatório Mensal:\n");
        relmen();
        continue;
      case 5:
        printf("\nRelatório Anual:\n");
        relanu();
        continue;
        ;
      case 6:
        printf("\nDepósito:\n");
        deposito();
        continue;
      case 7:
        printf("\nCadastrar Despesa:\n");
        despesa();
        continue;
      case 8:
        printf("\nCadastrar novo usuário:\n");
        cadastro();
        continue;
      case 0:
        printf("Saindo...\n");
        exit(1);
      default:
        printf("Opção inválida!\n");
        continue;
      }
    }
  }
  else
  {
    main();
  }
  printf("%d", cad);
  return 0;
}