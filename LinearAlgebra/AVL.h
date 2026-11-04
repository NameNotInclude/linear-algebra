#ifndef AVL_H
#define AVL_H
#include "ration_comput.h"

typedef struct n
{
    char name[6];
    int col;//列数
    int row;//行数
    ration** matrix;
    struct n* left;
    struct n* right;

    int height;
    int bf;
}node;

typedef node* PtrToNode;
typedef node* Tree;

void update(PtrToNode AVL);
PtrToNode RightRot(PtrToNode AVL);
PtrToNode LeftRot(PtrToNode AVL);
PtrToNode insert(PtrToNode AVL,PtrToNode new);
PtrToNode deleteNode(PtrToNode AVL,char* name);
PtrToNode find(PtrToNode AVL, char* name);
int collid(Tree Tree,PtrToNode newnode);
void inorder_traverse(PtrToNode AVL,void (*func)(PtrToNode));
void freeA(PtrToNode AVL);
#endif
