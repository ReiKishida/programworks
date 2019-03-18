//=============================================================================
//
// �A�C�e������ [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _DANGER_H_
#define _DANGER_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define MAX_DANGER (4)
#define MAX_TYPEDANGER (4)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef enum
{
	DANGERSTATE_NONE = 0,
	DANGERSTATE_USE,
	DANGERSTATE_MAX
}DANGER_STATE;
//state
typedef enum
{
	DANGERSTATUS_NONE = 0,
	DANGERSTATUS_NOW,
	DANGERSTATUS_NEXT
}DANGER_STATUS;
//status

typedef enum
{
	DANGERETYPE_0 = 0,
	DANGERTYPE_1,
	DANGERTYPE_2,
	DANGERTYPE_3,
	DANGERTYPE_MAX
}DANGERTYPE;

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
	DANGER_STATE state;
	DANGERTYPE Type;
	int nType;
	bool bUse;
	float fpos;
	int nCntAngle;
	int nIdxModelParent = -1;
}DANGER_POLYGON;

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
	DANGERTYPE type;
	DANGER_STATUS status;
	int nIdxModelParent = -1;
	DANGER_STATE state;
	DANGER_POLYGON aPolygon[MAX_DANGER];
}DANGER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitDanger(void);		//�A�C�e��������
void UninitDanger(void);	//�A�C�e���I��
void UpdateDanger(void);	//�A�C�e���X�V
void DrawDanger(void);		//�A�C�e���`��

void HitDanger(void);
//void SeItem(D3DXVECTOR3 pos, int nType);



#endif


