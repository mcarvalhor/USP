#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <consts.h>
#include <buffer.h>
#include <register.h>
#include <bindex.h>

BTR_T *BT_init(char *path) {
	char buff[BTREE_PAGE_SIZE];
	BTR_T *aux;

	aux = (BTR_T *) malloc(sizeof(BTR_T));
	aux->stream = fopen(path, "rb+");
	if(aux->stream == NULL) {
		free(aux);
		return NULL;
	}

	if(fread(buff, sizeof(char), BTREE_PAGE_SIZE, aux->stream) != BTREE_PAGE_SIZE) {
		fclose(aux->stream);
		free(aux);
		return NULL;
	}

	memcpy(&aux->header.status, buff, sizeof(char));
	memcpy(&aux->header.root, &buff[1], sizeof(int));
	if(aux->header.status != STATUS_CONSISTENTE) {
		fclose(aux->stream);
		free(aux);
		return NULL;
	}

	aux->header.status = STATUS_INCONSISTENTE;
	if(fseek(aux->stream, 0, SEEK_SET) != 0 || fwrite(&aux->header.status, sizeof(char), 1, aux->stream) != 1) {
		fclose(aux->stream);
		free(aux);
		return NULL;
	}

	aux->levelCount = 0;

	return aux;
}

BTR_T *BT_new(char *path) {
	char buff[BTREE_PAGE_SIZE];
	BTR_T *aux;

	aux = (BTR_T *) malloc(sizeof(BTR_T));
	aux->stream = fopen(path, "wb+");
	if(aux->stream == NULL) {
		free(aux);
		return NULL;
	}

	aux->header.status = STATUS_INCONSISTENTE;
	aux->header.root = L_NIL;
	aux->levelCount = 0;

	memset(buff, LIXO, sizeof(char) * BTREE_PAGE_SIZE);
	memcpy(buff, &aux->header.status, sizeof(char));
	memcpy(&buff[1], &aux->header.root, sizeof(int));
	if(fwrite(buff, sizeof(char), BTREE_PAGE_SIZE, aux->stream) != BTREE_PAGE_SIZE) {
		fclose(aux->stream);
		free(aux);
		return NULL;
	}

	return aux;
}

long BT_count(BTR_T *index) {
	return index->levelCount;
}

void BT_resetCount(BTR_T *index) {
	index->levelCount = 0;
}

BTR_NODE BT_read_node(int RRN, BTR_T *index) {
	char aux[BTREE_PAGE_SIZE], *R;
	BTR_NODE auxN;
	int i;

	/* skip header, jump to node position and read a single node */
	if(fseek(index->stream, BTREE_PAGE_SIZE + RRN * BTREE_PAGE_SIZE, SEEK_SET) != 0 || fread(aux, sizeof(char), BTREE_PAGE_SIZE, index->stream) != BTREE_PAGE_SIZE) {
		fprintf(stderr, "Error reading BTREE page %d.\n", RRN);
		exit(1);
	}

	auxN.nodeRRN = RRN;
	R = aux;
	memcpy(&auxN.leave, R, sizeof(char));
	R += sizeof(char);
	memcpy(&auxN.nItems, R, sizeof(int));
	R += sizeof(int);

	/* copy all pointers and keys */
	for(i = 0; i < BTR_M - 1; i++) {
		memcpy(&auxN.pointers[i], R, sizeof(int));
		R += sizeof(int);
		memcpy(&auxN.key[i], R, sizeof(int));
		R += sizeof(int);
		memcpy(&auxN.byteOffset[i], R, sizeof(long));
		R += sizeof(long);
	}
	memcpy(&auxN.pointers[i], R, sizeof(int));

	return auxN;
}

void BT_rewrite_node(BTR_NODE root, BTR_T *index) {
	char aux[BTREE_PAGE_SIZE], *R;
	int i;

	memset(aux, BTR_TRASH, sizeof(char) * BTREE_PAGE_SIZE);

	R = aux;
	memcpy(R, &root.leave, sizeof(char));
	R += sizeof(char);
	memcpy(R, &root.nItems, sizeof(int));
	R += sizeof(int);

	/* copy all pointers and keys */
	for(i = 0; i < root.nItems; i++) {
		memcpy(R, &root.pointers[i], sizeof(int));
		R += sizeof(int);
		memcpy(R, &root.key[i], sizeof(int));
		R += sizeof(int);
		memcpy(R, &root.byteOffset[i], sizeof(long));
		R += sizeof(long);
	}
	memcpy(R, &root.pointers[i], sizeof(int));

	/* skip header, jump to node position and write a single node */
	if(fseek(index->stream, BTREE_PAGE_SIZE + root.nodeRRN * BTREE_PAGE_SIZE, SEEK_SET) != 0 || fwrite(aux, sizeof(char), BTREE_PAGE_SIZE, index->stream) != BTREE_PAGE_SIZE) {
		fprintf(stderr, "Error writing BTREE page %d.\n", root.nodeRRN);
		exit(1);
	}
}

int BT_write_node(BTR_NODE root, BTR_T *index) {
	long pos;

	/* skip to the end of the file */
	if(fseek(index->stream, 0, SEEK_END) != 0) {
		fprintf(stderr, "Error seeking new BTREE page.\n");
		exit(1);
	}

	/* get file stream pointer position and calculate RRN */
	if((pos = ftell(index->stream)) < 0) {
		fprintf(stderr, "Error calculating new BTREE page RRN.\n");
		exit(1);
	}
	root.nodeRRN = (pos - BTREE_PAGE_SIZE) / BTREE_PAGE_SIZE;

	/* write a single new node */
	BT_rewrite_node(root, index);

	return root.nodeRRN;
}

long BT_search_node(BTR_NODE root, int key, BTR_T *index) {
	int i;
	index->levelCount++;

	for(i = 0; i < root.nItems; i++) {
		if(root.key[i] >= key) {
			break;
		}
	}

	if(i >= root.nItems) {
		if(root.pointers[root.nItems] < 0) {
			return -1;
		}
		return BT_search_node(BT_read_node(root.pointers[root.nItems], index), key, index);
	}

	if(key == root.key[i]) {
		return root.byteOffset[i];
	}

	if(root.pointers[i] < 0) {
		return -1;
	}

	return BT_search_node(BT_read_node(root.pointers[i], index), key, index);
}

long BT_search(int key, BTR_T *index) {
	int i, j, aux;

	if(index->header.root < 0) {
		return -1;
	}

	return BT_search_node(BT_read_node(index->header.root, index), key, index);
}

int BT_push_node(REG_DATA *reg, int *keySplit, long *offsetSplit, BTR_NODE root, BTR_T *index) {
	int i, key, pointer;
	long keyOffset;
	BTR_NODE newNode;

	index->levelCount++;

	key = reg->idServidor;
	keyOffset = reg->byteOffset;
	pointer = BTR_TRASH;

	for(i = 0; i < root.nItems; i++) {
		/*if(root.key[i] >= key) {
			break;
		}*/
		if(root.key[i] > key) {
			break;
		}
	}

	/*if(i < root.nItems && key == root.key[i]) { // not inserting, but actually updating instead
		if(keyOffset != root.byteOffset[i]) {
			root.byteOffset[i] = keyOffset;
			BT_rewrite_node(root, index);
		}
		return -2; // updated
	}*/

	if(root.pointers[i] >= 0 && (pointer = BT_push_node(reg, &key, &keyOffset, BT_read_node(root.pointers[i], index), index)) < 0) {
		/* not leave node, so the algorithm must iterate down */
		return -1;
	}

	if(root.nItems < BTR_M - 1) { /* Cabe nesse nó: só adiciona */
		memmove(&root.key[i + 1], &root.key[i], sizeof(int) * (BTR_M - 2 - i));
		memmove(&root.byteOffset[i + 1], &root.byteOffset[i], sizeof(long) * (BTR_M - 2 - i));
		memmove(&root.pointers[i + 2], &root.pointers[i + 1], sizeof(int) * (BTR_M - 2 - i));
		root.key[i] = key;
		root.byteOffset[i] = keyOffset;
		root.pointers[i + 1] = pointer;
		root.nItems++;
		BT_rewrite_node(root, index);
		return -1;
	}

	memset(&newNode, BTR_TRASH, sizeof(BTR_NODE));
	newNode.nItems = (BTR_M - 1) / 2;
	newNode.leave = root.leave;
	root.nItems = BTR_M / 2;
	if(i > BTR_M / 2) { /* A chave a ser adicionada está no segundo (novo) nó. */
		*keySplit = root.key[BTR_M / 2];
		*offsetSplit = root.byteOffset[BTR_M / 2];
		memcpy(newNode.key, &root.key[BTR_M / 2 + 1], sizeof(int) * (i - BTR_M / 2 - 1));
		memcpy(newNode.byteOffset, &root.byteOffset[BTR_M / 2 + 1], sizeof(long) * (i - BTR_M / 2 - 1));
		memcpy(newNode.pointers, &root.pointers[BTR_M / 2 + 1], sizeof(int) * (i - BTR_M / 2));
		memcpy(&newNode.key[i - BTR_M / 2], &root.key[i], sizeof(int) * (BTR_M - 1 - i));
		memcpy(&newNode.byteOffset[i - BTR_M / 2], &root.byteOffset[i], sizeof(long) * (BTR_M - 1 - i));
		memcpy(&newNode.pointers[i - BTR_M / 2 + 1], &root.pointers[i + 1], sizeof(int) * (BTR_M - 1 - i));
		newNode.key[i - BTR_M / 2 - 1] = key;
		newNode.byteOffset[i - BTR_M / 2 - 1] = keyOffset;
		newNode.pointers[i - BTR_M / 2] = pointer;
	} else if(i < BTR_M / 2) { /* A chave a ser adicionada está no primeiro nó. */
		*keySplit = root.key[BTR_M / 2 - 1];
		*offsetSplit = root.byteOffset[BTR_M / 2 - 1];
		memcpy(newNode.key, &root.key[BTR_M / 2], sizeof(int) * (BTR_M - BTR_M / 2 - 1));
		memcpy(newNode.byteOffset, &root.byteOffset[BTR_M / 2], sizeof(long) * (BTR_M - BTR_M / 2 - 1));
		memcpy(newNode.pointers, &root.pointers[BTR_M / 2], sizeof(int) * (BTR_M - BTR_M / 2));
		memmove(&root.key[i + 1], &root.key[i], sizeof(int) * (BTR_M / 2 - 1 - i));
		memmove(&root.byteOffset[i + 1], &root.byteOffset[i], sizeof(long) * (BTR_M / 2 - 1 - i));
		memmove(&root.pointers[i + 2], &root.pointers[i + 1], sizeof(int) * (BTR_M / 2 - 1 - i));
		root.key[i] = key;
		root.byteOffset[i] = keyOffset;
		root.pointers[i + 1] = pointer;
	} else { /* A chave a ser promovida está exatamente no centro. */
		*keySplit = key;
		*offsetSplit = keyOffset;
		memcpy(newNode.key, &root.key[BTR_M / 2], sizeof(int) * (BTR_M - BTR_M / 2 - 1));
		memcpy(newNode.byteOffset, &root.byteOffset[BTR_M / 2], sizeof(long) * (BTR_M - BTR_M / 2 - 1));
		memcpy(&newNode.pointers[1], &root.pointers[BTR_M / 2 + 1], sizeof(int) * (BTR_M - BTR_M / 2 - 1));
		newNode.pointers[0] = pointer;
	}

	BT_rewrite_node(root, index);
	return BT_write_node(newNode, index);
}

int BT_push(REG_DATA *reg, BTR_T *index) {
	int key, pointer;
	long keyOffset;
	BTR_NODE root;

	/* no root yet */
	if(index->header.root < 0) {
		root.nItems = 1;
		root.key[0] = reg->idServidor;
		root.byteOffset[0] = reg->byteOffset;
		root.leave = BTR_IS_LEAVE;
		root.pointers[0] = root.pointers[1] = -1;
		index->header.root = BT_write_node(root, index);
		return 3;
	}

	if((pointer=BT_push_node(reg, &key, &keyOffset, BT_read_node(index->header.root, index), index)) >= 0) {
		/* Caso em que é necessário alocar uma nova raiz (chave promovida abaixo) */
		memset(&root, BTR_TRASH, sizeof(BTR_NODE));
		root.leave = BTR_IS_NOT_LEAVE;
		root.nItems = 1;
		root.key[0] = key;
		root.byteOffset[0] = keyOffset;
		root.pointers[0] = index->header.root;
		root.pointers[1] = pointer;
		index->header.root = BT_write_node(root, index);
		return 2;
	}
	
	return 1;
}

int BT_flush(BTR_T *index) {
	if(index == NULL) {
		return 0;
	}

	fflush(index->stream);
	return 1;
}

int BT_close(BTR_T *index) {
	char aux[BTREE_PAGE_SIZE];
	if(index == NULL) {
		return 0;
	}

	memset(aux, LIXO, sizeof(char) * BTREE_PAGE_SIZE);

	aux[0] = STATUS_CONSISTENTE;
	memcpy(&aux[1], &index->header.root, sizeof(int));
	if(fseek(index->stream, 0, SEEK_SET) != 0 || fwrite(aux, sizeof(char), BTREE_PAGE_SIZE, index->stream) != BTREE_PAGE_SIZE) {
		return 0;
	}

	fclose(index->stream);
	free(index);
	return 1;
}


