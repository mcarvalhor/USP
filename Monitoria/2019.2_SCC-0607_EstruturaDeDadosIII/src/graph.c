#include <stdlib.h>
#include <limits.h>
#include <string.h>



typedef struct __graph_node_t {
	int dest;
	int weight;
	char *additionalInfo;
	struct __graph_node_t *next;
} GRAPH_NODE_T;

typedef struct __graph_t {
	GRAPH_NODE_T **edges;
	int nVertex;
	int nEdge;
} GRAPH_T;


GRAPH_T *G_new(int nVertex) {
	GRAPH_T *aux;
	int i;
	if(nVertex < 1) {
		return NULL;
	}

	aux = (GRAPH_T *) malloc(sizeof(GRAPH_T));
	if(aux == NULL) {
		return NULL;
	}

	aux->edges = (GRAPH_NODE_T **) calloc(1, nVertex * sizeof(GRAPH_NODE_T *));
	/*aux->edges = (GRAPH_NODE_T **) malloc(nVertex * sizeof(GRAPH_NODE_T *));
	for(i = 0; i < nVertex; i++) {
		aux->edges[i] = NULL;
	}*/
	if(aux->edges == NULL) {
		free(aux);
		return NULL;
	}

	aux->nVertex = nVertex;
	aux->nEdge = 0;
	return aux;
}

int G_addEdgeInfo(int source, int destination, int weight, char *additionalInformation, GRAPH_T *g) {
	GRAPH_NODE_T **link, *aux;
	size_t strN;
	if(g == NULL || source < 0 || source >= g->nVertex || destination < 0 || destination >= g->nVertex) {
		return 0;
	}
	link = &g->edges[source];
	aux = NULL;
	while(*link != NULL && (*link)->dest <= destination) {
		link = &(*link)->next;
	}
	if(*link != NULL) {
		aux = *link;
	}
	*link = (GRAPH_NODE_T *) malloc(sizeof(GRAPH_NODE_T));
	if(*link == NULL) {
		return 0;
	}
	if(additionalInformation != NULL) {
		strN = strlen(additionalInformation);
		(*link)->additionalInfo = (char *) malloc((strN + 1) * sizeof(char));
		if((*link)->additionalInfo == NULL) {
			free(*link);
			return 0;
		}
		strcpy((*link)->additionalInfo, additionalInformation);
	} else {
		(*link)->additionalInfo = NULL;
	}
	(*link)->dest = destination;
	(*link)->weight = weight;
	(*link)->next = aux;
	g->nEdge++;
	return 1;
}

int G_addEdge(int source, int destination, int weight, GRAPH_T *g) {
	return G_addEdgeInfo(source, destination, weight, NULL, g);
}

int G_resetEdges(GRAPH_T *g) {
	GRAPH_NODE_T *link, *aux;
	int i;
	if(g == NULL) {
		return 0;
	}
	for(i = 0; i < g->nVertex; i++) {
		link = g->edges[i];
		while(link != NULL) {
			aux = link;
			link = link->next;
			free(aux);
		}
		g->edges[i] = NULL;
	}
	g->nEdge = 0;
	return 1;
}

char *G_getInfo(int source, int destination, GRAPH_T *g) {
	GRAPH_NODE_T *link;
	if(g ==  NULL || g->nEdge < 1 || source < 0 || source >= g->nVertex || destination < 0 || destination >= g->nVertex) {
		return NULL;
	}
	link = g->edges[source];
	while(link != NULL) {
		if(link->dest == destination) {
			return link->additionalInfo;
		}
		link = link->next;
	}
	return NULL;
}

int G_dijkstra(int source, int **preV, int **disV, int *nVertex, GRAPH_T *g) {
	int i, j, u, v, *pre, *dis, aux; // Vector pi (predecessors), distance and queue.
	char *visitedStatus; // Vector visited
	GRAPH_NODE_T *link;
	if(g ==  NULL || g->nEdge < 1) {
		return 0;
	}
	pre = (int *) malloc(g->nVertex * sizeof(int));
	dis = (int *) malloc(g->nVertex * sizeof(int));
	visitedStatus = (char *) malloc(g->nVertex * sizeof(char));
	if(pre == NULL || dis == NULL || visitedStatus == NULL) {
		free(pre);
		free(dis);
		free(visitedStatus);
		return 0;
	}
	for(i = 0; i < g->nVertex; i++) {
		visitedStatus[i] = 0;
		dis[i] = INT_MAX;
		pre[i] = -1;
	}
	dis[source] = 0;
	pre[source] = source;
	for(i = 1; i < g->nVertex; i++) {
		aux = INT_MAX;
		u = 0;
		for(j = 0; j < g->nVertex; j++) {
			if(visitedStatus[j] == 0 && dis[j] < aux) {
				aux = dis[j];
				u = j;
			}
		}
		visitedStatus[u] = 1;
		link = g->edges[u];
		while(link != NULL) {
			v = link->dest;
			if(visitedStatus[v] == 0 && dis[u] != INT_MAX && dis[u] + link->weight < dis[v]) {
				dis[v] = dis[u] + link->weight;
				pre[v] = u;
			}
			link = link->next;
		}
	}
	if(preV != NULL) {
		*preV = pre;
	} else {
		free(pre);
	}
	if(disV != NULL) {
		*disV = dis;
	} else {
		free(dis);
	}
	if(nVertex != NULL) {
		*nVertex = g->nVertex;
	}
	free(visitedStatus);
	return 1;
}

int G_prim(int source, int **preV, int **keyV, int *nVertex, GRAPH_T *g) {
	int i, j, u, v, *pre, *dis, aux; // Vector pi (predecessors), distance and queue.
	char *visitedStatus; // Vector visited
	GRAPH_NODE_T *link;
	if(g ==  NULL || g->nEdge < 1) {
		return 0;
	}
	pre = (int *) malloc(g->nVertex * sizeof(int));
	dis = (int *) malloc(g->nVertex * sizeof(int));
	visitedStatus = (char *) malloc(g->nVertex * sizeof(char));
	if(pre == NULL || dis == NULL || visitedStatus == NULL) {
		free(pre);
		free(dis);
		free(visitedStatus);
		return 0;
	}
	for(i = 0; i < g->nVertex; i++) {
		visitedStatus[i] = 0;
		dis[i] = INT_MAX;
		pre[i] = -1;
	}
	dis[source] = 0;
	// pre[source] = source;
	for(i = 1; i < g->nVertex; i++) {
		aux = INT_MAX;
		u = 0;
		for(j = 0; j < g->nVertex; j++) {
			if(visitedStatus[j] == 0 && dis[j] < aux) {
				aux = dis[j];
				u = j;
			}
		}
		visitedStatus[u] = 1;
		link = g->edges[u];
		while(link != NULL) {
			v = link->dest;
			if(visitedStatus[v] == 0 && link->weight < dis[v]) {
				dis[v] = link->weight;
				pre[v] = u;
			}
			link = link->next;
		}
	}
	if(preV != NULL) {
		*preV = pre;
	} else {
		free(pre);
	}
	if(keyV != NULL) {
		*keyV = dis;
	} else {
		free(dis);
	}
	if(nVertex != NULL) {
		*nVertex = g->nVertex;
	}
	free(visitedStatus);
	return 1;
}

void G_printEdgesFor(int vertex, void *(printEdgeFunction)(int vertex, int distance, char *additionalInformation, void *ptr), void *ptr, GRAPH_T *g) {
	GRAPH_NODE_T *link;
	int u;
	if(g ==  NULL || printEdgeFunction == NULL || vertex < 0 || vertex >= g->nVertex) {
		return;
	}
	link = g->edges[vertex];
	while(link != NULL) {
		u = link->dest;
		printEdgeFunction(u, link->weight, link->additionalInfo, ptr);
		link = link->next;
	}
}

void G_destroy(GRAPH_T *g) {
	GRAPH_NODE_T *link, *aux;
	int i;
	if(g == NULL) {
		return;
	}
	for(i = 0; i < g->nVertex; i++) {
		link = g->edges[i];
		while(link != NULL) {
			aux = link;
			link = link->next;
			free(aux);
		}
		g->edges[i] = NULL;
	}
	free(g->edges);
	free(g);
}
