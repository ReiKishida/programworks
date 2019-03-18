//=============================================================================
//
// ����ʏ��� [Briefing.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _BRIEFING_H_
#define _BRIEFING_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BRIEFINGSTATE_NONE = 0,
	BRIEFINGSTATE_USE,
	BRIEFINGSTATE_MAX
}BRIEFING_STATE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���|
	int nType;			//���
	BRIEFING_STATE state;
}BRIEFING;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBriefing(void);
void UnInitBriefing(void);
void UpdateBriefing(void);
void DrawBriefing(void);


#endif


