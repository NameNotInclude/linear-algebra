#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "ration_comput.h"
#define max 50

typedef struct _node
{
    char name[6];
    int col;//列数
    int row;//行数
    ration** matrix;
    struct _node* next;
}node;
typedef node* PtrToNode;
typedef node* List;

int gcd(int a,int b);
int collid(List list,PtrToNode newnode);
void insert(List list,PtrToNode lastnode,PtrToNode newnode);
ration** matrix_input(int col,int row);
ration det(ration** D,int n);//行列式函数
PtrToNode find(List l,char s[6]);//寻找某个节点
void matrixmultiply(ration** a,ration** b,ration** c,int a_row,int a_col,int b_row,int b_col);//矩阵乘法
int main()
{
    printf("输入指令\na表示添加矩阵\np表示打印现有矩阵\nd表示求矩阵行列式\nm表示计算矩阵乘法\nl表示打印所有矩阵\nr表示求矩阵的逆\nx表示删除矩阵\nt表示求矩阵的转置矩阵\n+表示求两个矩阵的加法\n-表示求两个矩阵的减法\n*表示求矩阵的数乘\nw表示求矩阵的几次幂\n");
    List list=(List)malloc(sizeof(node));
    PtrToNode lastnode=NULL;
    list->next=NULL;
    while (1)
    {
        char s;
        end:
        s=getchar();
        if (s=='a')
        {
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            printf("为矩阵命名,不超过五个字符:\n");
            scanf("%5s",newnode->name);

            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }
            
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
                printf("覆盖成功!\n");
                continue;
            }

            insert(list,lastnode,newnode);
            
            printf("创建完成!\n");
        }
        else if (s=='p')
        {
            char n[6];
            printf("输入需要打印的矩阵名称:\n");
            scanf("%5s",n);
            PtrToNode loc=find(list,n);
            if (loc==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            for (int i=0;i<loc->row;i++)
            {
                for (int j=0;j<loc->col;j++)
                {
                    rationprint(loc->matrix[i][j]);
                    printf(" ");
                }
                printf("\n");
            }
            printf("打印完成!\n");
        }
        else if (s=='d')
        {
            char n[6];
            printf("输入需要计算行列式的矩阵:\n");
            scanf("%5s",n);
            PtrToNode loc=find(list,n);
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
            scanf("%s",newnode->name);
            PtrToNode loc1=find(list,s1);
            PtrToNode loc2=find(list,s2);
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
            
            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }

            newnode->row=loc1->row;
            newnode->col=loc2->col;

            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->col);
            for (int i=0;i<newnode->col;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*newnode->col);

            matrixmultiply(loc1->matrix,loc2->matrix,temp,loc1->row,loc1->col,loc2->row,loc2->col);
            newnode->matrix=temp;
            
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='l')
        {
            PtrToNode check=list->next;
            while(check!=NULL)
            {
                printf("%s\n",check->name);
                for (int i=0;i<check->row;i++)
                {
                    for (int j=0;j<check->col;j++)
                    {
                        rationprint(check->matrix[i][j]);
                        printf(" ");
                    }
                    printf("\n");
                }
                printf("\n");
                check=check->next;
            }
            printf("打印完成\n");
        }
        else if (s=='r')
        {
            char n[6];
            printf("输入需要逆转的矩阵:\n");
            scanf("%5s",n);
            if (find(list,n)==NULL)
            {
                printf("矩阵不存在!\n");
                continue;
            }
            PtrToNode check=find(list,n);
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

            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }
            
            ration D=det(check->matrix,check->col);

            ration** temp=(ration**)malloc(sizeof(ration*)*(check->col));
            for (int i=0;i<check->col;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*check->col);

            ration** subtemp=(ration**)malloc(sizeof(ration*)*(check->col-1));
            for (int i=0;i<check->col;i++)
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
            for (int i=0;i<check->col;i++)
                free(subtemp[i]);
            free(subtemp);

            newnode->row=check->row;
            newnode->col=check->col;
            newnode->matrix=temp;

            if (flag)
            {
                printf("覆盖完成\n");
                continue;
            }
            printf("创建完成!\n");
            
            insert(list,lastnode,newnode);
        }
        else if (s=='t')
        {
            int new_col,new_row;
            printf("输入需要转置的矩阵名称:\n");
            char n[6];
            scanf("%5s",n);
            PtrToNode Node=find(list,n);
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
            
            int flag=collid(list,newnode);

            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }

            ration** temp=(ration**)malloc(sizeof(ration*)*Node->col);
            for (int i=0;i<Node->row;i++)
                temp[i]=(ration*)malloc(sizeof(ration)*Node->row);

            for (int i=0;i<Node->row;i++)
            {
                for (int j=0;j<Node->col;j++)
                {
                    temp[j][i]=Node->matrix[i][j];
                }
            }
            newnode->matrix=temp;

            if (flag)
            {
                newnode->row=new_row;
                newnode->col=new_col;
                printf("覆盖完成\n");
                continue;
            }

            newnode->row=Node->col;
            newnode->col=Node->row;
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='x')
        {
            printf("输入需要删除的矩阵名称:\n");
            char n[6];
            scanf("%s",n);
            PtrToNode Node=find(list,n);
            if (Node==NULL)
            {
                printf("该矩阵不存在!\n");
                continue;
            }    
            PtrToNode check=list;
            while (check->next!=Node)
            {
                check=check->next;
            }
            if (Node==lastnode)
            {
                lastnode=check;
            }
            check->next=Node->next;
            for (int i=0;i<Node->row;i++)
                free(Node->matrix[i]);
            free(Node->matrix);
            free(Node);
            printf("已删除矩阵%s\n",n);
            
        }
        else if (s=='+')
        {
            char s1[6],s2[6];
            printf("输入需要相加的矩阵:\n");
            scanf("%5s %5s",s1,s2);
            PtrToNode t1=find(list,s1);
            PtrToNode t2=find(list,s2);
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
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newnode->name);

            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }

            newnode->row=t1->row;
            newnode->col=t1->col;
            ration** temp=(ration**)malloc(sizeof(ration)*newnode->row);
            for (int i=0;i<newnode->row;i++)
                temp[i]=(ration*)malloc(sizeof(newnode->col));

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
                printf("覆盖成功\n");
                continue;
            }
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='-')
        {
            char s1[6],s2[6];
            printf("输入需要相减的矩阵:\n");
            scanf("%5s %5s",s1,s2);
            PtrToNode t1=find(list,s1);
            PtrToNode t2=find(list,s2);
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
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            scanf("%5s",newnode->name);
            
            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }

            newnode->row=t1->row;
            newnode->col=t1->col;
            ration** temp=(ration**)malloc(sizeof(ration*)*newnode->row);
            for (int i=0;i<newnode->col;i++)
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
                printf("覆盖成功\n");
                continue;
            }
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='w')
        {
            char s[6];
            printf("输入需要计算的矩阵:\n");
            scanf("%5s",s);
            PtrToNode t=find(list,s);
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
            
            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }

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
                printf("覆盖成功\n");
                continue;
            }
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='*')
        {
            char s[6];
            printf("输入需要数乘的矩阵:\n");
            scanf("%5s",s);
            PtrToNode t=find(list,s);
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
            
            int flag=collid(list,newnode);
            if (flag)
            {
                for (int i=0;i<newnode->row;i++)
                    free(newnode->matrix[i]);
                free(newnode->matrix);
            }
            
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
                printf("覆盖成功\n");
                continue;
            }
            
            insert(list,lastnode,newnode);

            printf("创建完成!\n");
        }
        else if (s=='0')
        {
            printf("程序结束");
            PtrToNode check=list;
            while (check)
            {
                PtrToNode tmp=check->next;
                for (int i=0;i<check->row;i++)
                    free(check->matrix[i]);
                free(check->matrix);
                free(check);
                check=tmp; 
            }
            break;
        }
        else 
        {
            continue;
        }
    }
    return 0;
}

int collid(List list,PtrToNode newnode)
{
    if (find(list,newnode->name)!=NULL)
    {
        int flag;
        printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
        scanf("%d",&flag);
        if (flag)
        {
            free(newnode);
            newnode=find(list,newnode->name);
            return 1;
        }
        else while (1)
        {
            printf("输入新名称:\n");
            char n[6];
            scanf("%5s",n);
            if (find(list,n)!=NULL)
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
void insert(List list,PtrToNode lastnode,PtrToNode newnode)
{
    if (list->next==NULL)
    {
        list->next=newnode;
        newnode->next=NULL;
        lastnode=newnode;
    }
    else
    {
        lastnode->next=newnode;
        newnode->next=NULL;
        lastnode=newnode;
    }
}
ration** matrix_input(int col,int row)
{
    ration** re=(ration**)malloc(sizeof(ration*)*row);
    for (int i=0;i<row;i++)
        re[i]=(ration*)malloc(sizeof(ration)*col);

    for (int i=0;i<row;i++)
    {
        for (int j=0;j<col;j++)
        {
            ration a;
            a=rationinput();
            if (a.up==0 && a.down==0)
            {
                printf("输入出错!\n");
                for (int i=0;i<row;i++)
                    free(re[i]);
                free(re);

                return NULL;
            }
            re[i][j]=a;
        }
    }

    return re;
}
ration det(ration** D,int n)
{
    ration zero;
    zero.up=0;
    zero.down=1;

    ration curr;
    curr.up=1;
    curr.down=1;

    ration** E=(ration**)malloc(sizeof(ration*)*n);
            for (int i=0;i<n;i++)
                E[i]=(ration*)malloc(sizeof(ration)*n);

    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
            E[i][j]=D[i][j];

    int num_swap=0;

    for (int i=0;i<n;i++)
    {
        int p=i;
        while (p<n && E[p][i].up==0)
            p++;

        if (p==n)
        {
            for (int i=0;i<n;i++)
                free(E[i]);
            free(E);
            return zero;
        }

        if (p!=i)
        {
            for (int l=0;l<n;l++)
                swap(&E[i][l],&E[p][l]);
            num_swap=!num_swap;
        }

        for (int l=i+1;l<n;l++)
        {
            ration fc=divide(E[l][i],E[i][i]);
            for (int d=i;d<n;d++)
            {
                E[l][d]=minus(E[l][d],multiply(E[i][d],fc));
            }
        }
    }

    for (int i=0;i<n;i++)
        curr=multiply(curr,E[i][i]);

    for (int i=0;i<n;i++)
        free(E[i]);
    free(E);

    ration f;
    f.down=1;
    f.up=(num_swap)?-1:1;

    return multiply(curr,f);
}
PtrToNode find(List l,char s[6])
{
    PtrToNode check=l->next;
    while (check!=NULL && strcmp(s,check->name)!=0)
    {
        check=check->next;
    }
    return check;
}
void matrixmultiply(ration** a,ration** b,ration** c,int a_row,int a_col,int b_row,int b_col)
{
    for (int i=0;i<a_row;i++)
    {
        for (int j=0;j<b_col;j++)
        {
            ration sum={0,1};
            for (int k=0;k<a_col;k++)
            {
                sum=plus(sum,multiply(a[i][k],b[k][j]));
                normalize(&sum);
            }
            c[i][j]=sum;
        }
    }
}