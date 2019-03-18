//=============================================================================
//
// �G���� [Block.h]
// Author : KishidaRei
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOC_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BLOCK	(128)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BLOCKSTATE_NORMAL = 0,	//�ʏ���
	BLOCKSTATE_NORMAL2,	//�ʏ���2
	BLOCKSTATE_MOVE_X,	//�ړ���_X
	BLOCKSTATE_MOVE_Y,	//�ړ���_Y
	BLOCKSTATE_MOVE_XY,	//�ړ���_XY
	BLOCKSTATE_SPRING,	//�X�v�����O��
	BLOCKSTATE_CRASH,	//���鏰
	BLOCKSTATE_AREY,	//�S�A���C
	BLOCKSTATE_MAX
}BLOCKSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold; //�ړ��O�ʒu
	D3DXVECTOR3 FallenPoint;
	D3DXVECTOR3 move;   //�ړ���
	BLOCKSTATE state;	//���
	int nType;			//���
	float fWidth;		//��
	float fHeight;		//����
	float Move_X;	//�ړ��ʂ̃J�E���g(X)
	float Move_Y;	//�ړ��ʂ̃J�E���g(Y)
	
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BLOCK;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBlock(void);
void UnInitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, BLOCKSTATE state, float fMove_X, float fMove_Y);

BLOCK *GetBlock(void);

bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
bool CollisionBlockItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fLength);
bool CollisionBlockItem2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif
