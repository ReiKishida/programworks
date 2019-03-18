//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _PEDESTAL_H_
#define _PEDESTAL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PEDESTAL (10)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************

typedef struct
{
	DWORD g_nNumMatModel = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
						//DWORD g_nNumMatModel;	//�}�e���A���̏��̐�

	D3DXVECTOR3 VtxMinPedestal;
	D3DXVECTOR3 VtxMaxPedestal;

	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X

	int g_nIndexShadow;	//�e�̔ԍ�
	int nType;
	bool bUse;

}PEDESTAL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPedestal(void);
void UninitPedestal(void);
void UpdatePedestal(void);
void DrawPedestal(void);

//bool CollisionBill(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);
bool CollisionPedestal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetPedestal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

PEDESTAL *GetPedestal(void);
#endif
