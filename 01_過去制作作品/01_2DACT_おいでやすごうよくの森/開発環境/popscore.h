//=============================================================================
//
// �u���b�N���� [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _POPSCORE_H_
#define _POPSCORE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_POPSCORE	(255)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	POPSCORETYPE_COIN_SCORE = 0,	//�R�C���X�R�A�\��
	POPSCORETYPE_VODKA_SCORE,	//�E�H�b�J�X�R�A�\��
	POPSCORETYPE_SATSU_SCORE,	//���D�X�R�A�\��
	POPSCORETYPE_AREY_SCORE,	//�A���C�X�R�A�\��
	POPSCORETYPE_MAX		//��ނ̑���
}POPSCORETYPE;

typedef enum
{
	POPSCORESTATE_NOMAL = 0,	//�ʏ���
	POPSCORESTATE_LAND,			//���n���
	POPSCORESTATE_LOST,			//�������
	POPSCORESTATE_MAX			//�ő吔
}POPSCORESTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXVECTOR3 rot;
	D3DXCOLOR col;		//�J���[
	POPSCORETYPE PopScoreType;	//���
	POPSCORESTATE state;	//���
	float fWidth;		//��
	float fHeight;		//����
	float fLength;		//�Ίp���̊p�x
	float fAngle;		//�Ίp���̒���
	float fAngle2;		//�Ίp���̒���
	int nType;			//���
	int nCntAnim;		//�A�j���[�V�����̃J�E���g
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	int nCntLost;		//������܂ł̎���
	int nCntGet;		//������܂ł̎���2
	int nland;
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bItemLand;		//�A�C�e�����n
}POPSCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPopScore(void);
void UnInitPopScore(void);
void UpdatePopScore(void);
void DrawPopScore(void);



void SetPopScore(D3DXVECTOR3 pos, int type, D3DXVECTOR3 move);


POPSCORE *GetPopScore(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
