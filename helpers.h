#ifndef _helpers_h_
#define _helpers_h_

typedef int (*node_compare_fn)(void *, void *);
int compare_str(void *, void *);

struct node {
    struct node * prev;
    struct node * next;
    void * data;
};

struct node * new_node(struct node *, struct node *, void *);
int insert_node(struct node *, void *);
int remove_node(struct node *);
struct node * find_node(struct node *, void *, node_compare_fn);

unsigned long hash(const unsigned char *);

#endif /* _helpers_h_*/
