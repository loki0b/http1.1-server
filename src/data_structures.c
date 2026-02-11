#include <stdlib.h>

#include "data_structures.h"

struct node* create_node(struct node* next, void* element) {
    struct node* n;
    
    n = (struct node*) malloc(sizeof(struct node));
    if (n == NULL) return NULL;

    n->element = element;
    n->next = next;

    return n;
}

struct linked_list* create_linked_list() {
    struct linked_list* ll;
    
    ll = (struct linked_list*) malloc(sizeof(struct linked_list));
    if (ll == NULL) return NULL;

    ll->head = ll->tail = create_node(NULL, NULL);
    ll->length = 0;

    return ll;
}

int insert(struct linked_list* ll, void* element) {
    struct node* n;
    
    n = create_node(ll->head->next, element);
    if (n == NULL) return 0;

    ll->head->next = n;
    if (ll->tail == ll->head) ll->tail = ll->head->next;
    ll->length++;

    return 1;
}