/* 
 * REPRESENTACAO DE GRAFOS - Versao 2025/2
 */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<memory.h>
#include<stdbool.h>

#define BRANCO 0
#define CINZA  1
#define PRETO  2
#define INF INT_MAX
int ordemG; //Número total de vértices



// Estrutura para armazenar resultados das distâncias
typedef struct {
    int vertice;
    int distancia;
    char* localidade;
} ResultadoDistancia;

/* 
 * Estrutura de dados para representar grafos
 */

/*

O que deve ser adicionado nas estruturas

Aresta -> distancia entre v1 e v2

Vértice -> se é a minha casa, identificador do ponto de interesse

*/
typedef struct a{ /* Celula de uma lista de arestas */
	int    extremo2;
	struct a *prox;
	int    distancia;  
}Arest;

typedef struct v{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	int cor;
	char* localidade;
	bool minha_casa;
	Arest *prim;
}Vert;

char *lista_localidades[] = {"local01", "local02", "local03", "local04"};


/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, int distancia);
void imprimeGrafo(Vert G[], int ordem);
void dfs(Vert *graph, int vertex, int* visitado);
 
/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vert **G, int ordem){
	int i;
	*G= (Vert*) malloc(sizeof(Vert)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].localidade = lista_localidades[i];
		(*G)[i].nome= i;
		(*G)[i].cor= BRANCO;
		(*G)[i].minha_casa = (i == 0) ? true : false; // Supondo que o vértice 0 é a minha casa
		(*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vert **G, int ordem){
	int i;
    Arest *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
	}
    free(*G);  /* Remove o vetor de vertices */
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j, quando
 *   i != j, serao criadas, na estrutura de dados, arestas (i,j) e (j,i) .
 */
int acrescentaAresta(Vert G[], int ordem, int v1, int v2, int distancia){
    Arest * A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Arest *) malloc(sizeof(Arest));
	A1->extremo2= v2;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;
	A1->distancia = distancia;

	if (v1 == v2) return 1; /* Aresta e um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Arest *) malloc(sizeof(Arest));
	A2->extremo2= v1;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	A2->distancia = distancia;
	
	return 1;
}

int conexo(Vert *grafo){

		if(grafo == NULL) return -1;

		int numero_componentes = 0;
		int* visitado = calloc(ordemG, sizeof(int)); 
		if (visitado == NULL) return -1;

		dfs(grafo, 0, visitado);

		// Verifica se todos os vértices foram visitados
		for (int i = 0; i < ordemG; i++) {
			numero_componentes += (visitado[i] == 0);
			if (visitado[i] == 0) {
				free(visitado);
				return 0;
			}
		}

		//printf("Número de componentes conexas: %d\n", numero_componentes);

		free(visitado);
		return 1; // O grafo é conexo

}

void dfs(Vert *graph, int vertex, int* visitado){
  if (visitado[vertex]) return; // Já visitado

  visitado[vertex] = 1;
	
	Arest *temp = graph[vertex].prim;
  while (temp !=  NULL){
		dfs(graph, temp->extremo2, visitado);
		temp = temp->prox;
	}

}
/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vert G[], int ordem){
	int i;
	Arest *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia:\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d (%s): ", i, lista_localidades[i]);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			printf("  v%d ( %d metros )", aux->extremo2, aux->distancia);
	}
	printf("\n\n");

}

/*
 * Programa simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
  int i;
	Vert *G;
	ordemG= 4; /* Vertices identificado de 0 ate 9 */
		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G, ordemG, 0,1, 50);
	acrescentaAresta(G, ordemG, 0,2, 20);
	acrescentaAresta(G, ordemG, 1,3, 45);
	acrescentaAresta(G, ordemG, 2,3, 60);

	imprimeGrafo(G, ordemG);

	if (conexo(G) == 1){
		printf("\nO grafo é conexo\n");
	}
	else {
		printf("\nO grafo não é conexo \n");
	}
       
	destroiGrafo(&G, ordemG);
	
	printf("Pressione uma tecla para terminar\n");
    getchar();
	return(0);
}