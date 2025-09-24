/*
 *  PROJETO TEORIA DOS Grafos
 *
 *  Gabriel Alves de Freitas Spinola Sucupira - 10418133
 *  Gustavo Galhardo Rodrigues - 10403091
 *  Lucas Zanini da Silva - 10417361
 *
 */

#include "global.h"
#include "functions.c"

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