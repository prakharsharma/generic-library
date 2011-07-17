#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum color {NA, RED, BLACK};
typedef enum color nodeClr;

struct rb_nodeData {
    void *data;
    void *key;
    int rc;
};
typedef struct rb_nodeData rbNodeData;

struct rb_node {
    rbNodeData *data;
    nodeClr clr;
    struct rb_node *lChild;
    struct rb_node *rChild;
    struct rb_node *parent;
};
typedef struct rb_node rbNode;

rbNode* newRbTree (rbNodeData *val);
void insert (rbNode **root, rbNodeData *val, int (*cmp)(void *key1, void *key2));
rbNode* search (rbNode *root, rbNodeData *val, int (*cmp)(void *key1, void *key2));
void deleteNode (rbNode **root, rbNodeData *val, int (*cmp)(void *key1, void *key2), void (*freeNodedata)(rbNodeData **data));
void inorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr));
void postorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr));
void preorder (rbNode *root, void (*display)(rbNodeData *nodeData, nodeClr clr));

