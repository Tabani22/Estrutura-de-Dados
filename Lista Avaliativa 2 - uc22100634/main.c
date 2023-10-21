#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Music {
    char artist[100];
    char song[100];
    struct Music* next;
    struct Music* prev;
};

struct Music* createMusic(char artist[], char song[]) {
    struct Music* newMusic = (struct Music*)malloc(sizeof(struct Music));
    strcpy(newMusic->artist, artist);
    strcpy(newMusic->song, song);
    newMusic->next = newMusic;
    newMusic->prev = newMusic;
    return newMusic;
}

struct Music* loadPlaylist() {
    FILE* file = fopen("musicas.txt", "r");
    if (file == NULL) {
        return NULL;
    }

    struct Music* playlist = NULL;
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char artist[100], song[100];
        sscanf(line, "%[^;];%[^\n]", artist, song);
        insertMusic(&playlist, artist, song);
    }
    fclose(file);

    return playlist;
}

void savePlaylist(struct Music* playlist) {
    FILE* file = fopen("musicas.txt", "w");
    if (file == NULL) {
        printf("O arquivo 'musicas.txt' nao foi encontrado.\n");
        return;
    }

    struct Music* current = playlist;
    do {
        fprintf(file, "%s;%s\n", current->artist, current->song);
        current = current->next;
    } while (current != playlist);

    fclose(file);
}

void insertMusic(struct Music** playlist, char artist[], char song[]) {
    struct Music* current = *playlist;
    int alreadyExists = 0;
    if (current != NULL) {
        do {
            if (strcmp(current->artist, artist) == 0 && strcmp(current->song, song) == 0) {
                alreadyExists = 1;
                printf("Essa musica ja existe na playlist.\n");
                break;
            }
            current = current->next;
        } while (current != *playlist);
    }

    if (!alreadyExists) {
        struct Music* newMusic = createMusic(artist, song);
        if (*playlist == NULL) {
            *playlist = newMusic;
        } else {
            struct Music* last = (*playlist)->prev;
            last->next = newMusic;
            newMusic->prev = last;
            newMusic->next = *playlist;
            (*playlist)->prev = newMusic;
        }
    }
}

void removeMusic(struct Music** playlist, char artist[], char song[]) {
    if (*playlist == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    struct Music* current = *playlist;
    do {
        if (strcmp(current->artist, artist) == 0 && strcmp(current->song, song) == 0) {
            if (current == *playlist) {
                if (current->next == current) {
                    *playlist = NULL;
                } else {
                    *playlist = current->next;
                }
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            free(current);
            printf("Musica removida da playlist.\n");

            FILE* file = fopen("musicas.txt", "w");
            if (file == NULL) {
                printf("O arquivo 'musicas.txt' nao foi encontrado.\n");
                return;
            }

            struct Music* playlist_current = *playlist;
            while (playlist_current != NULL) {
                fprintf(file, "%s;%s\n", playlist_current->artist, playlist_current->song);
                playlist_current = playlist_current->next;
            }

            fclose(file);
            return;
        }
        current = current->next;
    } while (current != *playlist);

    printf("Musica nao encontrada na playlist.\n");
}

void displayPlaylist(struct Music* playlist) {
    if (playlist == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    struct Music* current = playlist;
    printf("Playlist pela ordem de cadastro:\n");
    do {
        printf("Artista: %s, Musica: %s\n", current->artist, current->song);
        current = current->next;
    } while (current != playlist);
}

void displaySortedPlaylist(struct Music* playlist) {
    int i, j;

    if (playlist == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    int count = 0;
    struct Music* current = playlist;
    do {
        count++;
        current = current->next;
    } while (current != playlist);

    struct Music* musicArray[count];
    current = playlist;
    for (i = 0; i < count; i++) {
        musicArray[i] = current;
        current = current->next;
    }

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(musicArray[i]->song, musicArray[j]->song) > 0) {
                struct Music* temp = musicArray[i];
                musicArray[i] = musicArray[j];
                musicArray[j] = temp;
            }
        }
    }

    printf("Playlist ordenada pelo nome das musicas:\n");
    for (i = 0; i < count; i++) {
        printf("Artista: %s, Musica: %s\n", musicArray[i]->artist, musicArray[i]->song);
    }
}

void searchMusic(struct Music* playlist, char song[]) {
    if (playlist == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }

    struct Music* current = playlist;
    int found = 0;
    do {
        if (strcmp(current->song, song) == 0) {
            found = 1;
            printf("Musica encontrada na playlist.\n");
            printf("Artista: %s, Musica: %s\n", current->artist, current->song);
        }
        current = current->next;
    } while (current != playlist);

    if (!found) {
        printf("Musica nao encontrada na playlist.\n");
    }
}

struct Music* nextMusic(struct Music* currentMusic) {
    if (currentMusic != NULL) {
        return currentMusic->next;
    }
    return NULL;
}

struct Music* previousMusic(struct Music* currentMusic) {
    if (currentMusic != NULL) {
        return currentMusic->prev;
    }
    return NULL;
}

int main() {
    struct Music* playlist = loadPlaylist();

    int choice;
    char song[100];
    char artist[100];

    struct Music* currentMusic = playlist;

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1. Exibir a playlist pela ordem de cadastro\n");
        printf("2. Exibir a playlist ordenada pelo nome das musicas\n");
        printf("3. Inserir uma nova musica\n");
        printf("4. Remover uma musica\n");
        printf("5. Buscar uma musica\n");
        printf("6. Avancar para a proxima musica\n");
        printf("7. Retornar a musica anterior\n");
        printf("0. Sair\n");
        scanf("%d", &choice);
        while ((getchar()) != '\n');

        switch (choice) {
            case 1:
                displayPlaylist(playlist);
                break;
            case 2:
                displaySortedPlaylist(playlist);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", artist);
                printf("Digite o nome da musica: ");
                scanf(" %[^\n]", song);
                insertMusic(&playlist, artist, song);
                printf("Musica inserida com sucesso.\n");
                break;
            case 4:
                printf("Digite o nome da musica a ser removida: ");
                scanf(" %[^\n]", song);
                removeMusic(&playlist, artist, song);
                break;
            case 5:
                printf("Digite o nome da musica a ser buscada: ");
                scanf(" %[^\n]", song);
                searchMusic(playlist, song);
                break;
            case 6:
                currentMusic = nextMusic(currentMusic);
                if (currentMusic != NULL) {
                    printf("Musica atual: Artista: %s, Musica: %s\n", currentMusic->artist, currentMusic->song);
                } else {
                    printf("Nao ha proxima musica.\n");
                }
                break;
            case 7:
                currentMusic = previousMusic(currentMusic);
                if (currentMusic != NULL) {
                    printf("Musica atual: Artista: %s, Musica: %s\n", currentMusic->artist, currentMusic->song);
                } else {
                    printf("Nao ha musica anterior.\n");
                }
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (choice != 0);

    savePlaylist(playlist);

    return 0;
}
