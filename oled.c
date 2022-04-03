#include "stm32f10x.h"
#include "oled_font.h"
#include "tetris.h"
#include "oled.h"
#include "delay.h"

// 引脚配置
#define OLED_W_SCL(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x) GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

// 地址配置
#define OLED_IIC_ADDR         0x78

#define OLED_COMMAND_REG      0x00
#define OLED_DATA_REG         0x40

/**
  * @brief  引脚初始化
  * @param  无
  * @retval 无
  */
void OLED_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  IIC开始
  * @param  无
  * @retval 无
  */
void OLED_IIC_Start(void)
{   
    OLED_W_SDA(1);
	OLED_W_SCL(1);  // 可加延时产生动画效果，有待尝试
	OLED_W_SDA(0);  // 开始信号：CLK为高，DATA由高变低
	OLED_W_SCL(0);  // 钳住IIC总线，准备发送或接收数据
}

/**
  * @brief  IIC停止
  * @param  无
  * @retval 无
  */
void OLED_IIC_Stop(void)
{
    OLED_W_SCL(0);
    OLED_W_SDA(0);  // 可加延时产生动画效果，有待尝试
	OLED_W_SCL(1);
	OLED_W_SDA(1);  // 停止信号：CLK为高，DATA由低变高
}

/**
  * @brief  IIC发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_IIC_SendByte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++) {
		OLED_W_SDA(byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);    
	}
	OLED_W_SCL(1);  // 额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(OLED_IIC_ADDR);  // 从机地址
	OLED_IIC_SendByte(OLED_COMMAND_REG);  // 写命令
	OLED_IIC_SendByte(Command); 
	OLED_IIC_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(OLED_IIC_ADDR);  // 从机地址
	OLED_IIC_SendByte(OLED_DATA_REG);  // 写数据
	OLED_IIC_SendByte(Data);
	OLED_IIC_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);  // 设置Y位置
	OLED_WriteCommand(0x10 | (((X+2) & 0xF0) >> 4));  // 设置X位置低4位
	OLED_WriteCommand(0x00 | ((X+2) & 0x0F));  // 设置X位置高4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++) {
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++) {
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	for (i = 0; i < 1000; i++) {  // 上电延时
		for (j = 0; j < 1000; j++);
	}
	
	OLED_IIC_Init();  // 端口初始化
	
	OLED_WriteCommand(0xAE);  // 关闭显示
	
	OLED_WriteCommand(0xD5);  // 设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);  // 设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);  // 设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);  // 设置显示开始行
	
	OLED_WriteCommand(0xA1);  // 设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);  // 设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);  // 设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);  // 设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);  // 设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);  // 设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);  // 设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);  // 设置正常/倒转显示

	OLED_WriteCommand(0x8D);  // 设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);  // 开启显示
		
	OLED_Clear();  // OLED清屏
}

/**
  * @brief  OLED显示游戏边框
  * @param  无
  * @retval 无
  */
void OLED_ShowEdge(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j++) {
		OLED_SetCursor(j, 0);
        for(i = 0; i < 128; i++) {
            if (j == 0 && i > 3 && i < 124) {
                OLED_WriteData(0x03);
                if (i - (i/6)*6 == 4) OLEDMap[j][(i-4)/6] |= 0x03;
            } else if (j ==7 && i > 3 && i < 124) {
                OLED_WriteData(0xC0);
                if (i - (i/6)*6 == 4) OLEDMap[j][(i-4)/6] |= 0xC0;
            } else if (j != 0 && j != 7 && i > 3 && i < 124) {
                OLED_WriteData(0x00);
            } else {
                OLED_WriteData(0xFF);
            }
        }
	}
}

/**
  * @brief  OLED显示游戏已有方块，即恢复界面
  * @param  无
  * @retval 无
  */
void OLED_ShowFace(void)
{
    uint8_t Line, Colunm, n;
    for (Line = 0; Line < 8; Line++) {
        for (Colunm = 0; Colunm < 20; Colunm++) {
            if (OLEDMap[Line][Colunm] != 0x00) {
                OLED_SetCursor(Line, 6*Colunm+4);
                for (n = 0; n < 6; n++) OLED_WriteData(OLEDMap[Line][Colunm]);
            }
        }
    }
}

/**
  * @brief  OLED显示一个方块
  * @param  MinoLine 方块行位置，范围：0~9
  * @param  MinoColumn 方块列位置，范围：0~20，20时不输出
  * @retval 无
  */
void OLED_ShowMino(uint8_t MinoLine, uint8_t MinoColunm)
{
    uint8_t Y, X, Line, Colunm, n;
    MinoMap[MinoLine][MinoColunm] = 1;  // 将方块显示位置置1 
    Y = 6 * MinoLine + 2;  // 求像素行位置
    X = 6 * MinoColunm + 4;  // 求像素列位置
    Line = Y / 8;  // 求地图行位置，即页数
    Colunm = MinoColunm;  // 求地图列位置，即方块列数
    
    OLEDMap[Line][Colunm] |= 0x3F << (Y - (Y/8) * 8);  // 先根据余数进行偏移，再和地图原本的像素点进行或运算
    OLED_SetCursor(Line, X);
    for (n = 0; (n < 6) && (Colunm < 20); n++) OLED_WriteData(OLEDMap[Line][Colunm]);
   
    if (Y - (Y/8) * 8 > 2) {  // 判断方块是否跨页
        OLEDMap[Line+1][Colunm] |= 0x3F >> (7 + (Y/8) * 8 - Y);
        OLED_SetCursor(Line+1, X);
        for (n = 0; (n < 6) && (Colunm < 20); n++) OLED_WriteData(OLEDMap[Line+1][Colunm]);
    }
}

/**
  * @brief  OLED消除一个方块
  * @param  MinoLine 方块行位置，范围：0~9
  * @param  MinoColumn 方块列位置，范围：0~20，20时不输出
  * @retval 无
  */
void OLED_ClearMino(uint8_t MinoLine, uint8_t MinoColunm)
{
    uint8_t Y, X, Line, Colunm, n;
    MinoMap[MinoLine][MinoColunm] = 0;  // 将方块显示位置置0
    Y = 6 * MinoLine + 2;  // 求像素行位置
    X = 6 * MinoColunm + 4;  // 求像素列位置
    Line = Y / 8;  // 求地图行位置，即页数
    Colunm = MinoColunm;  // 求地图列位置，即方块列数

    OLEDMap[Line][Colunm] &= ~(0x3F << (Y - (Y/8) * 8));  // 先根据余数进行偏移，取反后再和地图原本的像素点进行与运算，即进行显示方块的逆操作
    OLED_SetCursor(Line, X);
    for (n = 0; (n < 6) && (Colunm < 20); n++) OLED_WriteData(OLEDMap[Line][Colunm]);
   
    if (Y - (Y/8) * 8 > 2) {  // 判断方块是否跨页
        OLEDMap[Line+1][Colunm] &= ~(0x3F >> (7 + (Y/8) * 8 - Y));
        OLED_SetCursor(Line+1, X);
        for (n = 0; (n < 6) && (Colunm < 20); n++) OLED_WriteData(OLEDMap[Line+1][Colunm]);
    }
}
 
/**
  * @brief  OLED显示一个四格方块
  * @param  无
  * @retval 无
  */
void OLED_ShowTetromimo(void)
{
    uint8_t i;
    for (i = 0; i < 4; i++) OLED_ShowMino(Tetromino[i][0], Tetromino[i][1]);
}
 
/**
  * @brief  OLED消除一个四格方块
  * @param  无
  * @retval 无
  */
void OLED_ClearTetromimo(void)
{
    uint8_t i;
    for (i = 0; i < 4; i++) OLED_ClearMino(Tetromino[i][0], Tetromino[i][1]);
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 字符行位置，范围：1~8
  * @param  Column 字符列位置，范围：1~8
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Column - 1), (Line - 1) * 16);		//设置光标
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始字符行位置，范围：1~8
  * @param  Column 起始字符列位置，范围：1~8
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED居中显示数字（十进制，正数）
  * @param  Line 起始字符行位置，范围：1~8
  * @param  Number 要显示的数字，范围：0~99999999
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint32_t Number)
{
	uint8_t i, Length = 0, Column;
    uint32_t tmp = Number;
    do {
        Length++;
        tmp /= 10;
    } while (tmp);
    Column = 5 - Length/2;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
        Delay_ms(100);
	}
}
