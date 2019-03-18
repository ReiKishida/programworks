//=============================================================================
//
// �G����[2D �|���S��] [player.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _ENEMYPOW_H_
#define _ENEMYPOW_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMYPOW_PARTS (16)	//�G�̃p�[�c��
#define MAX_KEY_POW (20)			//�ő�L�[��
//*****************************************************************************
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CSceneGauge;
class CUiGaugeBillBoard;
//*****************************************************************************
//�G�̃N���X�錾
//*****************************************************************************
class CEnemyPower : public CScene
{
public:
	//�L�[�v�f
	typedef struct
	{
		int nFrame;								//�t���[����
		D3DXVECTOR3 pos[MAX_ENEMYPOW_PARTS];		//�ʒu���
		D3DXVECTOR3 rot[MAX_ENEMYPOW_PARTS];		//�p�x���
	}KEY;

	typedef struct
	{
		int nLoop;								//���[�v���邩
		int nNumKey;							//�L�[��
		KEY aKey[MAX_KEY_POW];						//�L�[
	}KEY_INFO;

	typedef enum
	{//�G�̎��
		ENEMYTYPE_HEAVY = 0,		// �d�ʌ^
		ENEMYTYPE_MAX			// �G�̎�ނ̍ő吔
	}ENEMYTYPE;

	typedef enum
	{//�G�̏��
		ENEMYSTATE_NORMAL = 0,	// �ʏ���
		ENEMYSTATE_STEEL,		// �d�����
		ENEMYSTATE_BOUND,		// �o�E���h
		ENEMYSTATE_DEATH,		// ���S
		ENEMYSTATE_APPEAR,		// �o�������
		ENEMYSTATE_MAX
	}ENEMYSTATE;
	typedef enum
	{// ���[�V�����̎��
		MOTION_NEUTRAL = 0,			// �j���[�g������
		MOTION_MOVE,				// �ړ���
		MOTION_LIGHTATTACK,			// ��U��
		MOTION_JUMP,				// �W�����v��
		MOTION_LANDING,				// ���n��
		MOTION_HEAVYATTACK,			// ���U��
		MOTION_NORMALDAMAGE,		// �_���[�W(�ʏ�)
		MOTION_FLOATDAMAGE,			// �_���[�W(������)
		MOTION_IMPACTDAMAGE,		// �_���[�W(�������)
		MOTION_DEATH,				// ���S
		MOTION_MAX					// ���[�V�����̑���
	}MOTIONSTATE;

	typedef enum
	{// �t���O�̎��
		ENEMYBOOLTYPE_JUMP = 0,		// �W�����v
		ENEMYBOOLTYPE_SLIDING,		// �X���C�f�B���O
		ENEMYBOOLTYPE_LANDING,		// ���n
		ENEMYBOOLTYPE_LIGHTATTACK,	// ��U��
		ENEMYBOOLTYPE_HEAVYATTACK,	// ���p���`
		ENEMYBOOLTYPE_NORMALDAMAGE,	// �_���[�W(�ʏ�)
		ENEMYBOOLTYPE_MEDIUMDAMAGE,	// �_���[�W(��)
		ENEMYBOOLTYPE_FLOATDAMAGE,	// �_���[�W(������)
		ENEMYBOOLTYPE_BOUNDDAMAGE,	// �_���[�W(�o�E���h)
		ENEMYBOOLTYPE_IMPACTDAMAGE,	// �_���[�W(�������)
		ENEMYBOOLTYPE_DEATH,		// ���S
		ENEMYBOOLTYPE_MAX			// ����
	}ENEMYBOOLTYPE;

	typedef enum
	{// �t���O�̎��
		HITTYPE_LIGHTPUNCH = 0,		//�v���C���[�̍U��(��p���`)
		HITTYPE_MEDIUMPUNCH,		//�v���C���[�̍U��(���p���`)
		HITTYPE_SHORYUKEN,			//�v���C���[�̍U��(������)
		HITTYPE_HEAVYPUNCH,			//�v���C���[�̍U��(���p���`)
		HITTYPE_LIGHTKICK,			//�v���C���[�̍U��(��L�b�N)
		HITTYPE_MEDIUMKICK,			//�v���C���[�̍U��(���L�b�N)
		HITTYPE_SPINKICK,			//�v���C���[�̍U��(��]�R��)
		HITTYPE_HEAVYKICK,			//�v���C���[�̍U��(���L�b�N)
		HITTYPE_SKILL,				//���K�E�Z
		HITTYPE_MAX
	}HITTYPE;

	CEnemyPower(int nPriority = 1, OBJTYPE type = OBJTYPE_ENEMY_POW);	// �R���X�g���N�^
	~CEnemyPower();													// �f�X�g���N�^
	static HRESULT Load(void);									// �e�N�X�`���̓ǂݍ���
	static void Unload(void);									// �e�N�X�`���̔j��
	static CEnemyPower *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, ENEMYTYPE type);		// �G�̐���
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	void DrawShadow(void);										// �G�̉e
	void DrawParts(void);										// �G�̃p�[�c�̕`��
	void EnemyMove(void);										// �G�̈ړ�
	void EnemyAction(void);										// �G�̃A�N�V����
	void EnemyFallen(void);										// �G�̗���
	void EnemyMotion(void);										// �G�̃��[�V����
	void EnemyState(void);										// �G�̏�ԕω�
	void EnemySetMove_Interia(void);							// �G�̈ړ��ʂ̑���Ɗ����ړ�
	void EnemyMotionState(bool *bAction);						// ���[�V�����̏�Ԑݒ�
	void EnemyRotFixes(D3DXVECTOR3 rot);						// �p�x�C��
	void EnemyRotFixes(float rot);								// �p�x�C��
	void EnemyDebugLog(void);									// �f�o�b�N���O
	void LoadEnemyText(void);									// �G���̓ǂݍ��݂ƃp�[�c�̐���
	void InitEnemy(void);										// �G���̏�����
	void InitMotion(void);										// ���[�V�������̏�����
	void DefaultParts(void);									// �p�[�c�̏����ݒ�
	void CollisionObjectEnemy(void);							// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionWallEnemy(void);								// �ǂƂ̓����蔻��
	void CollisionCylinderEnemy(void);							// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionAttack(void);									// �U���̓����蔻��
	void CollisionPlayer(void);									// �G�Ƃ̓����蔻��
	void EnemyLifeCheck(void);									//�G�̗̑͂̃`�F�b�N
	void CreateGauge(void);										// �Q�[�W�̐���
	void SetGauge(void);										// �Q�[�W�ʒu�̑��
	void UpdateCancelCombo(void);								// �L�����Z���t���[��
	void UpdateFind(void);										//�G�T��
	D3DXVECTOR3 GetPos(void);									// �ʒu���̎擾
	D3DXVECTOR3 GetRot(void);									// �p�x���̎擾
	D3DXVECTOR3 GetMove(void);									// ���x���̎擾
	bool FindPlayer(D3DXVECTOR3 *pos, float fRange);			//�G�T��
	bool AttackPlayer(D3DXVECTOR3 *pos, float fRange);			//�G�ւ̍U��
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//�G�Ƃ̓����蔻��


private:
	D3DXVECTOR3 m_Partspos[MAX_ENEMYPOW_PARTS];				// �p�[�c�̈ʒu
	D3DXVECTOR3 m_Partsrot[MAX_ENEMYPOW_PARTS];				// �p�[�c�̊p�x
	D3DXVECTOR3 m_PartsDefaultpos[MAX_ENEMYPOW_PARTS];		// �p�[�c�̈ʒu
	D3DXVECTOR3 m_PartsDefaultrot[MAX_ENEMYPOW_PARTS];		// �p�[�c�̊p�x
	D3DXVECTOR3 m_Motionpos[MAX_ENEMYPOW_PARTS];			// ���[�V������������p�ʒu
	D3DXVECTOR3 m_Motionrot[MAX_ENEMYPOW_PARTS];			// ���[�V������������p�p�x

	int m_aParent[MAX_ENEMYPOW_PARTS];						// �p�[�c�̐e
	int m_aIndex[MAX_ENEMYPOW_PARTS];						// �p�[�c�̔ԍ�

														//�G�̊�{���//
														//�ÓI�����o�ϐ�//
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	static char						 m_acFileName[MAX_ENEMYPOW_PARTS][256];	// �ǂݍ���
	static LPD3DXMESH				 m_apMesh[MAX_ENEMYPOW_PARTS];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				 m_apBuffMat[MAX_ENEMYPOW_PARTS];			// �}�e���A�����ւ̃|�C���^
	static DWORD					 m_aNumMat[MAX_ENEMYPOW_PARTS];			// �}�e���A���̏��̐�
	static CMeshOrbit *m_pMeshOrbit;										// �O�Ղ̃|�C���^
	float m_fLife;															// �G�̃��C�t

																			//�ʏ�̕ϐ�//
	CUiGaugeBillBoard  *m_apLifeGauge[3];	// Ui�̃|�C���^
	float m_fEnemyMove;						// �G�̈ړ���
	float m_fDestAngle;						// �ړI�̊p�x
	float m_fAngle;							// �p�x
	float m_fHeight;						// ����
	bool m_bUseDraw;						// �`�悷�邩
	bool m_bJump;							// �W�����v���Ă��邩
	bool m_bEffect;							// �c���̗L��
	int m_StateCnt;							// ��ԕω��̃J�E���^
	int m_Jumptime;							// �؋󂵂Ă��鎞��
	int	m_nId;								// ����p��ID
	int m_nCancelFlame;						// �L�����Z���t���[��
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// ���݂̈ʒu
	D3DXVECTOR3 m_posold;					// �ȑO�̈ʒu
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_jumppower;				// �W�����v��
	ENEMYSTATE m_state;						// �G�̏��
	ENEMYTYPE m_type;						// �G�̎��
	CModel *m_apModel[MAX_ENEMYPOW_PARTS];		// �L�����N�^�[�̃p�[�c


											/////���[�V�����֌W�̕ϐ�/////
	int m_nNumKey;							// �L�[�̑���
	KEY_INFO *m_pKeyInfo;					// �L�[���̃|�C���^
	KEY_INFO m_aKeyInfo[MOTION_MAX];		// �L�[���̃|�C���^
	MOTIONSTATE m_Motionstate;				// ���[�V�����̏��
	MOTIONSTATE m_MotionstateOld;			// �ȑO�̃��[�V�����̏��
	bool m_bAction[ENEMYBOOLTYPE_MAX];		// �s�����̃t���O
	bool m_bMotionBlend;					// �u�����h���邩
	int m_nKey;								// ���݂̃L�[�i���o�[
	int m_nAllFrame;						// ���݂̃L�[�i���o�[
	int m_nCountMotion;						// ���[�V�����J�E���^�[
	int m_nCounterFrame;					// �t���[���̃J�E���^�[
	int m_nCounterAllFrame;					// ���[�V�����J�E���^�[
	int m_nCounterKey;						// �L�[�̃J�E���^�[
	int m_nFrame;							// ���̃t���[��
	int m_nCooltime;						// �N�[���^�C��
	float m_fRadius;						// �����蔻��̔��a
};


#endif