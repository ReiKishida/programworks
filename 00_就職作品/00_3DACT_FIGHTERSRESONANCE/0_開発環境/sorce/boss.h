//=============================================================================
//
// �G����[2D �|���S��] [player.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BOSS_PARTS (16)	//�G�̃p�[�c��
#define MAX_KEY (20)			//�ő�L�[��
//*****************************************************************************
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CSceneGauge;
class CUiGauge;
//*****************************************************************************
//�G�̃N���X�錾
//*****************************************************************************
class CBoss : public CScene
{
public:
	//�L�[�v�f
	typedef struct
	{
		int nFrame;								//�t���[����
		D3DXVECTOR3 pos[MAX_BOSS_PARTS];		//�ʒu���
		D3DXVECTOR3 rot[MAX_BOSS_PARTS];		//�p�x���
	}KEY;

	typedef struct
	{
		int nLoop;								//���[�v���邩
		int nNumKey;							//�L�[��
		KEY aKey[MAX_KEY];						//�L�[
	}KEY_INFO;



	typedef enum
	{//�G�̏��
		BOSSSTATE_NORMAL = 0,	// �ʏ���
		BOSSSTATE_STEEL,		// �d�����
		BOSSSTATE_BOUND,		// �o�E���h
		BOSSSTATE_DEATH,		// ���S
		BOSSSTATE_APPEAR,		// �o�������
		BOSSSTATE_INVISIBLE,
		BOSSSTATE_MAX
	}BOSSSTATE;
	typedef enum
	{// ���[�V�����̎��
		MOTION_NEUTRAL = 0,			// �j���[�g������
		MOTION_MOVE,				// �ړ���
		MOTION_LIGHTATTACK,			// ��U��
		MOTION_JUMP,				// �W�����v��
		MOTION_LANDING,				// ���n��
		MOTION_HEAVYATTACK,			// ���U��
		MOTION_SKILL,				// ���K�E�Z
		MOTION_NORMALDAMAGE,		// �_���[�W(�ʏ�)
		MOTION_FLOATDAMAGE,			// �_���[�W(������)
		MOTION_IMPACTDAMAGE,		// �_���[�W(�������)
		MOTION_DEATH,				// ���S
		MOTION_MOVEATTACK,			// �ړ��U��
		MOTION_MAX					// ���[�V�����̑���
	}MOTIONSTATE;

	typedef enum
	{// �t���O�̎��
		BOSSBOOLTYPE_JUMP = 0,		// �W�����v
		BOSSBOOLTYPE_SLIDING,		// �X���C�f�B���O
		BOSSBOOLTYPE_LANDING,		// ���n
		BOSSBOOLTYPE_LIGHTATTACK,	// ��U��
		BOSSBOOLTYPE_HEAVYATTACK,	// ���p���`
		BOSSBOOLTYPE_SKILL,			// �X�L��
		BOSSBOOLTYPE_NORMALDAMAGE,	// �_���[�W(�ʏ�)
		BOSSBOOLTYPE_MEDIUMDAMAGE,	// �_���[�W(��)
		BOSSBOOLTYPE_FLOATDAMAGE,	// �_���[�W(������)
		BOSSBOOLTYPE_BOUNDDAMAGE,	// �_���[�W(�o�E���h)
		BOSSBOOLTYPE_IMPACTDAMAGE,	// �_���[�W(�������)
		BOSSBOOLTYPE_DEATH,			// ���S
		BOSSBOOLTYPE_MOVEATTACK,	// �ړ��U��
		BOSSBOOLTYPE_MAX			// ����
	}BOSSBOOLTYPE;

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
		HITTYPE_SKILL,				//�v���C���[�̍U��(�K�E�Z)
		HITTYPE_MAX
	}HITTYPE;

	CBoss(int nPriority = 1, OBJTYPE type = OBJTYPE_BOSS);	// �R���X�g���N�^
	~CBoss();													// �f�X�g���N�^
	static HRESULT Load(void);									// �e�N�X�`���̓ǂݍ���
	static void Unload(void);									// �e�N�X�`���̔j��
	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife);		// �G�̐���
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	void DrawShadow(void);										// �G�̉e
	void DrawParts(void);										// �G�̃p�[�c�̕`��
	void BossMove(void);										// �G�̈ړ�
	void BossAction(void);										// �G�̃A�N�V����
	void BossFallen(void);										// �G�̗���
	void BossMotion(void);										// �G�̃��[�V����
	void BossState(void);										// �G�̏�ԕω�
	void BossSetMove_Interia(void);								// �G�̈ړ��ʂ̑���Ɗ����ړ�
	void BossMotionState(bool *bAction);						// ���[�V�����̏�Ԑݒ�
	void BossRotFixes(D3DXVECTOR3 rot);							// �p�x�C��
	void BossRotFixes(float rot);								// �p�x�C��
	void BossDebugLog(void);									// �f�o�b�N���O
	void LoadBossText(void);									// �G���̓ǂݍ��݂ƃp�[�c�̐���
	void InitBoss(void);										// �G���̏�����
	void InitMotion(void);										// ���[�V�������̏�����
	void DefaultParts(void);									// �p�[�c�̏����ݒ�
	void CollisionObjectBoss(void);								// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionWallBoss(void);								// �ǂƂ̓����蔻��
	void CollisionCylinderBoss(void);							// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionAttack(void);									// �U���̓����蔻��
	void CollisionPlayer(void);									// �{�X�Ƃ̓����蔻��
	void BossLifeCheck(void);									//�{�X�̗̑͂̃`�F�b�N
	void CreateGauge(void);										// �Q�[�W�̐���
	void UpdateCancelCombo(void);								// �L�����Z���t���[��
	void UpdateFind(void);										//�{�X�T��
	D3DXVECTOR3 GetPos(void);									// �ʒu���̎擾
	D3DXVECTOR3 GetRot(void);									// �p�x���̎擾
	D3DXVECTOR3 GetMove(void);									// ���x���̎擾
	bool FindPlayer(D3DXVECTOR3 *pos, float fRange);			//�{�X�T��
	bool AttackPlayer(D3DXVECTOR3 *pos, float fRange);			//�{�X�ւ̍U��
	bool CollisionBoss(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//�G�Ƃ̓����蔻��


private:
	D3DXVECTOR3 m_Partspos[MAX_BOSS_PARTS];				// �p�[�c�̈ʒu
	D3DXVECTOR3 m_Partsrot[MAX_BOSS_PARTS];				// �p�[�c�̊p�x
	D3DXVECTOR3 m_PartsDefaultpos[MAX_BOSS_PARTS];		// �p�[�c�̈ʒu
	D3DXVECTOR3 m_PartsDefaultrot[MAX_BOSS_PARTS];		// �p�[�c�̊p�x
	D3DXVECTOR3 m_Motionpos[MAX_BOSS_PARTS];			// ���[�V������������p�ʒu
	D3DXVECTOR3 m_Motionrot[MAX_BOSS_PARTS];			// ���[�V������������p�p�x

	int m_aParent[MAX_BOSS_PARTS];						// �p�[�c�̐e
	int m_aIndex[MAX_BOSS_PARTS];						// �p�[�c�̔ԍ�

														//�G�̊�{���//
														//�ÓI�����o�ϐ�//
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	static char						 m_acFileName[MAX_BOSS_PARTS][256];	// �ǂݍ���
	static LPD3DXMESH				 m_apMesh[MAX_BOSS_PARTS];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				 m_apBuffMat[MAX_BOSS_PARTS];			// �}�e���A�����ւ̃|�C���^
	static DWORD					 m_aNumMat[MAX_BOSS_PARTS];			// �}�e���A���̏��̐�
	static CMeshOrbit *m_pMeshOrbit;										// �O�Ղ̃|�C���^
	float m_fLife;															// �G�̃��C�t
	float m_fSkill;														// �G�̃��C�t

																			//�ʏ�̕ϐ�//
	CUiGauge *m_apLifeGauge[4];				// Ui�̃|�C���^
	float m_fBossMove;						// �G�̈ړ���
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
	BOSSSTATE m_state;						// �G�̏��
	CModel *m_apModel[MAX_BOSS_PARTS];		// �L�����N�^�[�̃p�[�c


											/////���[�V�����֌W�̕ϐ�/////
	int m_nNumKey;							// �L�[�̑���
	KEY_INFO *m_pKeyInfo;					// �L�[���̃|�C���^
	KEY_INFO m_aKeyInfo[MOTION_MAX];		// �L�[���̃|�C���^
	MOTIONSTATE m_Motionstate;				// ���[�V�����̏��
	MOTIONSTATE m_MotionstateOld;			// �ȑO�̃��[�V�����̏��
	bool m_bAction[BOSSBOOLTYPE_MAX];		// �s�����̃t���O
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