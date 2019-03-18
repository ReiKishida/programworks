//=============================================================================
//
// ����ʏ��� [tips.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _Tips_H_
#define _Tips_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	TIPSSTATE_NONE = 0,	
	TIPSSTATE_USE,
	TIPSSTATE_MAX
}TIPS_STATE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���|
	int nType;			//���
	TIPS_STATE state;
}TIPS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTips(void);
void UnInitTips(void);
void UpdateTips(void);
void DrawTips(void);


#endif


