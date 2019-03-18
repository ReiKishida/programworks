//=============================================================================
//
// �X�R�A���� [Result.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

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
	int g_aResultNumber;
	int nResultScore;
}RESULT;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitResult(void);
void UnInitResult(void);
void UpdateResult(void);
void DrawResult(void);

void SetResult(int nScore);


#endif
