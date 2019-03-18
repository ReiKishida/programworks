//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : ��������
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "model.h"
#include "meshField.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY						(200)		// �G�̑���
#define MAX_ENEMY_MODEL					(20)		// �G�̍ő僂�f���̐�
#define ENEMY_MAX_KEY					(20)		// �ő�̃L�[�t���[����
#define ENEMY_MAX_MOTION				(20)		// �ő�̃��[�V������
#define ENEMY_MAX_COLLISION_ATTACK		(10)		// �U���̓����蔻��̍ő吔
#define ENEMY_MAX_COLLISION_BODY		(10)		// �̂̓����蔻��̍ő吔
#define ENEMY_MAX_DATUYOU_TYPE			(3)			// �D�x�R�}���h�̍ő吔

//*****************************************************************************
// �\����
//*****************************************************************************
typedef enum
{// �G�̎��
	ENEMY_TYPE_ARMOR = 0,		// �Z
	ENEMY_TYPE_HITOME,			// ���
	ENEMY_TYPE_HIGECHAN,		// �Ђ������
	ENEMY_TYPE_HONCHAN,			// �{�����
	ENEMY_TYPE_MAX,				// ����
}ENEMY_TYPE;

typedef enum
{// �G�̏�Ԃ̎��
	ENEMY_STATE_NORMAL = 0,		// �ʏ���
	ENEMY_STATE_DAMAGE,			// �_���[�W���
	ENEMY_STATE_DEATH,			// ���S���
	ENEMY_STATE_MAX				// ��Ԃ̑���
}ENEMY_STATE;

typedef enum
{// ���[�V�����̎��
	ENEMY_MOTION_NEUTRAL = 0,					// �j���[�g������
	ENEMY_MOTION_MOVE,							// �ړ���
	ENEMY_MOTION_ATTACK,						// �ʏ�U��
	ENEMY_MOTION_JUMP,							// �W�����v��
	ENEMY_MOTION_LANDING,						// ���n��
	ENEMY_MOTION_DAMAGE,						// �����莞
	ENEMY_MOTION_FLY,							// ������ю�
	ENEMY_MOTION_GETUP,							// �N���オ�莞
	ENEMY_MOTION_MAX							// ���[�V�����̑���
}ENEMY_MOTION;

typedef enum
{// �U���̎��
	ENEMY_ATTACKTYPE_ATTACK = 0,				// �ʏ�U��
	ENEMY_ATTACKTYPE_MAX						// �U���̎�ނ̑���
}ENEMY_ATTACKTYPE;

typedef enum
{// �s���p�^�[���̎��
	ENEMY_ACT_TYPE_NEUTRAL = 0,					// �_����
	ENEMY_ACT_TYPE_LOOK,						// �v���C���[������
	ENEMY_ACT_TYPE_WANDER,						// �����
	ENEMY_ACT_TYPE_APPROACHES,					// �v���C���[�ɋ߂Â�
	ENEMY_ACT_TYPE_ATTACK,						// �U������
	ENEMY_ACT_TYPE_MAX							// �s���p�^�[���̑���
}ENEMY_ACT_TYPE;

typedef enum
{// bool�̎��
	ENEMY_BOOLTYPE_JAMP = 0,				// �W�����v�����ǂ���
	ENEMY_BOOLTYPE_LANDING,					// ���n�����ǂ���
	ENEMY_BOOLTYPE_DAMAGE,					// �_���[�W�����ǂ���
	ENEMY_BOOLTYPE_FLY,						// ������ђ����ǂ���
	ENEMY_BOOLTYPE_GETUP,					// �N���オ�蒆���ǂ���
	ENEMY_BOOLTYPE_MAX						// �N���オ�蒆���ǂ���
}ENEMY_BOOLTYPE;

typedef struct
{// �L�[�t���[���̍\����
	__int8 nFrame;							// �t���[����
	D3DXVECTOR3 pos[MAX_ENEMY_MODEL];		// �ʒu���W
	D3DXVECTOR3 rot[MAX_ENEMY_MODEL];		// ��]���W
}Enemy_KyeInfo;

typedef struct
{// ���[�V�����̍\����
	__int8 nLoop;								// ���[�v���邩�ǂ���(0 : ���[�v���Ȃ� / 1 : ���[�v����)
	__int8 nNumKey;								// �L�[��
	Enemy_KyeInfo Motion[ENEMY_MAX_KEY];		// ���[�V�����̏��
	__int8 nAttackPower;						// ���[�V�����̍U����
	float fDamageSide;							// ���[�V�����̐��������ɐ�����΂���
	float fDamageVertical;						// ���[�V�����̐��������ɐ�����΂���
	
}Enemy_Motion;

typedef struct
{// ���f�����̍\����
	char cIP[50];
	LPD3DXMESH			pMesh;					// ���b�V��(���_���̏W�܂�)���ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMat;				// �}�e���A�����̐�
	LPDIRECT3DTEXTURE9	*pTexture;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3			vtxMin, vtxMax;			// ���f���̍ŏ��l�E�ő�l
}EnemyModelAddress;

typedef struct
{// �e�p�[�c�̃I�t�Z�b�g�̍\����
	D3DXVECTOR3 pos;						// �ʒu
	D3DXVECTOR3 rot;						// ����
	__int8		nIdxModel;					// �g�p���f���̃C���f�b�N�X
	__int8		nIdxParentModel;			// �e���f���̃C���f�b�N�X
}Enemy_ModelInfo;

typedef struct
{// �e�p�[�c�̏��
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3			pos;					// �ʒu
	D3DXVECTOR3			rot;					// ����
	__int8				nIdxModel;				// �g�p���f���̃C���f�b�N�X
	__int8				nIdxParentModel;		// �e���f���̃C���f�b�N�X
}Enemy_Model;

typedef struct
{// �U���̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			pos;					// �ʒu
	D3DXVECTOR3			rot;					// ����
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X
	__int8				nIdxParent;				// �e���f���̃C���f�b�N�X
	float				fRadius;				// ���a
	short int			nCollisionStart;		// �����蔻��̎n�܂�̃t���[����
	short int			nCollisionEnd;			// �����蔻��̏I���̃t���[����
	__int8				nMotionType;			// �����蔻�肪�t�����[�V�����̃^�C�v
	bool				bUse;					// �����蔻�肪�g���Ă邩�ǂ���
}CollisionAttackEnemy;

typedef struct
{// �̂̓����蔻��̃I�t�Z�b�g�̍\����
	D3DXVECTOR3			pos;								// �ʒu
	D3DXVECTOR3			rot;								// ����
	D3DXMATRIX			mtxWorld;							// ���[���h�}�g���b�N�X
	__int8				nIdxParent;							// �e���f���̃C���f�b�N�X
	float				fRadius;							// ���a
	bool				bUse;								// �����蔻�肪�g���Ă邩�ǂ���
}CollisionBodyEnemy;

typedef struct
{// �G�̎�ނ��Ƃ̃��f�������[�V�����̍\����
	EnemyModelAddress	ModelAddress[MAX_ENEMY_MODEL];					// ���f���̏��
	Enemy_ModelInfo		ModelInfo[MAX_ENEMY_MODEL];						// �e�p�[�c�̃I�t�Z�b�g�̍\����
	Enemy_Motion		Motion[ENEMY_MAX_MOTION];						// ���[�V�������
	int					MaxModel;										// ���f���̑���
	int					MaxParts;										// �p�[�c�̑���

	// �L�����N�^�[�ʂ̃X�e�[�^�X
	float				fMoveSpeed;										// �ړ����x
	float				fJamp;											// �W�����v��
	int					nLife;											// �̗�
	float				fRadius;										// ���a

	// �L�����N�^�[�̓����蔻��̍\����
	CollisionAttackEnemy CollisionAttack[ENEMY_MAX_COLLISION_ATTACK];	// �U���̓����蔻��
	int					 nCntMaxCollisionAttack;						// �U���̓����蔻��̍ő吔
	CollisionBodyEnemy   CollisionBody[ENEMY_MAX_COLLISION_BODY];		// �̂̓����蔻��
	int					 nCntMaxCollisionBody;							// �̂̓����蔻��̍ő吔

}Enemy_info;

typedef struct
{// �G�̍\����
	D3DXVECTOR3			 pos;										// �ʒu
	D3DXVECTOR3			 posOld;									// �O��̈ʒu
	D3DXVECTOR3			 InitPos;									// �����ʒu
	D3DXVECTOR3			 Move;										// �ʒu�̈ړ���
	D3DXVECTOR3			 rot;										// ����
	D3DXVECTOR3			 rotDest;									// �ړI�̌���
	D3DXMATRIX			 mtxWorld;									// ���[���h�}�g���b�N�X
	
	// �g�p���Ă���ԍ��̕ϐ�
	int					 nIdyShadow;								// �e�̔ԍ�
	int					 nIdxLight;									// ���C�g�̔ԍ�
	int					 nIdxLifeGauge;								// ���C�t�Q�[�W�̔ԍ�
	int					 nIdxDatuyou;								// �D�x��UI�̔ԍ�

	// �J�E���^�[�֌W
	int					 nAttackWaitTime;							// �U���̑ҋ@����
	int					 nAttackCounter;							// �U��������܂ł̃J�E���^�[�ϐ�
	int					 nFlyCounter;								// ������Ԏ��Ԃ̃J�E���^�[�ϐ�
	int					 nDownCounter;								// �_�E�����̃J�E���^�[�ϐ�
	int					 nActNowCounter;							// ���݂̍s���̃J�E���^�[
	int					 nActTime;									// �s������t���[����������ϐ�
	int					 nCounterState;								// ��Ԃ̃J�E���^�[�ϐ�

	// �L�����N�^�[�̃X�e�[�^�X�֌W
	ENEMY_STATE			 State;											// �G�̏��
	ENEMY_TYPE			 EnemyType;										// �G�̃^�C�v	
	int					 nLife;											// ���D��
	int					 nMaxLife;										// ���C�t�̍ő�l
	float				 fMoveSpeed;									// �ړ����x
	float				 fJamp;											// ��{�̃W�����v��
	float				 fJampPower;									// ���݂̃W�����v��
	float				 fRadius;										// �G�̓����蔻��̔��a
	D3DXVECTOR3			 fFlyPower;										// ������ї�

	// �s���n�̕ϐ�
	float				 fActProportion[ENEMY_ACT_TYPE_MAX];			// �G�̍s���p�^�[���̊���
	int					 nActType;										// �s���̃^�C�v������ϐ�
	float				 fWanderAngle;									// ��������̃����_���Ȋp�x
	bool				 bMoved;										// ��������treu(�Z�̂�)
	// ���낢��
	float				 fPosLeght;										// �v���C���[�Ƃ̋���
	bool				 bActionFlag[ENEMY_BOOLTYPE_MAX];				// �s�����̃t���O

	Enemy_Model			 aModel[MAX_ENEMY_MODEL];						// ���f���̐�
	ENEMY_MOTION	 Motionstate;									// ���[�V�����̏��
	ENEMY_MOTION	 MotionstateOld;								// �O��̃��[�V�����̏��
	CollisionAttackEnemy CollisionAttack[ENEMY_MAX_COLLISION_ATTACK];	// �G�̍U���̓����蔻��
	CollisionBodyEnemy   CollisionBody[ENEMY_MAX_COLLISION_BODY];		// �G�̑̂̓����蔻��
	bool				 bUse;											// �g�p���Ă��邩�ǂ���

	// ���[�V��������̕ϐ�
	D3DXVECTOR3			posMotion[MAX_ENEMY_MODEL];					// �ʒu�̍���
	D3DXVECTOR3			rotMotion[MAX_ENEMY_MODEL];					// ��]�̍���
	int					nCounterKey;								// ���݂̃L�[��
	int					nCounterFrame;								// ���݂̃t���[����
	int					nCounterAllFrame;							// ���[�V�����̑S�L�[�t���[���̃t���[����
	int					nFrame;										// ����̃t���[����
	bool				bEnemyAttack[ENEMY_ATTACKTYPE_MAX];			// �U�������ǂ���
	bool				bEnemyAttackReady[ENEMY_ATTACKTYPE_MAX];	// ���̍U���Ɉڂ邩�ǂ���

	Model				*pModel;									// ���f���̏��
	MeshField			*pMeshField;								// ���b�V���t�B�[���h���
}Enemy;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void EnemyController(int nCntEnemy);							// �G�̑���
void EnemyModelSet(void);										// �G���f���̐ݒ�
void EnemyMotion(int nCntEnemy);								// �G�̃��[�V����
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE EnemyType, bool bMove);				// �G�̐ݒu
void CollisionEnemy(D3DXVECTOR3 *pos, float fRadius);			// �L�����N�^�[���d�Ȃ�Ȃ����߂̓����蔻��
void CollisionEnemyBody(D3DXVECTOR3 pos, 
						D3DXVECTOR3 mtxWorld, 
						float fRadius,
						int nPower,
						float fDamageSide,
						float fDamageVertical);					// �G�̑̂ƍU���̓����蔻��
void DeletEnemy(int nCntEnemy,bool bKo);						// �G�̍폜
void EnemyLode(void);											// �G�̃��f��&���[�V���������e�L�X�g�t�@�C������擾
void EnemySetLode(void);										// �G�̔z�u�����e�L�X�g�t�@�C������擾
Enemy *GetEnemy(void);											// �G���̎擾

int GetNumEnemy(void);											// �G�̐l�����擾

#endif
