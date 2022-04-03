#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "animation.h"

/**
  * @brief  OLED闪烁选中列内的方块，制造消除动画效果
  * @param  MinoColunm[] 可被消除的列号，范围：0~19
  * @param  n 可消除列的数目，范围：1~4
  * @retval 无
  */
void BlinkMino(uint8_t MinoColunm[], uint8_t n)
{
    uint8_t i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 10; j++) OLED_ClearMino(j, MinoColunm[i]);
    }
    Delay_ms(250);
    for (i = 0; i < n; i++) {
        for (j = 0; j < 10; j++) OLED_ShowMino(j, MinoColunm[i]);
    }
    Delay_ms(250);
    for (i = 0; i < n; i++) {
        for (j = 0; j < 10; j++) OLED_ClearMino(j, MinoColunm[i]);
    }
    Delay_ms(250);
}

/**
  * @brief  OLED闪烁选中所有方块，制造死亡动画效果
  * @param  无
  * @retval 无
  */
void BlinkAllMino(void)
{
    uint8_t i, j, m = 20, n = 9, step;
    for (j = 0; j < 10; j++) OLED_ClearMino(j, 20);  // 清除死亡标记
    for (j = 0; j < 10; j++) {
        for (i = 0; i < 20; i++) OLED_ShowMino(j, i);
    }
    i = j = 0;
    while (1) {
        for (step = 0; step < m; ++step && i++) {
            OLED_ClearMino(j, i);
            Delay_ms(30);
        }
        m--;  // 减少次数
        i--;  // 移动20次，但坐标只改变19次，即回到最后打印点
        if (!n) break;  // 不需要横向移动意味着结束，退出循环
        j++;  // 横坐标移动，防止重复打印
        for (step = 0; step < n; ++step && j++) {
            OLED_ClearMino(j, i);
            Delay_ms(30);
        }
        n--;
        j--;
        if (!m) break;
        i--;
        for (step = 0; step < m; ++step && i--) {
            OLED_ClearMino(j, i);
            Delay_ms(30);
        }
        m--;
        i++;
        if (!n) break;
        j--;
        for (step = 0; step < n; ++step && j--) {
            OLED_ClearMino(j, i);
            Delay_ms(30);
        }
        n--;
        j++;
        if (!m) break;
        i++;
    }
}

/**
  * @brief  开始动画
  * @param  无
  * @retval 无
  */
void ShowStart(void)
{  
    char n[] = "Any Key";
    OLED_ShowMino(1, 16);  // T
    OLED_ShowMino(2, 16);
    OLED_ShowMino(3, 16);
    OLED_ShowMino(2, 15);
    OLED_ShowMino(2, 14);
    OLED_ShowMino(2, 13);
    OLED_ShowMino(2, 12);
    Delay_ms(100);
    OLED_ShowMino(4, 16);  // E
    OLED_ShowMino(5, 16);
    OLED_ShowMino(6, 16);
    OLED_ShowMino(4, 15);
    OLED_ShowMino(4, 14);
    OLED_ShowMino(5, 14);
    OLED_ShowMino(6, 14);
    OLED_ShowMino(4, 13);
    OLED_ShowMino(4, 12);
    OLED_ShowMino(5, 12);
    OLED_ShowMino(6, 12);
    Delay_ms(100);
    OLED_ShowMino(7, 16);  // T
    OLED_ShowMino(8, 16);
    OLED_ShowMino(9, 16);
    OLED_ShowMino(8, 15);
    OLED_ShowMino(8, 14);
    OLED_ShowMino(8, 13);
    OLED_ShowMino(8, 12);
    Delay_ms(100);
    OLED_ShowMino(1, 10);  // R
    OLED_ShowMino(2, 10);
    OLED_ShowMino(3, 10);
    OLED_ShowMino(1, 9);
    OLED_ShowMino(3, 9);
    OLED_ShowMino(1, 8);
    OLED_ShowMino(2, 8);
    OLED_ShowMino(3, 8);
    OLED_ShowMino(1, 7);
    OLED_ShowMino(2, 7);
    OLED_ShowMino(1, 6);
    OLED_ShowMino(3, 6);
    Delay_ms(100);
    OLED_ShowMino(4, 10);  // I
    OLED_ShowMino(5, 10);
    OLED_ShowMino(6, 10);
    OLED_ShowMino(5, 9);
    OLED_ShowMino(5, 8);
    OLED_ShowMino(5, 7);
    OLED_ShowMino(4, 6);
    OLED_ShowMino(5, 6);
    OLED_ShowMino(6, 6);
    Delay_ms(100);
    OLED_ShowMino(7, 10);  // S
    OLED_ShowMino(8, 10);
    OLED_ShowMino(9, 10);
    OLED_ShowMino(7, 9);
    OLED_ShowMino(7, 8);
    OLED_ShowMino(8, 8);
    OLED_ShowMino(9, 8);
    OLED_ShowMino(9, 7);
    OLED_ShowMino(7, 6);
    OLED_ShowMino(8, 6);
    OLED_ShowMino(9, 6);
    Delay_ms(100);
    OLED_ShowString(1, 2, n);
}

/**
  * @brief  清除开始动画
  * @param  无
  * @retval 无
  */
void ClearStart(void)
{  
    OLED_ClearMino(1, 16);  // T
    OLED_ClearMino(2, 16);
    OLED_ClearMino(3, 16);
    OLED_ClearMino(2, 15);
    OLED_ClearMino(2, 14);
    OLED_ClearMino(2, 13);
    OLED_ClearMino(2, 12);
    
    OLED_ClearMino(4, 16);  // E
    OLED_ClearMino(5, 16);
    OLED_ClearMino(6, 16);
    OLED_ClearMino(4, 15);
    OLED_ClearMino(4, 14);
    OLED_ClearMino(5, 14);
    OLED_ClearMino(6, 14);
    OLED_ClearMino(4, 13);
    OLED_ClearMino(4, 12);
    OLED_ClearMino(5, 12);
    OLED_ClearMino(6, 12);
    
    OLED_ClearMino(7, 16);  // T
    OLED_ClearMino(8, 16);
    OLED_ClearMino(9, 16);
    OLED_ClearMino(8, 15);
    OLED_ClearMino(8, 14);
    OLED_ClearMino(8, 13);
    OLED_ClearMino(8, 12);
    
    OLED_ClearMino(1, 10);  // R
    OLED_ClearMino(2, 10);
    OLED_ClearMino(3, 10);
    OLED_ClearMino(1, 9);
    OLED_ClearMino(3, 9);
    OLED_ClearMino(1, 8);
    OLED_ClearMino(2, 8);
    OLED_ClearMino(3, 8);
    OLED_ClearMino(1, 7);
    OLED_ClearMino(2, 7);
    OLED_ClearMino(1, 6);
    OLED_ClearMino(3, 6);
    
    OLED_ClearMino(4, 10);  // I
    OLED_ClearMino(5, 10);
    OLED_ClearMino(6, 10);
    OLED_ClearMino(5, 9);
    OLED_ClearMino(5, 8);
    OLED_ClearMino(5, 7);
    OLED_ClearMino(4, 6);
    OLED_ClearMino(5, 6);
    OLED_ClearMino(6, 6);
    
    OLED_ClearMino(7, 10);  // S
    OLED_ClearMino(8, 10);
    OLED_ClearMino(9, 10);
    OLED_ClearMino(7, 9);
    OLED_ClearMino(7, 8);
    OLED_ClearMino(8, 8);
    OLED_ClearMino(9, 8);
    OLED_ClearMino(9, 7);
    OLED_ClearMino(7, 6);
    OLED_ClearMino(8, 6);
    OLED_ClearMino(9, 6);
}

/**
  * @brief  显示成绩
  * @param  无
  * @retval 无
  */
void ShowScore(uint16_t line, uint8_t level, uint16_t score)
{
    char a[] = "line";
    char b[] = "level";
    char c[] = "score";
    char n[] = "Any Key";
    OLED_Clear();
    OLED_ShowString(8, 3, a);
    OLED_ShowNum(7, line);
    OLED_ShowString(6, 3, b);
    OLED_ShowNum(5, level);
    OLED_ShowString(4, 3, c);
    OLED_ShowNum(3, score);
    OLED_ShowString(1, 2, n);
}
