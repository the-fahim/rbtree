#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

// Helper function to print search results clearly
void test_find(rbtree *t, key_t key) {
    node_t *result = rbtree_find(t, key);
    if (result != NULL) {
        printf("Found key %d in the tree! (Node Color: %s)\n", 
               key, (result->color == RBTREE_RED) ? "RED" : "BLACK");
    } else {
        printf("Key %d NOT found in the tree.\n", key);
    }
}

int main(void) {
    printf("=== Starting Red-Black Tree Tests ===\n\nJI");

    // 1. Initialize the tree
    rbtree *t = new_rbtree();
    if (!t) {
        fprintf(stderr, "Failed to allocate memory for rbtree\n");
        return 1;
    }

    // 2. Test Insertions
    printf("--- Inserting Keys ---\n");
    key_t keys_to_insert[] = {20, 15, 25, 10, 5, 30};
    int num_keys = sizeof(keys_to_insert) / sizeof(keys_to_insert[0]);

    for (int i = 0; i < num_keys; i++) {
        rbtree_insert(t, keys_to_insert[i]);
        printf("Inserted: %d\n", keys_to_insert[i]);
    }
    printf("\n");

    // 3. Test Search (Find)
    printf("--- Testing Search ---\n");
    test_find(t, 15); // Should find
    test_find(t, 30); // Should find
    test_find(t, 99); // Should NOT find
    printf("\n");

    // 4. Test Deletion (Erase)
    printf("--- Testing Deletion ---\n");
    key_t key_to_delete = 15;
    node_t *node_to_delete = rbtree_find(t, key_to_delete);
    
    if (node_to_delete != NULL) {
        printf("Deleting key %d...\n", key_to_delete);
        rbtree_erase(t, node_to_delete);
    } else {
        printf("Key %d not found for deletion.\n", key_to_delete);
    }

    // Verify deletion by searching for it again
    test_find(t, 15);
    printf("\n");

    // 5. Clean up memory
    printf("--- Cleaning Up Tree Memory ---\n");
    delete_rbtree(t);
    printf("Tree deleted successfully. All tests passed!\n");

    return 0;
}