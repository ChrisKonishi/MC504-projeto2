#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define text_LEN 255

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    struct Node *next;
    char text[text_LEN];
} Node;

typedef struct {
    Node *ini;
    unsigned int len;
} LinkedList;


/*############## Functions ##############*/

LinkedList *create_list();
void del_list(LinkedList *list);
void del_list(LinkedList *list);
Node* get_node(LinkedList *list, int idx);
char* get_item(LinkedList *list, int idx);
void del_item(LinkedList *list, int idx);
void insert_item(LinkedList *list, char *text, int idx);
void append(LinkedList *list, char *text);
void print_list(LinkedList *list);



LinkedList *create_list(){
    LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
    list->ini = NULL;
    list->len = 0;
    return list;
}

void del_list(LinkedList *list){
    Node *node = list->ini;
    Node *aux;
    for (int i=0; i<list->len; i++){
        aux = node;
        node = node->next;
        free(aux);
    }
    free(list);
    return;
}

Node* get_node(LinkedList *list, int idx){
    if (idx >= list->len || idx < 0){
        fprintf(stderr, "Index out of range\n");
        exit(1);
    }

    Node *ret = list->ini;

    for (int i=1; i<=idx; i++){
        ret = ret->next;
    }
    return ret;
}


/*
Args:
    list: the linked list
    idx: index of the item to retrieve (0 index)

Returns:
    The item desired (string)
*/
char* get_item(LinkedList *list, int idx){
    return get_node(list, idx)->text;
}

void del_item(LinkedList *list, int idx){
    if (idx >= list->len || idx < 0){
        fprintf(stderr, "Index out of range\n");
        exit(1);
    }
    Node *aux;
    if (idx == 0){
        aux = list->ini;
        list->ini = list->ini->next;
        free(aux);
    }
    else{
        Node *prev = get_node(list, idx-1);
        aux = prev->next;
        prev->next = aux->next;
        free(aux);
    }
    list->len--;
}

void insert_item(LinkedList *list, char *text, int idx){
    if (idx > list->len || idx < 0){
        fprintf(stderr, "Index out of range\n");
        exit(1);
    }
    Node *new = (Node*) malloc(sizeof(Node));
    new->text;
    strcpy(new->text, text);
    if (idx == 0){
        new->next = list->ini;
        list->ini = new;
    }
    else{
        Node *aux = get_node(list, idx-1);
        new->next = aux->next;
        aux->next = new;
    }
    list->len++;
}

void append(LinkedList *list, char *text){
    insert_item(list, text, list->len);
}

void print_list(LinkedList *list){
    FILE *f = fopen("log.txt", "a");
    printf("List Status\n");
    for (int i=0; i<list->len; i++){
        fprintf(f, "%s\n", get_item(list, i));
    }
    fclose(f);
}


#endif
