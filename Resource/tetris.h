#ifndef __TETRIS_H
#define __TETRIS_H

void GenerateTetromino(void);
void RotateTetromino(void);
void LMoveTetromino(void);
void RMoveTetromino(void);
uint8_t DMoveTetromino(void);
void DropTetromino(void);
uint8_t If_Eliminate(void);
uint8_t If_Alive(void);

#endif
