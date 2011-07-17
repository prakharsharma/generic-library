#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct avl_nodeData {
    void *data;
    void *key;
    int rc;
};
typedef struct avl_nodeData avlNodeData;

struct avl_node {
    avlNodeData *data;
    int height;/*height of the subtree rooted at thsi node*/
    struct avl_node *lChild;
    struct avl_node *rChild;
    struct avl_node *parent;
};
typedef struct avl_node avlNode;

/*available methods*/
avlNode* newAvlTree (avlNodeData *val);
avlNode* insert (avlNode **root, avlNodeData *val, int (*cmp)(void *key1, void *key2));
avlNode* locate (avlNode *root, avlNodeData *val, int (*cmp)(void *key1, void *key2));
void removeNode (avlNode **node, void (*freeNodedata)(avlNodeData **data));
avlNode* deleteNode (avlNode **root, avlNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(avlNodeData **data));

void inorder (avlNode *root, void (*display)(avlNodeData *nodeData));
void postorder (avlNode *root, void (*display)(avlNodeData *nodeData));
void preorder (avlNode *root, void (*display)(avlNodeData *nodeData));

