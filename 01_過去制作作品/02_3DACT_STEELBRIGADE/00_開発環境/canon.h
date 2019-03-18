//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _CANON_H_
#define _CANON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CANON (5)
#define MAXTYPE_CANON (2)

//*****************************************************************************
// ���f���̍\����
//*****************************************************************************
typedef enum
{
	CANONTYPE_G,
	CANONTYPE_C,
	CANONTYPE_MAX
}CANONTYPE;

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̐�
	D3DXVECTOR3 VtxMinModelWall = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModelWall = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
}CANONMODEL;

typedef struct
{
	DWORD g_nNumMatModel = 0;	//�}�e���A���̏��̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
						//DWORD g_nNumMatModel;	//�}�e���A���̏��̐�
	CANONTYPE type;

	D3DXVECTOR3 VtxMinCanon;
	D3DXVECTOR3 VtxMaxCanon;

	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nLife;
	int g_nIndexShadow;	//�e�̔ԍ�
	int nType;
	bool bUse;
	CANONMODEL aCanonModel[MAX_CANON];
}CANON;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCanon(void);
void UninitCanon(void);
void UpdateCanon(void);
void DrawCanon(void);
void HitCanon(int nDamage, int nCntCanon);

bool CollisionCanon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void SetCanon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife);

CANON *GetCanon(void);
#endif
