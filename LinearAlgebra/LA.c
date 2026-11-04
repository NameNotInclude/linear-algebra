#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ration_comput.h"
#include "matrix_operat.h"
#include "AVL.h"

static void overwrite_node(Tree tree, PtrToNode newnode)
{
    PtrToNode old = find(tree, newnode->name);
    if (old == NULL)
        return;

    for (int i = 0; i < old->row; i++)
        free(old->matrix[i]);
    free(old->matrix);

    old->row = newnode->row;
    old->col = newnode->col;
    old->matrix = newnode->matrix;

    free(newnode);
}

int main()
{
    printf("输入指令\na表示添加矩阵\np表示打印现有矩阵\nd表示求矩阵行列式\nm表示计算矩阵乘法\nl表示打印所有矩阵\nr表示求矩阵的逆\nx表示删除矩阵\nt表示求矩阵的转置矩阵\n+表示求两个矩阵的加法\n-表示求两个矩阵的减法\n*表示求矩阵的数乘\nw表示求矩阵的几次幂\n");
    Tree AVL=NULL;
    while (1)
    {
        char s;
        end:
        s=getchar();
        if (s=='a')
        {
            char name[6];
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            printf("为矩阵命名,不超过五个字符:\n");
            scanf("%5s",name);
            strcpy(newnode->name,name);

            int flag=collid(AVL,newnode);
            
            printf("输入矩阵的行数与列数,按照row x col的格式:\n");
            scanf("%dx%d",&newnode->row,&newnode->col);
            getchar();
            printf("输入%d个有理数(形如5、3/4),两个有理数之间用空格或换行隔开:\n",newnode->col*newnode->row);

            ration** re=matrix_input(newnode->col,newnode->row);

            if (re==NULL) 
            {
                free(newnode);
                continue;
            }
            else newnode->matrix=re;
            
            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功!\n");
                continue;
            }

            AVL=insert(AVL,newnode);
            
            printf("创建完成!\n");
        }
        else if (s=='p')
        {
            char n[6];
            printf("输入需要打印的矩阵名称:\n");
            scanf("%5s",n);
            PtrToNode loc=find(AVL,n);
            if (loc==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            matrix_print(loc);
            printf("打印完成!\n");
        }
        else if (s=='d')
        {
            char n[6];
            printf("输入需要计算行列式的矩阵:\n");
            scanf("%5s",n);
            PtrToNode loc=find(AVL,n);
            if (loc==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            if (loc->col!=loc->row)
            {
                printf("行列式不成立!\n");
                continue;
            }
            ration result=det(loc->matrix,loc->col);
            rationprint(result);
            printf("\n计算完成!\n");
        }
        else if (s=='m')
        {
            char s1[6],s2[6];
            printf("输入需要计算乘法的两个矩阵名称,以及一个新的矩阵,用于储存结果:\n");
            scanf("%5s %5s",s1,s2);
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newnode->name);
            PtrToNode loc1=find(AVL,s1);
            PtrToNode loc2=find(AVL,s2);
            if (loc1==NULL || loc2==NULL)
            {
                printf("矩阵不存在!\n");
                free(newnode);
                continue;
            }
            if (loc1->col!=loc2->row)
            {
                printf("乘法不成立!\n");
                free(newnode);
                continue;
            }
            
            int flag=collid(AVL,newnode);

            newnode->row=loc1->row;
            newnode->col=loc2->col;

            ration** temp=(ration**)malloc(sizeof(ration*)*loc1->row);
            for (int i=0;i<loc1->row;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*loc2->col);

            matrixmultiply(loc1->matrix,loc2->matrix,temp,loc1->row,loc1->col,loc2->row,loc2->col);
            newnode->matrix=temp;
            
            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功\n");
                continue;
            }
            
            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='l')
        {
            inorder_traverse(AVL,matrix_print);
            
            printf("打印完成\n");
        }
        else if (s=='r')
        {
            char n[6];
            printf("输入需要逆转的矩阵:\n");
            scanf("%5s",n);
            PtrToNode check=find(AVL,n);
            if (check==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            
            if (check->col!=check->row)
            {
                printf("%s非方阵\n",n);
                continue;
            }
            if (det(check->matrix,check->col).up==0)
            {
                printf("%s不可逆\n",n);
                continue;
            }
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            char newname[6];
            printf("输入一个新的矩阵,用于存放逆矩阵:\n");
            scanf("%5s",newname);
            strcpy(newnode->name,newname);

            int flag=collid(AVL,newnode);
            
            ration D=det(check->matrix,check->col);

            ration** temp=(ration**)malloc(sizeof(ration*)*(check->col));
            for (int i=0;i<check->col;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*check->col);

            ration** subtemp=(ration**)malloc(sizeof(ration*)*(check->col-1));
            for (int i=0;i<check->col-1;i++)
                subtemp[i]=(ration*)malloc(sizeof(ration)*(check->col-1));
            for (int i=0;i<check->col;i++)
            {
                for (int j=0;j<check->col;j++)
                {
                    int k1=0,k2=0;
                    int s1=0,s2=0;
                    while (k1<check->col)
                    {
                        if (k1!=i)
                        {   
                            while (k2<check->col)
                            {
                                if (k2!=j)
                                {
                                    subtemp[s1][s2]=check->matrix[k1][k2];
                                    s2++;
                                }
                                k2++;
                            }
                            s1++;
                            s2=0;
                        }
                        k1++;
                        k2=0;
                    }
                    ration result=det(subtemp,check->col-1);
                    result=divide(result,D);
                    temp[j][i]=(i+j)%2==1?(multiply(result,neg)):result;
                }
            }
            for (int i=0;i<check->col-1;i++)
                free(subtemp[i]);
            free(subtemp);

            newnode->row=check->row;
            newnode->col=check->col;
            newnode->matrix=temp;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖完成\n");
                continue;
            }
            printf("创建完成!\n");
            
            AVL=insert(AVL,newnode);
        }
        else if (s=='t')
        {
            printf("输入需要转置的矩阵名称:\n");
            char n[6];
            scanf("%5s",n);
            PtrToNode Node=find(AVL,n);
            if (Node==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            char newname[6];
            printf("输入一个新的矩阵,用于存放转置的矩阵:\n");
            scanf("%5s",newname);
            strcpy(newnode->name,newname);
            
            int flag=collid(AVL,newnode);

            ration** temp=(ration**)malloc(sizeof(ration*)*Node->col);
            for (int i=0;i<Node->col;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*Node->row);

            for (int i=0;i<Node->row;i++)
            {
                for (int j=0;j<Node->col;j++)
                {
                    temp[j][i]=Node->matrix[i][j];
                }
            }
            newnode->matrix=temp;
            newnode->row=Node->col;
            newnode->col=Node->row;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖完成\n");
                continue;
            }

            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='x')
        {
            printf("输入需要删除的矩阵名称:\n");
            char n[6];
            scanf("%s",n);
            PtrToNode Node=find(AVL,n);
            if (Node==NULL)
            {
                printf("该矩阵不存在!\n");
                continue;
            }    
            
            AVL=deleteNode(AVL,n);
            printf("已删除矩阵%s\n",n);
        }
        else if (s=='+')
        {
            char s1[6],s2[6];
            printf("输入需要相加的矩阵:\n");
            scanf("%5s %5s",s1,s2);
            PtrToNode t1=find(AVL,s1);
            PtrToNode t2=find(AVL,s2);
            if (t1==NULL || t2==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            if (t1->row !=t2->row || t1->col!=t2->col)
            {
                printf("不可相加!\n");
            }
            printf("输入新的矩阵名称:\n");
            char newname[6];
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newname);
            strcpy(newnode->name,newname);

            int flag=collid(AVL,newnode);

            newnode->row=t1->row;
            newnode->col=t1->col;
            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int i=0;i<newnode->row;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*newnode->col);

            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    temp[i][j]=plus(t1->matrix[i][j],t2->matrix[i][j]);
                }
            }
            
            newnode->matrix=temp;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功\n");
                continue;
            }
            
            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='-')
        {
            char s1[6],s2[6];
            printf("输入需要相减的矩阵:\n");
            scanf("%5s %5s",s1,s2);
            PtrToNode t1=find(AVL,s1);
            PtrToNode t2=find(AVL,s2);
            if (t1==NULL || t2==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            if (t1->row !=t2->row || t1->col!=t2->col)
            {
                printf("不可相减!\n");
            }
            printf("输入新的矩阵名称:\n");
            char newname[6];
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newname);
            strcpy(newnode->name,newname);

            int flag=collid(AVL,newnode);

            newnode->row=t1->row;
            newnode->col=t1->col;
            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int i=0;i<newnode->row;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*newnode->col);

            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    temp[i][j]=minus(t1->matrix[i][j],t2->matrix[i][j]);
                }
            }
            newnode->matrix=temp;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功\n");
                continue;
            }
            
            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='w')
        {
            char s[6];
            printf("输入需要计算的矩阵:\n");
            scanf("%5s",s);
            PtrToNode t=find(AVL,s);
            if (t==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            if (t->row !=t->col)
            {
                printf("不可相乘!\n");
            }
            int x;
            printf("输入次数\n");
            scanf("%d",&x);
            printf("输入新的矩阵名称:\n");
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newnode->name);
            
            int flag=collid(AVL,newnode);

            newnode->row=t->row;
            newnode->col=t->col;
            ration** E=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int i=0;i<newnode->row;i++)
                E[i]=(ration*)malloc(sizeof(ration)*newnode->col);

            for (int i=0;i<newnode->row;i++)
            {
                for (int j=0;j<newnode->col;j++)
                {
                    E[i][j].down=1;
                    E[i][j].up=(i==j)?1:0;
                }
            }

            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int k=0;k<newnode->row;k++)
                temp[k]=(ration*)malloc(sizeof(ration)*newnode->col);

            for (int i=0;i<x;i++)
            {             
                matrixmultiply(E,t->matrix,temp,newnode->row,newnode->row,newnode->row,newnode->row);
                
                ration** t=E;
                E=temp;
                temp=t;
            }

            for (int i=0;i<newnode->row;i++)
                free(temp[i]);
            free(temp);
            
            newnode->matrix=E;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功\n");
                continue;
            }
            
            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='*')
        {
            char s[6];
            printf("输入需要数乘的矩阵:\n");
            scanf("%5s",s);
            PtrToNode t=find(AVL,s);
            if (t==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            printf("输入需要数乘的数字:\n");
            getchar();
            ration x;
            x=rationinput();
            if (x.up==0 && x.down==0)
            {
                printf("输入错误\n");
                continue;
            }
            printf("输入新的矩阵名称:\n");
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newnode->name);
            
            int flag=collid(AVL,newnode);
            
            newnode->row=t->row;
            newnode->col=t->col;
            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int i=0;i<newnode->row;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*newnode->col);

            for (int i=0;i<t->row;i++)
            {
                for (int j=0;j<t->col;j++)
                {
                    temp[i][j]=multiply(t->matrix[i][j],x);
                }
            }
            
            newnode->matrix=temp;

            if (flag)
            {
                overwrite_node(AVL,newnode);
                printf("覆盖成功\n");
                continue;
            }
            
            AVL=insert(AVL,newnode);

            printf("创建完成!\n");
        }
        else if (s=='0')
        {
            printf("程序结束");
            freeA(AVL);
            break;
        }
        else 
        {
            continue;
        }
    }
    return 0;
}
