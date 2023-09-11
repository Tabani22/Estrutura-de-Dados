#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXSIZE 10

typedef struct Aluno {
    char nome[20];
    int matricula;
    float nota;
} Aluno;

typedef struct ListaAlunos {
    Aluno alunos[MAXSIZE];
    int numElem;
} ListaAlunos;

void preencheAluno(Aluno* ptrAluno, int matricula, float nota, char* nome){
    (*ptrAluno).matricula = matricula;
    (*ptrAluno).nota = nota;
    strcpy((*ptrAluno).nome, nome);
}

void inicializaLista(ListaAlunos* listaAlunos){
    listaAlunos->numElem = 0;
}

void exibeLista(ListaAlunos* listaAlunos){
    int index;
    printf("Exibindo lista...\n");
    for(index = 0; index < listaAlunos->numElem; index++){
        printf("Aluno %d\n", index);
        printf("Nome: %s\n", listaAlunos->alunos[index].nome);
        printf("Matricula: %d\n", listaAlunos->alunos[index].matricula);
        printf("Nota: %.1f\n\n", listaAlunos->alunos[index].nota);
    }
}

bool insereAlunoNaLista(ListaAlunos* listaAlunos, Aluno aluno){
    if(listaAlunos->numElem >= MAXSIZE){
        printf("Lista cheia. Não é possível inserir mais alunos.\n");
        return false;
    }

    int posicao = 0;
    while (posicao < listaAlunos->numElem && strcmp(aluno.nome, listaAlunos->alunos[posicao].nome) > 0) {
        posicao++;
    }
    
    int i;

    for (i = listaAlunos->numElem; i > posicao; i--) {
        listaAlunos->alunos[i] = listaAlunos->alunos[i - 1];
    }

    listaAlunos->alunos[posicao] = aluno;
    listaAlunos->numElem++;
    return true;
}

Aluno* buscaSequencialPorNome(ListaAlunos* listaAlunos, char* nome){
    Aluno sentinela;
    strcpy(sentinela.nome, nome);
    listaAlunos->alunos[listaAlunos->numElem] = sentinela;

    int index = 0;
    while (strcmp(listaAlunos->alunos[index].nome, nome) != 0) {
        index++;
    }

    if (index < listaAlunos->numElem) {
        return &listaAlunos->alunos[index];
    } else {
        return NULL;
    }
}

Aluno* buscaBinariaPorNome(ListaAlunos* listaAlunos, char* nome) {
    int esquerda = 0;
    int direita = listaAlunos->numElem - 1;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        int comparacao = strcmp(listaAlunos->alunos[meio].nome, nome);

        if (comparacao == 0) {
            return &listaAlunos->alunos[meio];
        } else if (comparacao < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    return NULL;
}

int main(){
    ListaAlunos listaAlunos;
    inicializaLista(&listaAlunos);

    Aluno aluno0, aluno1, aluno2;
    preencheAluno(&aluno0, 15424242, 9.5, "Joao");
    preencheAluno(&aluno1, 10101011, 10, "Ana");
    preencheAluno(&aluno2, 22222222, 7, "Bia");

    insereAlunoNaLista(&listaAlunos, aluno0);
    insereAlunoNaLista(&listaAlunos, aluno1);
    insereAlunoNaLista(&listaAlunos, aluno2);

    exibeLista(&listaAlunos);

    Aluno* alunoBuscaNome = buscaSequencialPorNome(&listaAlunos, "Ana");
    if (alunoBuscaNome != NULL) {
        printf("Aluno encontrado por nome (busca sequencial): %s\n", alunoBuscaNome->nome);
    } else {
        printf("Aluno nao encontrado por nome (busca sequencial).\n");
    }

    Aluno* alunoBuscaNomeBinaria = buscaBinariaPorNome(&listaAlunos, "Ana");
    if (alunoBuscaNomeBinaria != NULL) {
        printf("Aluno encontrado por nome (busca binaria): %s\n", alunoBuscaNomeBinaria->nome);
    } else {
        printf("Aluno nao encontrado por nome (busca binária).\n");
    }

    return 0;
}

