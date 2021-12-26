#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/*
 *	~        SKIPLIST       ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



struct __node_t {
	char word[51]; /* Palavra */
	char meaning[141]; /* Definição */
	struct __node_t *left, *right, *topper, *bottom;
};

struct __dictionary_t {
	struct __node_t *first; /* Primeiro elemento da SkipList */
};



struct __dictionary_t *New_Dictionary(unsigned int RandomSeed){
	/*
	* Essa função cria um dicionário, ou seja, a estrutura que representa a SkipList.
	*
	* Ela retorna um ponteiro para esta estrutura já alocada na memória.
	*/
	struct __dictionary_t *Aux=(struct __dictionary_t *)calloc(1,sizeof(struct __dictionary_t));
	Aux->first=(struct __node_t *)calloc(1,sizeof(struct __node_t));
	srand(RandomSeed);
	return Aux;
}

int Insert_Word(char *Word, char *Meaning, struct __dictionary_t *Dic){
	/*
	* Essa função insere na SkipList 'Dic' a palavra 'Word', com definição 'Meaning'.
	*
	* Ela retorna 1 caso a inserção tenha ocorrido com sucesso, 0 caso a palavra já exista, e -1 em caso de erros.
	*/
	struct __node_t *Aux, *Last=NULL, *Curr;
	int cmp;
	if(Word==NULL || Meaning==NULL || Dic==NULL)	return -1;
	Curr=Dic->first;
	while(Curr!=NULL) {
		cmp=strcmp(Curr->word,Word);
		if(cmp<0)	if(Curr->right!=NULL) Curr=Curr->right; else if(Curr->bottom!=NULL) Curr=Curr->bottom; else break;
		else if(cmp>0)	if(Curr->left->bottom!=NULL) Curr=Curr->left->bottom; else { Curr=Curr->left; break; }
		else return 0;
	}
	do {
		Aux=(struct __node_t *)calloc(1,sizeof(struct __node_t));
		Aux->left=Curr;
		Aux->right=Curr->right;
		if(Last!=NULL) {
			Aux->bottom=Last;
			Last->topper=Aux;
		}
		if(Curr->right!=NULL)	Curr->right->left=Aux;
		Curr->right=Aux;
		strncpy(Aux->word, Word, 50);
		strncpy(Aux->meaning, Meaning, 140);
		Last=Aux;
		while(Curr->topper==NULL && Curr->left!=NULL)	Curr=Curr->left;
		if(Curr->topper==NULL) {
			Dic->first=Curr->topper=(struct __node_t *)calloc(1,sizeof(struct __node_t));
			Curr->topper->bottom=Curr;
			Curr=Curr->topper;
		} else Curr=Curr->topper;
	} while(rand()%2);
	return 1;
}

int Set_Word(char *Word, char *Meaning, struct __dictionary_t *Dic){
	/*
	* Essa função altera a definição da palavra 'Word' na SkipList 'Dic' para 'Meaning'.
	*
	* Ela retorna 1 caso a alteração tenha ocorrido com sucesso, 0 caso a palavra não exista, e -1 em caso de erros.
	*/
	struct __node_t *Curr;
	int cmp;
	if(Word==NULL || Meaning==NULL || Dic==NULL)	return -1;
	Curr=Dic->first;
	while(Curr!=NULL) {
		cmp=strcmp(Curr->word,Word);
		if(cmp<0)	if(Curr->right!=NULL) Curr=Curr->right; else Curr=Curr->bottom;
		else if(cmp>0)	Curr=Curr->left->bottom;
		else break;
	}
	if(Curr==NULL)	return 0;
	while(Curr!=NULL) {
		strncpy(Curr->meaning, Meaning, 140);
		Curr=Curr->bottom;
	}
	return 1;
}

int Remove_Word(char *Word, struct __dictionary_t *Dic){
	/*
	* Essa função remove a palavra 'Word' e sua definição da SkipList 'Dic'.
	*
	* Ela retorna 1 caso a remoção tenha ocorrido com sucesso, 0 caso a palavra não exista, e -1 em caso de erros.
	*/
	struct __node_t *Aux, *Curr;
	int cmp;
	if(Word==NULL || Dic==NULL)	return -1;
	Curr=Dic->first;
	while(Curr!=NULL) {
		cmp=strcmp(Curr->word,Word);
		if(cmp<0)	if(Curr->right!=NULL) Curr=Curr->right; else Curr=Curr->bottom;
		else if(cmp>0)	Curr=Curr->left->bottom;
		else break;
	}
	if(Curr==NULL)	return 0;
	while(Curr!=NULL) {
		if(Curr->left!=NULL)	Curr->left->right=Curr->right;
		if(Curr->right!=NULL)	Curr->right->left=Curr->left;
		Aux=Curr;
		Curr=Curr->bottom;
		free(Aux);
	}
	return 1;
}

int Print_Word(char *Word, FILE *FStream, struct __dictionary_t *Dic){
	/*
	* Essa função imprime em 'FStream' a palavra 'Word' e sua definição, presentes na SkipList 'Dic'.
	*
	* Ela retorna 1 caso a impressão tenha ocorrido com sucesso, 0 caso a palavra não exista, e -1 em caso de erros.
	*/
	struct __node_t *Curr;
	int cmp;
	if(Word==NULL || Dic==NULL)	return -1;
	Curr=Dic->first;
	while(Curr!=NULL) {
		cmp=strcmp(Curr->word,Word);
		if(cmp<0)	if(Curr->right!=NULL) Curr=Curr->right; else Curr=Curr->bottom;
		else if(cmp>0)	Curr=Curr->left->bottom;
		else {
			fprintf(FStream, "%s %s\n", Word, Curr->meaning);
			return 1;
		}
	}
	return 0;
}

int Print_Words_Starting_With(char Ch, FILE *FStream, struct __dictionary_t *Dic){
	/*
	* Essa função imprime em 'FStream' todas as palavras que começam com 'Ch' e suas respectivas definições, presentes na SkipList 'Dic'.
	*
	* Ela retorna 1 caso a impressão tenha ocorrido com sucesso, 0 caso nenhuma palavra que começa com 'Ch' foi encontrada, e -1 em caso de erros.
	*/
	struct __node_t *Curr;
	int cmp;
	if(Dic==NULL)	return -1;
	Curr=Dic->first;
	while(Curr!=NULL) {
		cmp=(*Curr->word)-Ch;
		if(cmp<0)	if(Curr->right!=NULL) Curr=Curr->right; else Curr=Curr->bottom;
		else if(cmp>0)	Curr=Curr->left->bottom;
		else {
			while(Curr->bottom!=NULL) {
				Curr=Curr->bottom;
				while(Curr->left!=NULL && (*Curr->left->word)==Ch)	Curr=Curr->left;
			}
			break;
		}
	}
	if(Curr==NULL)	return 0;
	while(Curr!=NULL && (*Curr->word)==Ch) {
		fprintf(FStream, "%s %s\n", Curr->word, Curr->meaning);
		Curr=Curr->right;
	}
	return 1;
}

void Destroy_Dictionary(struct __dictionary_t *Dic){
	/*
	* Essa função limpa da memória todas as palavras e suas respectivas definições de 'Dic'.
	* Ou seja, ela limpa da memória todos os nós de 'Dic', e a própria estrutura 'Dic'.
	*/
	struct __node_t *Aux, *Line=NULL, *Column;
	if(Dic==NULL)	return;
	Column=Dic->first;
	if(Dic->first!=NULL)	Line=Dic->first->bottom;
	while(Line!=NULL || Column!=NULL)
		if(Column!=NULL) {
			Aux=Column;
			Column=Column->right;
			free(Aux);
		} else {
			Column=Line;
			Line=Line->bottom;
		}
	free(Dic);
}


