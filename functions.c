#include "global.h"

// Criacao de um G com ordem predefinida (passada como argumento), e, inicilamente, sem nenhuma aresta 
void criaG(Vert **G, int ordem){

	int i;
	*G= (Vert*) malloc(sizeof(Vert)*ordem); ///Alcacao dinamica de um vetor de vertices
	
	for(i=0; i<ordem; i++){

		(*G)[i].localidade = lista_localidades[i];
		(*G)[i].nome= i;
		(*G)[i].minha_residencia = (i == 20) ? true : false; // Determino que o vértice 21 é minha residÊncia
		(*G)[i].prim= NULL;    // Cada vértice não possui nenhuma aresta incidente
	}
}

// Realiza o desalocação de memória onde o G estava armazenado
void destroiG(Vert **G, int ordem){

	int i;
  Arest *a, *n;
    
	for(i = 0; i < ordem; i++){  // Remove a lista de adjacência de cada vértice

    a = (*G)[i].prim;

    while (a!= NULL){

      n= a->prox;
      free(a);
      a = n;

    }

	}

  free(*G);  //Remove o vetor de vétices

}

// Função que realiza o acréscimo de arestas entre 2 vértices do grafo instânciado
int acrescentaAresta(Vert *G, int ordem, int v1, int v2, int distancia_quadras){

  Arest * A1, *A2;
  
	if (v1<0 || v1 >= ordem) 
	  return 0;
	if (v2<0 || v2 >= ordem)
	  return 0;
	
	// Acrescenta aresta na lista do vertice v1
	A1 = (Arest *) malloc(sizeof(Arest));
	A1->extremo2 = v2;
	A1->prox = G[v1].prim;
	G[v1].prim = A1;
	A1->distancia_quadras = distancia_quadras;

	if (v1 == v2) return 1; 

	// Acrescenta aresta na lista do vertice v2 se v2 != v1
	A2 = (Arest *) malloc(sizeof(Arest));
	A2->extremo2 = v1;
	A2->prox = G[v2].prim;
	G[v2].prim = A2;
	A2->distancia_quadras = distancia_quadras;
	
	return 1;

}

//Função que verifica se um grafo é conexo 
bool conexo(Vert *G, int ordem){

		if(G == NULL) exit(-1); //Se não exitir retorna

		int numero_componentes = 0;
		int* visitado = calloc(ordem, sizeof(int)); 
		if (visitado == NULL) exit(-1);

		dfs(G, 0, visitado);

		// Verifica se todos os vértices foram visitados
		for (int i = 0; i < ordem; i++) {
			numero_componentes += (visitado[i] == 0);
			if (visitado[i] == 0) {
				free(visitado);
				return false; // O G não é conexo
			}
		}

		free(visitado);
		return true; // O G é conexo

}

// Implementação do algoritmo de busca profunda para verificar se cada vértice do G permite formar um passeio a qualquer outro vértice do G
void dfs(Vert *graph, int vertex, int* visitado){

    if (visitado[vertex]) return;
    
    visitado[vertex] = 1;
    
    Arest *temp = graph[vertex].prim;

    while (temp != NULL) {

        dfs(graph, temp->extremo2, visitado);
        temp = temp->prox;

    }
}


//Imprime um G com uma notacao similar a uma lista de adjacencia
void imprimeG(Vert *G, int ordem){

    int i;
    Arest *aux;

    printf("\nGrafo do bairro do Belenzinho \n");
    printf("\nOrdem: %d",ordem);
    printf("\nLista de Adjacencia:\n");

    for (i = 0; i < ordem; i++){  // ✅ Inicializa i = 0
        printf("\n    v%d ", i);
        aux= G[i].prim;
        for( ; aux != NULL; aux= aux->prox)
            printf("  -> v%d ", aux->extremo2);
    }
    printf("\n\n");

}

// Implementação da busca em largura para definir as distâncias entre diferentes vértices
void bfs(Vert *G, int origem, int distancias[], int ordem) {

    bool visitado[ordem];
    int fila[ordem];
    int frente = 0, tras = 0;
    
    for (int i = 0; i < ordem; i++) {

        distancias[i] = -1; // -1 indica não visitado
        visitado[i] = false;

    }
    
    // Começar BFS da origem
    fila[tras++] = origem;

    distancias[origem] = 0;
    visitado[origem] = true;
    
    while (frente < tras) {

        int u = fila[frente++];
        
        Arest *adj = G[u].prim;

        while (adj != NULL) {

            int v = adj->extremo2;

						if (!visitado[v]) {

                visitado[v] = true;
                distancias[v] = distancias[u] + 1;
                fila[tras++] = v;
            }

            adj = adj->prox;

        }
    }
}

// Busca pelo vértice da residência determinada
int vertice_minha_residencia(Vert *G, int ordem){

	int index_residencia, i = 0;

	for(i; i < ordem; i++){

		if(G[i].minha_residencia == true){

			index_residencia = G[i].nome;
			return index_residencia;

		}

	}

	return index_residencia; //Retorna o índice da residência

}
