////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �~�̏��� [meshFar.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MESHFAR_H_
#define _MESHFAR_H_

#include "main.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	int nNumTex;
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
	char *pFileName;	// �e�N�X�`���ւ̃|�C���^

}MeshFarData;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 rot;					// ����
	D3DXCOLOR col;						// �F
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	float fDepth;						// ���s��
	int nBlock_X;						// �u���b�NX
	int nBlock_Z;						// �u���b�NZ
	int nType;				// ���
	float fMoveVtx;					// ���_���Ƃ̍���
	float fTexmove;				// �e�N�X�`���̈ړ����x
	float fTexPos;				// �e�N�X�`���̈ړ����x
	int nNumVertex;						// ���_��
	int nNumIndex;						// �C���f�b�N�X��
	int nNumPolygon;					// �|���S����
	bool bCull;							// �J�����O���邩�ǂ���
	bool bUse;							// �g�p����Ă��邩�ǂ���
}MeshFar;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitMeshFar(void);
void UninitMeshFar(void);
void UpdateMeshFar(void);
void DrawMeshFar(void);
void SetMeshFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nBlock_X, int nBlock_Z,  float fDepth, bool bCull);
MeshFar *GetMeshFar(void);
MeshFarData *GetMeshFarData(void);
#endif
