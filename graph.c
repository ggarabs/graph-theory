/* 
 * REPRESENTACAO DE GRAFOS - Versao 2024/2
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


/* 
 * Estrutura de dados para representar grafos
 */
typedef struct aresta{ /* Celula de uma lista de arestas */
	int    outroExtr;
	struct aresta *prox;
}Aresta;

typedef struct vertice{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	Aresta *prim;
}Vert;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  incluiNovaAresta(Vert G[], int ordem, int v1, int v2);
void imprimeGrafo(Vert G[], int ordem);

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vert **G, int ordem){
	int i;
	*G= (Vert*) malloc(sizeof(Vert)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vert **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int incluiNovaAresta(Vert G[], int ordem, int v1, int v2){
    Aresta *A1, *A2;
    
	if (v1 < 0 || v1 >= ordem || v2 < 0 || v2 >= ordem) return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1 = (Aresta *)malloc(sizeof(Aresta));
	A1->outroExtr = v2;
	A1->prox = G[v1].prim;
	G[v1].prim = A1;

	if (v1 == v2) return 1; /* Aresta e� um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2 = (Aresta *) malloc(sizeof(Aresta));
	A2->outroExtr = v1;
	A2->prox = G[v2].prim;
	G[v2].prim = A2;
	
	return 1;
}


/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vert G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia:\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d: ", i);
		aux= G[i].prim;
		for(; aux != NULL; aux = aux->prox) printf("  v%d", aux->outroExtr);
	}
	printf("\n\n");
}

/*
 * Programa simples simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
    int i;
	Vert *G;
	int ordemG= 8;
		
	criaGrafo(&G, ordemG);
	incluiNovaAresta(G,ordemG,0,0);
	incluiNovaAresta(G,ordemG,0,1);
	incluiNovaAresta(G,ordemG,0,1);
	incluiNovaAresta(G,ordemG,0,2);
	incluiNovaAresta(G,ordemG,0,3);
	incluiNovaAresta(G,ordemG,2,4);
	incluiNovaAresta(G,ordemG,2,5);
	incluiNovaAresta(G,ordemG,3,5);
	incluiNovaAresta(G,ordemG,4,5);
	incluiNovaAresta(G,ordemG,6,7);

	imprimeGrafo(G, ordemG);
       
	destroiGrafo(&G, ordemG);
    system("PAUSE");
	return 0;
}
