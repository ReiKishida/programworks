//=============================================================================
//
// �L�����N�^�[���� [character.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_CHARACTER	(128)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	CHARACTERSTATE_NORMAL = 0,	//�ʏ���
	CHARACTERSTATE_DAMAGE,		//�_���[�W���
	CHARACTERSTATE_MAX			//��ނ̑���
}CHARACTERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	CHARACTERSTATE state;	//�G�̏��
	//int nCountAnim;		//�A�j���[�V�����J�E���^�[
	//int nPatternAnim;	//�A�j���[�V�����p�^�[��No.
	float fWidth;
	float fHeight;
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nType;			//���
	//int nLife;			//�G�̗̑�
	int nCountTime;		//�J�E���g
	float fSpeed;		//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}CHARACTER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCharacter(void);
void UnInitCharacter(void);
void UpdateCharacter(void);
void DrawCharacter(void);


void SetCharacter(D3DXVECTOR3 pos, int nType);
CHARACTER *GetCharacter(void);


#endif
