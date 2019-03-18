//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// ���[�V�������� [motion.h]
// Author : �� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _MOTION_H_
#define _MOTION_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �C���N���[�h�t�@�C��
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel;
class CMotion;
class CCollision;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_PARTS					(20)	// �ő�p�[�c��
#define MAX_MOTION_TEX				(20)	// �e�N�X�`���̍ő吔
#define MAX_KEY						(20)	// �L�[�t���[���̍ő吔
#define MAX_MOTION					(10)	// ���[�V�����̍ő吔
#define COLLISION_SPHERE_CREATE		(0)		// �����蔻��m�F�p�̋��̂𐶐����邩	[0]���Ȃ�/[1]����

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �N���X��`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CMotionData
{ // ���[�V�����f�[�^�N���X
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �\���̒�`
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef struct
	{ // ���f�����
		D3DXVECTOR3 pos;								// �p�[�c�̈ʒu���
		D3DXVECTOR3 rot;								// �p�[�c�̉�]���
		LPD3DXMESH pMesh;								// ���b�V���i���_�̏W�܂�j���ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;							// �}�e���A�����ւ̃|�C���^
		DWORD nNumMat;									// �}�e���A�����̐�
		LPDIRECT3DTEXTURE9 pTexture[MAX_MOTION_TEX];	// ���f����ɑ΂���e�N�X�`��
		int nIdxParent;									// �e�p�[�c�̃C���f�b�N�X
	}Model;

	typedef struct
	{ // �L�[���
		int nFrame;								// �L�[�t���[��
		D3DXVECTOR3 aPos[MAX_PARTS];			// �L�[���Ƃ̈ʒu���
		D3DXVECTOR3 aRot[MAX_PARTS];			// �L�[���Ƃ̉�]���
	}Key;

	typedef struct
	{ // ���[�V�������
		bool bLoop;								// ���[�v���邩�ǂ���
		int nNumKey;							// �L�[��
		Key aKey[MAX_KEY];						// �L�[���
		int nAllFrame;							// ���[�V�����S�̂̃t���[����
	}Motion;

	//+*++*++*++*++*++*++*++*++*++*++*
	// �v���g�^�C�v�錾
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotionData::CMotionData();
	CMotionData::~CMotionData();
	HRESULT Init(void);
	HRESULT Load(char *cTextName);
	void UnLoad(void);

	// �e��l�ݒ�E�擾�֐�
	Model *GetModel(const int nPartsId) { return &m_aModel[nPartsId]; }		// ���f�����擾
	Motion *GetMotion(const int nMotion) { return &m_aMotion[nMotion]; }	// ���[�V�������擾
	int GetNumParts(void) { return m_nNumParts; }							// �p�[�c���擾
	CCollision *GetCollisionData(void) { return m_pCollisionData; }			// �����蔻����擾

	// �ÓI�����o�֐�
	static CMotionData *Create(void);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �ϐ��錾
	//+*++*++*++*++*++*++*++*++*++*++*
	Model		m_aModel[MAX_PARTS];	// ���f�����
	Motion		m_aMotion[MAX_MOTION];	// ���[�V�������
	int			m_nNumParts;			// �p�[�c��
	CCollision	*m_pCollisionData;		// �����蔻����
};

class CMotionMove
{ // ���[�V�������[�u�N���X
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �v���g�^�C�v�錾
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotionMove();
	~CMotionMove();
	HRESULT Init(void);
	void Uninit(void);
	void MotionUpdate(CMotionData *motionData, int nMotionType, int nMotionTypeOld);
	void FrameUpdate(CMotionData *motionData, int nMotionType);

	// �e��ݒ�E�擾�֐�
	D3DXVECTOR3 GetPos(int nParts) { return m_pos[nParts]; }	// ���[�V�����ʒu�擾
	D3DXVECTOR3 GetRot(int nParts) { return m_rot[nParts]; }	// ���[�V���������擾
	int GetKeyCnt(void) { return m_nKeyCnt; }					// �L�[�J�E���g�擾
	int GetFrameCounter(void) { return m_nFrameCounter; }		// �t���[���J�E���^�[�擾

	// �ÓI�����o�֐�
	static CMotionMove *Create(void);

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �ϐ��錾
	//+*++*++*++*++*++*++*++*++*++*++*
	int					m_nKeyCnt;				// �L�[�J�E���g
	int					m_nFrameCounter;		// �t���[���J�E���^�[
	int					m_nAllFrameCounter;		// �S�̂̃t���[���J�E���^�[
	bool				m_bBlend;				// �u�����h�����ǂ���
	D3DXVECTOR3			m_pos[MAX_PARTS];		// ���[�V�����ʒu
	D3DXVECTOR3			m_rot[MAX_PARTS];		// ���[�V��������
	D3DXVECTOR3			m_posAdd[MAX_PARTS];	// ���Z���[�V�����ʒu
	D3DXVECTOR3			m_rotAdd[MAX_PARTS];	// ���Z���[�V��������
};

class CMotion : public CScene
{ // ���[�V�����N���X
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �v���g�^�C�v�錾
	//+*++*++*++*++*++*++*++*++*++*++*
	CMotion(int nPriority = CScene::PRIORITY_INIT);
	virtual ~CMotion();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void CollisionUpdate(CCollision *pCollision);

	// �e��ݒ�E�擾�֐�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }			// �ʒu�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }					// �ʒu�擾
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			// �O��ʒu�擾
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		// �ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) { return m_move; }				// �ړ��ʎ擾
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }			// �����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }					// �����擾
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }	// �ړ����x�ݒ�
	float GetSpeed(void) { return m_fSpeed; }					// �ړ����x�擾
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld;}
	void SetMatrix(const D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetMotionType(const int nMotionType) { m_nMotionType = nMotionType; }	// ���[�V�����^�C�v�ݒ�
	int GetMotionType(void) { return m_nMotionType; }							// ���[�V�����^�C�v�擾
	int GetMotionTypeOld(void) { return m_nMotionTypeOld; }						// �O��̃��[�V�����^�C�v�擾

	CModel *GetModel(const int nPartsId) { return m_pModel[nPartsId]; }			// ���f�����擾
	CMotionMove *GetMotion(void) { return m_pMotion; }							// ���[�V�������擾
	void BindMotion(CMotionData *pMotionData) { m_pMotionData = pMotionData; }	// ���[�V�����̐ݒ�

	CCollision *GetCollision(void) { return m_pCollision; }						// �����蔻����擾

private:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �ϐ��錾
	//+*++*++*++*++*++*++*++*++*++*++*
	D3DXVECTOR3			m_pos;					// �ʒu�i���S���W�j
	D3DXVECTOR3			m_posOld;				// �O��ʒu�i���S���W�j
	D3DXVECTOR3			m_move;					// �ړ���
	D3DXVECTOR3			m_rot;					// ����
	D3DXMATRIX			m_mtxWorld;				// ���[���h�}�g���b�N�X
	float				m_fSpeed;				// �ړ����x
	CModel				*m_pModel[MAX_PARTS];	// ���f���N���X�̃|�C���^

	CMotionData			*m_pMotionData;			// ���[�V�����N���X�̃|�C���^
	int					m_nMotionType;			// ���[�V�����̎��
	int					m_nMotionTypeOld;		// �O��̃��[�V�����̎��
	CMotionMove			*m_pMotion;				// ���[�V�������[�u�N���X�̃|�C���^

	CCollision			*m_pCollision;			// �����蔻��N���X�̃|�C���^
};

#endif
