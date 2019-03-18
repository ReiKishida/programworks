//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : 
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"


//*****************************************************************************
// ���b�V���V�����_�[�̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(���S���W)
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;
	int nType;
}CYLINDER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshCylinder(void);		//���b�V���V�����_�[������
void UninitMeshCylinder(void);	//���b�V���V�����_�[�I��
void UpdateMeshCylinder(void);	//���b�V���V�����_�[�X�V
void DrawMeshCylinder(void);		//���b�V���V�����_�[�`��




void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif
