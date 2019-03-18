//=============================================================================
//
// �v���C���[���� [player.h]
// Author :
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
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
}PARTS;

typedef enum
{
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_OVERHEAT,
	PLAYERSTATE_MAX
}PLAYERSTATE;


typedef enum
{
	PLAYERSTATUS_NEUTRAL = 0,
	PLAYERSTATUS_MOVE,
	PLAYERSTATUS_MAX
}PLAYERSTATUS;


typedef struct
{
	DWORD nNumMatPlayer = 0;	//�}�e���A���̏��̐�
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posold;	//�ʒu
	D3DXVECTOR3 move;	//����
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 VtxMinPlayer = D3DXVECTOR3(10000.0f,10000.0f,10000.0f);
	D3DXVECTOR3 VtxMaxPlayer = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	PLAYERSTATE state;
	PLAYERSTATUS status;
	bool bUseJump;
	float fDestAngle;		//�ړI�̊p�x
	float fAngle;			//����
	int nIndexShadow;		//�e�̔ԍ�
	int nLife;
	bool bUse;
	int nCoolDown = 200;
	PARTS aModel[MAX_PARTS];
}PLAYER;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//����
	D3DXVECTOR3 rot;	//����
}VECTOR;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	int nIdxModelParent = -1;	//�e���f���̃C���f�b�N�X
}BULLET_POS;





//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);	//�v���C���[�̏���������
void UninitPlayer(void);//�v���C���[�̏I������
void UpdatePlayer(void);//�v���C���[�̍X�V����
void DrawPlayer(void);//�v���C���[�̕`�揈��

void HitPlayer(int nDamage);//�v���C���[�̔�e

PLAYER *GetPlayer(void);
PLAYERSTATUS *GetPlayerStatus(void);
#endif
