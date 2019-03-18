//=============================================================================
//
// �v���C���[����[2D �|���S��] [player.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER_PARTS (16)	//�v���C���[�̃p�[�c��
#define MAX_PLAYER_UI (6)		//�v���C���[�̃p�[�c��
#define MAX_KEY (20)			//�ő�L�[��
#define MAX_ATTACKPARTS (4)		//�U�����s���p�[�c��
//*****************************************************************************
//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
//*****************************************************************************
class CModel;
class CMeshOrbit;
class CUiGauge;
class CComboGauge;
class CSceneGauge;
class CCollision;
//*****************************************************************************
//�v���C���[�̃N���X�錾
//*****************************************************************************
class CPlayer : public CScene
{
public:
	//�L�[�v�f
	typedef struct
	{//�L�[���̍\����
		int nFrame;								//�t���[����
		D3DXVECTOR3 pos[MAX_PLAYER_PARTS];		//�ʒu���
		D3DXVECTOR3 rot[MAX_PLAYER_PARTS];		//�p�x���
	}KEY;

	typedef struct
	{//���[�V�����̍\����
		int nLoop;				//���[�v���邩
		int nNumKey;			//�L�[��
		KEY aKey[MAX_KEY];		//�L�[
	}KEY_INFO;

	typedef struct
	{// �U���̓����蔻��̃I�t�Z�b�g�̍\����
		D3DXVECTOR3			pos;				// �ʒu
		D3DXMATRIX			mtxWorld;			// ���[���h�}�g���b�N�X
		int					nIdxParent;			// �e���f���̃C���f�b�N�X
		float				fRadius;			// ���a
		short int			nCollisionStart;	// �����蔻��̎n�܂�̃t���[����
		short int			nCollisionEnd;		// �����蔻��̏I���̃t���[����
	}CollisionAttackPlayer;


	typedef enum
	{//�v���C���[�̏��
		PLAYERSTATE_NORMAL = 0,		//�ʏ���
		PLAYERSTATE_STEEL,			//�d�����
		PLAYERSTATE_DAMAGE,			//�_���[�W���
		PLAYERSTATE_INVISIBLE,		//���G���
		PLAYERSTATE_APPEAR,			//�o�������
		PLAYERSTATE_DEATH,			//���S���
		PLAYERSTATE_MAX				//��Ԃ̍ő吔
	}PLAYERSTATE;

	typedef enum
	{// ���[�V�����̎��
		MOTION_NEUTRAL = 0,			// �j���[�g������
		MOTION_MOVE,				// �ړ���
		MOTION_SLIDING,				// �X���C�f�B���O
		MOTION_JUMP,				// �W�����v��
		MOTION_LANDING,				// ���n��
		MOTION_LIGHTPUNCH,			// ��p���`
		MOTION_MIDDLEPUNCH,			// ���p���`
		MOTION_SHORYUKEN,			// ������
		MOTION_HEAVYPUNCH,			// ���p���`
		MOTION_LIGHTKICK,			// ��L�b�N
		MOTION_MIDDLEKICK,			// ���L�b�N
		MOTION_SPINKICK,			// ��]�L�b�N
		MOTION_HEAVYKICK,			// ���L�b�N
		MOTION_SMALLDAMAGE,			// �_���[�W(��)
		MOTION_LARGEDAMAGE,			// �_���[�W(��)
		MOTION_SKILL,				// �X�^��
		MOTION_DEATH,				// ���S��
		MOTION_MAX					// ���[�V�����̑���
	}PLAYER_MOTIONSTATE;

	typedef enum
	{// �t���O�̎��
		PLAYERBOOLTYPE_JUMP = 0,	// �W�����v
		PLAYERBOOLTYPE_SLIDING,		// �X���C�f�B���O
		PLAYERBOOLTYPE_LANDING,		// ���n
		PLAYERBOOLTYPE_LIGHTPUNCH,	// ��p���`
		PLAYERBOOLTYPE_MIDDLEPUNCH,	// ���p���`
		PLAYERBOOLTYPE_SHORYUKEN,	// ������
		PLAYERBOOLTYPE_HEAVYPUNCH,	// ���p���`
		PLAYERBOOLTYPE_LIGHTKICK,	// ��L�b�N
		PLAYERBOOLTYPE_MIDDLEKICK,	// ���L�b�N
		PLAYERBOOLTYPE_SPINKICK,	// ��]�L�b�N
		PLAYERBOOLTYPE_HEAVYKICK,	// ���L�b�N
		PLAYERBOOLTYPE_SMALLDAMAGE,	// �_���[�W(��)
		PLAYERBOOLTYPE_MEDIUMDAMAGE,// �_���[�W(��)
		PLAYERBOOLTYPE_LARGEDAMAGE,	// �_���[�W(��)
		PLAYERBOOLTYPE_SKILL,		// ���K�E�Z
		PLAYERBOOLTYPE_DEATH,		// ���S
		PLAYERBOOLTYPE_MAX			// ����
	}PLAYERBOOLTYPE;

	typedef enum
	{// �t���O�̎��
		ATTACKBOOLTYPE_PUNCH_0,			// 1�i��
		ATTACKBOOLTYPE_PUNCH_1,			// 2�i��
		ATTACKBOOLTYPE_PUNCH_2,			// 3�i��
		ATTACKBOOLTYPE_PUNCH_HEAVY,		// ���U��
		ATTACKBOOLTYPE_PUNCH_MAX		// ����
	}ATTACKBOOLTYPE_PUNCH;

	typedef enum
	{// �t���O�̎��
		ATTACKBOOLTYPE_KICK_0,			// 1�i��
		ATTACKBOOLTYPE_KICK_1,			// 2�i��
		ATTACKBOOLTYPE_KICK_2,			// 3�i��
		ATTACKBOOLTYPE_KICK_HEAVY,		// ���U��
		ATTACKBOOLTYPE_KICK_MAX			// ����
	}ATTACKBOOLTYPEKICK;

	typedef enum
	{// �t���O�̎��
		HITTYPE_LIGHTATTACK = 9,	//�G�̍U��(��)
		HITTYPE_HEAVYATTACK,		//�G�̍U��(��)
		HITTYPE_BOSS_LIGHTATTACK,	//�{�X�̍U��(��U��)
		HITTYPE_BOSS_HEAVYATTACK,	//�{�X�̍U��(���U��)
		HITTYPE_BOSS_SKILLATTACK_S,	//�{�X�̍U��(���K�E�Z_��)
		HITTYPE_BOSS_SKILLATTACK_L,	//�{�X�̍U��(���K�E�Z_��)

		HITTYPE_MAX
	}HITTYPE;

	CPlayer(int nPriority = 1, OBJTYPE type = OBJTYPE_PLAYER);	// �R���X�g���N�^
	~CPlayer();													// �f�X�g���N�^
	static HRESULT Load(void);									// �e�N�X�`���̓ǂݍ���
	static void Unload(void);									// �e�N�X�`���̔j��
	static CPlayer *Create(D3DXVECTOR3 pos);					// �v���C���[�̐���
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	void DrawShadow(void);										// �v���C���[�̉e
	void DrawParts(void);										// �v���C���[�̃p�[�c�̕`��
	void PlayerMove(void);										// �v���C���[�̈ړ�
	void PlayerAction(void);									// �v���C���[�̃A�N�V����
	void PlayerFallen(void);									// �v���C���[�̗���
	void PlayerMotion(void);									// �v���C���[�̃��[�V����
	void PlayerState(void);										// �v���C���[�̏�ԕω�
	void PlayerSetMove_Interia(void);							// �v���C���[�̈ړ��ʂ̑���Ɗ����ړ�
	void PlayerMotionState(bool *bAction);						// ���[�V�����̏�Ԑݒ�
	void PlayerRotFixes(D3DXVECTOR3 rot);						// �p�x�C��
	void PlayerRotFixes(float rot);								// �p�x�C��
	void LoadPlayerText(void);									// �v���C���[���̓ǂݍ��݂ƃp�[�c�̐���
	void InitPlayer(void);										// �v���C���[���̏�����
	void InitMotion(void);										// ���[�V�������̏�����
	void CreateGauge(void);										// �Q�[�W�̐���
	void DefaultParts(void);									// �p�[�c�̏����ݒ�
	void CollisionAttack(void);									//�U���Ƃ̓����蔻��
	void CollisionObjectPlayer(void);							// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionWallPlayer(void);								// �ǂƂ̓����蔻��
	void CollisionCylinderPlayer(void);							// �I�u�W�F�N�g�Ƃ̓����蔻��
	void CollisionEnemy(void);									// �G�Ƃ̓����蔻��
	void PlayerDebugLog(void);									// �f�o�b�O���O
	void PlayerLifeCheck(void);									//�v���C���[�̗̑͂̃`�F�b�N
	void PlayerSkillCheck(void);								//�v���C���[�̃X�L���̃`�F�b�N
	void CreateParticleFlame_SpinKick(D3DMATRIX *mtx);								//���̃p�[�e�B�N������
	void CreateParticleFlame_Shoryu(D3DMATRIX *mtx);								//���̃p�[�e�B�N������
	void CreateParticleFlame_Skill(D3DMATRIX *mtx);								//���̃p�[�e�B�N������

	void CreateParticleDust(void);								//�y���̃p�[�e�B�N������
	void CreateParticleDust_Shoryu(void);								//�y���̃p�[�e�B�N������

	static float GetDamage(void);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRadius);	//�G�Ƃ̓����蔻��
	D3DXVECTOR3 GetPos(void);									// �ʒu���̎擾
	D3DXVECTOR3 GetRot(void);									// �p�x���̎擾
	D3DXVECTOR3 GetMove(void);									// ���x���̎擾
	static CUiGauge *GetUiGauge(int nIndex);									//�Q�[�WUI��Ԃ�
	static void SetSkill(float fSkill);	//�X�L���̑��
private:
	static D3DXVECTOR3 m_Partspos[MAX_PLAYER_PARTS];			// �p�[�c�̈ʒu
	static D3DXVECTOR3 m_Partsrot[MAX_PLAYER_PARTS];			// �p�[�c�̊p�x
	static D3DXVECTOR3 m_PartsDefaultpos[MAX_PLAYER_PARTS];		// �p�[�c�̈ʒu
	static D3DXVECTOR3 m_PartsDefaultrot[MAX_PLAYER_PARTS];		// �p�[�c�̊p�x
	static D3DXVECTOR3 m_Motionpos[MAX_PLAYER_PARTS];			// ���[�V������������p�ʒu
	static D3DXVECTOR3 m_Motionrot[MAX_PLAYER_PARTS];			// ���[�V������������p�p�x

	static int m_aParent[MAX_PLAYER_PARTS];						// �p�[�c�̐e
	static int m_aIndex[MAX_PLAYER_PARTS];						// �p�[�c�̔ԍ�
	//�v���C���[�̊�{���//
	//�ÓI�����o�ϐ�//
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	static char						 m_acFileName[MAX_PLAYER_PARTS][256];	// �ǂݍ���
	static LPD3DXMESH				 m_apMesh[MAX_PLAYER_PARTS];			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				 m_apBuffMat[MAX_PLAYER_PARTS];			// �}�e���A�����ւ̃|�C���^
	static DWORD					 m_aNumMat[MAX_PLAYER_PARTS];			// �}�e���A���̏��̐�
	static CMeshOrbit *m_pMeshOrbit;										// �O�Ղ̃|�C���^
	static CUiGauge *m_apUi[MAX_PLAYER_UI];									// Ui�̃|�C���^
	static CSceneGauge *m_apGauge[MAX_PLAYER_UI];		 					// Ui�̃|�C���^
	static CCollision *m_pCollisionAttack;									// �U�����̓����蔻��̃|�C���^
	static float m_fLife;													// �v���C���[�̃��C�t
	static float m_fSkill;													// �X�^��
	static float m_fDamage;													// �v���C���[�̃��C�t
	//�ʏ�̕ϐ�//

	float m_fPlayerMove;					// �v���C���[�̈ړ���
	float m_fDestAngle;						// �ړI�̊p�x
	float m_fAngle;							// �p�x
	float m_fComboValue;					//�R���{�Q�[�W��
	bool m_bUseDraw;						// �`�悷�邩
	bool m_bJump;							// �W�����v���Ă��邩
	bool m_bEffect;							// �c���̗L��
	int m_StateCnt;							// ��ԕω��̃J�E���^
	int m_Jumptime;							// �؋󂵂Ă��鎞��
	int	m_nId;								// ����p��ID
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// ���݂̈ʒu
	D3DXVECTOR3 m_posold;					// �ȑO�̈ʒu
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_jumppower;				// �W�����v��
	PLAYERSTATE m_state;					// �v���C���[�̏��
	CModel *m_apModel[MAX_PLAYER_PARTS];	// �L�����N�^�[�̃p�[�c


	/////���[�V�����֌W�̕ϐ�/////
	int m_nNumKey;									// �L�[�̑���
	KEY_INFO *m_pKeyInfo;							// �L�[���̃|�C���^
	KEY_INFO m_aKeyInfo[MOTION_MAX];				// �L�[���̃|�C���^
	PLAYER_MOTIONSTATE m_Motionstate;				// ���[�V�����̏��
	PLAYER_MOTIONSTATE m_MotionstateOld;			// �ȑO�̃��[�V�����̏��
	bool m_bAction[PLAYERBOOLTYPE_MAX];				// �s�����̃t���O
	bool m_bAttackPunch[ATTACKBOOLTYPE_PUNCH_MAX];	// �U�����̃t���O[�p���`]
	bool m_bAttackKick[ATTACKBOOLTYPE_KICK_MAX];	// �U�����̃t���O[�L�b�N]
	bool m_bMotionBlend;							// �u�����h���邩
	int m_nKey;										// ���݂̃L�[�i���o�[
	int m_nAllFrame;								// ���݂̃L�[�i���o�[
	int m_nCountMotion;								// ���[�V�����J�E���^�[
	int m_nCounterFrame;							// �t���[���̃J�E���^�[
	int m_nCounterAllFrame;							// ���[�V�����J�E���^�[
	int m_nCounterKey;								// �L�[�̃J�E���^�[
	int m_nFrame;									// ���̃t���[��
	float m_fRadius;								// �����蔻��̔��a
	float m_fHeight;								// ����
};


#endif