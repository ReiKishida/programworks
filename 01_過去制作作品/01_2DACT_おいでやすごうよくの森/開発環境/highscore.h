//=============================================================================
//
// �n�C�X�R�A���� [highscore.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

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
	int g_aHighScoreNumber;
	int nHighScore;
}HIGHSCORE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitHighScore(void);
void UnInitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);

void SetHighScore(int nScore);


#endif
