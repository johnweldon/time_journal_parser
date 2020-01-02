#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

int
compare_str(void * l, void * r)
{
    fprintf(stderr, "comparing %p (%s) with %p (%s)\n", l, l, r, r);

    if (l == r) {
        return 0;
    }

    if (0 == l) {
        return -1;
    }

    if (0 == r) {
        return 1;
    }

    const char * lhs = l;
    const char * rhs = r;

    for (int i = 0; ; i++) {
        if (*(lhs + i) < * (rhs + i)) {
            return -1;
        }

        if (*(lhs + i) > * (rhs + i)) {
            return -1;
        }

        if (*(lhs + i) == 0) {
            break;
        }
    }

    return 0;
}

struct node * new_node(struct node * p, struct node * n, void * d)
{
    struct node * np = malloc(sizeof(struct node));
    np->prev = p;
    np->next = n;
    np->data = d;
    return np;
}

int
insert_node(struct node * after, void * data)
{
    if (0 == after) {
        return -1;
    }

    after->next = new_node(after, after->next, data);

    if (0 != after->next->next) {
        after->next->next->prev = after->next;
    }

    return 0;
}

int
remove_node(struct node * del)
{
    if (0 == del) {
        return -1;
    }

    if (0 != del->prev) {
        del->prev->next = del->next;
    }

    if (0 != del->next) {
        del->next->prev = del->prev;
    }

    del->next = 0;
    del->prev = 0;
    del->data = 0;
    free(del);
    del = 0;
    return 0;
}

struct node * find_node(struct node * from, void * data, node_compare_fn fn)
{
    struct node * bp = from, *fp = from->next;

    for(; 0 != bp; bp = bp->prev) {
        if (0 == fn(data, bp->data)) {
            return bp;
        }
    }

    for(; 0 != fp; fp = fp->next) {
        if (0 == fn(data, fp->data)) {
            return fp;
        }
    }

    return 0;
}

unsigned long
hash(const unsigned char * str)
{
    if (0 == str) {
        return 0;
    }

    // djb2
    unsigned long hash = 5381;
    int c;

    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}
