#include "iip_blas_lv3.h"

/*
 **  cblas_?gemm(layout,transA,transB,m,n,k,alpha,A,lda,B,ldb,beta,C,ldc)
 **
 **    layout :   i) --->CblasRowMajor
 **    			   [0][1]  =  {0,1,2,3}
 **                	   [2][3]
 **
 **             ii)  |  [0][2] = {0,1,2,3}
 **                  |  [1][3]
 **                 \_/ CblasColMajor
 **
 **   
 **   C := alpha * op(A)*op(B) + beta*C
 **
 **     op(X) =  	  i) X      when transX = CblasNoTrans
 **
 **     		 	 ii) X**T     ''        = CblasTrans
 **
 **     			iii) X**H     ''        = CblasConjTrans
 **
 **      m  = the number of rows of op(A)
 **      n  = the number of columns of op(B) and C 
 **      k  = the number of columns of op(A) and rows of op(B)
 **
 **
 **      lda : the first dimension of A
 **      ldb : 		''	     B
 **      ldc :		''	     C
 **
 ** 		-the first dimension : the number of columns when CblasRowMajor
 **		                 	   	''    rows   when CblasColMajor
*/

void gemm(char transA,char transB, DTYPE alpha, MAT*A,MAT*B, DTYPE beta, MAT*C)
{

UINT m,n,k;
UINT lda,ldb,ldc;

#if DEBUG
printf("%s\n",__func__);
#endif

if(transA == NoTran)
{
m = A->d0;
k = A->d1;

lda = m;
ldc = m;
}
else
{
m = A->d1;
k = A->d0;

lda = k;
ldc = m;
}

if(transB == NoTran)
{
n = B->d1;

ldb = B->d0;
}
else
{
n = B->d0;

ldb = B ->d0;
}


#if USE_CBLAS

	#if NTYPE == 0
	cblas_sgemm(CblasColMajor,transA,transB,m,n,k,alpha,A->data,lda,B->data,ldb,beta,C->data,ldc);
	#else
	cblas_dgemm(CblasColMajor,transA,transB,m,n,k,alpha,A->data,lda,B->data,ldb,beta,C->data,ldc);
	#endif
#else
	mp_gemm(transA,transB, m,n,k,alpha,A->data,lda,B->data,ldb,beta,C->data,ldc);
#endif
}

void mp_gemm(char transA, char transB, UINT m,UINT n,UINT k, DTYPE alpha,DTYPE* A,UINT lda,DTYPE*B,UINT ldb, DTYPE beta,DTYPE *C  ,UINT ldc)
{
ITER i,j,l;	
DTYPE temp;
#if DEBUG
printf("%s\n",__func__);
#endif

if((transA == NoTran) && (transB == NoTran))
{
#pragma omp parallel for shared(A,B,C) private(temp,i,j,l)
	for(l=0;l<m;l++)
	{
		for(j=0;j<n;j++)
		{
			temp = 0;
			for(i=0;i<k;i++)
			{
				temp+= A[i*lda + l]*B[i + j*k];		
			}
			C[l + m*j]*=beta;
		
			temp *=alpha;
			C[l + m*j] +=temp;
		}
	}
	
}


if((transA == Tran) && (transB == NoTran))
{

#pragma omp parallel for shared(A,B,C) private(temp,i,j,l)
for(l=0;l<m;l++)
	{
		for(j=0;j<n;j++)
		{
			temp = 0;
			for(i=0;i<k;i++)
			{
				temp+= A[i*lda + l]*B[i + j*k];		
			}
			C[l + m*j]*=beta;
		
			temp *=alpha;
			C[l + m*j] +=temp;
		}
	}
}
if((transA == Tran) && (transB == Tran));
if((transA == NoTran) && (transB == Tran));

}

void cgemm(char transA,char transB, CTYPE alpha, CMAT*A,CMAT*B, CTYPE beta, CMAT*C)
{

UINT m,n,k;
UINT lda,ldb,ldc;

#if DEBUG
printf("%s\n",__func__);
#endif

if(transA == NoTran)
{
m = A->d0;
k = A->d1;

lda = m;
ldc = m;
}
else
{
m = A->d1;
k = A->d0;

lda = k;
ldc = m;
}

if(transB == NoTran)
{
n = B->d1;

ldb = B->d0;
}
else
{
n = B->d0;

ldb = B ->d0;
}

#if USE_CBLAS

	#if NTYPE == 0
	cblas_cgemm(CblasColMajor,transA,transB,m,n,k,&alpha,A->data,lda,B->data,ldb,&beta,C->data,ldc);
	#else
	cblas_zgemm(CblasColMajor,transA,transB,m,n,k,&alpha,A->data,lda,B->data,ldb,&beta,C->data,ldc);
	#endif
#else
	mp_cgemm(transA,transB, m,n,k,alpha,A->data,lda,B->data,ldb,beta,C->data,ldc);
#endif
}

void mp_cgemm(char transA, char transB, UINT m,UINT n,UINT k, CTYPE alpha,CTYPE* A,UINT lda,CTYPE*B,UINT ldb, CTYPE beta,CTYPE *C  ,UINT ldc)
{
ITER i,j,l;	
CTYPE temp;
#if DEBUG
printf("%s\n",__func__);
#endif

if((transA == NoTran) && (transB == NoTran))
{
#pragma omp parallel for shared(A,B,C) private(temp,i,j,l)
	for(l=0;l<m;l++)
	{
		for(j=0;j<n;j++)
		{
			temp.re = 0;
			temp.im = 0;
			for(i=0;i<k;i++)
			{
				cadd_mul(temp,A[i*lda + l],B[i + j*k]);		
			}
			cmul(C[l + m*j],beta);
			cadd(temp ,alpha);
			cadd(C[l + m*j]  ,temp);
		}
	}
	
}


if((transA == Tran) && (transB == NoTran));
if((transA == Tran) && (transB == Tran));
if((transA == NoTran) && (transB == Tran));

}