#include <stdlib.h>
#include <string.h>



/*
 *	~      Rubro Negra      ~
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



enum __nodecolour_t { BLACK = 0, RED };

struct __node_t {
	void *value;
	enum __nodecolour_t colour;
	struct __node_t *p, *l, *r;
};

struct __redblacktree_t {
	struct __node_t *root, *nil;
	size_t memsize;
	int (*fcmp)(void *, void *);
	void (*ffree)(void *);
	unsigned long nitems;
};



struct __redblacktree_t *RBT_New(size_t memsize, int (*fcmp)(void *, void *), void (*ffree)(void *)){
	/*
	* Esta função aloca na memória a estrutura que representa uma árvore rubro-negra e retorna um ponteiro para tal.
	* É necessário passar para ela o tamanho 'memsize', em bytes, do valor que será armazenado em cada nó.
	* Além disso, um ponteiro para função 'fcmp' para comparar esses valores (maior, menor, igual).
	* E o usuário pode definir uma função 'ffree' para liberar os dados da memória, caso seja ua estrutura complexa com ponteiros dentro.
	*
	* Complexidade: O(1)
	*
	* Ela retorna o ponteiro para a árvore na HEAP, ou NULL em caso de erros.
	*/
	struct __redblacktree_t *Aux;
	if(memsize<1 || fcmp==NULL)	return NULL;
	Aux=(struct __redblacktree_t *)malloc(sizeof(struct __redblacktree_t));
	Aux->root=Aux->nil=(struct __node_t *)calloc(1, sizeof(struct __node_t));
	Aux->memsize=memsize;
	Aux->fcmp=fcmp;
	Aux->ffree=ffree;
	Aux->nitems=0;
	return Aux;
}

void RBT_Rotate_L(struct __node_t *Root, struct __redblacktree_t *T){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela rotaciona um nó 'Root' para a esquerda.
	*int cmp;
	* Complexidade: O(1)
	*
	* Ela não retorna nada.
	*/
	struct __node_t *Aux=Root->r;
	Root->r=Aux->l;
	if(Aux->l!=T->nil){
		Aux->l->p=Root;
	}
	Aux->p=Root->p;
	if(Root->p==T->nil)	T->root=Aux;
	else if(Root==Root->p->l)	Root->p->l=Aux;
	else	Root->p->r=Aux;
	Aux->l=Root;
	Root->p=Aux;
}

void RBT_Rotate_R(struct __node_t *Root, struct __redblacktree_t *T){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela rotaciona um nó 'Root' para a direita.
	*
	* Complexidade: O(1)
	*
	* Ela não retorna nada.
	*/
	struct __node_t *Aux=Root->l;
	Root->l=Aux->r;
	if(Aux->r!=T->nil){
		Aux->r->p=Root;
	}
	Aux->p=Root->p;
	if(Root->p==T->nil)	T->root=Aux;
	else if(Root==Root->p->r)	Root->p->r=Aux;
	else	Root->p->l=Aux;
	Aux->r=Root;
	Root->p=Aux;
}

void RBT_Insert_Colourise(struct __node_t *Root, struct __redblacktree_t *T){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela verifica os nós onde será realizada alguma rotação, e atualiza suas cores ("BLACK", "RED").
	*
	* Complexidade: O(logN)
	*
	* Ela não retorna nada.
	*/
	struct __node_t *Aux;
	if(Root==NULL || T==NULL)	return;
	while(Root->p->colour==RED){
		if(Root->p==Root->p->p->l){
			Aux=Root->p->p->r;
			if(Aux->colour==RED){
				Root->p->colour=Aux->colour=BLACK;
				Root->p->p->colour=RED;
				Root=Root->p->p;
			}else if(Root==Root->p->r){
				Root=Root->p;
				RBT_Rotate_L(Root, T);
			}else{
				Root->p->colour=BLACK;
				Root->p->p->colour=RED;
				RBT_Rotate_R(Root->p->p, T);
			}
		}else{
			Aux=Root->p->p->l;
			if(Aux->colour==RED){
				Root->p->colour=Aux->colour=BLACK;
				Root->p->p->colour=RED;
				Root=Root->p->p;
			}else if(Root==Root->p->l){
				Root=Root->p;
				RBT_Rotate_R(Root, T);
			}else{
				Root->p->colour=BLACK;
				Root->p->p->colour=RED;
				RBT_Rotate_L(Root->p->p, T);
			}
		}
	}
	T->root->colour=BLACK;
}

int RBT_Insert(void *Elem, struct __redblacktree_t *T){
	/*
	* Esta função insere 'Elem' na rubro-negra 'T'.
	*
	* Complexidade: O(logN)
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	struct __node_t **Aux, *Parent;
	int cmp;
	if(Elem==NULL || T==NULL)	return 0;
	Aux=&T->root;
	Parent=T->nil;
	while((*Aux)!=T->nil){
		cmp=T->fcmp(Elem, (*Aux)->value);
		Parent=*Aux;
		if(cmp>0)	Aux=&(*Aux)->l;
		else	Aux=&(*Aux)->r;
	}
	*Aux=(struct __node_t *)malloc(sizeof(struct __node_t));
	(*Aux)->value=(void *)malloc(T->memsize);
	(*Aux)->l=(*Aux)->r=T->nil;
	(*Aux)->p=Parent;
	(*Aux)->colour=RED;
	memcpy((*Aux)->value, Elem, T->memsize);
	RBT_Insert_Colourise(*Aux, T);
	T->nitems++;
	return 1;
}

void *RBT_Min_(struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela acha o menor valor armazenado na sub-árvore 'Root'.
	*
	* Complexidade: O(logN)
	*
	* Ela não retorna nada.
	*/
	struct __node_t *Aux;
	if(Root==NULL)	return NULL;
	Aux=Root;
	while(Aux!=NULL && Aux->value!=NULL){
		if(Aux->l==NULL || Aux->l->value==NULL)	break;
		else	Aux=Aux->l;
	}
	return Aux->value;
}

int RBT_Min(void *Dest, struct __redblacktree_t *T){
	/*
	* Esta função salva o menor elemento da rubro-negra 'T' em 'Dest'.
	*
	* Complexidade: O(logN)
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros ou árvore vazia.
	*/
	if(Dest==NULL || T==NULL)	return 0;
	if(T->nitems<1)	return 0;
	memcpy(Dest, RBT_Min_(T->root), T->memsize);
	return 1;
}

void *RBT_Max_(struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela acha o maior valor armazenado na sub-árvore 'Root'.
	*
	* Complexidade: O(logN)
	*
	* Ela não retorna nada.
	*/
	struct __node_t *Aux;
	if(Root==NULL)	return NULL;
	Aux=Root;
	while(Aux!=NULL && Aux->value!=NULL){
		if(Aux->r==NULL || Aux->r->value==NULL)	break;
		else	Aux=Aux->r;
	}
	return Aux->value;
}

int RBT_Max(void *Dest, struct __redblacktree_t *T){
	/*
	* Esta função salva o maior elemento da rubro-negra 'T' em 'Dest'.
	*
	* Complexidade: O(logN)
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros ou árvore vazia.
	*/
	if(Dest==NULL || T==NULL)	return 0;
	if(T->nitems<1)	return 0;
	memcpy(Dest, RBT_Max_(T->root), T->memsize);
	return 1;
}

int RBT_Predecessor(void *Dest, void *Elem, struct __redblacktree_t *T){
	/*
	* Esta função salva em 'Dest' o predecessor de um elemento 'Elem' na rubro-negra 'T'.
	*
	* Complexidade: O(logN)
	*
	* Ela retorna 1 em caso de sucesso, 0 caso 'Elem' não tenha predecessor, -1 caso 'Elem' não foi encontrado, ou -2 em caso de erros.
	*/
	struct __node_t *Aux, *Lefty;
	int cmp;
	if(Dest==NULL || Elem==NULL || T==NULL)	return -2;
	Aux=T->root;
	Lefty=NULL;
	while(Aux!=NULL && Aux!=T->nil){
		cmp=T->fcmp(Elem, Aux->value);
		if(cmp>0){
			Aux=Aux->l;
		}else if(cmp<0){
			Lefty=Aux;
			Aux=Aux->r;
		}else	break;
	}
	if(Aux==NULL || Aux==T->nil)	return -1; /* 'Elem' não encontrado. */
	if(Aux->l!=NULL && Aux->l!=T->nil)	memcpy(Dest, RBT_Max_(Aux->l), T->memsize);
	else if(Lefty!=NULL)	memcpy(Dest, Lefty->value, T->memsize);
	else return 0; /* 'Elem' não possui antecessor. */
	return 1; /* Ok */
}

int RBT_Successor(void *Dest, void *Elem, struct __redblacktree_t *T){
	/*
	* Esta função salva em 'Dest' o sucessor de um elemento 'Elem' na rubro-negra 'T'.
	*
	* Complexidade: O(logN)
	*
	* Ela retorna 1 em caso de sucesso, 0 caso 'Elem' não tenha sucessor, -1 caso 'Elem' não foi encontrado, ou -2 em caso de erros.
	*/
	struct __node_t *Aux, *Righty;
	int cmp;
	if(Dest==NULL || Elem==NULL || T==NULL)	return -2;
	Aux=T->root;
	Righty=NULL;
	while(Aux!=NULL && Aux!=T->nil){
		cmp=T->fcmp(Elem, Aux->value);
		if(cmp>0){
			Righty=Aux;
			Aux=Aux->l;
		}else if(cmp<0){
			Aux=Aux->r;
		}else	break;
	}
	if(Aux==NULL || Aux==T->nil)	return -1; /* 'Elem' não encontrado. */
	if(Aux->r!=NULL && Aux->r!=T->nil)	memcpy(Dest, RBT_Min_(Aux->r), T->memsize);
	else if(Righty!=NULL)	memcpy(Dest, Righty->value, T->memsize);
	else return 0; /* 'Elem' não possui sucessor. */
	return 1; /* Ok. */
}

void RBT_PreOrder_(void (*fact)(void *), struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela percorre ("traverse") a sub-árvore 'Root' de forma pré-ordem.
	*
	* Complexidade: O(N)
	*
	* Ela não retorna nada.
	*/
	if(fact==NULL || Root==NULL || Root->value==NULL)	return;
	fact(Root->value);
	RBT_PreOrder_(fact, Root->l);
	RBT_PreOrder_(fact, Root->r);
}

int RBT_PreOrder(void (*fact)(void *), struct __redblacktree_t *T){
	/*
	* Esta função percorre todos os elementos da rubro-negra 'T' de forma pré-ordem.
	* Ela chama a função 'fact' para cada elemento percorrido, uma função que o usuário pode definir.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros ou árvore vazia.
	*/
	if(fact==NULL || T==NULL)	return 0;
	if(T->nitems<1)	return 0;
	RBT_PreOrder_(fact, T->root);
	return 1;
}

void RBT_InOrder_(void (*fact)(void *), struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela percorre ("traverse") a sub-árvore 'Root' de forma em-ordem.
	*
	* Complexidade: O(N)
	*
	* Ela não retorna nada.
	*/
	if(fact==NULL || Root==NULL || Root->value==NULL)	return;
	RBT_InOrder_(fact, Root->l);
	fact(Root->value);
	RBT_InOrder_(fact, Root->r);
}

int RBT_InOrder(void (*fact)(void *), struct __redblacktree_t *T){
	/*
	* Esta função percorre todos os elementos da rubro-negra 'T' de forma em-ordem.
	* Ela chama a função 'fact' para cada elemento percorrido, uma função que o usuário pode definir.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros ou árvore vazia.
	*/
	if(fact==NULL || T==NULL)	return 0;
	if(T->nitems<1)	return 0;
	RBT_InOrder_(fact, T->root);
	return 1;
}

void RBT_PostOrder_(void (*fact)(void *), struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela percorre ("traverse") a sub-árvore 'Root' de forma pós-ordem.
	*
	* Complexidade: O(N)
	*
	* Ela não retorna nada.
	*/
	if(fact==NULL || Root==NULL || Root->value==NULL)	return;
	RBT_PostOrder_(fact, Root->l);
	RBT_PostOrder_(fact, Root->r);
	fact(Root->value);
}

int RBT_PostOrder(void (*fact)(void *), struct __redblacktree_t *T){
	/*
	* Esta função percorre todos os elementos da rubro-negra 'T' de forma pós-ordem.
	* Ela chama a função 'fact' para cada elemento percorrido, uma função que o usuário pode definir.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros ou árvore vazia.
	*/
	if(fact==NULL || T==NULL)	return 0;
	if(T->nitems<1)	return 0;
	RBT_PostOrder_(fact, T->root);
	return 1;
}

void RBT_Destroy_NULLFFREE_(struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela limpa da memória o valor e a própria estrutura de cada nó da sub-árvore 'Root'.
	*
	* Complexidade: O(N)
	*
	* Ela não retorna nada.
	*/
	if(Root==NULL || Root->value==NULL)	return;
	RBT_Destroy_NULLFFREE_(Root->l);
	RBT_Destroy_NULLFFREE_(Root->r);
	free(Root->value);
	free(Root);
}

void RBT_Destroy_FFREE_(void (*ffree)(void *), struct __node_t *Root){
	/*
	* Esta é uma função auxiliar, ou seja, não está disponível no TAD (.h) e é usada apenas internamente neste arquivo de código (.c).
	* Ela limpa da memória o valor e a própria estrutura de cada nó da sub-árvore 'Root'.
	* Antes de limpar um valor, ela chama uma função 'ffree' definida pelo usuário, para limpar estruturas complexas (como structs com ponteiros dentro).
	*
	* Complexidade: O(N)
	*
	* Ela não retorna nada.
	*/
	if(Root==NULL || Root->value==NULL)	return;
	RBT_Destroy_FFREE_(ffree, Root->l);
	RBT_Destroy_FFREE_(ffree, Root->r);
	ffree(Root->value);
	free(Root->value);
	free(Root);
}

void RBT_Destroy(struct __redblacktree_t *T){
	/*
	* Esta função limpa da memória a rubro-negra 'T' e todos seus nós e valores.
	* Caso o valor armazenado seja uma estrutura complexa, que tenha outros ponteiros que precisem ser desalocados externamente,
	* 	a função vai chamar outra função definida pelo usuário para liberar cada valor de cada nó.
	* Caso o valor armazenado seja uma estrutura simples (como um inteiro, por exemplo), nenhuma outra função é chamada ao liberar os nós.
	*
	* Ela não retorna nada.
	*/
	if(T==NULL)	return;
	if(T->ffree==NULL)	RBT_Destroy_NULLFFREE_(T->root);
	else RBT_Destroy_FFREE_(T->ffree, T->root);
	free(T->nil);
	free(T);
}


