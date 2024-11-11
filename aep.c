#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SENHAS 100
#define DESLOCAMENTO 3
#define ARQUIVO "contas.txt"

typedef struct {
    char usuario[50];
    char senha[50];
} Conta;

Conta contas[MAX_SENHAS];
int num_contas = 0;

int gerar_codigo_verificacao() {
    return rand() % 9000 + 1000;
}

void criptografar(char senha[]) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] += DESLOCAMENTO;
    }
}

void descriptografar(char senha[]) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] -= DESLOCAMENTO;
    }
}

void salvar_contas() {
    FILE *file = fopen(ARQUIVO, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    for (int i = 0; i < num_contas; i++) {
        fprintf(file, "%s %s\n", contas[i].usuario, contas[i].senha);
    }

    fclose(file);
}

void carregar_contas() {
    FILE *file = fopen(ARQUIVO, "r");
    if (file == NULL) {
        printf("Arquivo de contas não encontrado. Nenhuma conta carregada.\n");
        return;
    }

    while (fscanf(file, "%s %s", contas[num_contas].usuario, contas[num_contas].senha) != EOF) {
        descriptografar(contas[num_contas].senha);
        num_contas++;
    }

    fclose(file);
}

void cadastrar() {
    if (num_contas >= MAX_SENHAS) {
        printf("Limite de contas atingido.\n");
        return;
    }
    printf("Informe o usuario: ");
    scanf("%s", contas[num_contas].usuario);
    printf("Informe a senha: ");
    scanf("%s", contas[num_contas].senha);
    
    criptografar(contas[num_contas].senha);
    num_contas++;

    int codigo = gerar_codigo_verificacao();
    printf("Codigo de verificacao (temporario): %d\n", codigo);
    int codigo_usuario;
    printf("Digite o codigo de verificacao para confirmar: ");
    scanf("%d", &codigo_usuario);

    if (codigo_usuario == codigo) {
        printf("Conta cadastrada com sucesso!\n");
        salvar_contas();  // Salvar as contas após o cadastro
    } else {
        printf("Codigo de verificacao incorreto. Cadastro cancelado.\n");
    }
}

void editar() {
    char usuario[50];
    int encontrado = -1;
    printf("Informe o usuario para editar: ");
    scanf("%s", usuario);
    
    for (int i = 0; i < num_contas; i++) {
        if (strcmp(contas[i].usuario, usuario) == 0) {
            encontrado = i;
            break;
        }
    }
    
    if (encontrado == -1) {
        printf("Usuario nao encontrado.\n");
        return;
    }
    
    descriptografar(contas[encontrado].senha);
    printf("Informe a nova senha: ");
    scanf("%s", contas[encontrado].senha);
    criptografar(contas[encontrado].senha);

    int codigo = gerar_codigo_verificacao();
    printf("Codigo de verificacao (temporario): %d\n", codigo);
    int codigo_usuario;
    printf("Digite o codigo de verificacao para confirmar: ");
    scanf("%d", &codigo_usuario);

    if (codigo_usuario == codigo) {
        printf("Senha editada com sucesso!\n");
        salvar_contas();  // Salvar as contas após a edição
    } else {
        printf("Codigo de verificacao incorreto. Edicao cancelada.\n");
    }
}

void excluir() {
    char usuario[50];
    int encontrado = -1;
    printf("Informe o usuario para excluir: ");
    scanf("%s", usuario);
    
    for (int i = 0; i < num_contas; i++) {
        if (strcmp(contas[i].usuario, usuario) == 0) {
            encontrado = i;
            break;
        }
    }
    
    if (encontrado == -1) {
        printf("Usuario nao encontrado.\n");
        return;
    }
    
    for (int i = encontrado; i < num_contas - 1; i++) {
        contas[i] = contas[i + 1];
    }
    num_contas--;

    int codigo = gerar_codigo_verificacao();
    printf("Codigo de verificacao (temporario): %d\n", codigo);
    int codigo_usuario;
    printf("Digite o codigo de verificacao para confirmar: ");
    scanf("%d", &codigo_usuario);

    if (codigo_usuario == codigo) {
        printf("Conta excluida com sucesso!\n");
        salvar_contas();  // Salvar as contas após a exclusão
    } else {
        printf("Codigo de verificacao incorreto. Exclusao cancelada.\n");
    }
}

void listar() {
    if (num_contas == 0) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }
    
    printf("Lista de contas:\n");
    for (int i = 0; i < num_contas; i++) {
        printf("Usuario: %s, Senha (criptografada): %s\n", contas[i].usuario, contas[i].senha);
    }
}

int main() {
    srand(time(NULL));
    
    // Carregar as contas do arquivo ao iniciar
    carregar_contas();
    
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1- Cadastro\n");
        printf("2- Editar\n");
        printf("3- Excluir\n");
        printf("4- Listar\n");
        printf("5- Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                cadastrar();
                break;
            case 2:
                editar();
                break;
            case 3:
                excluir();
                break;
            case 4:
                listar();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 5);
    
    return 0;
}
