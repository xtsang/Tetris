#include "stm32f10x.h"
#include "oled.h"
#include "stdlib.h"
#include "animation.h"
#include "tetris.h"

// ������ʾ���飬21��ֻ�����ж���������������ʾ
uint8_t MinoMap[10][21] = {0};

// ��������ɱ�����Ƿ����һ��
uint8_t RandTable[8] = {0, 1, 2, 3, 4, 5, 6, 0};

// ���Ƶ��ķ�����ʾ���飬0ΪMinoLine��1ΪMinoColumn����Ҫ�ж��Ƿ�Ϊ����������з�������Tetromino[4][0]�����ķ������࣬Tetromino[4][1]������ת����
int8_t Tetromino[5][2] = {0};

// ��ת������Ըı�����
const int8_t RotateTable[7][4][8] = {
    0, 0,   0, 0,   0, 0,   0, 0,  // ��һ����ת
    0, 0,   0, 0,   0, 0,   0, 0,  // �ڶ�����ת
    0, 0,   0, 0,   0, 0,   0, 0,  // ��������ת
    0, 0,   0, 0,   0, 0,   0, 0,  // ���Ĵ���ת���ص���ʼ
    
    1, 1,   0, 0,   1,-1,  -1,-1,
    1,-1,   0, 0,  -1,-1,  -1, 1,
   -1,-1,   0, 0,  -1, 1,   1, 1,
   -1, 1,   0, 0,   1, 1,   1,-1,
    
    2, 1,   1, 0,   0,-1,  -1,-2,
    1,-2,   0,-1,  -1, 0,  -2, 1,
   -2,-1,  -1, 0,   0, 1,   1, 2,
   -1, 2,   0, 1,   1, 0,   2,-1,
    
    2, 0,   0, 0,   1,-1,  -1,-1,
    0,-2,   0, 0,  -1,-1,  -1, 1,
   -2, 0,   0, 0,  -1, 1,   1, 1,
    0, 2,   0, 0,   1, 1,   1,-1,
    
    1, 1,   0, 0,   1,-1,   0,-2,
    1,-1,   0, 0,  -1,-1,  -2, 0,
   -1,-1,   0, 0,  -1, 1,   0, 2,
   -1, 1,   0, 0,   1, 1,   2, 0,
   
    1, 1,   2, 0,   0, 0,  -1,-1,
    1,-1,   0,-2,   0, 0,  -1, 1,
   -1,-1,  -2, 0,   0, 0,   1, 1,
   -1, 1,   0, 2,   0, 0,   1,-1,
   
    1, 1,   0, 0,  -1,-1,   0,-2,
    1,-1,   0, 0,  -1, 1,  -2, 0,
   -1,-1,   0, 0,   1, 1,   0, 2,
   -1, 1,   0, 0,   1,-1,   2, 0,
};

/**
  * @brief  ����0~6������֣�7��һ����ע���ִ�����˹��������7��Ϊ1��(1 Bag)���ֵ�,����ÿһ��,���˳����ַ���I��O��T��L��J��S��Z
  * @param  ��
  * @retval ��
  */
uint8_t Randomizer(void)
{
    uint8_t i, n;
    do {
        srand(TIM_GetCounter(TIM2));
        i = rand()%7;
    } while (RandTable[i] == 7);  // ������ѳ��ֵķ�������������
    RandTable[i] = 7;
    if (++RandTable[7] == 7) {  // һ�����꣬�����������
        RandTable[7] = 0;
        for (n = 0; n < 7; n++) RandTable[n] = n;
    }
    return i;
}

/**
  * @brief  ����һ���ĸ񷽿鲢��ʾ
  * @param  �ĸ񷽿�����࣬��Χ��0~6
  * @retval ��
  */
void GenerateTetromino(void)
{
    uint8_t i = Randomizer();
    switch(i) {
      case 0:
        Tetromino[0][0] = 4;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 4;
        Tetromino[1][1] = 20;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 19;
        Tetromino[3][0] = 5;
        Tetromino[3][1] = 20;
        OLED_ShowTetromimo();
        break;
      case 1:     
        Tetromino[0][0] = 4;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 5;
        Tetromino[1][1] = 19;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 20;
        Tetromino[3][0] = 6;
        Tetromino[3][1] = 19;
        OLED_ShowTetromimo();
        break;
      case 2:
        Tetromino[0][0] = 3;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 4;
        Tetromino[1][1] = 19;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 19;
        Tetromino[3][0] = 6;
        Tetromino[3][1] = 19;
        OLED_ShowTetromimo();
        break;
      case 3:
        Tetromino[0][0] = 4;
        Tetromino[0][1] = 20;
        Tetromino[1][0] = 5;
        Tetromino[1][1] = 19;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 20;
        Tetromino[3][0] = 6;
        Tetromino[3][1] = 19;
        OLED_ShowTetromimo();
        break;
      case 4:
        Tetromino[0][0] = 3;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 4;
        Tetromino[1][1] = 19;
        Tetromino[2][0] = 4;
        Tetromino[2][1] = 20;
        Tetromino[3][0] = 5;
        Tetromino[3][1] = 20;
        OLED_ShowTetromimo();
        break;
      case 5:
        Tetromino[0][0] = 4;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 4;
        Tetromino[1][1] = 20;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 19;
        Tetromino[3][0] = 6;
        Tetromino[3][1] = 19;
        OLED_ShowTetromimo();
        break;
      case 6:
        Tetromino[0][0] = 3;
        Tetromino[0][1] = 19;
        Tetromino[1][0] = 4;
        Tetromino[1][1] = 19;
        Tetromino[2][0] = 5;
        Tetromino[2][1] = 19;
        Tetromino[3][0] = 5;
        Tetromino[3][1] = 20;
        OLED_ShowTetromimo();
        break;
    }
    Tetromino[4][0] = i;
    Tetromino[4][1] = 0;
}

/**
  * @brief  �ĸ񷽿������ת��ˢ����ʾ
  * @param  ��
  * @retval ��
  */
void RotateTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] += RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*i+0];
        Tetromino[i][1] += RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*i+1];
        if (Tetromino[i][0] < 0 || Tetromino[i][0] > 9 || Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // �ж��Ƿ񳬳��߿��ײ�����з���
            n++;
            break;
        }
    }
    Tetromino[4][1]++;  // ������ת����
    if (n) {  // ���������ת�����
        Tetromino[4][1]--;
        for (j = 0; j <= i; j++) {
            Tetromino[j][0] -= RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*j+0];
            Tetromino[j][1] -= RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*j+1];
        }
    }
    if (Tetromino[4][1] == 4) Tetromino[4][1] = 0;  // ת�Ĵ���ص���ʼ״̬
    OLED_ShowTetromimo();
}

/**
  * @brief  �ĸ񷽿������ƶ���ˢ����ʾ
  * @param  ��
  * @retval ��
  */
void LMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] -= 1;
        if (Tetromino[i][0] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // �ж��Ƿ񳬳��߿��ײ�����з���
            n++;
            break;
        }
    }
    if (n) {
        for (j = 0; j <= i; j++) Tetromino[j][0] += 1;  // ��������ƶ������
    }
    OLED_ShowTetromimo();
}

/**
  * @brief  �ĸ񷽿������ƶ���ˢ����ʾ
  * @param  ��
  * @retval ��
  */
void RMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] += 1;
        if (Tetromino[i][0] > 9 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // �ж��Ƿ񳬳��߿��ײ�����з���
            n++;
            break;
        }
    }
    if (n) {
        for (j = 0; j <= i; j++) Tetromino[j][0] -= 1;
    }
    OLED_ShowTetromimo();
}

/**
  * @brief  �ĸ񷽿����䲢ˢ����ʾ
  * @param  ��
  * @retval n==0ʱ�������ƶ���n==1ʱ���������ƶ�
  */
uint8_t DMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][1] -= 1;
        if (Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // �ж��Ƿ񳬳��߿��ײ�����з���
            n++;
            break;
        }
    }
    if (n) {
        for (j = 0; j <= i; j++) Tetromino[j][1] += 1;
    }
    OLED_ShowTetromimo();
    return n;
}

/**
  * @brief  �ĸ񷽿���׹��ˢ����ʾ�������䵽��ײ����Ϊֹ
  * @param  ��
  * @retval ��
  */
void DropTetromino(void)
{
    uint8_t i, j, n = 0;
    for (j = 0; j < 19 && n == 0; j++) {  // �������19�Σ�������ײ
        OLED_ClearTetromimo();
        for (i = 0; i < 4; i++) {
            Tetromino[i][1] -= 1;
            if (Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // �ж��Ƿ񳬳��߿��ײ�����з���
                n++;
                break;
            }
        }
        if (n) {
            for (j = 0; j <= i; j++) Tetromino[j][1] += 1;
        }
        OLED_ShowTetromimo();
    }
}

/**
  * @brief  �ж��Ƿ����п�����
  * @param  ��
  * @retval �������е���Ŀ�����Ϊ4
  */
uint8_t If_Eliminate(void)
{
    uint8_t i, j;
    uint8_t m = 0;  // �����и���֮��
    uint8_t n = 0;  // �����������е���Ŀ�����Ϊ4
    uint8_t Line[4];  // ����ɱ��������кţ���������
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 10; j++) m += MinoMap[j][i];
        if (m == 10) {
            Line[n] = i;  // ��¼����������           
            n++;
        }
        m = 0;  // ��m���ã����ܽ�������if�ڣ���ʹδ�������и��ź����ж�
    }
    if (n) {
        BlinkMino(Line, n);
        for (m = 0; m < n; m++) {
            for(i = Line[m]-m; i < 20; i++) {
                for (j = 0; j < 10; j++) MinoMap[j][i] = MinoMap[j][i+1];
                for (j = 0; j < 10; j++) {  // һ��һ��ˢ�£����춯��Ч��
                    if (MinoMap[j][i]) OLED_ShowMino(j, i);  // ��ʾ��ֵΪ1����
                    else OLED_ClearMino(j, i);  // �����ֵΪ0����
                }
            }
        }
    }
    return n;
}
    
/**
  * @brief  �ж���Ϸ�Ƿ�ʧ��
  * @param  ��
  * @retval ʧ�ܷ���1��ûʧ�ܷ���0
  */
uint8_t If_Alive(void)
{   
    if (MinoMap[3][20] || MinoMap[4][20] || MinoMap[5][20] || MinoMap[6][20]) { 
        BlinkAllMino();
        return 1;
    } else {
        return 0;
    }
}
