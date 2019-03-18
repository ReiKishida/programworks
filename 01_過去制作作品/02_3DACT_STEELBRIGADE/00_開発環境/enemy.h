//=============================================================================
//
// �G�l�~�[���� [Enemy.h]
// Author :
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY (3)

//#define MAX_PARTS (15)
#define MAX_PARTS (11)
//*****************************************************************************
// �v���C���[�̍\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh = NULL;		//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;	//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 VtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	D3DXVECTOR3 VecShot;	//�V���b�g�p�x�N�g��
	float fDestAngle;		//�ړI�̊p�x
	D3DXVECTOR3 fDestAngle_Axis;		//�ړI�̊p�x
	D3DXVECTOR3 fAngle_Axis;		//�ړI�̊p�x
	float fAngle;			//����
	float nIdxModelParent = -1;	//�e���f���̃C���f�b�N�X
	char FileName[256];
}PARTS_E;



typedef enum
{
	ENEMYSTATUS_NEUTRAL = 0,
	ENEMYSTATUS_MOVE,
	ENEMYSTATUS_MAX
}ENEMYSTATUS;


typedef struct
{
	DWORD nNumMatEnemy = 0;	//�}�e���A���̏��̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 move;	//����
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 VtxMinEnemy = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	D3DXVECTOR3 VtxMaxEnemy = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	D3DXVECTOR3 Rmove;	//�t�ړ�
	ENEMYSTATUS status;	//�G�̏��
	bool bUseJump;	//�W�����v�ł��邩
	float fDestAngle;		//�ړI�̊p�x
	float fAngle;			//����
	float fLength;	//����
	int nIndexShadow;		//�e�̔ԍ�
	int nLife;	//���C�t
	bool bUse;	//�g�p���Ă��邩
	bool bUseQick;	//�N�C�b�N�u�[�X�g���g�p���Ă��邩
	int nCoolDown = 200;	//�N�[���_�E��
	PARTS_E aModel[MAX_PARTS];
}ENEMY;




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);	//�G�̏���������
void UninitEnemy(void);	//�G�̏I������
void UpdateEnemy(void);	//�G�̍X�V����
void DrawEnemy(void);	//�G�̕`�揈��

//void HitEnemy(int nDamage);

ENEMY *GetEnemy(void);	//�G�̏��̎擾
ENEMYSTATUS *GetEnemyStatus(void);//�G�̏�Ԃ̎擾

void HitEnemy(int nDamage, int nCntEnemy);//�G�̔�e

bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin);//�G�̓����蔻��
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife);//�G�̐ݒu
#endif
