//=============================================================================
//
// ���f������ [model.h]
// Author :
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODEL (11)
#define MAXTYPE_MODEL (4)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̐�
	D3DXVECTOR3 VtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}OBJECT;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 VtxMinModel;
	D3DXVECTOR3 VtxMaxModel;
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int g_nIndexShadow;	//�e�̔ԍ�
	int nType;
	bool bUse;
	OBJECT aObject[MAX_MODEL];
}MODEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

MODEL *GetModel(void);
#endif
