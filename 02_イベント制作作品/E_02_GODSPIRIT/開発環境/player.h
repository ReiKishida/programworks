////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���C���[���� [player.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "meshField.h"
#include "sound.h"
#include "model.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_PLAYER		(1)			// �ő吔
#define MAX_PLAYERMODEL	(18)
#define MAX_PLAYER_KEY	(20)	// ���[�V����1�ɑ΂���L�[�t���[���̍ő吔
#define MAX_MOTION		(30)	// ���[�V������
#define MAX_COLLISION	(50)	// ����ő吔

//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{// ���[�V�����̎��
	PLAYER_MOTION_TYPE_NEUTRAL = 0,					// �j���[�g������
	PLAYER_MOTION_TYPE_MOVE,						// �ړ���
	PLAYER_MOTION_TYPE_JUMP,						// �W�����v��
	PLAYER_MOTION_TYPE_LANDING,						// ���n��
	PLAYER_MOTION_TYPE_DAMAGE,						// �_���[�W(������)��
	PLAYER_MOTION_TYPE_FLY,							// �_���[�W(�������)��
	PLAYER_MOTION_TYPE_GETUP,						// �N���オ�莞
	PLAYER_MOTION_TYPE_STEP,						// �X�e�b�v(���)��
	PLAYER_MOTION_TYPE_CHANGE,						// �ς���
	PLAYER_MOTION_TYPE_ATTACK_0,						// �U��0
	PLAYER_MOTION_TYPE_ATTACK_1,						// �U��0
	PLAYER_MOTION_TYPE_ATTACK_2,						// �U��0
	PLAYER_MOTION_TYPE_ATTACK_BIG,						// ���U��
	PLAYER_MOTION_TYPE_MAX							// ���[�V�����̑���
}PlayerMotionType;

typedef enum
{// �U���̎��
	PLAYER_ATTACK_TYPE_ATTACK_0 = 0,				// �U��0
	PLAYER_ATTACK_TYPE_ATTACK_1 ,					// �U��1
	PLAYER_ATTACK_TYPE_ATTACK_2,					// �U��2
	PLAYER_ATTACK_TYPE_ATTACK_BIG ,					// ���U��
	PLAYER_ATTACK_TYPE_MAX						// �U���̎�ނ̑���
}PlayerAttackType;
typedef enum
{// bool�̎��
	PLAYER_BOOL_RUN = 0,			// ����
	PLAYER_BOOL_JUMP,				// �W�����v
	PLAYER_BOOL_LANDING,			// ���n
	PLAYER_BOOL_DAMAGE,				// �_���[�W
	PLAYER_BOOL_FLY,				// �������
	PLAYER_BOOL_GETUP,				// �N���オ��
	PLAYER_BOOL_STEP,				// �X�e�b�v
	PLAYER_BOOL_CHANGE,				// �J�G��
	PLAYER_BOOL_MAX
}PlayerBool;
typedef enum
{
	PLAYERWEAPON_HAND = 0,
	PLAYERWEAPON_SORD,
	PLAYERWEAPON_LANCE,
	PLAYERWEAPON_HAMMER,
	PLAYERWEAPON_MAX,
}PLAYERWEAPON;
typedef enum
{// �v���C���[�̏��
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_DAMAGESTART,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DEATH,
}PLAYERSTATE;
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
}PartsKeyPlayer;
typedef struct
{
	int nFrame;									// �L�[�t���[��
	PartsKeyPlayer aPartsKey[MAX_PLAYERMODEL];	// �p�[�c���Ƃ̃L�[���
}KeyPlayer;
typedef struct
{
	bool bLoop;									// ���[�v���邩�ǂ���
	int nNumKey;								// �L�[��
	KeyPlayer aKey[MAX_PLAYER_KEY];				// �L�[���
	__int8 nAttackPower;						// ���[�V�����̍U����
	float fDamageHorizontal;					// ���[�V�����̐��������ɐ�����΂���
	float fDamageVertical;						// ���[�V�����̐��������ɐ�����΂���
}MotionPlayer;

typedef struct
{// �U���̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			ofset;				// �ʒu
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			mtxWorldOld;			// ���[���h�}�g���b�N�X
	__int8				nIdxParent;			// �e���f���̃C���f�b�N�X
	float				fRadius;			// ���a
	short int			nCollisionStart;	// �����蔻��̎n�܂�̃t���[����
	short int			nCollisionEnd;		// �����蔻��̏I���̃t���[����
	PlayerMotionType	nMotionType;		// �����蔻�肪�t�����[�V�����̃^�C�v
	bool				bUse;				// �����蔻�肪�g���Ă邩�ǂ���
}CollisionAttackPlayer;
typedef struct
{// �̂̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			ofset;				// �ʒu
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			mtxWorldOld;			// ���[���h�}�g���b�N�X
	__int8				nIdxParent;			// �e���f���̃C���f�b�N�X
	float				fRadius;			// ���a
	bool				bUse;				// �����蔻�肪�g���Ă邩�ǂ���
}CollisionBodyPlayer;

typedef struct
{
	char *pFileName;					// �t�@�C����
	LPD3DXMESH pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;						// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9 *pTextuer;		// �e�N�X�`��
	D3DXVECTOR3 rot;					// �p�x
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 Initpos;				// �����I�t�Z�b�g�ʒu
	D3DXVECTOR3	posAddMotion;			// ���Z���[�V�����ʒu
	D3DXVECTOR3	rotAddMotion;			// ���Z���[�V��������
	D3DXVECTOR3 DiffrotMotion;			// ����
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	int nIdxModel;						// �����̃��f���̔ԍ�
	int nIdxModelPareant;				// �e���f���̃C���f�b�N�X
}PlayerModel;

typedef struct
{
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 posOld;					// �O��̈ʒu
	D3DXVECTOR3 Diffpos;				// �ړI�̈ʒu
	D3DXVECTOR3 Initpos;				// �����ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXVECTOR3 CtrlMove;				// ����̈ړ���
	float JumpSpeed;					// �W�����v��
	float MoveSpeed;					// �ړ����x
	D3DXVECTOR3 Diffrot;				// �p�x�̍���
	D3DXVECTOR3 Destrot;				// �ړI�̊p�x
	D3DXVECTOR3 rot;					// �p�x
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	PlayerMotionType nMotionType;			// ���
	PlayerMotionType nMotionTypeOld;		// �O�̏��
	PlayerModel aModel[MAX_PLAYERMODEL][PLAYERWEAPON_MAX];// �p�[�c��
	MotionPlayer aMotion[MAX_MOTION][PLAYERWEAPON_MAX];	// ���[�V�����̏��
	PLAYERSTATE state;					// ���
	int nCntState;						// ���
	float fRadiusWidth;					// ���a��
	float fRadiusHeight;				// ���a����
	int nIdyShadow;						// �e�̔ԍ�
	int nIdyLine;						// ����̔ԍ�
	bool bMotionState[PLAYER_BOOL_MAX];				// ���[�V�����g�p����
	bool bMotionStateOld[PLAYER_BOOL_MAX];				// �O��̃��[�V�����g�p����
	bool bBlend;						// �u�����h�����ǂ���
	bool bJump;							// �W�����v���Ă��邩�ǂ���
	int nDamage;						// �_���[�W
	int nNumParts;						// �p�[�c��
	int nNumMotion;						// ���[�V�����ԍ�
	int nLife;							// ���C�t
	int nInitLife;						// �������C�t
	int nSpirit;						// �X�s���b�g
	int nInitSpirit;					// �����X�s���b�g
	bool bAttention;					// ���ڂ��Ă��邩�ǂ���
	int nIdyLockEnemy;					// ���b�N���Ă���G�̔ԍ�
	bool bSound[SOUND_LABEL_MAX];		// ��
	MeshField *pMeshFieldLand;			// �ǂ̏��ɏ���Ă��邩
	CollisionAttackPlayer CollisionAttack[MAX_COLLISION][PLAYERWEAPON_MAX];	// �U������
	CollisionBodyPlayer CollisionBody[MAX_COLLISION];		// �_���[�W����
	D3DXVECTOR3 fFlyPower;									// ������ё��x
	int nFlyCounter;										// ���ł��鎞��
	float fRotSpeed;										// ��]�̌����X�s�[�h
	bool bAttackType[PLAYER_ATTACK_TYPE_MAX];				// �A�^�b�N�̎��
	Model *pModel;											// �����������f���̏��
	int nAirCounter;										// ���ł鎞�̃R���{����
	int nLoopCounter;										// ���[�V�����̃��[�v�J�E���^
	PLAYERWEAPON nWeaponState;								// ����̏��
	bool bWeaponNow[PLAYERWEAPON_MAX];						// �ǂ̕��풆��
}Player;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(int nIdxPlayer);
void DrawPlayer(int nIdxPlayer);
Player *GetPlayer(void);
void MotionInfoPlayer(int nIdxPlayer);
void SetAttackCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset,float fRadius,  int nCollisionStart, int nCollisionEnd,PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState);
void SetBodyCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius);
void CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical);
void PlayerController(int nCntPlayer);					// �v���C���[�̑���
void PlayerMotionEffect(int nCntPlayer,int nCollision, PLAYERWEAPON nWeaponState);
void PlayerSpirit(int nCntPlayer);
void PlayerSpiritAdd(int nSpirit);
#endif
