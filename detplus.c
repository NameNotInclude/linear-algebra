#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
int main()
{
    printf("行列式计算器，请输入一个正整数\n");
    int n;
    scanf("%d",&n);
    printf("请输入%d个整数或分数(形如a/b)，两个数字之间使用空格或者换行隔开\n",n*n);
    getchar();
    ration D[max][max];
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            int positive=1;
            int first=1;
            D[i][j].up=0,D[i][j].down=1;
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
                    return 0;
                }
                //输入数字字符，存储到分子
                else if (c>='0' && c<='9')
                {
                    if (positive)
                    {
                        D[i][j].up=D[i][j].up*10+(c-'0');
                    }
                    else
                    {
                        D[i][j].up=D[i][j].up*10-(c-'0');
                    }
                }
                //输入分数线，开始处理分母
                else if (c=='/')
                {
                    D[i][j].down=0;
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
                            return 0;
                        }
                        else if (d>='0' && d<='9')
                        {
                            if (positive1)
                            {
                                D[i][j].down=D[i][j].down*10+(d-'0');
                            }
                            else
                            {
                                D[i][j].down=D[i][j].down*10-(d-'0');
                            }
                        }
                        else if (d==' ' || d=='\n')
                        {
                            if (D[i][j].down==0)
                            {
                                printf("Divide 0 error");
                                return 0;
                            }
                            goto end;
                        }
                        else
                        {
                            printf("Input error");
                            return 0;
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
                    return 0;
                }
            }
            if (D[i][j].up<0 && D[i][j].down<0)
            {
                D[i][j].up=-D[i][j].up;
                D[i][j].down=-D[i][j].down;
            }
            normalize(&D[i][j]);
        }
    }
    printf("\n分数矩阵：\n");
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            printf("%d/%d ",D[i][j].up,D[i][j].down);
        }
        printf("\n");
    }
    printf("\n");

    printf("\n结果:\n");
    ration answer=det(D,n);
    if (answer.down==1)
    {
        printf("%d",answer.up);
    }
    else if (answer.down==-1)
    {
        printf("%d",-answer.up);
    }
    else if (answer.up==0)
    {
        printf("0");
    }
    else
    {
        printf("%d/%d",answer.up,answer.down);
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
