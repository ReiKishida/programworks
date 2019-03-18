//=============================================================================
//
// �|���S���̏��� [Cursor.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _Cursor_H_
#define _Cursor_H_

#include "main.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	CURSORSTATE_NORMAL = 0,	//�ʏ���
	CURSORSTATE_OVERLAP,		//�d�Ȃ���
	
	CursorYSTATE_MAX			//��ނ̑���
}CURSORSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	CURSORSTATE state;	//�v���C���[�̏��
	bool bDisp;			//�g�p����Ă��邩
	bool bUpdate;
	int nIndex;
}CURSOR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCursor(void);
void UnInitCursor(void);
void UpdateCursor(void);
void DrawCursor(void);

CURSOR GetCursor000(void);


#endif