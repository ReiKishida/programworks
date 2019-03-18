//=============================================================================
//
// �u���b�N���� [item.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ITEM	(500)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	//ITEMTYPE_NONE = 0,
	ITEMTYPE_COIN = 0,		//�R�C��
	ITEMTYPE_VODKA,		//�E�H�b�J
	ITEMTYPE_AREY,		//�S�A���C
	ITEMTYPE_PAPER,		//���v
	ITEMTYPE_H_COIN,		//HUD�p�A�C�e���摜
	ITEMTYPE_H_VODKA,		//HUD�p�A�C�e���摜
	ITEMTYPE_H_AREY,		//HUD�p�A�C�e���摜
	ITEMTYPE_H_PAPER,		//HUD�p�A�C�e���摜
	ITEMTYPE_COIN_SCORE,	//�R�C���X�R�A�\��
	ITEMTYPE_VODKA_SCORE,	//�E�H�b�J�X�R�A�\��
	ITEMTYPE_SATSU_SCORE,	//���D�X�R�A�\��
	ITEMTYPE_AREY_SCORE,	//�A���C�X�R�A�\��
	ITEMTYPE_MAX		//��ނ̑���
}ITEMTYPE;

typedef enum
{
	ITEMSTATE_NOMAL = 0,	//�ʏ���
	ITEMSTATE_LAND,			//���n���
	ITEMSTATE_LOST,			//�������
	ITEMSTATE_MAX			//�ő吔
}ITEMSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 move;   //�ړ���
	D3DXVECTOR3 rot;
	D3DXCOLOR col;		//�J���[
	ITEMTYPE ItemType;	//���
	ITEMSTATE state;	//���
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
}ITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitItem(void);
void UnInitItem(void);
void UpdateItem(void);
void DrawItem(void);



void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, D3DXVECTOR3 move);


ITEM *GetItem(void);

//bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float *fWidth, float *fHeight);
#endif
