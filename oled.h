#ifndef __OLED_H
#define __OLED_H

void OLED_Clear(void);
void OLED_Init(void);
void OLED_ShowEdge(void);
void OLED_ShowFace(void);
void OLED_ShowMino(uint8_t, uint8_t);
void OLED_ClearMino(uint8_t, uint8_t);
void OLED_ShowTetromimo(void);
void OLED_ClearTetromimo(void);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint32_t Number);

#endif
