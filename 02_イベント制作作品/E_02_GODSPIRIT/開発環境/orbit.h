////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �O���̏��� [orbit.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "player.h"
#include "enemy.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_ORBIT	(50)
#define MAX_ORBIT_LENGTH	(100)
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{
	ORBITTYPE_0 = 0,
	ORBITTYPE_1,
	ORBITTYPE_2,
	ORBITTYPE_PLAYER,
	ORBITTYPE_MAX,
}ORBITTYPE;
typedef enum
{
	ORBIT_PLAYER = 0,
	ORBIT_ENEMY,
	ORBIT_MAX,
}ORBIT_PLAYER_ENEMY;
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff= NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxStart;// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxEnd;	// ���[���h�}�g���b�N�X
	ORBITTYPE nType;	// ���
	D3DXVECTOR3 Start;	// �n�_
	D3DXVECTOR3 End;	// ����
	D3DXVECTOR3 InitStart;	// �n�_
	D3DXVECTOR3 InitEnd;	// ����
	PlayerMotionType nMotionType;
	ORBIT_PLAYER_ENEMY nCharacterType;
	PLAYERWEAPON nWeaponState;
	int nLength;		// ����
	int nPlus;			// ���₷��
	int nIdxParent;		// �e���f��
	int nCntTrueTime;	// �\������
	bool bUse;			// �g�p����Ă��邩�ǂ���
}Orbit;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitOrbit(void);
void UninitOrbit(void);
void UpdateOrbit(void);
void DrawOrbit(void);
void SetOrbit(int nIdxParent, int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState);
void SetPosOrbit(PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState);

#endif
