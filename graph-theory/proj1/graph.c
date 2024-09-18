/* 
 * REPRESENTACAO DE GRAFOS - Versao 2024/2
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct aresta{ /* Celula de uma lista de arestas */
	int outroExtr;
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

int eConexo(Vert G[], int ordem){
	bool* visitado = (bool*)calloc(ordem, sizeof(bool));
	int* lista = (int*)calloc(ordem, sizeof(int));

	int verticesMarcados = 1, pontLento = 0, pontRapido = 0;
	Vert atual = G[0];
	visitado[atual.nome] = 1;

	bool foiMarcado = true;
	while(foiMarcado){
		Aresta* aux = atual.prim;
		foiMarcado = false;
		for(; aux != NULL; aux = aux->prox){
			if(!visitado[aux->outroExtr]){
				visitado[aux->outroExtr] = true;
				if(aux->outroExtr != atual.nome){
					lista[pontRapido++] = aux->outroExtr;
					verticesMarcados++;
					foiMarcado = true;
				}
			}
		}
		atual = G[lista[pontLento++]];
	}

	free(visitado);
	free(lista);

	return ordem == verticesMarcados;
}

void marcaVertices(Vert G[], int ordem, int index, int* componente, int compIndex){
	int* lista = (int*)calloc(ordem, sizeof(int));

	int verticesMarcados = 1, pontLento = 0, pontRapido = 0;
	Vert atual = G[index];
	componente[atual.nome] = compIndex;

	bool foiMarcado = true;
	while(foiMarcado){
		Aresta* aux = atual.prim;
		foiMarcado = false;
		for(; aux != NULL; aux = aux->prox){
			if(!componente[aux->outroExtr]){
				componente[aux->outroExtr] = compIndex;
				if(aux->outroExtr != atual.nome){
					lista[pontRapido++] = aux->outroExtr;
					foiMarcado = true;
				}
			}
		}
		atual = G[lista[pontLento++]];
	}

	free(lista);
}

void marcaVerticesComVerticeExcluido(Vert G[], int ordem, int index, int* componente, int compIndex, Vert vertice){
	int* lista = (int*)calloc(ordem, sizeof(int));

	int verticesMarcados = 1, pontLento = 0, pontRapido = 0;
	Vert atual = G[index];
	componente[atual.nome] = compIndex;

	bool foiMarcado = true;
	while(foiMarcado || pontLento <= pontRapido){
		// printf("lento: %d rapido: %d\n", pontLento, pontRapido);
		Aresta* aux = atual.prim;
		// printf("Atual: %d\n", atual.nome);
		foiMarcado = false;
		for(; aux != NULL; aux = aux->prox){
			// printf("O for entrou no %d\n", aux->outroExtr);
			if(aux->outroExtr == vertice.nome){
				continue;
			}else if(!componente[aux->outroExtr]){
				// printf("Marquei o %d\n", aux->outroExtr);
				componente[aux->outroExtr] = compIndex;
				if(aux->outroExtr != atual.nome){
					lista[++pontRapido] = aux->outroExtr;
					foiMarcado = true;
				}
			}
		}
		atual = G[lista[++pontLento]];
	}

	free(lista);
}

void marcaVerticesComArestaExcluida(Vert G[], int ordem, int index, int* componente, int compIndex, Vert extr, Aresta* arestaExcluida){
	int* lista = (int*)calloc(ordem, sizeof(int));

	int verticesMarcados = 1, pontLento = 0, pontRapido = 0;
	Vert atual = G[index];
	componente[atual.nome] = compIndex;

	bool foiMarcado = true;
	while(foiMarcado || pontLento <= pontRapido){
		// printf("lento: %d rapido: %d\n", pontLento, pontRapido);
		Aresta* aux = atual.prim;
		// printf("Atual: %d\n", atual.nome);
		foiMarcado = false;
		for(; aux != NULL; aux = aux->prox){
			// printf("O for entrou no %d\n", aux->outroExtr);
			if(aux == arestaExcluida){
				continue;
			}else if(!componente[aux->outroExtr]){
				// printf("Marquei o %d\n", aux->outroExtr);
				componente[aux->outroExtr] = compIndex;
				if(aux->outroExtr != atual.nome){
					lista[++pontRapido] = aux->outroExtr;
					foiMarcado = true;
				}
			}
		}
		atual = G[lista[++pontLento]];
	}

	free(lista);
}

int contaComponentes(Vert G[], int ordem){
	int* componente = (int*)calloc(ordem, sizeof(int));
	int compIndex = 0;

	for(int i = 0; i < ordem; i++)
		if(!componente[G[i].nome]) 
			marcaVertices(G, ordem, i, componente, ++compIndex);

	free(componente);

	return compIndex;
}

int contaComponentesExcluindoVertice(Vert G[], int ordem, Vert verticeExcluido){
	int* componente = (int*)calloc(ordem, sizeof(int));
	int compIndex = 0;

	for(int i = 0; i < ordem; i++){
		if(G[i].nome != verticeExcluido.nome && !componente[G[i].nome]){
			// printf("Entrei no %d\n", G[i].nome);
			marcaVerticesComVerticeExcluido(G, ordem, i, componente, ++compIndex, verticeExcluido);
		}
	}

	free(componente);

	return compIndex;
}

int contaComponentesExcluindoAresta(Vert G[], int ordem, Vert extr, Aresta* outroExtr){
	int* componente = (int*)calloc(ordem, sizeof(int));
	int compIndex = 0;

	// printf("Eliminei a aresta v%d-v%d\n", extr.nome, outroExtr->outroExtr);

	for(int i = 0; i < ordem; i++){
		// printf("To aqui no %d\n", G[i].nome);
		if(!componente[G[i].nome]){
			// printf("Chamei a função no %d\n", G[i].nome);
			marcaVerticesComArestaExcluida(G, ordem, i, componente, ++compIndex, extr, outroExtr);
		}
	}

	return compIndex;
}

Vert* obtemVerticesCorte(Vert G[], int ordem, int* numeroVerticesCorte){
	Vert* verticesCorte = (Vert*)calloc(ordem, sizeof(Vert*));
	int numeroComponentes = contaComponentes(G, ordem);
	for(int i = 0; i < ordem; i++){
		// printf("Visitei o %d\n", G[i].nome);
		if(contaComponentesExcluindoVertice(G, ordem, G[i]) != numeroComponentes){
			verticesCorte[(*numeroVerticesCorte)++] = G[i];
		}
		// printf("\n");
	}

	return verticesCorte;
}

Vert* obtemArestasCorte(Vert G[], int ordem, Vert* ArestasCorte){
	int numeroComponentes = contaComponentes(G, ordem);
	for(int i = 0; i < ordem; i++){
		// printf("Entrei no vértice %d\n", G[i].nome);
		Vert atual = G[i];
		Aresta *aux = atual.prim;

		for(; aux != NULL; aux = aux->prox){
			// printf("Acessei o vizinho %d\n", aux->outroExtr);
			if(contaComponentesExcluindoAresta(G, ordem, G[i], aux) != numeroComponentes){
				incluiNovaAresta(ArestasCorte, ordem, G[i].nome, aux->outroExtr);
			}
		}
	}

	return ArestasCorte;
}

/*
 * Programa simples simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
    int i;
	Vert *G;
	int ordemG= 8;
		
	criaGrafo(&G, ordemG);
	// incluiNovaAresta(G,ordemG,0,0);
	// incluiNovaAresta(G,ordemG,0,1);
	// incluiNovaAresta(G,ordemG,0,1);
	// incluiNovaAresta(G,ordemG,0,2);
	// incluiNovaAresta(G,ordemG,0,3);
	// incluiNovaAresta(G,ordemG,2,4);
	// incluiNovaAresta(G,ordemG,2,5);
	// incluiNovaAresta(G,ordemG,3,5);
	// incluiNovaAresta(G,ordemG,4,5);
	// incluiNovaAresta(G,ordemG,6,7);

	incluiNovaAresta(G,ordemG,4,5);
	incluiNovaAresta(G,ordemG,2,4);
	incluiNovaAresta(G,ordemG,2,5);
	incluiNovaAresta(G,ordemG,1,2);
	incluiNovaAresta(G,ordemG,1,6);
	incluiNovaAresta(G,ordemG,1,3);
	incluiNovaAresta(G,ordemG,3,6);
	incluiNovaAresta(G,ordemG,6,7);
	incluiNovaAresta(G,ordemG,3,7);

	imprimeGrafo(G, ordemG);

	if(eConexo(G, ordemG)) printf("Esse grafo é conexo.\n");
	else printf("Esse grafo não é conexo.\n");

	printf("O número de componentes conexas desse grafo é: %d\n", contaComponentes(G, ordemG));

	Vert* verticesCorte = (Vert*)calloc(ordemG, sizeof(Vert));
	int numeroVerticesCorte = 0;
	verticesCorte = obtemVerticesCorte(G, ordemG, &numeroVerticesCorte);

	printf("Vértices de corte: ");
	for(int i = 0; i < numeroVerticesCorte; i++) printf("%d ", verticesCorte[i].nome);
	printf("\n");

	Vert *arestasCorte;
	criaGrafo(&arestasCorte, ordemG);
	arestasCorte = obtemArestasCorte(G, ordemG, arestasCorte);

	printf("Arestas de corte: \n");
	imprimeGrafo(arestasCorte, ordemG);
	printf("\n");

	destroiGrafo(&G, ordemG);
//    system("PAUSE");
	return 0;
}
