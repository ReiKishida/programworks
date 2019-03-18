//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODELWALL_H_
#define _MODELWALL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODELWALL (15)
#define MAXTYPE_MODELWALL (5)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̐�
	D3DXVECTOR3 VtxMinModelWall = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModelWall = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}MODELWALL;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
						//DWORD g_nNumMatModel;	//�}�e���A���̏��̐�

	D3DXVECTOR3 VtxMinModelWall;
	D3DXVECTOR3 VtxMaxModelWall;

	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X

	int g_nIndexShadow;	//�e�̔ԍ�
	int nCntWall;
	int nType;
	bool bUse;
	MODELWALL aBuilding[MAX_MODELWALL];
}WALL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitModelWall(void);
void UninitModelWall(void);
void UpdateModelWall(void);
void DrawModelWall(void);

bool CollisionModelWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetModelWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

WALL *GetModelWall(void);
#endif
