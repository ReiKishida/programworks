//=============================================================================
//
// ���C�t���� [life.h]
// Author : 
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//****************************************************************************
#define MAX_LIFE (128)
//*****************************************************************************
// �\���̐錾
//****************************************************************************

typedef enum
{
	LIFESTATE_NONE = 0,
	LIFESTATE_USE,
	LIFESTATE_MAX
}LIFE_STATE;
typedef enum
{
	LIFETYPE_NONE = 0,
	LIFETYPE_NOW,
	LIFETYPE_NEXT
}LIFE_TYPE;



typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(���S���W)
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 ParentPos;
	D3DXVECTOR3 ParentRot;
	D3DXCOLOR col;
	LIFE_STATE state;
	bool bUse;
}LIFE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLife(void);		//���C�t������
void UninitLife(void);	//���C�t�I��
void UpdateLife(void);	//���C�t�X�V
void DrawLife(void);		//���C�t�`��

void SetLife(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Parentpos, D3DXVECTOR3 Parentrot, D3DXCOLOR col);
void HitDamage(int nDamage);

#endif
