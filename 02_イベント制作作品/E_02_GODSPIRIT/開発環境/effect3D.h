////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g�RD���� [effect3D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	EFFECT3D_ARM = 0,
	EFFECT3D_ARM2,
	EFFECT3D_SORD ,
	EFFECT3D_MAX,
}EFFECT3DTYPE;

typedef struct
{
	LPD3DXMESH pMesh;							// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;						// �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;								// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9 *pTextuer;			// �e�N�X�`��
	char *pFileName;
}Effect3DData;
typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 posOld;					// �O��̈ʒu
	D3DXVECTOR3 Initpos;				// �����ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXCOLOR col;						// �J���[
	EFFECT3DTYPE nType;					// ���
	D3DXVECTOR3 Diffrot;				// �p�x�̍���
	D3DXVECTOR3 Destrot;				// �ړI�̊p�x
	D3DXVECTOR3 rot;					// �p�x
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	int nIdyShadow;						// �e�̔ԍ�
	int nIdy;						// �ԍ�
	bool bCollision;					// �����蔻��
	bool bUse;
	bool bDraw;
}Effect3D;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitEffect3D(void);
void UninitEffect3D(void);
void UpdateEffect3D(void);
void DrawEffect3D(void);
void SetEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, EFFECT3DTYPE nType);
Effect3D *GetEffect3D(void);
Effect3DData *GetEffect3DData(void);
#endif
