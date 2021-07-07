#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Matrix/matrix.h"
#include "stdlib.h"
PAGE_EXPORT(Game2048);
#define STANDARDGREY LV_COLOR_MAKE(0x00, 0x99, 0xff)
#define STANDARDWHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define DEEPDGREY LV_COLOR_MAKE(0x33, 0x33, 0x33)
#define pageRow  5
#define pageCol  5

#define USE_DEBUG 0

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

typedef struct
{
    int row;
    int col;
}blockCoord;

//static const int color[] =
//{
//    0xccffff,
//    0xffcc99,

//};
static const lv_coord_t Btn_Width = 48;
static const lv_coord_t Btn_Height = 48;
static lv_obj_t* contBtnGrp;
//static lv_obj_t* SingleBtn;
static lv_obj_t* BtnGrp[pageRow][pageCol];
static lv_obj_t* LabelGrp[pageRow][pageCol];
Matrix btnMatrix;
blockCoord initBlockGrp[5];
blockCoord OutCoord;
static blockCoord GetRandBlock()
{
    blockCoord coord;
    coord.row = rand() % pageRow;
    coord.col = rand() % pageCol;
    return coord;
}

static void BtnCreate(lv_obj_t* par, int labelVal, int row, int col)
{
    lv_obj_t* btn = lv_btn_create(par, NULL);
    lv_obj_set_size(btn, Btn_Width, Btn_Height);
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x9966ff));
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, DEEPDGREY);
    lv_obj_set_style_local_radius(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_width(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_state(btn, LV_STATE_DISABLED);
    lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, col * Btn_Width, row -1* Btn_Height);
    lv_obj_t* label = lv_label_create(btn, NULL);
    lv_label_set_text_fmt(label, "%d", labelVal);
    LabelGrp[row][col] = label;
    btnMatrix.matrix[row][col] = labelVal;
    BtnGrp[row][col] = btn;
}

static void NewRowBtnCreate()
{
    int numOfBtn = rand() % 3+1;
    //int GrpVal[pageRow] = {0};
    int numBtnIndex = 0;
    int numLabelVal = 0;
    for (; numOfBtn > 0; numOfBtn--)
    {
        if (BtnGrp[0][numBtnIndex] == NULL)
        {
            numBtnIndex = rand() % pageRow;
            numLabelVal = (rand() % 2 + 1) * 8;
            BtnCreate(contBtnGrp, numLabelVal, 0, numBtnIndex);
            LV_OBJ_ADD_ANIM(BtnGrp[0][numBtnIndex], y, 0, 100);
        }
        while (BtnGrp[0][numBtnIndex] != NULL)
        {
            numBtnIndex = rand() % pageRow;
            if (BtnGrp[0][numBtnIndex] == NULL)
            {
                numLabelVal = (rand() % 2+1) * 8;
                BtnCreate(contBtnGrp, numLabelVal, 0, numBtnIndex);
                LV_OBJ_ADD_ANIM(BtnGrp[0][numBtnIndex], y, 0, 100);
                break;
            }
        }
    }
}

static void BtnShift(Matrix* m, int nowRow, int nowCol, int g_row, int g_col)
{
    MatrixSingleShift(m, nowRow, nowCol, g_row, g_col);
    if (g_col == nowCol)
        LV_OBJ_ADD_ANIM(BtnGrp[nowRow][nowCol], y, g_row * Btn_Height, 100);
    else if (g_row == nowRow)
        LV_OBJ_ADD_ANIM(BtnGrp[nowRow][nowCol], x, g_col * Btn_Height, 100);
    lv_obj_t* temp = BtnGrp[nowRow][nowCol];
    BtnGrp[g_row][g_col] = temp;
    BtnGrp[nowRow][nowCol] = NULL;
    lv_obj_t* labelTemp = LabelGrp[nowRow][nowCol];
    LabelGrp[g_row][g_col] = labelTemp;
    LabelGrp[nowRow][nowCol] = NULL;
}

static void BtnPlusDel(Matrix* m,int lastRow,int lastCol,int nowRow,int nowCol)
{
    m->matrix[lastRow][lastCol] += m->matrix[nowRow][nowCol];
    //if(lastCol== nowCol)
    //    LV_OBJ_ADD_ANIM(BtnGrp[nowRow][nowCol], y, lastRow * Btn_Height, 100);
    //else if (lastRow == nowRow)
    //    LV_OBJ_ADD_ANIM(BtnGrp[nowRow][nowCol], x, lastCol * Btn_Height, 100);
    if (m->matrix[lastRow][lastCol]>=128)
    {
        lv_obj_set_style_local_text_font(LabelGrp[lastRow][lastCol], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    }
    lv_label_set_text_fmt(LabelGrp[lastRow][lastCol],"%d", m->matrix[lastRow][lastCol]);
    lv_obj_del(LabelGrp[nowRow][nowCol]);
    lv_obj_del(BtnGrp[nowRow][nowCol]);
    BtnGrp[nowRow][nowCol] = NULL;
    LabelGrp[nowRow][nowCol] = NULL;
    m->matrix[nowRow][nowCol] = 0;
}


static void BtnGrpPrint()
{
    DEBUG_LOG("BtnGrpPrint: \n");
    for (int i = 0; i < pageRow; i++)
    {
        for (int j = 0; j < pageCol; j++)
        {
            if (BtnGrp[i][j] == NULL)
                DEBUG_LOG("%d ", 0);
            else
                DEBUG_LOG("%d ", 1);
        }
        DEBUG_LOG("\n");
    }
}

static void RowRightShift(Matrix* m,int row)
{
    blockCoord ReallCoord[5];
    int reallCoordIndex = 0;
    blockCoord lastBtnCoord;
    lastBtnCoord.row = -1;
    lastBtnCoord.col = -1;

    for (int i = 0; i < pageCol; i++)
    {
        if (m->matrix[row][i] != 0)
        {
            ReallCoord[reallCoordIndex].row = row;
            ReallCoord[reallCoordIndex].col = i;
            reallCoordIndex++;
        }
    }
    if (reallCoordIndex == 0)
    {
        return;
    }
    for (int i = 4; i >= 0; i--)
    {
        int nowRow = ReallCoord[reallCoordIndex - 1].row;
        int nowCol = ReallCoord[reallCoordIndex - 1].col;
        if (m->matrix[row][i] == 0)
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0 && m->matrix[nowRow][nowCol] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
            {
                BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, nowRow, nowCol);
                reallCoordIndex--;
                if (reallCoordIndex > 0)
                {
                    nowRow = ReallCoord[reallCoordIndex - 1].row;
                    nowCol = ReallCoord[reallCoordIndex - 1].col;
                    BtnShift(m,nowRow, nowCol, row, i);
                    lastBtnCoord.row = row;
                    lastBtnCoord.col = i;
                }
            }
            else
            {
                BtnShift(m,nowRow, nowCol, row, i);
                lastBtnCoord.row = row;
                lastBtnCoord.col = i;
            }
            reallCoordIndex--;
        }
        else
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0)
            {
                if (m->matrix[row][i] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
                {
                    BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, row, i);
                    reallCoordIndex--;
                    if (reallCoordIndex > 0)
                    { 
                        nowRow = ReallCoord[reallCoordIndex - 1].row;
                        nowCol = ReallCoord[reallCoordIndex - 1].col;
                        BtnShift(m,nowRow, nowCol, row, i);
                        lastBtnCoord.row = row;
                        lastBtnCoord.col = i;
                    }
                }
            }
            lastBtnCoord.row = row;
            lastBtnCoord.col = i;
            reallCoordIndex--;
        }
        if (reallCoordIndex <= 0)
        {
            return;
        }
    }
}

static void RowLeftShift(Matrix* m, int row)
{
    blockCoord ReallCoord[5];
    int reallCoordIndex = 0;
    blockCoord lastBtnCoord;
    lastBtnCoord.row = -1;
    lastBtnCoord.col = -1;
    for (int i = pageCol-1; i >=0 ; i--)
    {
        if (m->matrix[row][i] != 0)
        {
            ReallCoord[reallCoordIndex].row = row;
            ReallCoord[reallCoordIndex].col = i;
            reallCoordIndex++;
        }
    }
    if (reallCoordIndex == 0)
    {
        return;
    }
    for (int i = 0; i < pageCol; i++)
    {
        int nowRow = ReallCoord[reallCoordIndex - 1].row;
        int nowCol = ReallCoord[reallCoordIndex - 1].col;
        if (m->matrix[row][i] == 0)
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0 && m->matrix[nowRow][nowCol] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
            {
                BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, nowRow, nowCol);
                reallCoordIndex--;
                if (reallCoordIndex > 0)
                {
                    nowRow = ReallCoord[reallCoordIndex - 1].row;
                    nowCol = ReallCoord[reallCoordIndex - 1].col;
                    BtnShift(m,nowRow, nowCol, row, i);
                    lastBtnCoord.row = row;
                    lastBtnCoord.col = i;
                }
            }
            else
            {
                BtnShift(m,nowRow, nowCol, row, i);
                lastBtnCoord.row = row;
                lastBtnCoord.col = i;
            }
            reallCoordIndex--;
        }
        else
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0)
            {
                if (m->matrix[row][i] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
                {
                    BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, row, i);
                    reallCoordIndex--;
                    if (reallCoordIndex > 0)
                    { 
                        nowRow = ReallCoord[reallCoordIndex - 1].row;
                        nowCol = ReallCoord[reallCoordIndex - 1].col;
                        BtnShift(m,nowRow, nowCol, row, i);
                        lastBtnCoord.row = row;
                        lastBtnCoord.col = i;
                    }
                }
            }
            lastBtnCoord.row = row;
            lastBtnCoord.col = i;
            reallCoordIndex--;
        }
        if (reallCoordIndex <= 0)
        {
            return;
        }
    
    }
}

static void ColDownShift(Matrix* m, int col)
{
    blockCoord ReallCoord[5];
    int reallCoordIndex = 0;
    blockCoord lastBtnCoord;
    lastBtnCoord.row = -1;
    lastBtnCoord.col = -1;
    for (int i = 0; i < pageRow; i++)
    {
        if (m->matrix[i][col] != 0)
        {
            ReallCoord[reallCoordIndex].row = i;
            ReallCoord[reallCoordIndex].col = col;
            reallCoordIndex++;
        }
    }
    if (reallCoordIndex == 0)
    {
        return;
    }
    for (int i = 4; i >= 0; i--)
    {
        int nowRow = ReallCoord[reallCoordIndex - 1].row;
        int nowCol = ReallCoord[reallCoordIndex - 1].col;
        if (m->matrix[i][col] == 0)
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0 && m->matrix[nowRow][nowCol] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
            {             
                BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, nowRow, nowCol);
                reallCoordIndex--;
                if (reallCoordIndex > 0)
                {
                    nowRow = ReallCoord[reallCoordIndex - 1].row;
                    nowCol = ReallCoord[reallCoordIndex - 1].col;
                    BtnShift(m,nowRow, nowCol, i, col);
                    lastBtnCoord.row = i;
                    lastBtnCoord.col = col;
                }   
            }
            else
            {
                BtnShift(m,nowRow, nowCol, i, col);
                lastBtnCoord.row = i;
                lastBtnCoord.col = col;
            }
            reallCoordIndex--;
        }
        else
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0)
            {
                if (m->matrix[i][col] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
                {
                    BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, i, col);
                    reallCoordIndex--;
                    if (reallCoordIndex > 0)
                    {   
                        nowRow = ReallCoord[reallCoordIndex - 1].row;
                        nowCol = ReallCoord[reallCoordIndex - 1].col;
                        BtnShift(m,nowRow, nowCol, i, col);
                        lastBtnCoord.row = i;
                        lastBtnCoord.col = col;
                    }
                }
            }
            lastBtnCoord.row = i;
            lastBtnCoord.col = col;
            reallCoordIndex--;
        }
        if (reallCoordIndex <= 0)
        {
            return;
        }
    }
}

static void ColUpShift(Matrix* m, int col)
{
    blockCoord ReallCoord[5];
    int reallCoordIndex = 0;
    blockCoord lastBtnCoord;
    lastBtnCoord.row = -1;
    lastBtnCoord.col = -1;
    for (int i = 4; i >= 0; i--)
    {
        if (m->matrix[i][col] != 0)
        {
            ReallCoord[reallCoordIndex].row = i;
            ReallCoord[reallCoordIndex].col = col;
            reallCoordIndex++;
        }
    }
    if (reallCoordIndex == 0)
    {
        return;
    }
    for (int i = 0; i < pageRow; i++)
    {
        int nowRow = ReallCoord[reallCoordIndex - 1].row;
        int nowCol = ReallCoord[reallCoordIndex - 1].col;
        if (m->matrix[i][col] == 0)
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0 && m->matrix[nowRow][nowCol] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
            {
                BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, nowRow, nowCol);
                reallCoordIndex--;
                if (reallCoordIndex > 0)
                {
                    nowRow = ReallCoord[reallCoordIndex - 1].row;
                    nowCol = ReallCoord[reallCoordIndex - 1].col;
                    BtnShift(m,nowRow, nowCol, i, col);
                    lastBtnCoord.row = i;
                    lastBtnCoord.col = col;
                }
            }
            else
            {
                BtnShift(m,nowRow, nowCol, i, col);
                lastBtnCoord.row = i;
                lastBtnCoord.col = col;
            }
            reallCoordIndex--;
        }
        else
        {
            if (lastBtnCoord.row >= 0 && lastBtnCoord.col >= 0)
            {
                if (m->matrix[i][col] == m->matrix[lastBtnCoord.row][lastBtnCoord.col])
                {
                    BtnPlusDel(m, lastBtnCoord.row, lastBtnCoord.col, i, col);
                    reallCoordIndex--;
                    if (reallCoordIndex > 0)
                    {   
                        nowRow = ReallCoord[reallCoordIndex - 1].row;
                        nowCol = ReallCoord[reallCoordIndex - 1].col;
                        BtnShift(m,nowRow, nowCol, i, col);
                        lastBtnCoord.row = i;
                        lastBtnCoord.col = col;
                    }
                }
            }
            lastBtnCoord.row = i;
            lastBtnCoord.col = col;
            reallCoordIndex--;
        }
        if (reallCoordIndex <= 0)
        {
            return;
        }
    }
}

static void AllShift(Matrix* m, int dir)
{
    for (int i = 0; i <pageCol; i++)
    {
        switch (dir)
        {
        case SD_Right: RowRightShift(m, i); break;
        case SD_Left: RowLeftShift(m, i); break;
        case SD_Up: ColUpShift(m, i); break;
        case SD_Down: ColDownShift(m, i); break;
        default: break;
        }
    }
    if (dir == SD_Down)
    {
        NewRowBtnCreate();
    }
    DEBUG_LOG("****AllShift: ****\n");
    MatrixPrint(&btnMatrix);
    BtnGrpPrint();
}

static void GamePrepare()
{
    MatrixInit(&btnMatrix, pageRow, pageCol);
    for (int i = 0; i < pageRow; i++)
    {
        initBlockGrp[i] = GetRandBlock();
        btnMatrix.matrix[initBlockGrp[i].row][initBlockGrp[i].col] = 8;
    }
    MatrixPrint(&btnMatrix);
}

static void BtnGrpCreate(lv_obj_t* par)
{
    for (int i = 0; i < pageRow; i++)
    {
        int nowRow = initBlockGrp[i].row;
        int nowCol = initBlockGrp[i].col;
        lv_obj_t* btn = lv_btn_create(par, NULL);
        lv_obj_set_size(btn, Btn_Width, Btn_Height);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, DEEPDGREY);
        lv_obj_set_style_local_radius(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
        lv_obj_set_state(btn, LV_STATE_DISABLED);
        lv_obj_set_style_local_border_width(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 2);
        lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, nowCol * Btn_Width, nowRow * Btn_Height);
        lv_obj_t*  label = lv_label_create(btn, NULL);
        lv_label_set_text_fmt(label, "%d", btnMatrix.matrix[nowRow][nowCol]);
        LabelGrp[nowRow][nowCol] = label;
        BtnGrp[nowRow][nowCol] = btn;
    }
    BtnGrpPrint();
}


static void ContBtnGrp_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_width(cont, APP_WIN_WIDTH);
    lv_obj_set_height(cont, APP_WIN_HEIGHT);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);

    for (int i = 0; i < (pageRow-1); i++)
    {
        static lv_point_t line_points[] = { {0, 0}, {0, 240} };
        lv_obj_t* line = lv_line_create(cont, NULL);
        lv_line_set_points(line, line_points, 2);
        lv_obj_set_style_local_line_width(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 1);
        lv_obj_set_style_local_line_color(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x99, 0x99, 0x99));
        lv_obj_align(line, cont, LV_ALIGN_IN_TOP_LEFT, (i+1)* Btn_Width, 0);
    }
    for (int i = 0; i < (pageCol-1); i++)
    {
        static lv_point_t line_points[] = { {0, 0}, {240,0} };
        lv_obj_t* line = lv_line_create(cont, NULL);
        lv_line_set_points(line, line_points, 2);
        lv_obj_set_style_local_line_width(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 1);
        lv_obj_set_style_local_line_color(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x99, 0x99, 0x99));
        lv_obj_align(line, cont, LV_ALIGN_IN_TOP_LEFT, 0,(i + 1) * Btn_Height);
    }
    contBtnGrp = cont;
}

static void Setup()
{
    lv_obj_move_foreground(appWindow);
    GamePrepare();
    ContBtnGrp_Create(appWindow);
    BtnGrpCreate(contBtnGrp);
}

static void Exit()
{
    lv_obj_clean(appWindow);
}

static void Event(void* obj, uint8_t event)
{
    if (event == LV_GESTURE_DIR_TOP)
    {
        AllShift(&btnMatrix, SD_Up);
    }
    else if (event == LV_GESTURE_DIR_RIGHT)
    {
        AllShift(&btnMatrix, SD_Right);
    }
    else if (event == LV_GESTURE_DIR_LEFT)
    {
        AllShift(&btnMatrix, SD_Left);
    }
    else if (event == LV_GESTURE_DIR_BOTTOM)
    {
        AllShift(&btnMatrix, SD_Down);
    }
    if (event == LV_EVENT_LEAVE)
    {
        Page->Pop();
    }
}
