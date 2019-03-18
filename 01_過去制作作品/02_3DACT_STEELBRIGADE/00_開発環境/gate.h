//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _GATE_H_
#define _GATE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_GATE (3)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatModel = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 VtxMinGate;
	D3DXVECTOR3 VtxMaxGate;
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int g_nIndexShadow;	//�e�̔ԍ�
	int nType;
	bool bUse;
	int nLife;
	int nCnt;
}GATE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGate(void);
void UninitGate(void);
void UpdateGate(void);
void DrawGate(void);

bool CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);

void HitGate(int nDamage, int nCntGate);

GATE *GetGate(void);
#endif
