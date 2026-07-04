#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define max 50
typedef struct
{
    int up;
    int down;
}ration;
ration neg={-1,1};
typedef struct _node
{
    char name[6];
    int col;//列数
    int row;//行数
    ration matrix[max][max];
    struct _node* next;
}node;
typedef node* PtrToNode;
typedef node* List;
int gcd(int a,int b);
ration plus(ration a,ration b);//分数加法
ration minus(ration a,ration b);//分数减法
ration multiply(ration a,ration b);//分数乘法
ration divide(ration a,ration b);//分数除法
void normalize(ration *r);//约分函数
ration det(ration D[max][max],int n);//行列式函数
ration rationinput();//有理数输入
void rationprint(ration x);//有理数输出
PtrToNode find(List l,char s[6]);//寻找某个节点
void matrixmultiply(ration a[max][max],ration b[max][max],ration c[max][max],
    int a_row,int a_col,int b_row,int b_col);//矩阵乘法
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
            int flag=0;
            PtrToNode newnode=(PtrToNode)malloc(sizeof(node));
            printf("为矩阵命名,不超过五个字符:\n");
            scanf("%5s",newnode->name);
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
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
            printf("输入矩阵的行数与列数,按照row x col的格式:\n");
            scanf("%dx%d",&newnode->row,&newnode->col);
            getchar();
            printf("输入%d个有理数(形如5、3/4),两个有理数之间用空格或换行隔开:\n",newnode->col*newnode->row);
            for (int i=0;i<newnode->row;i++)
            {
                for (int j=0;j<newnode->col;j++)
                {
                    ration a;
                    a=rationinput();
                    if (a.up==0 && a.down==0)
                    {
                        printf("输入出错!\n");
                        free(newnode);
                        goto end;
                    }
                    newnode->matrix[i][j]=a;
                }
            }
            if (flag)
            {
                printf("覆盖成功!\n");
                continue;
            }
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
            int flag=0;
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
                continue;
            }
            if (loc1->col!=loc2->row)
            {
                printf("乘法不成立!\n");
                free(newnode);
                continue;
            }
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%s",n);
                    if (find(list,n)!=NULL)
                    {
                        printf("新名称不能和旧名称相同\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            newnode->row=loc1->row;
            newnode->col=loc2->col;
            ration temp[max][max];
            matrixmultiply(loc1->matrix,loc2->matrix,temp,loc1->row,loc1->col,loc2->row,loc2->col);
            for(int i=0;i<loc1->row;i++)
            {
                for (int j=0;j<loc2->col;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
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
            int flag=0;
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
            if (find(list,newname)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newname);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%5s",n);
                    if (strcmp(n,newname)==0)
                    {
                        printf("新名称不能和旧名称相同\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            ration D=det(check->matrix,check->col);
            ration temp[max][max];
            for (int i=0;i<check->col;i++)
            {
                for (int j=0;j<check->col;j++)
                {
                    ration subtemp[max][max];
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
            newnode->row=check->row;
            newnode->col=check->col;
            for (int i=0;i<check->col;i++)
            {
                for (int j=0;j<check->col;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖完成\n");
                continue;
            }
            printf("创建完成!\n");
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
        else if (s=='t')
        {
            int flag=0;
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
            if (find(list,newname)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newname);
                    new_col=newnode->row;
                    new_row=newnode->col;
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%5s",n);
                    if (strcmp(n,newname)==0)
                    {
                        printf("新名称不能和旧名称相同\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            ration temp[max][max];
            for (int i=0;i<Node->row;i++)
            {
                for (int j=0;j<Node->col;j++)
                {
                    temp[j][i]=Node->matrix[i][j];
                }
            }
            for (int i=0;i<Node->col;i++)
            {
                for (int j=0;j<Node->row;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                newnode->row=new_row;
                newnode->col=new_col;
                printf("覆盖完成\n");
                continue;
            }
            newnode->row=Node->col;
            newnode->col=Node->row;
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
            free(Node);
            printf("已删除矩阵%s\n",n);
            
        }
        else if (s=='+')
        {
            int flag=0;
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
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%s",n);
                    if (find(list,n)!=NULL)
                    {
                        printf("该名称已存在\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            newnode->row=t1->row;
            newnode->col=t1->col;
            ration temp[max][max];
            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    temp[i][j]=plus(t1->matrix[i][j],t2->matrix[i][j]);
                }
            }
            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
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
            printf("创建完成!\n");
        }
        else if (s=='-')
        {
            int flag=0;
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
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%s",n);
                    if (find(list,n)!=NULL)
                    {
                        printf("该名称已存在\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            newnode->row=t1->row;
            newnode->col=t1->col;
            ration temp[max][max];
            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    temp[i][j]=minus(t1->matrix[i][j],t2->matrix[i][j]);
                }
            }
            for (int i=0;i<t1->row;i++)
            {
                for (int j=0;j<t2->col;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
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
            printf("创建完成!\n");
        }
        else if (s=='w')
        {
            int flag=0;
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
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%s",n);
                    if (find(list,n)!=NULL)
                    {
                        printf("该名称已存在\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            newnode->row=t->row;
            newnode->col=t->col;
            ration E[max][max];
            for (int i=0;i<newnode->row;i++)
            {
                for (int j=0;j<newnode->col;j++)
                {
                    E[i][j].down=1;
                    E[i][j].up=(i==j)?1:0;
                }
            }
            for (int i=0;i<x;i++)
            {
                ration temp[max][max];
                matrixmultiply(E,t->matrix,temp,newnode->row,newnode->row,newnode->row,newnode->row);
                for (int j=0;j<newnode->row;j++)
                {
                    for (int k=0;k<newnode->col;k++)
                    {
                        E[j][k]=temp[j][k];
                    }
                }
            }
            for (int i=0;i<newnode->col;i++)
            {
                for (int j=0;j<newnode->col;j++)
                {
                    newnode->matrix[i][j]=E[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
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
            printf("创建完成!\n");
        }
        else if (s=='*')
        {
            int flag=0;
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
            if (find(list,newnode->name)!=NULL)
            {
                printf("此名称已存在,是否覆盖?覆盖请输入1,否则输入0:\n");
                scanf("%d",&flag);
                if (flag)
                {
                    newnode=find(list,newnode->name);
                }
                else while (1)
                {
                    printf("输入新名称:\n");
                    char n[6];
                    scanf("%s",n);
                    if (find(list,n)!=NULL)
                    {
                        printf("该名称已存在\n");
                        continue;
                    }
                    strcpy(newnode->name,n);
                    break;
                }
            }
            newnode->row=t->row;
            newnode->col=t->col;
            ration temp[max][max];
            for (int i=0;i<t->row;i++)
            {
                for (int j=0;j<t->col;j++)
                {
                    temp[i][j]=multiply(t->matrix[i][j],x);
                }
            }
            for (int i=0;i<t->row;i++)
            {
                for (int j=0;j<t->col;j++)
                {
                    newnode->matrix[i][j]=temp[i][j];
                }
            }
            if (flag)
            {
                printf("覆盖成功\n");
                continue;
            }
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
            printf("创建完成!\n");
        }
        else if (s=='0')
        {
            printf("程序结束");
            PtrToNode check=list;
            while (check)
            {
                PtrToNode tmp=check->next;
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
int gcd(int a,int b)
{
    a=abs(a);
    b=abs(b);
    while (a!=0)
    {
        int temp=a;
        a=b%a;
        b=temp;
    }
    return b;
}
ration plus (ration a,ration b)
{
    ration m;
    m.down=(a.down/gcd(a.down,b.down))*b.down;
    m.up=a.up*m.down/a.down+b.up*m.down/b.down;
    int d=gcd(m.up,m.down);
    m.up/=d;
    m.down/=d;
    if (m.up==0)
        m.down=1;
    return m;
}
ration minus(ration a,ration b)
{
    ration m;
    m.down=(a.down/gcd(a.down,b.down))*b.down;
    m.up=a.up*m.down/a.down-b.up*m.down/b.down;
    int d=gcd(m.up,m.down);
    m.up/=d;
    m.down/=d;
    if (m.up==0)
        m.down=1;
    return m;
}
ration multiply(ration a,ration b)
{
    ration m;
    m.up=a.up*b.up;
    m.down=a.down*b.down;
    int d=gcd(m.up,m.down);
    m.up/=d;
    m.down/=d;
    if (m.up==0)
        m.down=1;
    return m;
}
ration divide(ration a,ration b)
{
    ration m;
    m.up=a.up*b.down;
    m.down=a.down*b.up;
    int d=gcd(m.up,m.down);
    m.up/=d;
    m.down/=d;
    if (m.up==0)
        m.down=1;
    return m;
}
void normalize(ration *r) 
{
    int d = gcd(r->up, r->down);
    r->up /= d;
    r->down /= d;
    if (r->up == 0)
    {
        r->down = 1;
    }
}
ration det(ration D[max][max],int n)
{
    if (n==1)
    {
        normalize(&D[0][0]);
        return D[0][0];
    }
    else
    {
        int i;
        for (i=0;i<n && D[0][i].up==0;i++);
        if (i==n)
        {
            ration answer={0,1};
            return answer;
        }
        else
        {
            int p=i;
            ration temp[max][max];
            for (int j=0;j<n;j++)
            {
                for (int k=0;k<n;k++)
                {
                    temp[j][k]=D[j][k];
                    normalize(&temp[j][k]); 
                }
            }
            int row = 1;
            while (row < n)
            {
                if (temp[row][p].up != 0) 
                {
                    ration div = divide(temp[row][p], temp[0][p]);
                    normalize(&div);
                    for (int col=0; col<n; col++) 
                    {
                        ration mul = multiply(temp[0][col], div);
                        normalize(&mul);
                        temp[row][col] = minus(temp[row][col], mul);
                        normalize(&temp[row][col]);
                    }
                }
                row++;
            }
            ration arr[max][max];
            for (int s=1;s<n;s++)
            {
                for (int j=0;j<p;j++)
                {
                    normalize(&temp[s][j]);
                    arr[s-1][j]=temp[s][j];
                }
                for (int j=p+1;j<n;j++)
                {
                    normalize(&temp[s][j]);
                    arr[s-1][j-1]=temp[s][j];
                }
            }
            if (p%2 !=0)
            {
                temp[0][p].up=-temp[0][p].up;
            }
            normalize(&temp[0][p]);
            ration sub_det = det(arr, n-1);
            normalize(&sub_det);
            return multiply(sub_det, temp[0][p]);
        }
    }
}
ration rationinput()
{
    ration null={0,0};
    ration x;
    int positive=1;
    int first=1;
    x.up=0,x.down=1;
    while (1)
    {
        char c=getchar();
       //判断符号
        if (c=='-' && first)
        {
            positive=0;
            first=0;
        }
        else if (c=='-' && !first)
        {
            printf("Input error");
            return null;
        }
        //输入数字字符，存储到分子
        else if (c>='0' && c<='9')
        {
            if (positive)
            {
            x.up=x.up*10+(c-'0');
            }
            else
            {
                x.up=x.up*10-(c-'0');
            }
        }
        //输入分数线，开始处理分母
        else if (c=='/')
        {
            x.down=0;
            int positive1=1;
            int first1=1;
            while(1)
            {
                char d=getchar();
                //输入数字字符，储存到分母
                if (d=='-' &&  first1)
                {
                    positive1=0;
                    first1=0;
                }
                else if (d=='-' && !first1)
                {
                    printf("Input error");
                    return null;
                }
                else if (d>='0' && d<='9')
                {
                    if (positive1)
                    {
                        x.down=x.down*10+(d-'0');
                    }
                    else
                    {
                        x.down=x.down*10-(d-'0');
                    }
                }
                else if (d==' ' || d=='\n')
                {
                    if (x.down==0)
                    {
                        printf("Divide 0 error");
                        return null;
                    }
                    goto end;
                }
                else
                {
                    printf("Input error");
                    return null;
                }
            }
        }
         //输入' '或'\n'，表示转换到下一数字
        else if (c==' ' || c=='\n')
        {
            end:
            break;
        }
        else
        {
            printf("Input error");
            return null;
        }
    }
    if (x.up<0 && x.down<0)
    {
        x.up=-x.up;
        x.down=-x.down;
    }
    normalize(&x);
    return x;
}
void rationprint(ration x)
{
    if (x.up<0 && x.down<0)
    {
        printf("%d/%d",-x.up,-x.down);
        return;
    }
    if (x.down<0 && x.up>0)
    {
        printf("%d/%d",-x.up,-x.down);
        return;
    }
    if (x.up==0)
    {
        printf("0");
        return;
    }
    if (x.down==1)
    {
        printf("%d",x.up);
        return;
    }
    if (x.down==-1)
    {
        printf("%d",-x.up);
    }
    printf("%d/%d",x.up,x.down);
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
void matrixmultiply(ration a[max][max],ration b[max][max],ration c[max][max],int a_row,int a_col,int b_row,int b_col)
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