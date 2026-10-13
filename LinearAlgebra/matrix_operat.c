#include "ration_comput.h"
#include "matrix_operat.h"
#include <stdlib.h>
#include <stdio.h>

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
