//=============================================================================
//
// �e�̔��ˏ��� [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BULLET (200)



//*****************************************************************************
// �ǂ̍\����
//*****************************************************************************

typedef enum
{
	BULLETTYPE_NONE = 0,
	BULLETTYPE_PLAYER,
	BULLETTYPE_PLAYER_HOMING,
	BULLETTYPE_ENEMY,
	BULLETTYPE_ENEMY_HOMING,
	BUULLETTYPE_MAX
}BULLETTYPE;


typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(���S���W)
	D3DXVECTOR3 posold;		//�ʒu(���S���W)
	D3DXVECTOR3 rot;		//��]
	D3DXVECTOR3 move;		//�ړ����x
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	BULLETTYPE type;
	bool bUse;				//�g�p���Ă��邩
	int IndexShadow;		//�e�̔ԍ�
	int nLife;				//�e�̎���
	float fRadius;			//���a
	float fLimif;
}BULLET;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 VecA;	//�x�N�g��
	D3DXVECTOR3 VecB;	//�x�N�g��
	D3DXVECTOR3 VecC;	//�x�N�g��
	float fLimit;	//�ʒu

}STAGE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);		//�Ǐ�����
void UninitBullet(void);	//�ǏI��
void UpdateBullet(void);	//�ǍX�V
void DrawBullet(void);		//�Ǖ`��

//void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 rot, int nLife);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, float fRadius, BULLETTYPE type);
bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);

void CollisionVec(void);
BULLET *GetBullet(void);
#endif
