#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

// Helper function to create a new node
static node_t *create_node(rbtree *t, const key_t key) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) return NULL;
    new_node->key = key;
    new_node->color = RBTREE_RED; // New nodes are always red by default
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->p = t->nil;
    return new_node;
}

rbtree *new_rbtree(void) {
    rbtree *t = (rbtree *)malloc(sizeof(rbtree));
    if (!t) return NULL;

    // Allocate and initialize the sentinel NIL node
    t->nil = (node_t *)malloc(sizeof(node_t));
    t->nil->color = RBTREE_BLACK;
    t->nil->left = t->nil;
    t->nil->right = t->nil;
    t->nil->p = t->nil;

    t->root = t->nil;
    return t;
}

// Recursive helper to free nodes
static void free_nodes(rbtree *t, node_t *x) {
    if (x != t->nil) {
        free_nodes(t, x->left);
        free_nodes(t, x->right);
        free(x);
    }
}

void delete_rbtree(rbtree *t) {
    if (!t) return;
    free_nodes(t, t->root);
    free(t->nil);
    free(t);
}

// --- ROTATIONS ---

static void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == t->nil) {
        t->root = y;
    } else if (x == x->p->left) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
}

static void right_rotate(rbtree *t, node_t *x) {
    node_t *y = x->left;
    x->left = y->right;
    if (y->right != t->nil) {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p == t->nil) {
        t->root = y;
    } else if (x == x->p->right) {
        x->p->right = y;
    } else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
}

// --- INSERTION & FIXUP ---

static void rbtree_insert_fixup(rbtree *t, node_t *z) {
    while (z->p->color == RBTREE_RED) {
        if (z->p == z->p->p->left) {
            node_t *y = z->p->p->right; // Uncle
            if (y->color == RBTREE_RED) { // Case 1: Uncle is RED
                z->p->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->p->p->color = RBTREE_RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) { // Case 2: z is right child
                    z = z->p;
                    left_rotate(t, z);
                }
                // Case 3: z is left child
                z->p->color = RBTREE_BLACK;
                z->p->p->color = RBTREE_RED;
                right_rotate(t, z->p->p);
            }
        } else {
            node_t *y = z->p->p->left; // Uncle
            if (y->color == RBTREE_RED) { // Case 1 (Mirror)
                z->p->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->p->p->color = RBTREE_RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) { // Case 2 (Mirror)
                    z = z->p;
                    right_rotate(t, z);
                }
                // Case 3 (Mirror)
                z->p->color = RBTREE_BLACK;
                z->p->p->color = RBTREE_RED;
                left_rotate(t, z->p->p);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *z = create_node(t, key);
    if (!z) return NULL;

    node_t *y = t->nil;
    node_t *x = t->root;

    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->p = y;
    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    rbtree_insert_fixup(t, z);
    return z;
}

// --- SEARCH ---

node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *curr = t->root;
    while (curr != t->nil) {
        if (key == curr->key) {
            return curr;
        } else if (key < curr->key) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return NULL; // Return NULL if not found
}

// --- DELETION HELPER & FIXUP ---

static void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->p == t->nil) {
        t->root = v;
    } else if (u == u->p->left) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }
    v->p = u->p;
}

static node_t *rbtree_minimum(rbtree *t, node_t *x) {
    while (x->left != t->nil) {
        x = x->left;
    }
    return x;
}

static void rbtree_delete_fixup(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        if (x == x->p->left) {
            node_t *w = x->p->right; // Sibling
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->p->color = RBTREE_RED;
                left_rotate(t, x->p);
                w = x->p->right;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->p;
            } else {
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotate(t, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotate(t, x->p);
                x = t->root;
            }
        } else {
            node_t *w = x->p->left; // Sibling
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->p->color = RBTREE_RED;
                right_rotate(t, x->p);
                w = x->p->left;
            }
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->p;
            } else {
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotate(t, x->p);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
    if (!t || !z || z == t->nil) return 0;

    node_t *y = z;
    node_t *x;
    color_t y_original_color = y->color;

    if (z->left == t->nil) {
        x = z->right;
        rbtree_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rbtree_transplant(t, z, z->left);
    } else {
        y = rbtree_minimum(t, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->p == z) {
            x->p = y;
        } else {
            rbtree_transplant(t, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rbtree_transplant(t, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if (y_original_color == RBTREE_BLACK) {
        rbtree_delete_fixup(t, x);
    }

    free(z);
    return 1;
}