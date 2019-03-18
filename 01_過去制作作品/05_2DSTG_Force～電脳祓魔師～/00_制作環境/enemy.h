//=============================================================================
//
// �G�̏���[2D �|���S��] [enemy.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMYTYPE (5)			//�G�̎��

//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CManager;

class CEnemy : public CScene2D
{
public:
	typedef enum
	{
		ENEMYTYPE_NONE = 0,		//�󔒂̓G
		ENEMYTYPE_NORMAL,		//�ʏ�̓G
		ENEMYTYPE_ATTACK,		//�U���^�̓G
		ENEMYTYPE_SPREAD,		//�g�U�^�^�̓G
		ENEMYTYPE_SPEED,		//�e���̑����^�̓G
		ENEMYTYPE_DEFENSE,		//�ϋv�^�^�̓G
		ENEMYTYPE_MAX			//�G�̎�ނ̍ő吔
	}ENEMYTYPE;//�G�̎��
	typedef enum
	{
		ENEMYSTATE_NONE = 0,	//�󔒂̏��
		ENEMYSTATE_NORMAL,		//�ʏ���
		ENEMYSTATE_COOLDOWN,	//�N�[���_�E��(���̒e���˂܂ł̊Ԋu)
		ENEMYSTATE_MAX			//�G�̏�Ԃ̍ő吔
	}ENEMYSTATE;//�G�̏��
	typedef enum
	{
		ENEMYMOVE_NONE = 0,			//��̓G�̈ړ�
		ENEMYMOVE_THROUGH_UPPER,	//�G�̈ړ�(��������)
		ENEMYMOVE_THROUGH_UNDER,	//�G�̈ړ�(�ォ�牺��)
		ENEMYMOVE_THROUGH_LEFT,		//�G�̈ړ�(�E���獶��)
		ENEMYMOVE_THROUGH_RIGHT,	//�G�̈ړ�(������E��)
		ENEMYMOVE_FAN_R_LONG,		//�G�̈ړ�(��`(������)�F�E��)
		ENEMYMOVE_FAN_R_MIDIUM,		//�G�̈ړ�(��`(������)�F�E��)
		ENEMYMOVE_FAN_L_LONG,		//�G�̈ړ�(��`(������)�F����)
		ENEMYMOVE_FAN_L_MIDIUM,		//�G�̈ړ�(��`(������)�F����)
		ENEMYMOVE_STOP_R,			//�G�̈ړ�(��ʒ�������E���ւ͂���)
		ENEMYMOVE_STOP_L,			//�G�̈ړ�(��ʒ������獶���ւ͂���)
		ENEMYMOVE_STOP,				//�G�̈ړ�(�~�܂�)
		ENEMYMOVE_MAX				//�G�̈ړ��̍ő吔
	}ENEMYMOVE;//�G�̈ړ����@
	CEnemy::CEnemy();	//�R���X�g���N�^
	CEnemy::~CEnemy();	//�f�X�g���N�^
	static CEnemy *Create(D3DXVECTOR3 pos, ENEMYTYPE type, ENEMYMOVE movetype);	//�G�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̔j��
	D3DXVECTOR3 GetPosEnemy(void);	//�ʒu���̎擾
	void CheckLife(int nLife);
private:
	int m_nCntFrame;	//�J�E���^�p
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_ENEMYTYPE]; //���L�e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_move;	//�ړ���
	D3DXVECTOR3 m_pos;	//�ړ���
	D3DXVECTOR3 m_rot;//��]�p�p�x
	int m_nLife;	//���C�t
	static int m_nNumEnemy;//�G�̐�
	ENEMYTYPE m_type;	//�^�C�v
	ENEMYMOVE m_movetype;//�ړ����@
	ENEMYSTATE m_state;//�G�̏��
	int m_nCoolDown;//�N�[���_�E��
};

#endif