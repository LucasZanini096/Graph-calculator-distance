/*
 * REPRESENTACAO DE GRAFOS - Projeto Bairro Belenzinho
 * Teoria dos Grafos - 2025/2
 *
 * Implementacao com algoritmo de Dijkstra para calcular
 * caminhos minimos entre localidades no bairro
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRANCO 0
#define CINZA 1
#define PRETO 2
#define INFINITO INT_MAX

/*
 * Estrutura de dados para representar grafos
 */
typedef struct a {
  int extremo2;
  struct a *prox;
  int dist_ant;  /* Se possui_localidade: distancia do vertice ate a localidade
                    Se NAO possui_localidade: distancia total da aresta */
  int dist_prox; /* Se possui_localidade: distancia da localidade ate extremo2
                    Se NAO possui_localidade: valor 0 (nao usado) */
  char localidade[100];
  bool minha_residencia;
  bool possui_localidade;
} Arest;

typedef struct v {
  int nome;
  int cor;
  Arest *prim;
} Vert;

/*
 * Declaracoes das funcoes para manipulacao de grafos
 */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int acrescentaAresta(Vert G[], int ordem, int v1, int v2, int dist_v1_loc,
                     int dist_v2_loc, char *localidade, bool minha_residencia,
                     bool possui_localidade);
void imprimeGrafo(Vert G[], int ordem);
void dijkstra(Vert G[], int ordem, int inicio, int *dist, int *ant);
void encontrarCaminho(Vert G[], int ordem, char *startLoc, char *endLoc);
void inicializarGrafo(Vert **G, int *ordem);
void listarLocalidades(Vert G[], int ordem);
void obterLocalidadePorIndice(Vert G[], int ordem, int indice, char *buffer);

/* Funcoes auxiliares para facilitar adicao de arestas */
void adicionaArestaComLocalidade(Vert G[], int ordem, int v1, int v2,
                                 int dist_v1, int dist_v2, char *nome);
void adicionaArestaSemLocalidade(Vert G[], int ordem, int v1, int v2,
                                 int distancia);
void adicionaMinhaCasa(Vert G[], int ordem, int v1, int v2, int dist_v1,
                       int dist_v2, char *nome);

/*
 * Criacao de um grafo com ordem predefinida
 */
void criaGrafo(Vert **G, int ordem) {
  int i;
  *G = (Vert *)malloc(sizeof(Vert) * ordem);

  for (i = 0; i < ordem; i++) {
    (*G)[i].nome = i;
    (*G)[i].cor = BRANCO;
    (*G)[i].prim = NULL;
  }
}

/*
 * Desaloca a memoria dinamica usada para armazenar um grafo
 */
void destroiGrafo(Vert **G, int ordem) {
  int i;
  Arest *a, *n;

  for (i = 0; i < ordem; i++) {
    a = (*G)[i].prim;
    while (a != NULL) {
      n = a->prox;
      free(a);
      a = n;
    }
  }
  free(*G);
}

/*
 * Acrescenta uma nova aresta em um grafo previamente criado
 */
int acrescentaAresta(Vert G[], int ordem, int v1, int v2, int dist_v1_loc,
                     int dist_v2_loc, char *localidade, bool minha_residencia,
                     bool possui_localidade) {
  Arest *A1, *A2;

  if (v1 < 0 || v1 >= ordem)
    return 0;
  if (v2 < 0 || v2 >= ordem)
    return 0;

  /* Acrescento aresta na lista do vertice v1 */
  A1 = (Arest *)malloc(sizeof(Arest));
  A1->extremo2 = v2;
  A1->possui_localidade = possui_localidade;

  if (possui_localidade) {
    A1->dist_ant = dist_v1_loc;
    A1->dist_prox = dist_v2_loc;
    strcpy(A1->localidade, localidade);
    A1->minha_residencia = minha_residencia;
  } else {
    A1->dist_ant = dist_v1_loc; // distancia total
    A1->dist_prox = 0;
    strcpy(A1->localidade, "");
    A1->minha_residencia = false;
  }

  A1->prox = G[v1].prim;
  G[v1].prim = A1;

  if (v1 == v2)
    return 1;

  /* Acrescento aresta na lista do vertice v2 */
  A2 = (Arest *)malloc(sizeof(Arest));
  A2->extremo2 = v1;
  A2->possui_localidade = possui_localidade;

  if (possui_localidade) {
    A2->dist_ant = dist_v2_loc;
    A2->dist_prox = dist_v1_loc;
    strcpy(A2->localidade, localidade);
    A2->minha_residencia = minha_residencia;
  } else {
    A2->dist_ant = dist_v1_loc; // mesma distancia total
    A2->dist_prox = 0;
    strcpy(A2->localidade, "");
    A2->minha_residencia = false;
  }

  A2->prox = G[v2].prim;
  G[v2].prim = A2;

  return 1;
}

/*
 * Imprime um grafo com notacao de lista de adjacencia
 */
void imprimeGrafo(Vert G[], int ordem) {
  int i;
  Arest *aux;

  printf("\n========================================");
  printf("\nGRAFO DO BAIRRO - BELENZINHO");
  printf("\n========================================");
  printf("\nOrdem: %d vertices\n", ordem);
  printf("\nLista de Adjacencia:\n");

  for (i = 0; i < ordem; i++) {
    printf("\nv%d: ", i);
    aux = G[i].prim;
    int count = 0;
    for (; aux != NULL; aux = aux->prox) {
      if (count > 0)
        printf("     ");
      if (aux->possui_localidade) {
        printf("-> v%d [%s: %dm + %dm = %dm]%s\n", aux->extremo2,
               aux->localidade, aux->dist_ant, aux->dist_prox,
               aux->dist_ant + aux->dist_prox,
               aux->minha_residencia ? " ***MINHA CASA***" : "");
      } else {
        printf("-> v%d [%dm]\n", aux->extremo2, aux->dist_ant);
      }
      count++;
    }
    if (count == 0)
      printf("(sem arestas)\n");
  }
  printf("\n========================================\n");
}

/*
 * Algoritmo de Dijkstra para encontrar caminho mais curto
 */
void dijkstra(Vert G[], int ordem, int inicio, int *dist, int *ant) {
  int i, v;
  int *visitado = (int *)malloc(ordem * sizeof(int));
  Arest *a;

  for (i = 0; i < ordem; i++) {
    dist[i] = INFINITO;
    ant[i] = -1;
    visitado[i] = 0;
  }

  dist[inicio] = 0;

  for (i = 0; i < ordem; i++) {
    int u = -1;
    int min_dist = INFINITO;

    /* Encontra o vertice nao visitado com menor distancia */
    for (v = 0; v < ordem; v++) {
      if (!visitado[v] && dist[v] < min_dist) {
        min_dist = dist[v];
        u = v;
      }
    }

    if (u == -1 || dist[u] == INFINITO)
      break;

    visitado[u] = 1;

    /* Relaxamento das arestas */
    for (a = G[u].prim; a != NULL; a = a->prox) {
      v = a->extremo2;

      int peso;
      if (a->possui_localidade) {
        peso = a->dist_ant + a->dist_prox;
      } else {
        peso = a->dist_ant;
      }

      if (dist[u] != INFINITO && dist[u] + peso < dist[v]) {
        dist[v] = dist[u] + peso;
        ant[v] = u;
      }
    }
  }
  free(visitado);
}

/*
 * Lista todas as localidades presentes no grafo
 */
void listarLocalidades(Vert G[], int ordem) {
  int i, count = 0;
  Arest *a;

  printf("\n========================================");
  printf("\nLOCALIDADES NO BAIRRO");
  printf("\n========================================\n");

  // Array para evitar duplicatas
  char localidades[100][100];
  int localidades_count = 0;

  for (i = 0; i < ordem; i++) {
    for (a = G[i].prim; a != NULL; a = a->prox) {
      if (a->possui_localidade) {
        // Verifica se ja foi listada
        int ja_listada = 0;
        for (int j = 0; j < localidades_count; j++) {
          if (strcmp(localidades[j], a->localidade) == 0) {
            ja_listada = 1;
            break;
          }
        }

        if (!ja_listada) {
          strcpy(localidades[localidades_count], a->localidade);
          localidades_count++;
          count++;
          printf("L%d - %s (entre v%d e v%d)%s\n", count, a->localidade, i,
                 a->extremo2, a->minha_residencia ? " ***MINHA CASA***" : "");
        }
      }
    }
  }

  printf("\nTotal: %d localidades\n", count);
  printf("========================================\n");
}

/*
 * Obtem o nome da localidade pelo indice (baseado na ordem de listagem)
 */
void obterLocalidadePorIndice(Vert G[], int ordem, int indice, char *buffer) {
  int i, count = 0;
  Arest *a;
  char localidades[100][100];
  int localidades_count = 0;

  strcpy(buffer, ""); // Inicializa vazio

  for (i = 0; i < ordem; i++) {
    for (a = G[i].prim; a != NULL; a = a->prox) {
      if (a->possui_localidade) {
        // Verifica se ja foi listada para manter a mesma ordem
        int ja_listada = 0;
        for (int j = 0; j < localidades_count; j++) {
          if (strcmp(localidades[j], a->localidade) == 0) {
            ja_listada = 1;
            break;
          }
        }

        if (!ja_listada) {
          strcpy(localidades[localidades_count], a->localidade);
          localidades_count++;
          count++;

          if (count == indice) {
            strcpy(buffer, a->localidade);
            return;
          }
        }
      }
    }
  }
}

/*
 * Encontra o caminho mais curto entre duas localidades
 */
void encontrarCaminho(Vert G[], int ordem, char *startLoc, char *endLoc) {
  int i;
  Arest *a;

  struct EdgeInfo {
    int u, v;
    int dist_u, dist_v;
  };

  struct EdgeInfo startEdge = {-1, -1, 0, 0};
  struct EdgeInfo endEdge = {-1, -1, 0, 0};

  printf("\n========================================");
  printf("\nCALCULANDO CAMINHO");
  printf("\n========================================");
  printf("\nOrigem: %s", startLoc);
  printf("\nDestino: %s\n", endLoc);

  /* Busca as arestas correspondentes */
  for (i = 0; i < ordem; i++) {
    for (a = G[i].prim; a != NULL; a = a->prox) {
      if (a->possui_localidade && strcmp(a->localidade, startLoc) == 0) {
        startEdge.u = i;
        startEdge.v = a->extremo2;
        startEdge.dist_u = a->dist_ant;
        startEdge.dist_v = a->dist_prox;
      }
      if (a->possui_localidade && strcmp(a->localidade, endLoc) == 0) {
        endEdge.u = i;
        endEdge.v = a->extremo2;
        endEdge.dist_u = a->dist_ant;
        endEdge.dist_v = a->dist_prox;
      }
    }
  }

  if (startEdge.u == -1) {
    printf("\n[ERRO] Localidade de inicio '%s' nao encontrada.\n", startLoc);
    return;
  }
  if (endEdge.u == -1) {
    printf("\n[ERRO] Localidade de destino '%s' nao encontrada.\n", endLoc);
    return;
  }

  /* Executa Dijkstra a partir dos extremos da aresta de inicio */
  int *dist_u1 = (int *)malloc(ordem * sizeof(int));
  int *ant_u1 = (int *)malloc(ordem * sizeof(int));
  dijkstra(G, ordem, startEdge.u, dist_u1, ant_u1);

  int *dist_v1 = (int *)malloc(ordem * sizeof(int));
  int *ant_v1 = (int *)malloc(ordem * sizeof(int));
  dijkstra(G, ordem, startEdge.v, dist_v1, ant_v1);

  /* Calcula as 4 possibilidades com protecao contra overflow */
  long d1 = INFINITO, d2 = INFINITO, d3 = INFINITO, d4 = INFINITO;

  if (dist_u1[endEdge.u] != INFINITO)
    d1 = (long)startEdge.dist_u + dist_u1[endEdge.u] + endEdge.dist_u;
  if (dist_u1[endEdge.v] != INFINITO)
    d2 = (long)startEdge.dist_u + dist_u1[endEdge.v] + endEdge.dist_v;
  if (dist_v1[endEdge.u] != INFINITO)
    d3 = (long)startEdge.dist_v + dist_v1[endEdge.u] + endEdge.dist_u;
  if (dist_v1[endEdge.v] != INFINITO)
    d4 = (long)startEdge.dist_v + dist_v1[endEdge.v] + endEdge.dist_v;

  long min_dist = d1;
  int best_path = 1;

  if (d2 < min_dist) {
    min_dist = d2;
    best_path = 2;
  }
  if (d3 < min_dist) {
    min_dist = d3;
    best_path = 3;
  }
  if (d4 < min_dist) {
    min_dist = d4;
    best_path = 4;
  }

  if (min_dist >= INFINITO) {
    printf("\n[ERRO] Nao ha caminho entre '%s' e '%s'.\n", startLoc, endLoc);
  } else {
    printf("\n>>> Distancia minima: %ld metros\n", min_dist);
    printf("\nCaminho detalhado:\n");
    printf("  %s -> ", startLoc);

    int startNode = (best_path <= 2) ? startEdge.u : startEdge.v;
    int endNode = (best_path % 2 != 0) ? endEdge.u : endEdge.v;
    int *parentArray = (best_path <= 2) ? ant_u1 : ant_v1;

    /* Reconstroi o caminho */
    int path[100];
    int count = 0;
    int curr = endNode;

    while (curr != startNode && curr != -1) {
      path[count++] = curr;
      curr = parentArray[curr];
    }
    path[count++] = startNode;

    for (int k = count - 1; k >= 0; k--) {
      printf("v%d", path[k]);
      if (k > 0)
        printf(" -> ");
    }

    printf(" -> %s\n", endLoc);
  }

  printf("========================================\n");

  free(dist_u1);
  free(ant_u1);
  free(dist_v1);
  free(ant_v1);
}

/*
 * Inicializa o grafo com os dados do Bairro Belenzinho
 */
void inicializarGrafo(Vert **G, int *ordem) {
  *ordem = 37; // vertices de 0 a 36 (ajustando indices)
  criaGrafo(G, *ordem);

  /* ============================================
   * ARESTAS COM LOCALIDADES
   * ============================================ */

  // L1 - UBS Belenzinho (Entre v2 e v3) - 210m total
  acrescentaAresta(*G, *ordem, 2, 3, 105, 105, "UBS Belenzinho", false, true);

  // L2 - Projeto Viver (Entre v3 e v4) - 150m total
  acrescentaAresta(*G, *ordem, 3, 4, 75, 75, "Projeto Viver", false, true);

  // L3 - Parque do Belem (Entre v4 e v5) - 280m total
  acrescentaAresta(*G, *ordem, 4, 5, 140, 140, "Parque do Belem", false, true);

  // L4 - Hospital Santa Virginia (Entre v5 e v10) - 190m total
  acrescentaAresta(*G, *ordem, 5, 10, 95, 95, "Hospital Santa Virginia", false,
                   true);

  // L5 - Igreja Evangelica (Entre v9 e v10) - Estimado 280m
  acrescentaAresta(*G, *ordem, 9, 10, 140, 140, "Igreja Evangelica", false,
                   true);

  // L6 - Hidroja (Entre v8 e v9) - 150m total
  acrescentaAresta(*G, *ordem, 8, 9, 75, 75, "Hidroja", false, true);

  // L7 - Condominio Villaggio di Roma (Entre v7 e v8) - 250m total
  acrescentaAresta(*G, *ordem, 7, 8, 125, 125, "Condominio Villaggio di Roma",
                   false, true);

  // L8 - Distribuidora de Agua (Entre v1 e v6) - 300m total
  acrescentaAresta(*G, *ordem, 1, 6, 150, 150, "Distribuidora de Agua", false,
                   true);

  // L9 - Condominio Lumina Parque Clube (Entre v16 e v17) - 170m total
  acrescentaAresta(*G, *ordem, 16, 17, 85, 85, "Condominio Lumina Parque Clube",
                   false, true);

  // L10 - Patinhas de Luxo PetShop (Entre v9 e v15) - 120m total
  acrescentaAresta(*G, *ordem, 9, 15, 60, 60, "Patinhas de Luxo PetShop", false,
                   true);

  // L11 - Condominio Collori (Entre v15 e v21) - 50m total - Residência
  acrescentaAresta(*G, *ordem, 15, 21, 25, 25, "Minha Casa", true, true);

  // L12 - Padaria Bonjour Dona Helena (Entre v21 e v23) - 50m total
  acrescentaAresta(*G, *ordem, 21, 23, 25, 25, "Padaria Bonjour Dona Helena",
                   false, true);

  // L13 - Super mercado Dia (Entre v23 e v24) - 140m total
  acrescentaAresta(*G, *ordem, 23, 24, 70, 70, "Super mercado Dia", false,
                   true);

  // L14 - Fuga Pizzaria (Entre v17 e v25) - 90m total
  acrescentaAresta(*G, *ordem, 17, 25, 45, 45, "Fuga Pizzaria", false, true);

  // L15 - Colegio Ranieri (Entre v20 e v23) - 150m total
  acrescentaAresta(*G, *ordem, 20, 23, 75, 75, "Colegio Ranieri", false, true);

  // L16 - Pizzaria Dei Cugini (Entre v23 e v29) - 110m total
  acrescentaAresta(*G, *ordem, 23, 29, 65, 65, "Pizzaria Dei Cugini", false,
                   true);

  // L17 - Padaria Belga (Entre v13 e v14) - 90m total
  acrescentaAresta(*G, *ordem, 13, 14, 45, 45, "Padaria Belga", false, true);

  // L18 - Otica Max (Entre v12 e v18) - 90m total
  acrescentaAresta(*G, *ordem, 12, 18, 45, 45, "Otica Max", false, true);

  // L19 - Metro Belem (Entre v35 e v36) - 140m total
  acrescentaAresta(*G, *ordem, 35, 36, 80, 80, "Metro Belem", false, true);

  // L20 - Sodie Doces (Entre v31 e v37) - 160m total
  acrescentaAresta(*G, *ordem, 31, 37, 80, 80, "Sodie Doces", false, true);

  /* ============================================
   * ARESTAS SEM LOCALIDADES (apenas distancias)
   * ============================================ */

  acrescentaAresta(*G, *ordem, 1, 2, 135, 135, "", false, false);
  acrescentaAresta(*G, *ordem, 2, 7, 105, 105, "", false, false);
  acrescentaAresta(*G, *ordem, 3, 8, 80, 80, "", false, false);
  acrescentaAresta(*G, *ordem, 4, 9, 80, 80, "", false, false);
  acrescentaAresta(*G, *ordem, 6, 7, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 6, 11, 50, 50, "", false, false);
  acrescentaAresta(*G, *ordem, 7, 11, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 7, 8, 80, 80, "", false, false);
  acrescentaAresta(*G, *ordem, 8, 14, 80, 80, "", false, false);
  acrescentaAresta(*G, *ordem, 10, 17, 50, 50, "", false, false);
  acrescentaAresta(*G, *ordem, 11, 12, 40, 40, "", false, false);
  acrescentaAresta(*G, *ordem, 12, 13, 30, 30, "", false, false);
  acrescentaAresta(*G, *ordem, 13, 19, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 14, 20, 60, 60, "", false, false);
  acrescentaAresta(*G, *ordem, 15, 16, 70, 70, "", false, false);
  acrescentaAresta(*G, *ordem, 16, 22, 25, 25, "", false, false);
  acrescentaAresta(*G, *ordem, 18, 19, 30, 30, "", false, false);
  acrescentaAresta(*G, *ordem, 18, 26, 50, 50, "", false, false);
  acrescentaAresta(*G, *ordem, 19, 20, 40, 40, "", false, false);
  acrescentaAresta(*G, *ordem, 19, 27, 50, 50, "", false, false);
  acrescentaAresta(*G, *ordem, 20, 28, 55, 55, "", false, false);
  acrescentaAresta(*G, *ordem, 21, 22, 70, 70, "", false, false);
  acrescentaAresta(*G, *ordem, 22, 24, 25, 25, "", false, false);
  acrescentaAresta(*G, *ordem, 24, 25, 85, 85, "", false, false);
  acrescentaAresta(*G, *ordem, 24, 30, 55, 55, "", false, false);
  acrescentaAresta(*G, *ordem, 25, 31, 70, 70, "", false, false);
  acrescentaAresta(*G, *ordem, 26, 27, 40, 40, "", false, false);
  acrescentaAresta(*G, *ordem, 26, 32, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 27, 28, 40, 40, "", false, false);
  acrescentaAresta(*G, *ordem, 27, 33, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 28, 29, 75, 75, "", false, false);
  acrescentaAresta(*G, *ordem, 29, 30, 75, 75, "", false, false);
  acrescentaAresta(*G, *ordem, 29, 35, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 30, 31, 45, 45, "", false, false);
  acrescentaAresta(*G, *ordem, 32, 33, 30, 30, "", false, false);
  acrescentaAresta(*G, *ordem, 33, 34, 70, 70, "", false, false);
  acrescentaAresta(*G, *ordem, 34, 35, 70, 70, "", false, false);
  acrescentaAresta(*G, *ordem, 36, 37, 80, 80, "", false, false);
}

/*
 * Programa principal
 */
int main(int argc, char *argv[]) {
  Vert *G;
  int ordemG;
  int opcao;
  char destino[100];
  int indiceDestino;

  inicializarGrafo(&G, &ordemG);

  printf("\n");
  printf("================================================\n");
  printf("  PROJETO GRAFOS - BAIRRO BELENZINHO\n");
  printf("  Teoria dos Grafos - 2025/2\n");
  printf("================================================\n");

  do {
    printf("\nMENU PRINCIPAL:\n");
    printf("1. Ver localidades\n");
    printf("2. Calcular rota\n");
    printf("3. Encerrar programa\n");
    printf("Escolha uma opcao: ");

    if (scanf("%d", &opcao) != 1) {
      // Limpa o buffer se nao for numero
      while (getchar() != '\n')
        ;
      opcao = 0;
    }

    switch (opcao) {
    case 1:
      listarLocalidades(G, ordemG);
      break;
    case 2:
      listarLocalidades(G, ordemG);
      printf("\nDigite o numero da localidade de destino (para sair da minha "
             "casa): ");
      if (scanf("%d", &indiceDestino) == 1) {
        obterLocalidadePorIndice(G, ordemG, indiceDestino, destino);
        if (strlen(destino) > 0) {
          encontrarCaminho(G, ordemG, "Minha Casa", destino);
        } else {
          printf("\n[ERRO] Localidade invalida.\n");
        }
      } else {
        while (getchar() != '\n')
          ; // Limpa buffer
        printf("\n[ERRO] Entrada invalida.\n");
      }
      break;
    case 3:
      printf("\nEncerrando programa...\n");
      break;
    default:
      printf("\nOpcao invalida! Tente novamente.\n");
    }

    if (opcao != 3) {
      printf("\nPressione ENTER para voltar ao menu...");
      while (getchar() != '\n')
        ;        // Garante que o buffer esteja limpo
      getchar(); // Espera o ENTER
    }

  } while (opcao != 3);

  destroiGrafo(&G, ordemG);
  return 0;
}
