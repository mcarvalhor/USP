#include <stdlib.h>
#include <string.h>



/*
 *	~       HashTable       ~
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



struct __hashtable_element_t{
	void *data;
	unsigned long n;
	struct __hashtable_element_t *next;
};

struct __hashtable_t {
	struct __hashtable_element_t *table, *first;
	size_t memsize;
	unsigned long M;
	void (*ffree)(void *);
};



struct __hashtable_t *New_HashTable(size_t memsize, void (*ffree)(void *), unsigned long M){
	/*
	* Esta função aloca uma HashTable na memória e retorna um ponteiro para tal.
	*
	* Ela retorna um ponteiro para a HashTable alocada, ou NULL em caso de erros.
	*/
	if(memsize<1 || M<1)	return NULL;
	struct __hashtable_t *Aux=(struct __hashtable_t *)malloc(sizeof(struct __hashtable_t));
	Aux->table=(struct __hashtable_element_t *)calloc(M, sizeof(struct __hashtable_element_t));
	Aux->first=NULL;
	Aux->memsize=memsize;
	Aux->M=M;
	Aux->ffree=ffree;
	return Aux;
}

unsigned long HashTable_Function(long Key, unsigned long M){
	/*
	* Função Hash.
	*/
	return abs(Key)%M;
}

int Insert_Into_HashTable(long Key, void *Data, struct __hashtable_t *HT){
	/*
	* Esta função insere um elemento 'Data' de chave 'Key' na HashTable 'HT'.
	* A colisão é solucionada de modo "Chaining", utilizando um vetor dinâmico.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	unsigned long key_position;
	struct __hashtable_element_t *Aux=HT->first, *LastAux=NULL;
	key_position=HashTable_Function(Key, HT->M); // Achar posição da chave através da função hashing.
	HT->table[key_position].data=(void *)realloc(HT->table[key_position].data, HT->memsize*(++HT->table[key_position].n));
	memcpy(HT->table[key_position].data+(HT->table[key_position].n-1)*HT->memsize, Data, HT->memsize); // Copiar dados da chave.
	if(HT->table[key_position].n > 1)	return 1;
	while(Aux!=NULL && Aux < (&HT->table[key_position])){
		LastAux=Aux;
		Aux=Aux->next;
	}
	if(LastAux==NULL)	HT->first=&HT->table[key_position];
	else	LastAux->next=&HT->table[key_position];
	HT->table[key_position].next=Aux;
	return 1;
}

unsigned long Search_From_HashTable(long Key, void *Dest, struct __hashtable_t *HT){
	/*
	* Esta função verifica se existe na HashTable 'HT' um elemento de chave 'Key'.
	*
	* Ela retorna o número de elementos existentes com essa chave (colisão), ou 0 em caso de erros.
	*/
	if(Dest==NULL || HT==NULL)	return 0;
	unsigned long n, key_position;
	key_position=HashTable_Function(Key, HT->M); // Achar posição da chave através da função hashing.
	if( (n=HT->table[key_position].n) > 0){
		memcpy(Dest, &HT->table[key_position].data, sizeof(void *));
	}
	return n;
}

int Traverse_HashTable(void (*fact)(void *), struct __hashtable_t *HT){
	/*
	* Esta função passa por todos os elementos da HashTable em que há algum tipo de elemento.
	* É utilizado ponteiros para melhor performance.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(fact==NULL || HT==NULL)	return 0;
	struct __hashtable_element_t *Aux=HT->first;
	while(Aux!=NULL){
		fact(Aux->data);
		Aux=Aux->next;
	}
	return 1;
}

void Destroy_HashTable(struct __hashtable_t *HT){
	/*
	* Esta função limpa da memória a HashTable 'HT' e todos os elementos dentro dela.
	*
	* Ela retorna nada.
	*/
	if(HT==NULL)	return;
	struct __hashtable_element_t *Aux=HT->first;
	if(HT->ffree==NULL){
		while(Aux!=NULL){
			free(Aux->data);
			Aux=Aux->next;
		}
	}else{
		while(Aux!=NULL){
			HT->ffree(Aux->data);
			free(Aux->data);
			Aux=Aux->next;
		}
	}
	free(HT->table);
	free(HT);
}


