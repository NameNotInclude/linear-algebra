#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AVL.h"

void update(PtrToNode AVL)
{
    if (AVL == NULL)
        return;

    int h_l = AVL->left == NULL ? -1 : AVL->left->height;
    int h_r = AVL->right == NULL ? -1 : AVL->right->height;

    AVL->height = h_l > h_r ? h_l + 1 : h_r + 1;
    AVL->bf = h_l - h_r;
}

PtrToNode RightRot(PtrToNode AVL)
{
    PtrToNode L = AVL->left;

    AVL->left = L->right;
    L->right = AVL;

    update(AVL);
    update(L);

    return L;
}

PtrToNode LeftRot(PtrToNode AVL)
{
    PtrToNode R = AVL->right;

    AVL->right = R->left;
    R->left = AVL;

    update(AVL);
    update(R);

    return R;
}

PtrToNode insert(PtrToNode AVL,PtrToNode new)
{
    if (AVL==NULL)
    {
        new->left = NULL;
        new->right = NULL;
        new->height = 0;
        new->bf = 0;
        return new;
    }

    int cmp = strcmp(new->name, AVL->name);

    if (cmp < 0)
        AVL->left = insert(AVL->left, new);
    else if (cmp > 0)
        AVL->right = insert(AVL->right, new);
    else
        return AVL;

    update(AVL);

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRot(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRot(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRot(AVL->left);
        return RightRot(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRot(AVL->right);
        return LeftRot(AVL);
    }

    return AVL;
}

static PtrToNode FindMin(PtrToNode AVL)
{
    PtrToNode curr=AVL;

    while (curr != NULL && curr->left != NULL)
        curr = curr->left;

    return curr;
}

PtrToNode deleteNode(PtrToNode AVL,char* name)
{
    if (AVL == NULL)
        return NULL;

    int cmp = strcmp(AVL->name, name);

    if (cmp < 0)
        AVL->right = deleteNode(AVL->right, name);
    else if (cmp > 0)
        AVL->left = deleteNode(AVL->left, name);
    else
    {
        if (AVL->left == NULL || AVL->right == NULL)
        {
            PtrToNode temp = AVL->left ? AVL->left : AVL->right;
            if (AVL->matrix!=NULL) 
            {
                for (int i=0;i<AVL->row;i++)
                    free(AVL->matrix[i]);
                free(AVL->matrix);
            }
            free(AVL);
            return temp;
        }

        PtrToNode temp = FindMin(AVL->right);
        strcpy(AVL->name,temp->name);
        AVL->col = temp->col;
        AVL->row = temp->row;
        AVL->matrix = temp->matrix;
        temp->matrix = NULL;
        AVL->right = deleteNode(AVL->right, temp->name);
    }

    update(AVL);

    if (AVL == NULL)
        return NULL;

    // LL
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf >= 0)
        return RightRot(AVL);

    // RR
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf <= 0)
        return LeftRot(AVL);

    // LR
    if (AVL->bf > 1 && AVL->left != NULL && AVL->left->bf < 0)
    {
        AVL->left = LeftRot(AVL->left);
        return RightRot(AVL);
    }

    // RL
    if (AVL->bf < -1 && AVL->right != NULL && AVL->right->bf > 0)
    {
        AVL->right = RightRot(AVL->right);
        return LeftRot(AVL);
    }

    return AVL;
}

PtrToNode find(PtrToNode AVL, char* name)
{
    if (AVL==NULL)
        return NULL;

    int cmp = strcmp(AVL->name, name);

    if (cmp == 0)
        return AVL;
    else if (cmp > 0)
        return find(AVL->left,name);
    else
        return find(AVL->right,name);
    
}

int collid(Tree Tree,PtrToNode newnode)
{
    PtrToNode curr=find(Tree,newnode->name);
    if (curr!=NULL)
    {
        int flag;
        printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
        scanf("%d",&flag);
        if (flag)
            return 1;

        else while (1)
        {
            printf("输入新名称:\n");
            char n[6];
            scanf("%5s",n);
            if (find(Tree,n)!=NULL)
            {
                printf("该名称已存在\n");
                continue;
            }
            strcpy(newnode->name,n);
            break;
        }
    }
    return 0;
}

void inorder_traverse(PtrToNode AVL,void (*func)(PtrToNode))
{
    if (AVL==NULL)
        return ;
    
    inorder_traverse(AVL->left,func);
    func(AVL);
    inorder_traverse(AVL->right,func);
}

void freeA(PtrToNode AVL)
{
    if (AVL==NULL)
        return;

    freeA(AVL->left);
    freeA(AVL->right);
    for (int i=0;i<AVL->row;i++)
        free(AVL->matrix[i]);
    free(AVL->matrix);
    free(AVL);
}