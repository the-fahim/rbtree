#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

// Define colors
typedef enum {
    RBTREE_RED = 0,
    RBTREE_BLACK = 1
} color_t;

// Define the key type (usually an integer, change if needed)
typedef int key_t;

// Node structure for the Red-Black Tree
typedef struct node_t {
    color_t color;
    key_t key;
    struct node_t *left;
    struct node_t *right;
    struct node_t *p; // Parent pointer
} node_t;

// Tree container structure
typedef struct rbtree {
    node_t *root;
    node_t *nil;  // Sentinel leaf node (crucial for clean boundary conditions)
} rbtree;

// Core API Function Prototypes
rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
int rbtree_erase(rbtree *, node_t *);

// Optional: Traversal / Array extraction helpers if required by your project
// int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif // _RBTREE_H_