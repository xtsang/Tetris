#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"

#define GPIO_A(x)  GPIO_ReadInputDataBit(GPIOA, (((uint16_t)0x0001) << x))

uint8_t KeyNum = 5;

/**
  * @brief  引脚初始化
  * @param  无
  * @retval 无
  */
void Key_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;  // 该结构体实际上也是一种局部变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 时钟开启

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;  // 0暂停1旋转2左移3右移4下坠
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);  // 第二个参数是一个结构体的地址

	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
}

uint8_t Key_GetNum(void)
{
    if (KeyNum == 5) {
        if (GPIO_A(0) == 0) {  // 依次检测是否有按键被按下
            Delay_ms(20);
            if (GPIO_A(0) == 0) {  // 防止干扰
                KeyNum = 0;     // 修改键值
                return KeyNum;  // 刚被按下则返回真实键值
            }
        }
        if (GPIO_A(1) == 0) {
            Delay_ms(20);
            if (GPIO_A(1) == 0) {
                KeyNum = 1;
                return KeyNum;
            }
        }
        if (GPIO_A(2) == 0) {
            Delay_ms(20);
            if (GPIO_A(2) == 0) {
                KeyNum = 2;
                return KeyNum;
            }
        }
        if (GPIO_A(3) == 0) {
            Delay_ms(20);
            if (GPIO_A(3) == 0) {
                KeyNum = 3;
                return KeyNum;
            }
        }
        if (GPIO_A(4) == 0) {
            Delay_ms(20);
            if (GPIO_A(4) == 0) {
                KeyNum = 4;
                return KeyNum;
            }
        }
        return KeyNum;  //无按键则返回5
    } else {
        if (GPIO_A(KeyNum) == 1) KeyNum = 5;  // 无按键则修改键值
        return 5;  // 长按则返回虚假键值，无按键返回真实键值
    }
}
