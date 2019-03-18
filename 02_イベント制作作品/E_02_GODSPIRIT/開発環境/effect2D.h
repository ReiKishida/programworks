////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g�̏��� [effect2D.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

#include "main.h"

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_2D_ARROW = 0, // ���傤��
	EFFECTTYPE_2D_SUNDER,
	EFFECTTYPE_2D_MAX
}EFFECTTYPE_2D;
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fRadius;			// ���a(�傫��)
	int nLife;				// �\������(����)
	int nCounterAnim;	// �J�E���^�[
	int nPatternAnim;	// �p�^�[��
	EFFECTTYPE_2D nType;		// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EFFECT2D;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitEffect2D(void);
void UninitEffect2D(void);
void UpdateEffect2D(void);
void DrawEffect2D(void);

void SetEffect2D(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE_2D nType);
float SetRandomEffect2D(void);
EFFECT2D *GetEffect2D(void);
#endif
