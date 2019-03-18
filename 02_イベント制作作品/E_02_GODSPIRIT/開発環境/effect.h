////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �G�t�F�N�g�̏��� [effect.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"


//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_EXPLOSION = 0,	// ����
	EFFECTTYPE_PLAYER,			// ��
	EFFECTTYPE_WATER,			// ��
	EFFECTTYPE_DRAIN,			// ��
	EFFECTTYPE_AIM,				// ����
	EFFECTTYPE_DEATH,			// �G���j
	EFFECTTYPE_WATERFALL,		// �ꐅ
	EFFECTTYPE_IMPACT,			// �Ռ���
	EFFECTTYPE_SMOKE,			// ��
	EFFECTTYPE_SMOKEDOWN,		// ��������
	EFFECTTYPE_HIT,				// �T��
	EFFECTTYPE_SUNDER,			// �}�q
	EFFECTTYPE_SUNDER1,			// �}�q
	EFFECTTYPE_POISON,			// ��
	EFFECTTYPE_POISONSMOKE,		// ��
	EFFECTTYPE_HITEXPLOSION,	// �q�b�g
	EFFECTTYPE_FIRE,			// ��
	EFFECTTYPE_WIND,			// ����
	EFFECTTYPE_FLOZE,			// �X
	EFFECTTYPE_SUNDER_DOWN,			// �}�q
	EFFECTTYPE_WATER_DOWN,			// �}�q

	EFFECTTYPE_MAX
}EFFECTTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 Destpos;// �ړI�̈ʒu
	D3DXVECTOR3 posOld;	// �O��̈ʒu
	D3DXVECTOR3 Initpos;// �����ʒu
	D3DXVECTOR3 move;	// ����
	D3DXVECTOR3 rot;	// ����
	float fAngle;		// �p�x
	float fLength;		// ����
	D3DXCOLOR col;		// �F
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
	float fWidth;		// ��
	float fHeight;		// ����
	float fDepth;		// ���s��
	float fRadius;		// ����
	int nLife;			// ����
	float fSinCosMove;	// ���
	EFFECTTYPE nType;// ���
	int nIdyShadow;		// �e��ID
	float fChangeRadius;// �g��k��
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int nPatternAnim;	// �A�j���[�V�����p�^�[��
	int nSpeedCounter;	// �J�E���^�[
	bool bUse;			// �g�p����Ă��邩�ǂ���
}Effect;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType);
float SetRandomEffect(void);
Effect *GetEffect(void);

#endif
