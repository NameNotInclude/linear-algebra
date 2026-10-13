#include "ration_comput.h"
#include <stdlib.h>
#include <stdio.h>

ration neg={-1,1};

void swap(ration* A,ration* B)
{
    ration temp=*A;
    *A=*B;
    *B=temp;
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
