#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "animation.h"
#include "tetris.h"
#include "controller.h"

uint16_t line = 0;
uint8_t level = 0;
uint32_t score = 0;  // ���÷֣�65535

// ��������֡�ʱ�
const uint8_t FrameTable[] = {53, 49, 45, 41, 37, 33, 28, 22, 17, 11, 10, 9, 8, 7, 6, 6, 5, 5, 4, 4, 3};

/**
  * @brief  �������ж��Ƿ�������������
  * @param  ��
  * @retval ��
  */
void Timer2CountInitial(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 6000 - 1;  // �Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 12000 - 1;  // ʱ��Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���

    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);  
    TIM_Cmd(TIM2, ENABLE);
}    
    
/**
  * @brief  �������ж��Ƿ�������������
  * @param  ��
  * @retval 1������������������0����������������
  */
uint8_t If_Time(void)
{
    if (TIM_GetCounter(TIM2) > 100 * FrameTable[level]-1) {  // ��ע�⣬��������֡�������е�����Ҫ��д����ֵ
        TIM_SetCounter(TIM2, 0);  // ����������
        return 1;
    } else {
        return 0;
    }
}

/**
  * @brief  ���·���������������
  * @param  ����������
  * @retval ��
  */
void UpdateScore(uint8_t i)
{
    if (i) {
        line += i;
        switch (i) {
          case 1:
            score += 4 * (level+1);  // ������Ϸ�ȼ��÷ַ���
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
        if (line%10 == 0) {  // ��������10�ı������ȼ�����
            level++;
            //TIM_SetAutoreload(TIM2, 100 * FrameTable[level]-1);  // �Զ���װֵ�����޸�
        }
    }
}

/**
  * @brief  ��ʾ��������
  * @param  ��
  * @retval ��
  */
void Start(void)
{
    OLED_Init();
	Key_Init();
    Timer2CountInitial();  // ��ʼ��ʱ
	ShowStart();
	while (Key_GetNum() == 5) {};
    ClearStart();
}

/**
  * @brief  ��ʾ������������
  * @param  ��
  * @retval ��
  */
void End(void)
{
    OLED_Clear();
	while (Key_GetNum() == 5) {};
}
/**
  * @brief  ������Ϸ����
  * @param  ��
  * @retval ��
  */
void DrawGame(void)
{
    OLED_Clear();
    OLED_ShowEdge();
}

/**
  * @brief  ������ͣ����
  * @param  ��
  * @retval ��
  */
void Menu(void)
{
    ShowScore(line, level, score);
	while (Key_GetNum() == 5) {};
    OLED_ShowEdge();
    OLED_ShowFace();
}

/**
  * @brief  ���ƽ�������
  * @param  ��
  * @retval ��
  */
void GameOver(void)
{
    ShowScore(line, level, score);
	while (Key_GetNum() == 5) {};
}

/**
  * @brief  ��Ϸ����ѭ��
  * @param  ��
  * @retval ��
  */
void PlayGame(void)
{
    line = level = score = 0;  // ��������
    GenerateTetromino();
    TIM_SetCounter(TIM2, 0);  // ���������㣬���¿�ʼ��ʱ
    while (1) {
        if (If_Time() && DMoveTetromino()) {
            UpdateScore(If_Eliminate());  // ���������ƶ�ʱ�ȼ���Ƿ��������ͬʱ���·���
            if (If_Alive()) goto OVER;  // �ٿ�ʼ�ж��Ƿ������������������ѭ��
            GenerateTetromino();  // ��������µķ���
            TIM_SetCounter(TIM2, 0);  // ���������㣬���¿�ʼ��ʱ
        }
        switch (Key_GetNum()) {
		  case 5:
            break;
          case 0:
			TIM_Cmd(TIM2, DISABLE);  // ��ⰴ�����Ƿ񻽳���ͣ�˵�
		    Menu();
            Timer2CountInitial();  // ���¿�ʼ��ʱ
            break;
          case 1:
            RotateTetromino();  // ��ⰴ�����Ƿ���ת
            break;
          case 2:
            LMoveTetromino();  // ��ⰴ�����Ƿ�����
            break;
          case 3:
            RMoveTetromino();  // ��ⰴ�����Ƿ�����
            break;
          case 4:
            DropTetromino();  // ��ⰴ�����Ƿ���׹
            UpdateScore(If_Eliminate());  // ���������ƶ�ʱ�ȼ���Ƿ��������ͬʱ���·���
            if (If_Alive()) goto OVER;  // �ٿ�ʼ�ж��Ƿ������������������ѭ��         
            GenerateTetromino();  // ��������µķ���
            TIM_SetCounter(TIM2, 0);  // ���������㣬���¿�ʼ��ʱ
            break;   
        }    
    }
    
	OVER:
    TIM_Cmd(TIM2, DISABLE);  // ��Ϸ������������ʱ��Ϸ������������ʱ  
    GameOver();
}
