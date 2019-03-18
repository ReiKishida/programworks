//=============================================================================
//
// �A�C�e������ [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define MAX_ITEM (4)
#define MAX_TYPEITEM (4)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef enum
{
	HITSTATE_NONE = 0,
	HITSTATE_USE,
	HITSTATE_MAX
}HIT_STATE;
//state
typedef enum
{
	HITSTATUS_NONE = 0,
	HITSTATUS_NOW,
	HITSTATUS_NEXT
}HIT_STATUS;
//status

typedef enum
{
	ITEMTYPE_0 = 0,
	ITEMTYPE_1,
	ITEMTYPE_2,
	ITEMTYPE_3,
	ITEMTYPE_MAX
}ITEMTYPE;

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXCOLOR col;
	float fDestAngle;		//�ړI�̊p�x
	float fAngle;			//����
	ITEMTYPE Type;
	HIT_STATE state;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}ITEM_POLYGON;

typedef struct
{
	DWORD nNumMatPlayer = 0;	//�}�e���A���̏��̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 move;	//����
	D3DXVECTOR3 rot;	//����
	D3DXCOLOR col;

	bool bUseJump;
	float fDestAngle;		//�ړI�̊p�x
	float fAngle;			//����
	int nIndexShadow;	//�e�̔ԍ�
	int nLife;
	bool bUse;
	int nType;
	ITEMTYPE type;
	HIT_STATUS status;
	int nIdxModelParent = -1;
	HIT_STATE state;
	ITEM_POLYGON aPolygon[MAX_ITEM];
}ITEM;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu

	D3DXVECTOR3 rot;	//����

	float fDestAngle;		//�ړI�̊p�x
	float fAngle;			//����

}POLYGONROT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitItem(void);		//�A�C�e��������
void UninitItem(void);	//�A�C�e���I��
void UpdateItem(void);	//�A�C�e���X�V
void DrawItem(void);		//�A�C�e���`��

void HitCall(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


