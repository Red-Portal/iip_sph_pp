#ifndef IIP_TYPE_H
#define IIP_TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DEBUG 0

/***********************************
* �� �κ��� ���� ���ּ���
************************************/

#define DTYPE double
/*
* If DTYPE = float  ->  set NTYPE = 0
* If DTYPE = double ->  set NTYPE = 1
*
* */
#define NTYPE 1
/************************************
*������� �������ּ���
*********************************** */

/*
INDEX
iip_matrix
iip_blas_lv1

*/

#define UINT uint32_t
#define SINT int32_t
#define ITER long

/*
* #define AA BB
* str(AA) -> 'AA'
* xstr(AA) -> 'BB'
* */
#define str(x) #x
#define xstr(x) str(x)

typedef struct MAT
{
	DTYPE* data;
	UINT ndim;
	UINT d0;
	UINT d1;
	UINT d2;
}MAT;

typedef struct CTYPE
{
	DTYPE re;
	DTYPE im;
}CTYPE;

typedef struct CMAT
{
	CTYPE* data;
	UINT ndim;
	UINT d0;
	UINT d1;
	UINT d2;
}CMAT;


/*
�����ε�
#define �����ε���ũ��(_x(�Լ��� ���� ���ڰ� �������� ���ڼ���ŭ), �Լ� ����ŭ, ...)�� ������ ��
#define �����ε����Լ�(...) �����ε���ũ��(__VA_ARGS__, �Լ���)(__VA_ARGS__)

__VA_ARGS__�� �Լ��� ������ ��ũ��

EX)

void f1(arg1,arg2)
void f2(arg1,arg2,arg3)

���ڰ� �������� �Լ��� ���ڰ� 2���̱� ������
_x,_xx
�Լ��� 2���ϱ�
_2,_1

#define o_f(_x,_xx, _2, _1) _1
#define f(...) o_f(__VA_ARGS__, f2, f1)(__VA_ARGS__)

*/

/***************************
**** pre main for CUDA ****
***************************/

/*
//This only works in GNU
void __attribute__ ((constructor)) premain()
{


}

*/

#if USE_CBLAS 
#include "cblas.h"
#endif
#if BLAS_CU
#include "cublas_v2.h"
#endif



#endif