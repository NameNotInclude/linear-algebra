#include <stdio.h>
#include <math.h>
#define max 100
double det(double a[][max],int N)
{
    if (N==1)
    {
        return a[0][0];
    }
    else
    {
        int i;
        for (i=0;i<N && fabs(a[0][i])<1e-9;i++);
        if (i==N) return 0.0;
        else 
        {
            int p=i;
            double temp[max][max];
	    	for (int x = 0; x < N; x++) 
			{
	        	for (int y = 0; y < N; y++) 
				{
	            	temp[x][y] = a[x][y];
	        	}
	    	} 
	    	i++;
	        while (i<N)
            {
                if (fabs(temp[i][p])>1e-9)
                {
                    double divide=temp[i][p]/temp[0][p];
                    for (int k=0;k<N;k++)
                    {
                        temp[i][k]-=divide*temp[0][k];
                    }
                }
                i++;
            }
            double arr[max][max];
            for (int i=1;i<N;i++)
            {
                for (int j=0;j<p;j++)
                {
                    arr[i-1][j]=temp[i][j];
                }
                for (int j=p+1;j<N;j++)
                {
                    arr[i-1][j-1]=temp[i][j];
                }
            }
            return a[0][p]*((p%2)?-1:1)*det(arr,N-1);
        }
    }
}
int main()
{
    int num;
    scanf("%d",&num);
    double array[max][max]={0.0};
    for (int i=0;i<num;i++)
    {
        for (int j=0;j<num;j++)
        {
            scanf("%lf",&array[i][j]);
        }
    }
	double dete= det(array,num);
    printf("%.6lf",dete);
    return 0;
}
