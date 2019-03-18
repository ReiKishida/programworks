//=============================================================================
//
// �e�̏��� [shadow.h]
// Author : 
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SHADOW (30)

//*****************************************************************************
// �e�̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(���S���W)
	D3DXVECTOR3 rot;		//����	
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
	float fWidth;			//�e�̕�
	float fDepth;			//�e�̉��s��
}SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitShadow(void);		//�V���h�E������
void UninitShadow(void);	//�V���h�E�I��
void UpdateShadow(void);	//�V���h�E�X�V
void DrawShadow(void);		//�V���h�E�`��

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float Depth);

void SetPositionShadow(int IndexShadow, D3DXVECTOR3 pos);
void SetPositionBulletShadow(int IndexShadow, D3DXVECTOR3 pos);

void DeleteShadow(int IndexShadow);
void SetScaleShadow(int IndexShadow);

#endif
