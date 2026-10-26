#ifndef MATRIX_OPERAT_H
#define MATIRX_OPERAT_H


ration** matrix_input(int col,int row);
ration det(ration** D,int n);//行列式函数
void matrixmultiply(ration** a,ration** b,ration** c,int a_row,int a_col,int b_row,int b_col);//矩阵乘法

#endif