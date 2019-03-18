//=============================================================================
//
// �G�t�F�N�g�̏��� [Shadow.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOWJUMP_POWER (-23.0f)	//�W�����v��
#define SHADOWGRAVITY (1.2f)		//�d��
#define SHADOWGRAVITY_MAX (50.0f)	//�d�͂̍ő��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	SHADOWSTATE_APPEAR = 0,	//�o�����
	SHADOWSTATE_NORMAL,		//�ʏ���
	SHADOWSTATE_DAMAGE,		//�_���[�W���
	SHADOWSTATE_MAX			//��ނ̑���
}SHADOWSTATE;


typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�O��̈ʒu
	D3DXVECTOR3 move;	//�ړ���
	SHADOWSTATE state;
	int nCounterState;	//�X�e�[�g�̃J�E���^�[
	int nCounterAnim;	//�A�j���[�V�����p�^�[���̃J�E���g
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	int nJumpCount;		//�W�����v�̃J�E���g
	bool bDisp;			//�g�p����Ă��邩
	bool bPress;		//������Ă��邩
	bool bUseJump;	//�W�����v���g�p����Ă��邩
	float fWidth;		//��
	float fHeight;		//����
						//BLOCK *pBlock;		//�u���b�N
}SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitShadow(void);
void UnInitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

//void SetShadow(D3DXVECTOR3 pos, float fRadius, int nLife);
//void SetShadow(D3DXVECTOR3 pos);

#endif
