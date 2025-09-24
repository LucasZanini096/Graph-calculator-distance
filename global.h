#ifndef GLOBAL_H
#define GLOBAL_H

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<memory.h>
#include<stdbool.h>

#define INF INT_MAX


// Estrutura de cada aresta do grafo
typedef struct a{ //Celula de uma lista de arestas 
	int    extremo2;
	struct a *prox;
	int    distancia_quadras;  
}Arest;

//Estrutura de cada vértice do G
typedef struct v{  //Cada vertice tem um ponteiro para uma lista de arestas incidentes nele 
	int nome;
	char* localidade;
	bool minha_residencia;
	Arest *prim;
}Vert;

// Estrutura para armazenar as informações das arestas
typedef struct ai{
		int v1, v2, distancia_quadra;
} ArestaInfo;

// Lista das arestas do grafo modelado
 // Formato: {v1, v2, peso}
ArestaInfo arestas[] = {
        {0, 1, 1},   // v1-v2
        {0, 5, 1},   // v1-v6
        {1, 2, 1},   // v2-v3
        {1, 6, 1},   // v2-v7
        {2, 3, 1},   // v3-v4
        {2, 7, 1},   // v3-v8
        {3, 4, 1},   // v4-v5
        {3, 8, 1},   // v4-v9
        {4, 9, 1},   // v5-v10
        {5, 6, 1},   // v6-v7
				{5, 10, 1},  // v6-v11
        {6, 10, 1},  // v7-v11
        {6, 12, 1},  // v7-v13
        {7, 14, 1},  // v8-v15
        {8, 9, 1},   // v9-v10
        {8, 14, 1},  // v9-v15
        {8, 15, 1},  // v9-v16
        {9, 16, 1},  // v10-v17
        {10, 11, 1}, // v11-v12
        {11, 17, 1}, // v12-v18
        {12, 13, 1}, // v13-v14
        {12, 18, 1}, // v13-v19
        {13, 19, 1}, // v14-v20
        {14, 15, 1}, // v15-v16
        {14, 20, 1}, // v15-v21
        {14, 22, 1}, // v15-v23
        {15, 16, 1}, // v16-v17
        {15, 21, 1}, // v16-v22
        {16, 24, 1}, // v17-v25
        {17, 18, 1}, // v18-v19
        {17, 25, 1}, // v18-v26
        {18, 26, 1}, // v19-v27
        {19, 27, 1}, // v20-v28
        {20, 21, 1}, // v21-v22
   			{20, 22, 1}, // v21-v23
        {21, 23, 1}, // v22-v24
        {22, 23, 1}, // v23-v24
        {22, 28, 1}, // v23-v29
        {23, 24, 1}, // v24-v25
        {23, 29, 1}, // v24-v30
        {24, 30, 1}, // v25-v31
        {25, 26, 1}, // v26-v27
        {25, 31, 1}, // v26-v32
        {26, 32, 1}, // v27-v33
        {27, 33, 1}, // v28-v34
        {28, 29, 1}, // v29-v30
        {28, 34, 1}, // v29-v35
        {29, 30, 1}, // v30-v31
        {29, 35, 1}, // v30-v36
        {30, 36, 1}, // v31-v37
        {31, 32, 1}, // v32-v33
        {32, 33, 1}, // v33-v34
        {33, 34, 1}, // v34-v35
        {34, 35, 1}, // v35-v36
        {35, 36, 1}  // v36-v37
};

// Definição da lista de localidade do grafo modelado
char *lista_localidades[] = {
    "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10",
    "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19", "v20",
    "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30",
    "v31", "v32", "v33", "v34", "v35", "v36", "v37"
};


// Declaração das funções que compõem o programa
void criaG(Vert **G, int ordem); 
void destroiG(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, int distancia_quadras);
void imprimeG(Vert G[], int ordem);
bool conexo(Vert *G, int ordem);
void dfs(Vert *graph, int vertex, int* visitado);
void bfs(Vert G[], int origem, int distancias[], int ordem);



#endif