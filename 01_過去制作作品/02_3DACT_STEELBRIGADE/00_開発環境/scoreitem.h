//=============================================================================
//
// �X�R�A�A�C�e������ [scoreitem.h]
// Author : 
//
//=============================================================================
#ifndef _ScoreItem_H_
#define _ScoreItem_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCOREITEM (20)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatScoreItem = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
						//DWORD g_nNumMatScoreItem;	//�}�e���A���̏��̐�

	D3DXVECTOR3 VtxMinScoreItem;
	D3DXVECTOR3 VtxMaxScoreItem;

	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X

	int nIndexShadow;	//�e�̔ԍ�
	int nType;
	bool bUse;
}SCOREITMEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitScoreItem(void);
void UninitScoreItem(void);
void UpdateScoreItem(void);
void DrawScoreItem(void);

bool CollisionScoreItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetScoreItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

SCOREITMEM *GetScoreItem(void);
#endif
