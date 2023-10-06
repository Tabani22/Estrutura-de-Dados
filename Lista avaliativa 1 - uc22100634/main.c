#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ARTISTAS 100
#define MAX_ALBUNS 1000

typedef struct {
    char nome[100];
    char estilo[100];
    char origem[100];
    char albums[MAX_ALBUNS];
} Artista;

int compararArtistas(const void *a, const void *b) {
    const Artista *artistaA = (const Artista *)a;
    const Artista *artistaB = (const Artista *)b;
    return strcasecmp(artistaA->nome, artistaB->nome);
}

void lerArtistasOrdenados(Artista lista[], int *numArtistas) {
    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    *numArtistas = 0;
    char linha[1000];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "%[^\n]\n", lista[*numArtistas].nome);
        linha[strcspn(linha, "\n")] = '\0';
        fgets(lista[*numArtistas].estilo, sizeof(lista[*numArtistas].estilo), arquivo);
        fgets(lista[*numArtistas].origem, sizeof(lista[*numArtistas].origem), arquivo);

        char album[1000] = "";
        char *line;
        while (1) {
            line = fgets(album, sizeof(album), arquivo);
            if (line == NULL || strcasecmp(album, "==========\n") == 0) {
                break;
            }
            album[strcspn(album, "\n")] = '\0';
            strcat(lista[*numArtistas].albums, album);
        }

        (*numArtistas)++;
    }

    fclose(arquivo);

    qsort(lista, *numArtistas, sizeof(Artista), compararArtistas);
}

void escreverArtistas(Artista lista[], int numArtistas) {
    FILE *arquivo = fopen("artistas.txt", "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    int i;
    for (i = 0; i < numArtistas; i++) {
        fprintf(arquivo, "%s\n", lista[i].nome);
        fprintf(arquivo, "%s\n", lista[i].estilo);
        fprintf(arquivo, "%s\n", lista[i].origem);
        fprintf(arquivo, "%s", lista[i].albums);

        if (i < (numArtistas - 1)) {
            fputs("\n", arquivo);
        }

        fputs("==========\n", arquivo);
    }

    fclose(arquivo);
}

void inserirArtista(Artista lista[], int *numArtistas) {
    Artista novoArtista;

    printf("Nome do artista: ");
    fgets(novoArtista.nome, sizeof(novoArtista.nome), stdin);
    strtok(novoArtista.nome, "\n"); 

    printf("Genero musical: ");
    fgets(novoArtista.estilo, sizeof(novoArtista.estilo), stdin);
    strtok(novoArtista.estilo, "\n"); 

    printf("Local de criacao/nascimento: ");
    fgets(novoArtista.origem, sizeof(novoArtista.origem), stdin);
    strtok(novoArtista.origem, "\n"); 

    printf("Albuns (digite 'fim' para parar):\n");
    novoArtista.albums[0] = '\0'; 
    char album[1000];

    while (1) {
        fgets(album, sizeof(album), stdin);
        if (strcasecmp(album, "fim\n") == 0) {
            break;
        }
        strtok(album, "\n"); 
        strcat(novoArtista.albums, album);
        strcat(novoArtista.albums, "\n"); 
    }

    FILE *arquivo = fopen("artistas.txt", "a"); 
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    fprintf(arquivo, "%s\n", novoArtista.nome);
    fprintf(arquivo, "%s\n", novoArtista.estilo);
    fprintf(arquivo, "%s\n", novoArtista.origem);
    fprintf(arquivo, "%s", novoArtista.albums); 
    fputs("==========\n", arquivo); 

    fclose(arquivo);

    int i = *numArtistas;
    while (i > 0 && strcasecmp(novoArtista.nome, lista[i - 1].nome) < 0) {
        lista[i] = lista[i - 1];
        i--;
    }
    lista[i] = novoArtista;
    (*numArtistas)++;
    printf("Artista adicionado com sucesso.\n");
}

void removerArtista(Artista lista[], int *numArtistas) {
    char nomeParaRemover[100];
    printf("Digite o nome do artista a ser removido: ");
    fgets(nomeParaRemover, sizeof(nomeParaRemover), stdin);
    strtok(nomeParaRemover, "\n"); 

    int i, j;
    int encontrado = 0;

    for (i = 0; i < *numArtistas; i++) {
        if (strcasecmp(nomeParaRemover, lista[i].nome) == 0) {
            encontrado = 1;

            for (j = i; j < *numArtistas - 1; j++) {
                lista[j] = lista[j + 1];
            }

            (*numArtistas)--;
            printf("Artista removido com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Artista nao encontrado.\n");
        return;
    }

    FILE *arquivo = fopen("artistas.txt", "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    for (i = 0; i < *numArtistas; i++) {
        fprintf(arquivo, "%s\n", lista[i].nome);
        fprintf(arquivo, "%s\n", lista[i].estilo);
        fprintf(arquivo, "%s\n", lista[i].origem);
        fprintf(arquivo, "%s", lista[i].albums);

        if (i < (*numArtistas - 1)) {
            fputs("\n", arquivo);
        }

        fputs("==========\n", arquivo);
    }

    fclose(arquivo);
}

void editarArtista(Artista lista[], int numArtistas) {
    char nomeParaEditar[100];
    printf("Digite o nome do artista a ser editado: ");
    fgets(nomeParaEditar, sizeof(nomeParaEditar), stdin);

    int len = strlen(nomeParaEditar);
    if (nomeParaEditar[len - 1] == '\n') {
        nomeParaEditar[len - 1] = '\0';
    }

    int i;
    int encontrado = 0;
    for (i = 0; i < numArtistas; i++) {
        if (strcasecmp(nomeParaEditar, lista[i].nome) == 0) {
            encontrado = 1;
            printf("Escolha o campo a ser editado:\n");
            printf("1. Nome\n");
            printf("2. Estilo\n");
            printf("3. Local de origem\n");
            printf("4. Albuns\n");
            int opcao;
            printf("Opcao: ");
            scanf("%d", &opcao);
            getchar();

            switch (opcao) {
                case 1:
                    printf("Novo nome: ");
                    fgets(lista[i].nome, sizeof(lista[i].nome), stdin);
                    break;
                case 2:
                    printf("Novo estilo: ");
                    fgets(lista[i].estilo, sizeof(lista[i].estilo), stdin);
                    break;
                case 3:
                    printf("Novo local de origem: ");
                    fgets(lista[i].origem, sizeof(lista[i].origem), stdin);
                    break;
                case 4:
                    printf("Novos albuns (digite 'fim' para parar):\n");
                    lista[i].albums[0] = '\0';
                    char album[1000];
                    while (1) {
                        fgets(album, sizeof(album), stdin);
                        if (strcasecmp(album, "fim\n") == 0) {
                            break;
                        }
                        strcat(lista[i].albums, album);
                    }
                    break;
                default:
                    printf("Opção inválida.\n");
                    break;
            }
            escreverArtistas(lista, numArtistas);
            printf("Artista editado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Artista nao encontrado.\n");
    }
}

void buscarArtistaBinaria(Artista lista[], int numArtistas) {
    char nomeBusca[100];
    printf("Digite o nome do artista que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);

    int len = strlen(nomeBusca);
    if (nomeBusca[len - 1] == '\n') {
        nomeBusca[len - 1] = '\0';
    }

    int inicio = 0;
    int fim = numArtistas - 1;
    int encontrado = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int comparacao = strcasecmp(nomeBusca, lista[meio].nome);
        if (comparacao == 0) {
            printf("Artista encontrado:\n");
            printf("Nome: %s\n", lista[meio].nome);
            printf("Genero: %s\n", lista[meio].estilo);
            printf("Local: %s\n", lista[meio].origem);
            printf("Albuns:\n%s", lista[meio].albums);
            encontrado = 1;
            break;
        } else if (comparacao < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    if (!encontrado) {
        printf("Artista nao encontrado.\n");
    }
}


void buscarAlbums(Artista lista[], int numArtistas) {
    char nomeAlbum[100];
    printf("Digite o nome do álbum para buscar o artista: ");
    fgets(nomeAlbum, sizeof(nomeAlbum), stdin);

    int len = strlen(nomeAlbum);
    if (nomeAlbum[len - 1] == '\n') {
        nomeAlbum[len - 1] = '\0';
    }

    int encontrado = 0, i;
    for (i = 0; i < numArtistas; i++) {
        char *token = strtok(lista[i].albums, "\n");
        while (token != NULL) {
            if (strcasecmp(token, nomeAlbum) == 0) {
                printf("Artista do album '%s' encontrado:\n", nomeAlbum);
                printf("Nome: %s\n", lista[i].nome);
                printf("Genero: %s\n", lista[i].estilo);
                printf("Local: %s\n", lista[i].origem);
                encontrado = 1;
                break;
            }
            token = strtok(NULL, "\n");
        }
    }

    if (!encontrado) {
        printf("Album não encontrado.\n");
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Artista lista[MAX_ARTISTAS];
    int numArtistas = 0;
    lerArtistasOrdenados(lista, &numArtistas);

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Inserir artista\n");
        printf("2. Remover artista\n");
        printf("3. Editar artista\n");
        printf("4. Buscar artista\n");
        printf("5. Buscar albuns\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                inserirArtista(lista, &numArtistas);
                break;
            case 2:
                removerArtista(lista, &numArtistas);
                break;
            case 3:
                editarArtista(lista, numArtistas);
                break;
            case 4:
                buscarArtistaBinaria(lista, numArtistas);
                break;
            case 5:
                buscarAlbums(lista, numArtistas);
                break;
            case 6:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}
