#include "stm32f10x.h"
#include "oled.h"
#include "stdlib.h"
#include "animation.h"
#include "tetris.h"

// 方块显示数组，21列只用于判断死亡，不用于显示
uint8_t MinoMap[10][21] = {0};

// 随机数生成表，检测是否完成一组
uint8_t RandTable[8] = {0, 1, 2, 3, 4, 5, 6, 0};

// 控制的四方块显示数组，0为MinoLine，1为MinoColumn，需要判定是否为负，故设成有符号数，Tetromino[4][0]代表四方块种类，Tetromino[4][1]代表旋转次数
int8_t Tetromino[5][2] = {0};

// 旋转表，查表以改变坐标
const int8_t RotateTable[7][4][8] = {
    0, 0,   0, 0,   0, 0,   0, 0,  // 第一次旋转
    0, 0,   0, 0,   0, 0,   0, 0,  // 第二次旋转
    0, 0,   0, 0,   0, 0,   0, 0,  // 第三次旋转
    0, 0,   0, 0,   0, 0,   0, 0,  // 第四次旋转，回到初始
    
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
  * @brief  生成0~6随机数字，7个一包。注：现代俄罗斯方块是以7个为1包(1 Bag)出现的,对于每一包,随机顺序出现方块I，O，T，L，J，S，Z
  * @param  无
  * @retval 无
  */
uint8_t Randomizer(void)
{
    uint8_t i, n;
    do {
        srand(TIM_GetCounter(TIM2));
        i = rand()%7;
    } while (RandTable[i] == 7);  // 如果是已出现的方块则重新生成
    RandTable[i] = 7;
    if (++RandTable[7] == 7) {  // 一包用完，则重置随机数
        RandTable[7] = 0;
        for (n = 0; n < 7; n++) RandTable[n] = n;
    }
    return i;
}

/**
  * @brief  生成一个四格方块并显示
  * @param  四格方块的种类，范围：0~6
  * @retval 无
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
  * @brief  四格方块进行旋转并刷新显示
  * @param  无
  * @retval 无
  */
void RotateTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] += RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*i+0];
        Tetromino[i][1] += RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*i+1];
        if (Tetromino[i][0] < 0 || Tetromino[i][0] > 9 || Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // 判定是否超出边框或撞到已有方块
            n++;
            break;
        }
    }
    Tetromino[4][1]++;  // 增加旋转次数
    if (n) {  // 如果不能旋转则回退
        Tetromino[4][1]--;
        for (j = 0; j <= i; j++) {
            Tetromino[j][0] -= RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*j+0];
            Tetromino[j][1] -= RotateTable[Tetromino[4][0]][Tetromino[4][1]][2*j+1];
        }
    }
    if (Tetromino[4][1] == 4) Tetromino[4][1] = 0;  // 转四次则回到初始状态
    OLED_ShowTetromimo();
}

/**
  * @brief  四格方块向左移动并刷新显示
  * @param  无
  * @retval 无
  */
void LMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] -= 1;
        if (Tetromino[i][0] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // 判定是否超出边框或撞到已有方块
            n++;
            break;
        }
    }
    if (n) {
        for (j = 0; j <= i; j++) Tetromino[j][0] += 1;  // 如果不能移动则回退
    }
    OLED_ShowTetromimo();
}

/**
  * @brief  四格方块向右移动并刷新显示
  * @param  无
  * @retval 无
  */
void RMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][0] += 1;
        if (Tetromino[i][0] > 9 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // 判定是否超出边框或撞到已有方块
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
  * @brief  四格方块下落并刷新显示
  * @param  无
  * @retval n==0时能向下移动，n==1时不能向下移动
  */
uint8_t DMoveTetromino(void)
{
    uint8_t i, j, n = 0;
    OLED_ClearTetromimo();
    for (i = 0; i < 4; i++) {
        Tetromino[i][1] -= 1;
        if (Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // 判定是否超出边框或撞到已有方块
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
  * @brief  四格方块下坠并刷新显示，即下落到碰撞底面为止
  * @param  无
  * @retval 无
  */
void DropTetromino(void)
{
    uint8_t i, j, n = 0;
    for (j = 0; j < 19 && n == 0; j++) {  // 最多下落19次，且无碰撞
        OLED_ClearTetromimo();
        for (i = 0; i < 4; i++) {
            Tetromino[i][1] -= 1;
            if (Tetromino[i][1] < 0 || MinoMap[Tetromino[i][0]][Tetromino[i][1]] == 1) {  // 判定是否超出边框或撞到已有方块
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
  * @brief  判断是否有列可消除
  * @param  无
  * @retval 可消除列的数目，最多为4
  */
uint8_t If_Eliminate(void)
{
    uint8_t i, j;
    uint8_t m = 0;  // 计数列各数之和
    uint8_t n = 0;  // 计数可消除列的数目，最多为4
    uint8_t Line[4];  // 保存可被消除的列号，用于消除
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 10; j++) m += MinoMap[j][i];
        if (m == 10) {
            Line[n] = i;  // 记录可消除的列           
            n++;
        }
        m = 0;  // 将m重置，不能将语句放在if内，会使未填满的列干扰后列判断
    }
    if (n) {
        BlinkMino(Line, n);
        for (m = 0; m < n; m++) {
            for(i = Line[m]-m; i < 20; i++) {
                for (j = 0; j < 10; j++) MinoMap[j][i] = MinoMap[j][i+1];
                for (j = 0; j < 10; j++) {  // 一列一列刷新，制造动画效果
                    if (MinoMap[j][i]) OLED_ShowMino(j, i);  // 显示新值为1方块
                    else OLED_ClearMino(j, i);  // 清除新值为0方块
                }
            }
        }
    }
    return n;
}
    
/**
  * @brief  判断游戏是否失败
  * @param  无
  * @retval 失败返回1，没失败返回0
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
