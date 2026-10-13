#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define max 100
typedef struct rational
{
    int up;
    int down;
}ration;
int gcd(int a,int b);
ration plus(ration a,ration b);//分数加法
ration minus(ration a,ration b);//分数减发
ration multiply(ration a,ration b);//分数乘法
ration divide(ration a,ration b);//分数除法
void normalize(ration *r);//约分函数
ration det(ration D[max][max],int n);//行列式函数
ration rationinput();//有理数输入
void rationprint(ration x);//有理数输出
int main()
{
    ration null={0,0};
    printf("求解线性方程组,请输入一个正整数n,表示方程个数与未知数个数(请确保方程有唯一解)\n");
    int n;
    scanf("%d",&n);
    printf("请输入%d个整数或分数(如5,3/4)，两个数字之间使用空格或者换行隔开,表示方程的系数矩阵\n",n*n);
    getchar();
    ration D[max][max];
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            D[i][j]=rationinput();
            if (D[i][j].up==null.up &&D[i][j].down==null.down)
            {
                return 0;
            }
        }
    }
    ration d=det(D,n);//系数矩阵行列式
    if (d.up==0)
    {
        printf("not sigle root");
        return 0;
    }
    printf("请输入%d个有理数,表示n个等式的值\n",n);
    ration B[max];
    for (int i=0;i<n;i++)
    {
        B[i]=rationinput();
        if (B[i].up==null.up && B[i].down==null.down)
        {
            return 0;
        }
    }
    printf("\n解为:\n");
    for (int k=0;k<n;k++)
    {
        ration sub[max][max];
        for (int i=0;i<n;i++)
        {
            for (int j=0;j<n;j++)
            {
                if (j==k)
                {
                    sub[i][j]=B[i];
                }
                else
                {
                    sub[i][j]=D[i][j];
                }
            }
        }
        ration sub_d=det(sub,n);
        ration result=divide(sub_d,d);
        normalize(&result);
        rationprint(result);
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
        printf("%d/%d\n",-x.up,-x.down);
        return;
    }
    if (x.down<0 && x.up>0)
    {
        printf("%d/%d\n",-x.up,-x.down);
        return;
    }
    if (x.up==0)
    {
        printf("0\n");
        return;
    }
    if (x.down==1)
    {
        printf("%d\n",x.up);
        return;
    }
    if (x.down==-1)
    {
        printf("%d\n",-x.up);
    }
    printf("%d/%d\n",x.up,x.down);
}