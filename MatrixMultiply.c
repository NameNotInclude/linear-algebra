#include <stdio.h>
#define max_size 100
int gcd(int a,int b)
{
	while (b!=0)
	{
		int t=b;
		b=a%b;
		a=t;
	}
	return a;
}
void juzhen(int m,int n,int a[][max_size],int b[][max_size])
{
	for(int i=0;i<m;i++)
	{
		for (int k=0;k<n;k++)
		{
			scanf("%d/%d",&a[i][k],&b[i][k]);				
		}
	}
	printf("\n");
} 
void chengfa(int result_up[][max_size],int result_down[][max_size],
			int c_up[][max_size],int c_down[][max_size],
			int d_up[][max_size],int d_down[][max_size],
			int e[],int f[])//c与d对应两个矩阵，e与f存储矩阵的长宽
{
	if (e[1]==f[0])	
	{
		for (int A=0;A<e[0];A++)
		{
			for (int B=0;B<f[1];B++)//A,B参数坐标 e[0]行数 e[1]列数 f[0]行数 f[1]列数 e[1]=f[0]
			{
				int up=c_up[A][0]*d_up[0][B],down=c_down[A][0]*d_down[0][B];
				int divide=gcd(up,down);
				up/=divide;
				down/=divide;
				for(int k=1;k<e[1];k++) 
				{
					int sub_up=c_up[A][k]*d_up[k][B];
					int sub_down=c_down[A][k]*d_down[k][B];
					int x=sub_down*down/gcd(sub_down,down);//最小公倍数 
					up=x/down*up+x/sub_down*sub_up;
					down=x;
					divide=gcd(up,down);
					up/=divide;
					down/=divide;					
				}
				result_up[A][B]=up;
				result_down[A][B]=down;
			}
		}
	}
} 
int main()
{
	printf("--- 分数矩阵链乘法计算器 ---\n");
    printf("输入格式：\n");
    printf("1. 首先输入第一个矩阵的维度，例如 `2x3`。\n");
    printf("2. 然后逐行输入矩阵元素，每个元素为分数形式，例如 `1/2`。\n");
    printf("3. 输入下一个矩阵的维度，程序会自动进行乘法。\n");
    printf("4. 当输入维度小于等于0时，程序将输出最终结果并退出。\n\n");
	int o,p;
	scanf("%dx%d",&o,&p);
	int a2[2];
	a2[0]=o;
	a2[1]=p;
	int E_up[max_size][max_size];//生成初等矩阵，存储分子 
	int e1[2]={o,o}; 
	for (int i=0;i<o;i++)
	{
		for (int j=0;j<o;j++)
		{
			if (i==j)
			{
				E_up[i][j]=1;	
			}	
			else
			{
				E_up[i][j]=0;
			}
		}	
	} 
	int E_down[max_size][max_size];//生成初等矩阵,存储分母 
	for (int i=0;i<o;i++)
	{
		for (int j=0;j<o;j++)
		{
			E_down[i][j]=1;
		}	
	} 
	while (1)
	{
		int a_up[max_size][max_size],a_down[max_size][max_size];
		juzhen(e1[1],p,a_up,a_down);
		int D_up[max_size][max_size],D_down[max_size][max_size];
		chengfa(D_up,D_down,E_up,E_down,a_up,a_down,e1,a2);
		for (int i=0;i<o;i++)//将D_up的值复制给E_up
		{
			for (int j=0;j<p;j++)
			{
				E_up[i][j]=D_up[i][j];
			}
		}	
		for (int i=0;i<o;i++)//将D_down的值复制给E_down
		{
			for (int j=0;j<p;j++)
			{
				E_down[i][j]=D_down[i][j];
			}
		}
		printf("\ncurrent result:\n"); 
		for (int i=0;i<e1[0];i++)//打印结果 
		{
			printf("|");
			for (int j=0;j<a2[1];j++)
			{
				if (E_down[i][j]!=1)
					printf("%d/%d",E_up[i][j],E_down[i][j]);
				else
					printf("%d",E_up[i][j]);
				if (j!=a2[1]-1)
				{
					printf(" ");
				}
			}
			printf("|\n");
		}
		printf("\n");	
		e1[1]=a2[1]; 
		printf("%dx",e1[1]);
		scanf("%d",&p);
		if (p<=0) 
		{
			break; //输入ax-1输出当前结果 
		}
		a2[0]=e1[1];
		a2[1]=p;
	}
	printf("\nfinal result:\n"); 
	for (int i=0;i<e1[0];i++)//打印结果 
	{
		printf("|");
		for (int j=0;j<e1[1];j++)
		{
			printf("%d/%d",E_up[i][j],E_down[i][j]);
			if (j!=e1[1]-1)
			{
				printf(" ");
			}
		}
		printf("|\n");
	}
	return 0;
}