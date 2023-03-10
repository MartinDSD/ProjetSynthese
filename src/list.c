#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "tree.h"

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * @brief
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * 
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return struct list_node_t* Le nouveau nœud créé.
 */
static struct list_node_t * new_list_node(void * data) {
	struct list_node_t * newListNode = malloc(sizeof(struct list_node_t));
	newListNode->data = data;
	newListNode->successor = NULL;
	newListNode->predecessor = NULL;
	return newListNode;
}

void * get_list_node_data(const struct list_node_t * node) {
	return node->data;
}

struct list_node_t * get_successor(const struct list_node_t* node) {
	return node->successor;
}

struct list_node_t * get_predecessor(const struct list_node_t * node) {
	return node->predecessor;
}

void set_list_node_data(struct list_node_t * node, void * newData) {
	node->data = newData;
}

void set_successor(struct list_node_t * node, struct list_node_t * newSucc) {
	node->successor = newSucc;
}

void set_predecessor(struct list_node_t * node, struct list_node_t * newPred) {
	node->predecessor = newPred;
}

/********************************************************************
 * list_t
 ********************************************************************/

struct list_t * new_list(void (*viewData)(const void*), void (*freeData)(void*)) {
	struct list_t * newList = malloc(sizeof(struct list_t));
	newList->head = NULL;
	newList->tail = NULL;
	newList->numelm = 0;
	newList->viewData = viewData;
	newList->freeData = freeData;
	return newList;
}

int list_is_empty(struct list_t * L) {
	if (L->head == NULL){
		return 1;
	}
	else{
		return 0;
	}
}

int get_list_size(const struct list_t * L) {
	return L->numelm;
}

struct list_node_t * get_list_head(const struct list_t * L) {
	return L->head;
}

struct list_node_t * get_list_tail(const struct list_t * L) {
	return L->tail;
}

void increase_list_size(struct list_t * L) {
	L->numelm ++;
}

void decrease_list_size(struct list_t * L) {
	if (list_is_empty(L) == 1) {
		ShowMessage("src.list.c:decrease_list_size : La liste est vide, inutile de la decrementer !", 0);
	}
	L->numelm --;
}

void set_list_size(struct list_t * L, int newSize) {
	L->numelm = newSize;
}

void set_head(struct list_t * L, struct list_node_t * newHead) {
	if (list_is_empty(L) == 1){
		// On ne fait rien car la liste est vide et donc la tête est NULL
		// Pour pouvoir mettre la tête à newHead, il faut creer un noeud puis l'inserer dans la liste avec list_insert_first, list_insert_last ou list_insert_after
		ShowMessage("src.list.c:set_head : La liste est vide !", 0);
	} 
	else {
		L->head = newHead;
	}
	
}

void set_tail(struct list_t * L, struct list_node_t * newTail) {
	if (list_is_empty(L) == 1){
		// Meme raisonnement que pour set_head
		ShowMessage("src.list.c:set_tail : La liste est vide !", 0);
	} 
	else {
		L->tail = newTail;
	}
}

void delete_list(struct list_t * L, int deleteData) {
	if (list_is_empty(L) == 1){
		ShowMessage("src.list.c:delete_list : La liste est déjà vide !", 0);
	}
	else {
		if (deleteData == 1){
			while (L->head != NULL){
				struct list_node_t * temp = L->head;
				L->head = L->head->successor;
				L->freeData(temp->data);
				free(temp);
			}
		}
		else {
			if (deleteData == 0){
				while (L->head != NULL){
					struct list_node_t * temp = L->head;
					L->head = L->head->successor;
					free(temp);
					}
				}
			else {
				ShowMessage("Erreur src.list.c:delete_list , deleteData doit être égal à 0 ou 1  ", 1);
			}
			
		}
	}
}

void view_list(const struct list_t * L) {
	for(struct list_node_t *temp = L->head; temp != NULL ; temp = temp->successor){
		L->viewData(temp->data);
	}
}

void list_insert_first(struct list_t * L, void * data) {
	struct list_node_t * temp = new_list_node(data);

	if(list_is_empty(L)){
		set_head(L, temp);
		set_tail(L, temp);
	}else{
		set_successor(temp, get_list_head(L));
		set_head(L, temp);
	}
	L->numelm++;
}

void list_insert_last(struct list_t * L, void * data) {
	struct list_node_t * newListNode = new_list_node(data);
	if (list_is_empty(L)) {
		set_head(L, newListNode);
		L->tail = newListNode;
	}
	else {
		L->tail->successor = newListNode;
		newListNode->predecessor = L->tail;
		L->tail = newListNode;
	}
	increase_list_size(L);
}

void list_insert_after(struct list_t * L, void * data, struct list_node_t * ptrelm) {
	increase_list_size(L);
	if (list_is_empty(L)) {
		list_insert_first(L, data);
	}
	else {
		struct list_node_t * newListNode = new_list_node(data);
		newListNode->successor = ptrelm->successor;
		ptrelm->successor = newListNode;
	}
}

void * list_remove_first(struct list_t * L) {
	assert(get_list_head(L));
	struct list_node_t * temp = get_list_head(L);
	set_head(L, get_successor(get_list_head(L)));
	set_predecessor(get_list_head(L), NULL);
	decrease_list_size(L);
	void * data = get_list_node_data(temp);
	free(temp);
	return data;
}

void * list_remove_last(struct list_t * L) {
	assert(get_list_head(L));
	struct list_node_t *temp = get_list_tail(L);
	set_tail(L, get_predecessor(L->tail));
	set_successor(get_list_tail(L), NULL);
	decrease_list_size(L);
	void* data = get_list_node_data(temp);
	free(temp);
	return data;
}

void * list_remove_node(struct list_t * L, struct list_node_t * node) {
	assert(get_list_head(L) && get_list_tail(L));
	if (node == get_list_head(L)) {
		return list_remove_first(L);
	}
	else {
		if (node == get_list_tail(L)) {
			return list_remove_last(L);
		}
		else {
			set_successor(get_predecessor(node), get_successor(node));
			set_predecessor(get_successor(node), get_predecessor(node));
			decrease_list_size(L);
			void * data = get_list_node_data(node);
			free(node);
			return data;
		}
	}
}