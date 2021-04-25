#include <stdio.h>
#include "linked_list.h"


int main(){
    LinkedList *list = create_list();

    print_list(list);
    insert_item(list, "Olá, tudo bem?", 0);
    append(list, "Meu nome é Jugemu Jugemu");
    append(list, "dsadafsdgsdgsd");
    append(list, "Adeus");
    print_list(list);
    del_item(list, 2);
    print_list(list);
    insert_item(list, "Olá, tudo bem?", 10);
    del_list(list);
    return 0;
}
