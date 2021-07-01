
#ifndef _MATRIX_
#define _MATRIX_ 
#include "stdio.h"
#include "stdbool.h"
#include "MM.h"
#ifdef __cplusplus
extern "C" {
#endif

//#define _MatrixInit(_m ,_r ,_c,_type) MatrixInit( (_m),(_r),(_c),sizeof(_type))

typedef struct {
	int row;
	int col;
	int** matrix;
}Matrix;

enum ShiftDirction
{
	SD_Right,
	SD_Left,
	SD_Up,
	SD_Down
};

bool MatrixInit(Matrix* m, int row, int col);
Matrix MatrixCreate(int row, int col);
bool MatrixSetAll(Matrix* m, int num);
void MatrixPrint(Matrix* m);
bool MatrixSingleShift(Matrix* m, int row, int col, int g_row, int g_col);


#ifdef __cplusplus
}// extern "C"
#endif
#endif // !_MATRIX_
