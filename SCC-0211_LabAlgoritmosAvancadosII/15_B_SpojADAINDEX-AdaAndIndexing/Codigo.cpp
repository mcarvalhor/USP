#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>

#define ALPHABET_START ('a')
#define ALPHABET_END ('z')
#define ALPHABET_SIZE (ALPHABET_END - ALPHABET_START + 1)



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



using namespace std;
typedef long long ll;

typedef struct __node {
	struct __node *alphabet[ALPHABET_SIZE];
	ll nWords, nTotalWords;
} NODE;



void add_to_index(char *str, NODE *root) {
	NODE *aux;
	ll i;
	if(*str == '\0') {
		root->nWords++;
		root->nTotalWords++;
		return;
	}

	if(root->alphabet[*str - ALPHABET_START] == NULL) {
		aux = (NODE *) malloc(sizeof(NODE));
		memset(aux->alphabet, 0, sizeof(NODE *) * ALPHABET_SIZE);
		aux->nWords = aux->nTotalWords = 0;
		root->alphabet[*str - ALPHABET_START] = aux;
	}

	add_to_index(str + 1, root->alphabet[*str - ALPHABET_START]);
	root->nTotalWords++;
}

/*ll find_all_words(NODE *root) {
	ll i, count;
	if(root == NULL) {
		return 0;
	}
	count = 0;
	for(i = 0; i < ALPHABET_SIZE; i++) {
		count += find_all_words(root->alphabet[i]);
	}
	count += root->nWords;
	return count;
}*/

ll found(char *q, NODE *root) {
	if(root == NULL) {
		return 0;
	}
	if(*q != '\0') {
		return found(q + 1, root->alphabet[*q - ALPHABET_START]);
	}
	return root->nTotalWords;
}

void destroy(NODE *root) {
	ll i;
	if(root == NULL) {
		return;
	}
	for(i = 0; i < ALPHABET_SIZE; i++) {
		destroy(root->alphabet[i]);
	}
	free(root);
}

void testCase() {
	ll i, N, Q;
	NODE *root;
	char str[1000001];

	root = (NODE *) malloc(sizeof(NODE));
	memset(root->alphabet, 0, sizeof(NODE *) * ALPHABET_SIZE);
	root->nWords = root->nTotalWords = 0;

	cin >> N >> Q;
	for(i = 0; i < N; i++) {
		cin >> str;
		add_to_index(str, root);
	}

	for(i = 0; i < Q; i++) {
		cin >> str;
		cout << found(str, root) << endl;
	}

	destroy(root);
}



int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}


