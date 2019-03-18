//=============================================================================
//
// �A�C�e������ [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define MAX_DAMAGE (4)
#define MAX_TYPEDAMAGE (4)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef enum
{
	DAMAGESTATE_NONE = 0,
	DAMAGESTATE_USE,
	DAMAGESTATE_MAX
}DAMAGE_STATE;
//state
typedef enum
{
	DAMAGESTATUS_NONE = 0,
	DAMAGESTATUS_NOW,
	DAMAGESTATUS_NEXT
}DAMAGE_STATUS;
//status

typedef enum
{
	DAMAGETYPE_0 = 0,
	DAMAGETYPE_1,
	DAMAGETYPE_2,
	DAMAGETYPE_3,
	DAMAGETYPE_MAX
}DAMAGETYPE;

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
	DAMAGETYPE Type;
	DAMAGE_STATE state;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}DAMAGE_POLYGON;

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
	DAMAGETYPE type;
	DAMAGE_STATUS status;
	int nIdxModelParent = -1;
	DAMAGE_STATE state;
	DAMAGE_POLYGON aPolygon[MAX_DAMAGE];
}DAMAGE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitDamage(void);		//�A�C�e��������
void UninitDamage(void);	//�A�C�e���I��
void UpdateDamage(void);	//�A�C�e���X�V
void DrawDamage(void);		//�A�C�e���`��

void HitDamage(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


