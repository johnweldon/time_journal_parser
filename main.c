#include <stdio.h>

#include "helpers.h"

int
main(int argc, char ** argv)
{
    fprintf(stdout, "Hello World\n");
    struct node * root = new_node(0, 0, 0);
    insert_node(root, "Goodbye Again");
    insert_node(root, "Hello Again");
    insert_node(root, "Peace Again");
    struct node * found = find_node(root, "Hello Again", compare_str);
    struct node * found2 = find_node(found, "\0", compare_str);
    fprintf(stdout, "root: %p - found: %p - found2: %p\n", root, found, found2);
}
