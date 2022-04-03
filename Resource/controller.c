#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "animation.h"
#include "tetris.h"
#include "controller.h"

uint16_t line = 0;
uint8_t level = 0;
uint32_t score = 0;  // 最大得分：65535

// 方块下落帧率表
const uint8_t FrameTable[] = {53, 49, 45, 41, 37, 33, 28, 22, 17, 11, 10, 9, 8, 7, 6, 6, 5, 5, 4, 4, 3};

/**
  * @brief  计数，判断是否满足下落条件
  * @param  无
  * @retval 无
  */
void Timer2CountInitial(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 6000 - 1;  // 自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 12000 - 1;  // 时钟预分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数

    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);  
    TIM_Cmd(TIM2, ENABLE);
}    
    
/**
  * @brief  计数，判断是否满足下落条件
  * @param  无
  * @retval 1代表满足下落条件，0代表不满足下落条件
  */
uint8_t If_Time(void)
{
    if (TIM_GetCounter(TIM2) > 100 * FrameTable[level]-1) {  // 需注意，若出现跳帧现象，则判等条件要多写几个值
        TIM_SetCounter(TIM2, 0);  // 计数器清零
        return 1;
    } else {
        return 0;
    }
}

/**
  * @brief  更新分数及总消除列数
  * @param  消除的列数
  * @retval 无
  */
void UpdateScore(uint8_t i)
{
    if (i) {
        line += i;
        switch (i) {
          case 1:
            score += 4 * (level+1);  // 根据游戏等级得分翻倍
            break;
          case 2:
            score += 10 * (level+1);
            break;
          case 3:
            score += 30 * (level+1);
            break;
          case 4:
            score += 120 * (level+1);
            break;
        }
        if (line%10 == 0) {  // 消除数到10的倍数，等级上升
            level++;
            //TIM_SetAutoreload(TIM2, 100 * FrameTable[level]-1);  // 自动重装值无需修改
        }
    }
}

/**
  * @brief  显示开场动画
  * @param  无
  * @retval 无
  */
void Start(void)
{
    OLED_Init();
	Key_Init();
    Timer2CountInitial();  // 开始计时
	ShowStart();
	while (Key_GetNum() == 5) {};
    ClearStart();
}

/**
  * @brief  显示结束待机界面
  * @param  无
  * @retval 无
  */
void End(void)
{
    OLED_Clear();
	while (Key_GetNum() == 5) {};
}
/**
  * @brief  绘制游戏界面
  * @param  无
  * @retval 无
  */
void DrawGame(void)
{
    OLED_Clear();
    OLED_ShowEdge();
}

/**
  * @brief  绘制暂停界面
  * @param  无
  * @retval 无
  */
void Menu(void)
{
    ShowScore(line, level, score);
	while (Key_GetNum() == 5) {};
    OLED_ShowEdge();
    OLED_ShowFace();
}

/**
  * @brief  绘制结束界面
  * @param  无
  * @retval 无
  */
void GameOver(void)
{
    ShowScore(line, level, score);
	while (Key_GetNum() == 5) {};
}

/**
  * @brief  游戏二级循环
  * @param  无
  * @retval 无
  */
void PlayGame(void)
{
    line = level = score = 0;  // 重置数据
    GenerateTetromino();
    TIM_SetCounter(TIM2, 0);  // 计数器清零，重新开始计时
    while (1) {
        if (If_Time() && DMoveTetromino()) {
            UpdateScore(If_Eliminate());  // 不能向下移动时先检测是否可消除，同时更新分数
            if (If_Alive()) goto OVER;  // 再开始判断是否死亡，如果死亡跳出循环
            GenerateTetromino();  // 最后生成新的方块
            TIM_SetCounter(TIM2, 0);  // 计数器清零，重新开始计时
        }
        switch (Key_GetNum()) {
		  case 5:
            break;
          case 0:
			TIM_Cmd(TIM2, DISABLE);  // 检测按键，是否唤出暂停菜单
		    Menu();
            Timer2CountInitial();  // 重新开始计时
            break;
          case 1:
            RotateTetromino();  // 检测按键，是否旋转
            break;
          case 2:
            LMoveTetromino();  // 检测按键，是否左移
            break;
          case 3:
            RMoveTetromino();  // 检测按键，是否右移
            break;
          case 4:
            DropTetromino();  // 检测按键，是否下坠
            UpdateScore(If_Eliminate());  // 不能向下移动时先检测是否可消除，同时更新分数
            if (If_Alive()) goto OVER;  // 再开始判断是否死亡，如果死亡跳出循环         
            GenerateTetromino();  // 最后生成新的方块
            TIM_SetCounter(TIM2, 0);  // 计数器清零，重新开始计时
            break;   
        }    
    }
    
	OVER:
    TIM_Cmd(TIM2, DISABLE);  // 游戏结束并结束计时游戏结束并结束计时  
    GameOver();
}
