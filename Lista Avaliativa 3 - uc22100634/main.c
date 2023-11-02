#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_LENGTH 100

typedef struct {
    char name[MAX_LENGTH];
    char author[MAX_LENGTH];
} Book;

typedef struct {
    Book items[MAX_SIZE];
    int front, rear;
} Queue;

typedef struct {
    Book items[MAX_SIZE];
    int top;
} Stack;

void saveQueueToFile(Queue *q, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    if (!isQueueEmpty(q)) {
        int i = q->front;
        while (i != q->rear) {
            fprintf(file, "%s;%s\n", q->items[i].name, q->items[i].author);
            i = (i + 1) % MAX_SIZE;
        }
        fprintf(file, "%s;%s\n", q->items[i].name, q->items[i].author);
    }

    fclose(file);
}

void saveStackToFile(Stack *s, const char *filename) {
    int i;
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    if (!isStackEmpty(s)) {
        for (i = s->top; i >= 0; i--) {
            fprintf(file, "%s;%s\n", s->items[i].name, s->items[i].author);
        }
    }

    fclose(file);
}

void initializeQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

void initializeStack(Stack *s) {
    s->top = -1;
}

int isQueueEmpty(Queue *q) {
    return (q->front == -1 && q->rear == -1);
}

int isStackEmpty(Stack *s) {
    return (s->top == -1);
}

int isQueueFull(Queue *q) {
    return (q->rear + 1) % MAX_SIZE == q->front;
}

int isStackFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

void enqueue(Queue *q, Book newBook) {
    if (isQueueFull(q)) {
        printf("A fila de livros está cheia.\n");
        return;
    } else if (isQueueEmpty(q)) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_SIZE;
    }
    q->items[q->rear] = newBook;
}

void push(Stack *s, Book newBook) {
    if (isStackFull(s)) {
        printf("A pilha de livros está cheia.\n");
        return;
    }
    s->top++;
    s->items[s->top] = newBook;
}

Book dequeue(Queue *q) {
    Book dequeuedBook;
    if (isQueueEmpty(q)) {
        printf("A fila de livros está vazia.\n");
        strcpy(dequeuedBook.name, "");
        strcpy(dequeuedBook.author, "");
    } else if (q->front == q->rear) {
        dequeuedBook = q->items[q->front];
        initializeQueue(q);
    } else {
        dequeuedBook = q->items[q->front];
        q->front = (q->front + 1) % MAX_SIZE;
    }
    return dequeuedBook;
}

Book pop(Stack *s) {
    Book poppedBook;
    if (isStackEmpty(s)) {
        printf("A pilha de livros está vazia.\n");
        strcpy(poppedBook.name, "");
        strcpy(poppedBook.author, "");
    } else {
        poppedBook = s->items[s->top];
        s->top--;
    }
    return poppedBook;
}

void displayQueue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("A fila de livros está vazia.\n");
        return;
    }
    int i = q->front;
    printf("Fila de livros:\n");
    while (i != q->rear) {
        printf("%s; %s\n", q->items[i].name, q->items[i].author);
        i = (i + 1) % MAX_SIZE;
    }
    printf("%s; %s\n", q->items[i].name, q->items[i].author);
}

void displayStack(Stack *s) {
    int i;
    if (isStackEmpty(s)) {
        printf("A pilha de livros está vazia.\n");
        return;
    }
    printf("Pilha de livros:\n");
    for (i = s->top; i >= 0; i--) {
        printf("%s; %s\n", s->items[i].name, s->items[i].author);
    }
}

int main() {
    FILE *file;
    Queue queue;
    Stack stack;
    initializeQueue(&queue);
    initializeStack(&stack);

    file = fopen("fila_de_livros.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo fila_de_livros.txt\n");
        return 1;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        Book newBook;
        sscanf(line, "%[^;];%[^\n]", newBook.name, newBook.author);
        enqueue(&queue, newBook);
    }
    fclose(file);

    file = fopen("pilha_de_livros.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo pilha_de_livros.txt\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        Book newBook;
        sscanf(line, "%[^;];%[^\n]", newBook.name, newBook.author);
        push(&stack, newBook);
    }
    fclose(file);

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1- Exibir fila de livros\n");
        printf("2- Inserir novo livro na fila\n");
        printf("3- Inserir novo livro na pilha\n");
        printf("4- Remover livro da fila\n");
        printf("5- Remover livro da pilha\n");
        printf("6- Exibir pilha de livros\n");
        printf("0- Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayQueue(&queue);
                break;
            case 2: {
                Book newBook;
                printf("Digite o nome do novo livro: ");
                scanf(" %[^\n]", newBook.name);
                printf("Digite o autor do novo livro: ");
                scanf(" %[^\n]", newBook.author);
                enqueue(&queue, newBook);
                saveQueueToFile(&queue, "fila_de_livros.txt");
                break;
            }
            case 3: {
                Book newBook;
                printf("Digite o nome do novo livro: ");
                scanf(" %[^\n]", newBook.name);
                printf("Digite o autor do novo livro: ");
                scanf(" %[^\n]", newBook.author);
                push(&stack, newBook);
                saveStackToFile(&stack, "pilha_de_livros.txt");
                break;
            }
            case 4: {
                Book removedBook = dequeue(&queue);
                printf("Livro removido da fila: %s, %s\n", removedBook.name, removedBook.author);
                saveQueueToFile(&queue, "fila_de_livros.txt");
                break;
            }
            case 5: {
                Book removedBook = pop(&stack);
                printf("Livro removido da pilha: %s, %s\n", removedBook.name, removedBook.author);
                saveStackToFile(&stack, "pilha_de_livros.txt");
                break;
            }
            case 6:
                displayStack(&stack);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Escolha novamente.\n");
        }
    } while (choice != 0);

    return 0;
}
