////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����L���O���� [ranking.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//******************************************************************************************************************************
// �\���̂̒�`
//******************************************************************************************************************************
typedef enum
{
	RANKINGTYPE_SCOREBG1 = 0,	// �����L���O�w�i
	RANKINGTYPE_SCOREBG2,	// �����L���O�w�i
	RANKINGTYPE_SCOREBG3,	// �����L���O�w�i
	RANKINGTYPE_SCOREBG4,	// �����L���O�w�i
	RANKINGTYPE_SCOREBG5,	// �����L���O�w�i
	RANKINGTYPE_SCORE1,	// �����L���O
	RANKINGTYPE_SCORE2,	// �����L���O
	RANKINGTYPE_SCORE3,	// �����L���O
	RANKINGTYPE_SCORE4,	// �����L���O
	RANKINGTYPE_SCORE5,	// �����L���O
	RANKINGTYPE_SCORENUM1,	// �����L���O����
	RANKINGTYPE_SCORENUM2,	// �����L���O����
	RANKINGTYPE_SCORENUM3,	// �����L���O����
	RANKINGTYPE_SCORENUM4,	// �����L���O����
	RANKINGTYPE_SCORENUM5,	// �����L���O����
	RANKINGTYPE_MAX,
}RANKINGTYPE;
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	RANKINGTYPE nType;
	float fWidth;
	float fHeight;
	bool bUse;
}Ranking;
//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(D3DXVECTOR3 pos, D3DXCOLOR col, RANKINGTYPE nType, float fWidth, float fHeight);
void SortRanking(int nScore);

#endif
