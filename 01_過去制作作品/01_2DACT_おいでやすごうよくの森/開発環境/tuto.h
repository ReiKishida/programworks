//=============================================================================
//
// ����ʏ��� [tuto.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _TUTO_H_
#define _TUTO_H_

#include "main.h"
#include "input.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	TUTOSTATE_NONE = 0,
	TUTOSTATE_USE,
	TUTOSTATE_MAX
}TUTO_STATE;

typedef enum
{
	PASE_ONE = 0,
	PASE_TWO,
	PASE_THREE,
	PASE_FOUR,
	PASE_FIVE,
	PASE_MAX
}PASE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�J���|
	int nType;			//���
	TUTO_STATE state;
	PASE pase;
	bool bUse;
}TUTO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTuto(void);
void UnInitTuto(void);
void UpdateTuto(void);
void DrawTuto(void);


#endif


