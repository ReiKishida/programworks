//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : 
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMeshField(void);		//�|���S��������
void UninitMeshField(void);	//�|���S���I��
void UpdateMeshField(void);	//�|���S���X�V
void DrawMeshField(void);		//�|���S���`��

//bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

#endif
