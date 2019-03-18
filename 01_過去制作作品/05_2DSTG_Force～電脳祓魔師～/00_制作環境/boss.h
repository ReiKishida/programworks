//=============================================================================
//
// �{�X����[2D �|���S��] [boss.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CManager;

class CBoss : public CScene2D
{
public:
	typedef enum
	{
		BOSSSTATE_NONE = 0,	//��̏��
		BOSSSTATE_APPEAR,	//�o�����
		BOSSSTATE_NORMAL,	//�ʏ���
		BOSSSTATE_DAMAGE,	//�_���[�W���
		BOSSSTATE_DEATH,	//���S���
		BOSSSTATE_HACK,		//�n�b�N���
		BOSSSTATE_MAX		//��Ԃ̍ő吔
	}BOSSSTATE;//�{�X�̏��
	typedef enum
	{
		BOSS_LOGIC_0 = 0,	//���W�b�N�p�^�[��0
		BOSS_LOGIC_1,		//���W�b�N�p�^�[��1
		BOSS_LOGIC_2,		//���W�b�N�p�^�[��2
	}BOSSLOGIC;//���W�b�N�p�^�[��
	typedef enum
	{
		BOSS_ATACKTYPE_NONE = 0,
		BOSS_ATACKTYPE_NORMAL,//�ʏ�e
		BOSS_ATACKTYPE_BALKAN,//�o���J��
		BOSS_ATACKTYPE_SPREAD,//�g�U�e
		BOSS_ATACKTYPE_LASER,//���[�U�[
		BOSS_ATTACK_MAX//�U���p�^�[���̍ő吔
	}BOSSATTACK;
	CBoss::CBoss();	//�R���X�g���N�^
	CBoss::~CBoss();	//�f�X�g���N�^
	static CBoss *Create(D3DXVECTOR3 pos);	//�{�X�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);//�e�N�X�`���̓ǂݍ���
	static void Unload(void);//�e�N�X�`���̔j��
	void HitDamage(int nValue);//�_���[�W����
	static BOSSSTATE GetState(void);	//��Ԏ擾
	static void SetState(BOSSSTATE state);//��Ԑݒ�
	void MovePattern0(void);//�ړ��p�^�[��(1:�o����)
	void MovePattern1(void);//�ړ��p�^�[��(2:���E�ړ�)
	void MovePattern2(void);//�ړ��p�^�[��(3:�O�p�ړ�)
	void AttackPattern0(void);//�U���p�^�[��(1:�ʏ�U���A���[�U�[)
	void AttackPattern1(void);//�U���p�^�[��(2:�ʏ�U���A�g�U)
	void AttackPattern2(void);//�U���p�^�[��(3:�S��)



	static void MoveInit(float dest_x, float dest_y, int state);//�ړ��p�^�[��(3:�O�p�ړ�)

private:
	int m_nCntFlame;//�t���[�����̃J�E���g
	float m_fBossMove;//�{�X�̈ړ���
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	static D3DXVECTOR3 m_pos;	//���W
	static D3DXVECTOR3 m_posold;	//�ȑO�̍��W
	static D3DXVECTOR3 m_Destpos;	//�ړI�̍��W
	static D3DXVECTOR3 m_move;	//�ړ���
	static D3DXVECTOR3 m_Moverot;	//�ړ���
	static BOSSSTATE m_state;	//�{�X�̏��
	int m_nLife;	//���C�t
	static bool m_bUse;	//�����Ă��邩�ǂ���
	BOSSATTACK m_BossAttack;//�{�X�̍U���p�^�[��
	BOSSLOGIC m_Logic;//�{�X�̃��W�b�N�p�^�[��
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
	int m_nCounterAnim;//�A�j���[�V�����J�E���^�[
	int m_nTex_Y;//�e�N�X�`���ړ��p
	int m_nAttack;//�U����
	int m_nSpread;//�g�U
	int m_nSpeed;//���x
	static int m_nCntWait;	//�ҋ@����
	static int m_nCntCoolTime;	//�N�[���^�C��
	static int m_nHackTime;//�n�b�N�^�C��
	static bool m_bWait;	//�ҋ@����
	static bool m_bSwitch;	//�ҋ@����
	static int m_nMoveTime;	//�ړ��p������
	static float m_fAngle;	//�A���O��
	static int m_nMovePattern;//�ړ��p�^�[��
	static int m_MoveState;//�ړ�
};

#endif