#include "stm32f10x.h"	// Device header
#include "controller.h"

int main(void)
{
    while (1) {  // ��Ϸһ��ѭ������Ϊһ����ѭ����ֱ���˳���Ϸʱѭ������
        Start();  // ��ʼ����        
        DrawGame();  // ������Ϸ����
        PlayGame(); // ������Ϸ����ѭ��
        End();
    }   
}
