
#ifndef GRAPH_H_
#define GRAPH_H_

	typedef struct __graph_t GRAPH_T;

	GRAPH_T *G_new(int nVertex);
	int G_addEdgeInfo(int source, int destination, int weight, char *additionalInformation, GRAPH_T *g);
	int G_addEdge(int source, int destination, int weight, GRAPH_T *g);
	char *G_getInfo(int source, int destination, GRAPH_T *g);
	int G_resetEdges(GRAPH_T *g);
	void G_destroy(GRAPH_T *g);

	void G_printEdgesFor(int vertex, void *(printEdgeFunction)(int vertex, int distance, char *additionalInformation, void *ptr), void *ptr, GRAPH_T *g);
	
	int G_dijkstra(int source, int **preV, int **disV, int *nVertex, GRAPH_T *g);
	int G_prim(int source, int **preV, int **keyV, int *nVertex, GRAPH_T *g);

#endif