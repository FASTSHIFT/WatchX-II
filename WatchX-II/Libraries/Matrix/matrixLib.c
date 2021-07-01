#include "matrix.h"

#define USE_DEBUG 1

#if ( USE_DEBUG != 0 )
#include <stdio.h>
#define DEBUG_LOG(format, ...)\
do{\
    printf("File:%s Line:%d Function:%s >>", __FILE__, __LINE__, __FUNCTION__);\
    printf(format, ##__VA_ARGS__);\
    printf("\r\n");\
    while(1);\
}while(0)
#else
#define DEBUG_LOG(...)
#endif

bool MatrixInit(Matrix* m,int row,int col)
{
	int** enterMatrix;
	enterMatrix = (int**)MM_Alloc(row * col * sizeof(int*));
	if (enterMatrix != NULL)
	{
		for (int i = 0; i < row; i++)
		{
			enterMatrix[i] = (int*)MM_Alloc(col * sizeof(int));
			
		}
		for (int i = 0; i < row; i++)
		{
			if (enterMatrix[i] == NULL)
			{
				DEBUG_LOG("Matrix Row Alloc Faild");
				break;
			}			
			for (int j = 0; j < col; j++)
			{

				enterMatrix[i][j] = 0;
			}
		}
	}
	else
	{
		DEBUG_LOG("Matrix Alloc Faild");
		return 0;
	}
		
	m->row = row;
	m->col = col;
	m->matrix = enterMatrix;
	return 1;
}

Matrix MatrixCreate(int row, int col)
{
	Matrix m;
	int** enterMatrix;
	enterMatrix = (int**)MM_Alloc(row * col * sizeof(int*));
	if (enterMatrix != NULL)
	{
		for (int i = 0; i < row; i++)
		{
			enterMatrix[i] = (int*)MM_Alloc(col * sizeof(int));
		}
		for (int i = 0; i < row; i++)
		{
			if (enterMatrix[i] == NULL)
			{
				DEBUG_LOG("Matrix Row Alloc Faild");
				break;
			}
			for (int j = 0; j < col; j++)
			{
				enterMatrix[i][j] = 0;
			}
		}
	}
	else
		DEBUG_LOG("Matrix Alloc Faild");
	m.row = row;
	m.col = col;
	m.matrix = enterMatrix;
	return m;
}

bool MatrixSetAll(Matrix* m,int num)
{
	if (m->matrix == NULL)
		return 0;
	int row = m->row;
	int col = m->col;
	int** tempMatrix = m->matrix;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			tempMatrix[i][j] = num;
		}
	}
	return 1;
}

void MatrixPrint(Matrix* m)
{
	printf("MatrixPrint: \n");
	if (m->matrix == NULL)
	{
		printf("Matrix is NULL");
	}
	for (int i = 0; i < m->row; i++)
	{
		for (int j = 0; j < m->col; j++)
		{
			printf("%d ", m->matrix[i][j]);
		}
		printf("\n");
	}
}

bool MatrixSingleShift(Matrix* m, int row,int col,int g_row,int g_col)
{
	if (m->matrix == NULL)
		return 0;
	int temp = m->matrix[row][col];
	m->matrix[g_row][g_col] = temp; 
	m->matrix[row][col] = 0;
	return 1;
}

