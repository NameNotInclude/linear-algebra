#ifndef RATION_COMPUT_H
#define RATION_COMPUT_H

typedef struct
{
    int up;
    int down;
}ration;

extern ration neg;

void swap(ration* A,ration* B);
ration plus(ration a,ration b);//分数加法
ration minus(ration a,ration b);//分数减法
ration multiply(ration a,ration b);//分数乘法
ration divide(ration a,ration b);//分数除法
void normalize(ration *r);//约分函数
ration rationinput();//有理数输入
void rationprint(ration x);//有理数输出

#endif