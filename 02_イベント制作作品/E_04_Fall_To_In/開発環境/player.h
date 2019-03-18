//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// �v���C���[���� [player.h]
// Author:�� �a��
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �C���N���[�h�t�@�C��
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#include "main.h"
#include "scene.h"
#include "motion.h"
#include "model.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �O���錾
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CModel;
class CCollision;
class CAnimal;
class CFish;
class CVegetable;
class CProvideDisp;
class CButtonMotion;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �}�N����`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_KUSHI		(4)
#define MAX_KUSHI_NUM	(3)

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// �N���X��`
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
class CPlayer : public CMotion
{ // �v���C���[�N���X
public:
	//+*++*++*++*++*++*++*++*++*++*++*
	// �\���̒�`
	//+*++*++*++*++*++*++*++*++*++*++*
	typedef enum
	{ // ����̎��
		CONTROLLER_LEFT_MOVE = 0,	// ���ړ�
		CONTROLLER_RIGHT_MOVE,		// �E�ړ�
		CONTROLLER_UP_MOVE,			// ��ړ�
		CONTROLLER_LEFT_UP_MOVE,	// ����ړ�
		CONTROLLER_RIGHT_UP_MOVE,	// �E��ړ�
		CONTROLLER_LEFT_DOWN_MOVE,	// �����ړ�
		CONTROLLER_RIGHT_DOWN_MOVE,	// �E���ړ�
		CONTROLLER_DOWN_MOVE,		// ���ړ�
		CONTROLLER_ATTACK,			// �U��
		CONTROLLER_JUMP,			// �W�����v
		CONTROLLER_MAX,				// ����
	}CONTROLLER;

	typedef enum
	{ // ���
		MOTION_DATA_TYPE_TAKERU = 0,	// �s�`�j�d�q�t
		MOTION_DATA_TYPE_ZOZO_RED,			// ZOZO
		MOTION_DATA_TYPE_ZOZO_BLUE,			// ZOZO
		MOTION_DATA_TYPE_MAX,			// �ő吔
	}MOTION_DATA_TYPE;

	typedef enum
	{ // ���[�V�����̎��
		MOTION_TYPE_NEUTRAL = 0,	// [0]	�j���[�g����
		MOTION_TYPE_POSE_00,		// [1]	�|�[�Y00
		MOTION_TYPE_POSE_01,		// [2]	�|�[�Y01
		MOTION_TYPE_POSE_02,		// [3]	�|�[�Y02
		MOTION_TYPE_POSE_03,		// [4]	�|�[�Y03
		MOTION_TYPE_JUMP,			// [5]  �W�����v
		MOTION_TYPE_LOOP,			// [6]  ���[�v
		MOTION_TYPE_WIN,			// [7]  kati
		MOTION_TYPE_LOOSE,			// [8]  make
		MOTION_TYPE_MAX,			// ����
	}MOTION_TYPE;

	typedef enum
	{ // bool�ϐ��̎��
		BOOL_JUMP = 0,		// �W�����v
		BOOL_MAX
	}BOOL;

	typedef enum
	{ // bool�ϐ��̎��
		POSE_BOTTAN_A = 0,		// �{�^��A
		POSE_BOTTAN_B,			// �{�^��B
		POSE_BOTTAN_X,			// �{�^��X
		POSE_BOTTAN_Y,			// �{�^��Y
		POSE_BUTTON_MAX
	}POSE_BUTTON;

	//+*++*++*++*++*++*++*++*++*++*++*
	// �v���g�^�C�v�錾
	//+*++*++*++*++*++*++*++*++*++*++*
	CPlayer();
	virtual ~CPlayer();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void PlayerOverLap(void);

	// �e��ݒ�擾�֐�
	void SetId(const int nId) { m_nId = nId; }	// ID�ݒ�
	int GetId(void) { return m_nId; }
	float GetRadius(void) { return m_fRadius; }	// ���a�擾
	void SetMotion(int nId);

	// �ÓI�����o�֐�
	static CPlayer *Create(int nId);
	static HRESULT Load(void);
	static void UnLoad(void);
	static void RandomPose(void);
	static void ButtonDisp(const float fHeight);

	static MOTION_TYPE GetButtonMotion(POSE_BUTTON button) { return m_buttonMotionType[button]; }

private:
	void Collision(void);
	void Controller(void);
	void Motion(void);
	void MotionTypeUpdate(void);
	bool GetController(CONTROLLER Controller);




	//+*++*++*++*++*++*++*++*++*++*++*
	// �ϐ��錾
	//+*++*++*++*++*++*++*++*++*++*++*
	float				m_fRadius;					// �����蔻��p���a
	float				m_fHeifht;					// �����蔻��p����
	MOTION_DATA_TYPE	m_nType;					// ���[�V�����f�[�^�̎��
	bool				m_bBool[BOOL_MAX];			// ���[�V�����p�u�[��
	bool				m_bBoolOld[BOOL_MAX];		// ���[�V�����p�u�[��
	bool				m_bBotton[POSE_BUTTON_MAX];	// �{�^���p�u�[��
	int					m_nId;						// �������̔���p��ID

	// �ÓI�����o�ϐ�
	static 	CMotionData *m_apMotion[MOTION_DATA_TYPE_MAX];			// ���[�V�����f�[�^�ւ̃|�C���^
	static char			*m_apMotionInfo[MOTION_DATA_TYPE_MAX];		// ���[�V�����e�L�X�g��

	static MOTION_TYPE		m_buttonMotionType[POSE_BUTTON_MAX];			// �{�^���Ɋ��蓖�Ă郂�[�V����

	static CButtonMotion	*m_buttonMotionDisp;					// �{�^���\���N���X
	static bool				m_bBottonCommand[POSE_BUTTON_MAX];		// �{�^���p�u�[��
	static bool				m_bControl;								// Control�ł��邩�ۂ�
	static int				m_nControlLevel;						// ����Level

	static int				m_n1PlayerMotion;						// 1P�̃��[�V����
	static int				m_nNumAll;						// 1P�̃��[�V����
};

#endif
