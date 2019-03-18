//=============================================================================
//
// �Ǐ]�t�H�[�X����[2D �|���S��] [followforce.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _FOLLOWFORCE_H_
#define _FOLLOWFORCE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FOLLOWFORCE (4)

class CManager;

class CFollowForce : public CScene
{
public:
	typedef enum
	{
		FOLLOWFORCETYPE_ATTACK = 0,		//�U�̃t�H�[�X
		FOLLOWFORCETYPE_SPREAD,			//�g�̃t�H�[�X
		FOLLOWFORCETYPE_SPEED,			//���̃t�H�[�X
		FOLLOWFORCETYPE_DEFENSE,		//��̃t�H�[�X
		FOLLOWFORCETYPE_NONE,			//��̃t�H�[�X
		FOLLOWFORCETYPE_MAX				//�t�H�[�X�̍ő吔
	}FOLLOWFORCETYPE;
	typedef struct
	{
		FOLLOWFORCETYPE type;
		D3DXCOLOR col;
	}FORCE;
	CFollowForce();	//�R���X�g���N�^
	~CFollowForce();	//�f�X�g���N�^
	static CFollowForce *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetForce(int nCntForce, FOLLOWFORCETYPE type);	//�t�H�[�X�̐ݒ�
	static HRESULT Load(void);
	static void Unload(void);


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FOLLOWFORCE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apPolygon[MAX_FOLLOWFORCE];	//����
	static int  m_aForce[MAX_FOLLOWFORCE];	//�t�H�[�X����p
	FOLLOWFORCETYPE m_atype[MAX_FOLLOWFORCE];//�^�C�v
	FORCE m_aForceStruct[MAX_FOLLOWFORCE];//�t�H�[�X�\����
	int m_nCnt;	//�t�H�[�X�̃J�E���g
	D3DXVECTOR3 m_ForceRot;//��]�p�p�x

	float fLengthForce[MAX_FOLLOWFORCE]; //�v���C���[�Ƃ̒���
	float fAngleForce; //�v���C���[�Ƃ̒���
	D3DXCOLOR m_ForceColor[MAX_FOLLOWFORCE];//�t�H�[�X�̐F
	static int m_nNumAttack;	//�U�̐�
	static int m_nNumSpread;	//���̐�
	static int m_nNumSpeed;		//�g�̐�
	static int m_nNumDefense;	//��̐�
	static bool m_bUse;	//�����Ă��邩�ǂ���



};

#endif