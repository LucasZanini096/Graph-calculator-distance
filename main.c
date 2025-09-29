/*
 *  PROJETO TEORIA DOS Grafos - 6N 2025.2
 *
 *  Gabriel Alves de Freitas Spinola Sucupira - 10418133
 *  Lucas Zanini da Silva - 10417361
 *  Gustavo - 10403091
 *
 */


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
        {6, 7, 1},   // v7-v8
        {7, 8, 1},  // v8-v9
        {7, 13, 1},  // v8-v14
        {8, 9, 1},   // v9-v10
        {8, 14, 1},  // v9-v15
        {9, 16, 1},  // v10-v17
        {10, 11, 1}, // v11-v12
        {11, 17, 1}, // v12-v18
        {11, 12, 1}, // v12-v13
        {12, 13, 1}, // v13-v14
        {12, 18, 1}, // v13-v19
        {13, 19, 1}, // v14-v20
        {14, 15, 1}, // v15-v16
        {14, 20, 1}, // v15-v21
        {15, 16, 1}, // v16-v17
        {15, 21, 1}, // v16-v22
        {16, 24, 1}, // v17-v25
        {17, 18, 1}, // v18-v19
        {17, 25, 1}, // v18-v26
        {18, 19, 1}, // v19-v20
        {18, 26, 1}, // v19-v27
        {19, 22, 1}, // v20-v23
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
        {26, 27, 1}, // v27-v28
        {26, 32, 1}, // v27-v33
        {27, 28, 1}, // v28-v29
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
// Definição da lista de localidade do grafo modelado
char *lista_localidades[] = {
    "Posto Ipiranga",                           // v1
    "UBS Belenzinho",                          // v2
    "Colégio Insa",                            // v3
    "Condomínio Projeto Viver",                // v4
    "Parque do Belém",                         // v5
    "Bazar Belém",                             // v6
    "Destaque da Beleza",                      // v7
    "Nat Fruit Ice - Sorvete Natural",        // v8
    "Pizzaria Monte Castelo",                  // v9
    "EMEI Guilherme Rudge",                    // v10
    "Coratto.ink Tattoo",                      // v11
    "Ótica Max",                               // v12
    "Padaria Belga",                           // v13
    "Peru's Sandubas",                         // v14
    "Empório Belém - Fornecedor De Água",     // v15
    "Condomínio Lúmina Parque Clube",         // v16
    "Condomínio Edifício Panorama",           // v17
    "EE Amadeu Amaral",                        // v18
    "Ultrafarma Belem",                        // v19
    "Colégio Ranieri",                         // v20
    "Condomínio Collori (residência)",        // v21 - MINHA CASA
    "Olemkão Centro Veterinário",             // v22
    "Farmácia Pague Menos",                    // v23
    "Home Office Gastrobar",                   // v24
    "Bandeira Barbearia",                      // v25
    "Spot Lâmpadas",                           // v26
    "Nova Flores Sucos e Lanches",            // v27
    "Kopenhagen Belenzinho SP",               // v28
    "OXXO - Belerval",                        // v29
    "Condomínio Edifício Itaí Itaici",        // v30
    "Sodiê Doces",                            // v31
    "Ecoponto Belém",                         // v32
    "Tapeçaria Silva Jardim",                 // v33
    "Max Belém",                              // v34
    "Terminal de Ônibus",                      // v35
    "Metrô Belém",                            // v36
    "Lanchonete Central do Belém"             // v37
};


// Declaração das funções que compõem o programa
void criaG(Vert **G, int ordem); 
void destroiG(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2, int distancia_quadras);
void imprimeG(Vert G[], int ordem);
bool conexo(Vert *G, int ordem);
void dfs(Vert *graph, int vertex, int* visitado);
void bfs(Vert G[], int origem, int distancias[], int ordem);
int vertice_minha_residencia(Vert *G, int ordem);

int main(int argc, char *argv[]) {

	int ordemG; //Número total de vértices do grafo
  int i;
	Vert *G; //Defição do grafo
	int num_arestas; //Definiçao do número de arestas do grafo
	int residencia; //índice da residencia
	bool G_conexo; //Variável que verifica se o grafo é conexo ou não
	

	ordemG= sizeof(lista_localidades) / sizeof(lista_localidades[0]); //Atribuindo o tamanho da lista de localidades, já qua cada localidade pé um vértice
	num_arestas = sizeof(arestas) / sizeof(arestas[0]); //Atribuindo o tamanho da lista de arestas do grafo
		
	criaG(&G, ordemG); //Criação do grafo

	for(i = 0; i < num_arestas; i++){ //Adição das arestas ao G

		acrescentaAresta(G, ordemG, arestas[i].v1, arestas[i].v2, arestas[i].distancia_quadra);

	}

	imprimeG(G, ordemG); //Imprime o grafo ( lista de adjacência )

	G_conexo = conexo(G, ordemG); //Verifica se é conexo o grafo

	if (G_conexo == true){ //Verfica se o G é conexo

		int distancias[ordemG]; //Array de distâncias do grafo
		residencia = vertice_minha_residencia(G, ordemG); //Obtém o vertice da residência
		
		bfs(G, residencia, distancias, ordemG); //Realiza a busca em largura para obter as distâncias entre diferentes vértices
		
		printf("\nDistâncias a partir da minha residencia:\n");

		for (i = 0; i < ordemG; i++) { //Realiza o print das das distancias entre diferentes vértices

				if (distancias[i] != -1) {

						printf("v%d (%s): %d quadras\n", i, G[i].localidade, distancias[i]); //Print da distância entre um vértice e minha residência

				}

		}
	}

	else {

		printf("\nNão é possível determinar as distâncias pois o G não é conexo\n"); //Saída informativa ao usuário que o grafo não é conexo
	}

	destroiG(&G, ordemG); //Retira da memória o grafo alocado anteriomente
	
	return(0); //Fim do programa
}

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

    for (i = 0; i < ordem; i++){ 
        printf("\n    v%d ", i+1);
        aux= G[i].prim;
        for( ; aux != NULL; aux= aux->prox)
            printf("  -> v%d ", (aux->extremo2+1));
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
