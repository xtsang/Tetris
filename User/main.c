#include "stm32f10x.h"	// Device header
#include "controller.h"

int main(void)
{
    while (1) {  // 游戏一级循环可视为一个死循环，直到退出游戏时循环结束
        Start();  // 开始界面        
        DrawGame();  // 绘制游戏界面
        PlayGame(); // 开启游戏二级循环
        End();
    }   
}
