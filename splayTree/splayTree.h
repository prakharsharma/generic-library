#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct splay_nodeData {
    void *data;
    void *key;
    int rc;
};
typedef struct splay_nodeData splayNodeData;

struct splay_node {
    splayNodeData *data;
    struct splay_node *lChild;
    struct splay_node *rChild;
    struct splay_node *parent;
};
typedef struct splay_node splayNode;

splayNode* newSplayTree (splayNodeData *val);
splayNode* insert (splayNode **root, splayNodeData *val, int (*cmp)(void *key1, void *key2));
splayNode* locate (splayNode *node, splayNodeData *val, int (*cmp)(void *key1, void *key2));
splayNode* deleteNode (splayNode **root, splayNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(splayNodeData **data));
void inorder (splayNode *root, void (*display)(splayNodeData *nodeData));
void postorder (splayNode *root, void (*display)(splayNodeData *nodeData));
void preorder (splayNode *root, void (*display)(splayNodeData *nodeData));

