//=============================================================================
//
// �|���S���̏��� [player.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define JUMP_POWER (-23.2f)	//�W�����v��
#define GRAVITY (1.2f)		//�d��
#define GRAVITY_MAX (50.0f)	//�d�͂̍ő��



//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//�o�����
	PLAYERSTATE_NORMAL,		//�ʏ���
	PLAYERSTATE_DAMAGE,		//�_���[�W���
	PLAYERYSTATE_MAX			//��ނ̑���
}PLAYERSTATE;



typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�O��̈ʒu
	D3DXVECTOR3 move;	//�ړ���
	PLAYERSTATE state;	//�v���C���[�̏��
	int nCounterState;	//�X�e�[�g�̃J�E���^�[
	int nCounterAnim;	//�A�j���[�V�����p�^�[���̃J�E���g
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	int nJumpCount;		//�W�����v�̃J�E���g
	int nStock;			//�c�@
	bool bDisp;			//�g�p����Ă��邩
	bool bPress;		//������Ă��邩
	bool bUseJump;	//�W�����v���g�p����Ă��邩
	float fWidth;		//��
	float fHeight;		//����
	//BLOCK *pBlock;		//�u���b�N
}PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);
void UnInitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER GetPlayer();

bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif