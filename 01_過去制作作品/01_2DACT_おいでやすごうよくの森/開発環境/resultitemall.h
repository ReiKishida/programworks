//=============================================================================
//
// ���U���g��ʂ̃A�C�e���������� [resultitemall.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _RESULTITEMALL_H_
#define _RESULTITEMALL_H_

#include "main.h"


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	RESULTTYPE_NUMBER = 0, //����
	RESULTTYPE_CHARACTER,
	RESULTTYPE_MAX
}RESULTTYPE;




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitResultItemAll(void);
void UnInitResultItemAll(void);
void UpdateResultItemAll(void);
void DrawResultItemAll(void);

void SetResultItemAll(int nNumCoin, int nNumVodka, int nNumSatsu, int nNumArey);

//void SetResultItemAllpos(D3DXVECTOR3 pos);

#endif
