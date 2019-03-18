//=============================================================================
//
// �n�C�X�R�A���� [highscore.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _GAMEHIGHSCORE_H_
#define _GAMWHIGHSCORE_H_

#include "main.h"


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;
	bool bUse;
	int nType;
	int g_aGameHighScoreNumber;
	int nGameHighScore;
}GAMEHIGHSCORE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGameHighScore(void);
void UnInitGameHighScore(void);
void UpdateGameHighScore(void);
void DrawGameHighScore(void);



void SetGameHighScore(int nScore);


#endif
