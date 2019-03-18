//=============================================================================
//
// �t�H�[�X����[2D �|���S��] [force.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _FORCE_H_
#define _FORCE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FORCE (4)

class CManager;

class CForce : public CScene
{
public:
	typedef enum
	{
		FORCETYPE_ATTACK = 0,	//�U�̃t�H�[�X
		FORCETYPE_SPREAD,		//�g�̃t�H�[�X
		FORCETYPE_SPEED,		//���̃t�H�[�X
		FORCETYPE_DEFENSE,		//��̃t�H�[�X
		FORCETYPE_NONE,			//��̃t�H�[�X
		FORCETYPE_MAX			//�t�H�[�X�̍ő吔
	}FORCETYPE;


	CForce();	//�R���X�g���N�^
	~CForce();	//�f�X�g���N�^
	static CForce *Create(D3DXVECTOR3 pos);	//�����̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetForce(int nCntForce,FORCETYPE type);	//�t�H�[�X�̐ݒ�

	static int GetNumAttack(void);//�U�̃t�H�[�X���̎擾
	static int GetNumSpread(void);//�g�̃t�H�[�X���̎擾
	static int GetNumSpeed(void);//���̃t�H�[�X���̎擾
	static int GetNumDefense(void);//��̃t�H�[�X���̎擾

	static void CheckDamage(int nDamage);//DPS�`�F�b�N

	static int GetnCnt(void);//DPS�擾

	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FORCE]; //���L�e�N�X�`���̃|�C���^
	static CScene2D *m_apScene2D[MAX_FORCE];	//����
	static int  m_aForce[MAX_FORCE];	//�t�H�[�X����p

	FORCETYPE m_type[MAX_FORCE];	//�^�C�v
	int m_nCnt;	//�t�H�[�X�̃J�E���g

	static int m_nNumAttack;	//�U�̐�
	static int m_nNumSpread;	//���̐�
	static int m_nNumSpeed;		//�g�̐�
	static int m_nNumDefense;	//��̐�

	static int m_nCntDPS;//DPS�v�Z�p
	static int  m_nDPS;//DPS�v�Z�p
	static int m_nDamage;//DPS�v�Z�p


};

#endif