////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���C���[���� [player.h]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sound.h"
#include "meshField.h"
#include "model.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_PLAYER		(1)			// �ő吔
#define MAX_PLAYERMODEL	(18)
#define MAX_PLAYER_KEY	(20)	// ���[�V����1�ɑ΂���L�[�t���[���̍ő吔
#define MAX_MOTION		(30)	// ���[�V������
#define MAX_COLLISION	(50)	// ����ő吔
#define MAX_PLAYER_ORBIT (10)	// �O�Ղ̍ő吔
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef enum
{// ���[�V�����̎��
	PLAYER_MOTION_TYPE_NEUTRAL = 0,					// �j���[�g������
	PLAYER_MOTION_TYPE_KICK_R,						// R
	PLAYER_MOTION_TYPE_KICK_L ,						// L
	PLAYER_MOTION_TYPE_JUMP,						// �W�����v
	PLAYER_MOTION_TYPE_LANDING,						// ���n
	PLAYER_MOTION_TYPE_FLY,						// �������
	PLAYER_MOTION_TYPE_HAPPY,						// ���
	PLAYER_MOTION_TYPE_MAX							// ���[�V�����̑���
}PlayerMotionType;

typedef enum
{// bool�̎��
	PLAYER_BOOL_JUMP=0,				// �W�����v
	PLAYER_BOOL_KICK_L,
	PLAYER_BOOL_KICK_R,
	PLAYER_BOOL_LANDING,			// ���n
	PLAYER_BOOL_FLY,				// �������
	PLAYER_BOOL_HAPPY,				// ����
	PLAYER_BOOL_MAX
}PlayerBool;

typedef enum
{// �v���C���[�̏��
	PLAYERSTATE_NONE = 0,		// �����Ȃ�
	PLAYERSTATE_DAMAGESTART,	// ���G
	PLAYERSTATE_DAMAGE,			// �_���[�W
	PLAYERSTATE_FALL,			// ������
	PLAYERSTATE_INVINCIBLE,		// ���G
	PLAYERSTATE_DEATH,			// ����
}PLAYERSTATE;

typedef struct
{
	int nIdyOrbit;					// �ԍ�
	int nOrbitLength;				// �O�Ղ̒���
	D3DXVECTOR3 OrbitStart;			// �O�Ղ̎n�_
	D3DXVECTOR3 OrbitEnd;			// �O�Ղ̏I�_
	int nType;						// �^�C�v
	PlayerMotionType motion;		// ���[�V�����^�C�v
}PlayerOrbit;

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
	float fAttackPower;						// ���[�V�����̍U����
	float fDamageH;					// ���[�V�����̐��������ɐ�����΂���
	float fDamageV;						// ���[�V�����̐��������ɐ�����΂���
}MotionPlayer;

typedef struct
{// �U���̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			ofset;				// �ʒu
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			mtxWorldOld;		// ���[���h�}�g���b�N�X
	__int8				nIdxParent;			// �e���f���̃C���f�b�N�X
	float				fRadius;			// ���a
	short int			nCollisionStart;	// �����蔻��̎n�܂�̃t���[����
	short int			nCollisionEnd;		// �����蔻��̏I���̃t���[����
	PlayerMotionType	nMotionType;		// �����蔻�肪�t�����[�V�����̃^�C�v
	int					nStopTime;			// ��~����
	bool				bUse;				// �����蔻�肪�g���Ă邩�ǂ���
}CollisionAttackPlayer;
typedef struct
{// �̂̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			ofset;				// �ʒu
	D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			mtxWorldOld;		// ���[���h�}�g���b�N�X
	__int8				nIdxParent;			// �e���f���̃C���f�b�N�X
	float				fRadius;			// ���a
	bool				bUse;				// �����蔻�肪�g���Ă邩�ǂ���
}CollisionBodyPlayer;

typedef struct
{
	char *pFileName;								// �t�@�C����
	LPD3DXMESH pMesh;								// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;							// �}�e���A�����ւ̃|�C���^
	DWORD nNumMat;									// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9 *pTextuer;	// �e�N�X�`��
	D3DXVECTOR3 rot;								// �p�x
	D3DXVECTOR3 pos;								// �ʒu
	D3DXVECTOR3 Initpos;							// �����I�t�Z�b�g�ʒu
	D3DXVECTOR3	posAddMotion;						// ���Z���[�V�����ʒu
	D3DXVECTOR3	rotAddMotion;						// ���Z���[�V��������
	D3DXVECTOR3 DiffrotMotion;						// ����
	D3DXMATRIX mtxWorld;							// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxWorldOld;							// ���[���h�}�g���b�N�X
	int nIdxModel;									// �����̃��f���̔ԍ�
	int nIdxModelPareant;							// �e���f���̃C���f�b�N�X
}PlayerModel;
typedef struct
{
	CollisionAttackPlayer ColAttack[MAX_COLLISION];	// �U������
	CollisionBodyPlayer ColBody[MAX_COLLISION];		// �_���[�W����
	PlayerModel aModel[MAX_PLAYERMODEL];// �p�[�c��
	MotionPlayer aMotion[MAX_MOTION];	// ���[�V�����̏��
	PlayerOrbit orbit[PLAYER_MOTION_TYPE_MAX][MAX_PLAYER_ORBIT];			// �O�Ղ̍ő�
	float fRadiusWidth;					// ���a��
	float fRadiusHeight;				// ���a����
	int nNumParts;						// �p�[�c��
	int nNumMotion;						// ���[�V�����ԍ�
	float JumpSpeed;					// �W�����v��
	float MoveSpeed;					// �ړ����x
	int nInitLife;						// �������C�t
}PlayerData;
typedef struct
{
	CollisionAttackPlayer ColAttack[MAX_COLLISION];	// �U������
	CollisionBodyPlayer ColBody[MAX_COLLISION];		// �_���[�W����
	PlayerModel aModel[MAX_PLAYERMODEL];// �p�[�c��
	PlayerOrbit orbit[PLAYER_MOTION_TYPE_MAX][MAX_PLAYER_ORBIT];			// �O�Ղ̍ő�

	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 posOld;					// �O��̈ʒu
	D3DXVECTOR3 Diffpos;				// �ړI�̈ʒu
	D3DXVECTOR3 Initpos;				// �����ʒu
	D3DXVECTOR3 move;					// �ړ���
	D3DXVECTOR3 moveR;					// �ړ���
	D3DXVECTOR3 moveL;					// �ړ���
	D3DXVECTOR3 CtrlMove;				// ����̈ړ���
	D3DXCOLOR col;						// �F
	float fRadiusWidth;					// ���a��
	float fRadiusHeight;				// ���a����
	float JumpSpeed;					// �W�����v��
	float MoveSpeed;					// �ړ����x
	D3DXVECTOR3 Diffrot;				// �p�x�̍���
	D3DXVECTOR3 Destrot;				// �ړI�̊p�x
	D3DXVECTOR3 rot;					// �p�x
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	PlayerMotionType nMotionType;		// ���
	PlayerMotionType nMotionTypeOld;	// �O�̏��
	PlayerMotionType nMotionGetCol;		// �_���[�W�^�����Ƃ��̏��
	PlayerMotionType nMotionGetColOld;	// �_���[�W�^�����Ƃ��̑O�̏��
	PLAYERSTATE state;					// ���
	int nCntState;						// ���
	int nIdyShadow;						// �e�̔ԍ�
	int nIdyLine;						// ����̔ԍ�
	bool bMotionState[PLAYER_BOOL_MAX];			// ���[�V�����g�p����
	bool bMotionStateOld[PLAYER_BOOL_MAX];		// �O��̃��[�V�����g�p����
	bool bBlend;						// �u�����h�����ǂ���
	bool bJump;							// �W�����v���Ă��邩�ǂ���
	int nDamage;						// �_���[�W
	float fLife;							// ���C�t
	bool bSound[SOUND_LABEL_MAX];		// ��
	int nIdy;							// �ԍ�
	MeshField *pMeshFieldLand;			// �ǂ̏��ɏ���Ă��邩
	D3DXVECTOR3 fFlyPower;									// ������ё��x
	int nFlyCounter;										// ���ł��鎞��
	float fRotSpeed;										// ��]�̌����X�s�[�h
	Model *pModel;											// �����������f���̏��
	Model *pModelAtkData;									// �����������f���̏��
	Model *pModelAtk;										// �����������f���̏��
	int nAirCounter;										// ���ł鎞�̃R���{����
	int nLoopCounter;										// ���[�V�����̃��[�v�J�E���^
	int nKeyCounter;										// �L�[��
	int nFrameCounter;										// �t���[����
	int nFrameAllCounter;									// �ő�t���[����
	int nFrame;												// �t���[
	bool bUse;
	bool bDraw;
}Player;

//******************************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(int nIdy);
void DrawPlayer(int nIdy,int nCamera);
Player *GetPlayer(void);
PlayerData *GetPlayerData(void);
void MotionInfoPlayer(int nIdy);
void SetAttackCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset,float fRadius,  int nCollisionStart, int nCollisionEnd,int nStopTime,PlayerMotionType nMotionType);
void SetBodyCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius);
bool CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, float nPower, float fDamageSide, float fDamageV, int nIdy);
void PlayerController(int nIdy);					// �v���C���[�̑���
void PlayerOverLap(int nIdy);
void PlayerColInfo(int nIdy);
void PlayerMotionEffect(int nIdy,int nCollision);
void SetPlayer(int nIdy, D3DXVECTOR3 pos);
void ReleasePlayer(int nIdy);
int CheckLife(void);
#endif
