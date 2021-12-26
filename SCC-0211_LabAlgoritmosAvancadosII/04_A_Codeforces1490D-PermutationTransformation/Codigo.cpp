#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>



/*
 *      ==    AlgAvançados   ==
 *
 *      Matheus Carvalho Raimundo (Nº 10369014)
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/

typedef struct __node {
	struct __node *left, *right;
	int value, index;
} NODE;



using namespace std;

NODE *build_subtree(NODE *root, int *A, int start, int end) {
	NODE *aux;
	int i;
	if(end - start + 1 < 1) {
		return NULL;
	}

	aux = (NODE *) malloc(sizeof(NODE));
	aux->left = aux->right = NULL;
	aux->value = -1;
	aux->index = start;
	for(i = start; i <= end; i++) {
		if(A[i] > aux->value) {
			aux->value = A[i];
			aux->index = i;
		}
	}

	aux->left = build_subtree(aux, A, start, aux->index - 1);
	aux->right = build_subtree(aux, A, aux->index + 1, end);
	return aux;
}

void count_subtree(NODE *root, int *D, int level) {
	if(root == NULL) {
		return;
	}
	D[root->index] = level;
	count_subtree(root->left, D, level + 1);
	count_subtree(root->right, D, level + 1);
}

void free_subtree(NODE *root) {
	if(root == NULL) {
		return;
	}
	free_subtree(root->left);
	free_subtree(root->right);
	free(root);
}

void testCase() {
	int N, i, *A, *D;
	NODE *root;

	cin >> N;
	root = (NODE *) malloc(sizeof(NODE));
	A = (int *) malloc(sizeof(int) * N);
	D = (int *) malloc(sizeof(int) * N);
	root->left = root->right = NULL;
	root->value = -1;
	root->index = 0;
	for(i = 0; i < N; i++) {
		cin >> A[i];
		if(A[i] > root->value) {
			root->value = A[i];
			root->index = i;
		}
	}

	root->left = build_subtree(root, A, 0, root->index - 1);
	root->right = build_subtree(root, A, root->index + 1, N - 1);

	count_subtree(root, D, 0);

	free_subtree(root);
	free(A);

	for(i = 0; i < N; i++) {
		cout << D[i] << " ";
	}
	cout << endl;
	free(D);
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
