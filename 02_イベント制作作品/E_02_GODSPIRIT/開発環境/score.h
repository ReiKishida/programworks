////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�R�A���� [score.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	SCORETEX_SCORE_BG = 0,	// �X�R�A�̔w�i
	SCORETEX_SCORE_NAME,	// �X�R�A���O
	SCORETEX_SCORE,			// �X�R�A
	SCORETEX_MAX,			// �ő吔
}SCORETEX;
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	SCORETEX nType;			// ���
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p����Ă��邩�ǂ���
}Score;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, SCORETEX nType, float fWidth, float fHeight);
int AddScore(int nValue);
#endif
