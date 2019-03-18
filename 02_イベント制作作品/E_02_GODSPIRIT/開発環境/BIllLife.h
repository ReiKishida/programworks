////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �r���{�[�h���C�t���� [BillLife.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BILLLIFE_H_
#define _BILLLIFE_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	BILLLIFETYPE_ENEMYBG = 0,
	BILLLIFETYPE_ENEMYBGRED,
	BILLLIFETYPE_ENEMY,
	BILLLIFETYPE_MAX
}BILLLIFETYPE;
typedef struct
{
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
	char	*pFileName;	// �e�N�X�`���ւ̃|�C���^
}BillLifeData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXCOLOR col;						// �F
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	float fWidth;						// ��
	float fHeight;						// ����
	int nBlock_X;						// �u���b�NX
	int nBlock_Y;						// �u���b�NY
	BILLLIFETYPE nType;					// ���
	float fMoveVtx;						// ���_���Ƃ̓���
	float fTexmoveMesh;					// �e�N�X�`���̈ړ����x
	int nNumVertex;						// ���_��
	int nNumIndex;						// �C���f�b�N�X��
	int nNumPolygon;					// �|���S����
	int nLife;							// ���C�t
	int nInitLife;						// �������C�t
	bool bUseDown;						// �Ȃ��Ȃ����ǂ���
	bool bDown;							// �Ȃ��Ȃ��Ă���Œ����ǂ���
	int nDrawDown;						// �����p
	bool bDownRed;						// �Ԃ����C�t
	int nDownRedCounter;				// �Ԃ����C�t�����Ԋu�ۊ�
	int nIdy;							// �ԍ�
	float fOffset;						// �I�t�Z�b�g
	int nCntTime;						// ���ʎ���
	bool bUse;							// �g�p����Ă��邩�ǂ���
}BillLife;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitBillLife(void);
void UninitBillLife(void);
void UpdateBillLife(void);
void DrawBillLife(void);
void SetBillLife(int nIdy, D3DXVECTOR3 pos,int nLife,D3DXCOLOR col, BILLLIFETYPE nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight,float fOffset, bool bUseDown);
BillLife *GetBillLife(void);
BillLifeData *GetBillLifeData(void);
void GetBillLife(int nIdy, int nLife);
void SetPositionBillLife(int nIdy, D3DXVECTOR3 pos);
void DeleteBillLife(int nIdy);
#endif
