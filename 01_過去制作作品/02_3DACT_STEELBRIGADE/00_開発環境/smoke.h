//=============================================================================
//
// ������ [smoke.h]
// Author : 
//
//=============================================================================
#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SMOKE (50)



//*****************************************************************************
// �ǂ̍\����
//*****************************************************************************


typedef enum
{
	SMOKETYPE_SHOWER = 0, //�V�����[
	SMOKETYPE_EXPLOSION,	//����
	SMOKETYPE_SMOKE,		//��
	SMOKETYPE_MAX
}SMOKETYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(���S���W)
	D3DXVECTOR3 posold;		//�O��̈ʒu(���S���W)
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//��]
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nLife;				//���C�t
	bool bUse;				//�g�p���Ă��邩
	float fRadius;			//���a
	SMOKETYPE type;		//�p�[�e�B�N���̎��
}SMOKE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitSmoke(void);		//�p�[�e�B�N��������
void UninitSmoke(void);	//�p�[�e�B�N���I��
void UpdateSmoke(void);	//�p�[�e�B�N���X�V
void DrawSmoke(void);		//�p�[�e�B�N���`��

void SetSmoke(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col);
#endif
