//=============================================================================
//
// �X�R�A���� [Ranking.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _MENURANKING_H_
#define _MENURANKING_H_

#include "main.h"


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	MENURANKINGTYPE_NUMBER = 0, //����
	MENURANKINGTYPE_CHARACTER,
	MENURANKINGTYPE_MAX
}MENURANKINGTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;
	bool bUse;
	int nType;
	int g_aRankingNumber;
	int nRankScore;
}MENURANKING;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMenuRanking(void);
void UnInitMenuRanking(void);
void UpdateMenuRanking(void);
void DrawMenuRanking(void);

void SetMenuRanking(int nScore);

//void SetRankingpos(D3DXVECTOR3 pos);

#endif
