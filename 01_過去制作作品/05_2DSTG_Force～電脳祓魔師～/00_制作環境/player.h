//=============================================================================
//
// �v���C���[����[2D �|���S��] [player.h]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//�O���錾 --�X�y���~�X�ɒ��ӁI(�Ȃ�ł��ʂ邩��I)
class CManager;

class CPlayer : public CScene2D
{
public:
	typedef enum
	{
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_APPEAR,
		PLAYERSTATE_NORMAL,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_DEATH,
		PLAYERSTATE_HACK,
		PLAYERSTATE_MAX
	}PLAYERSTATE;
	CPlayer::CPlayer();	//�R���X�g���N�^
	CPlayer::~CPlayer();	//�f�X�g���N�^
	static CPlayer *Create(D3DXVECTOR3 pos);	//�v���C���[�̐���
	HRESULT Init(D3DXVECTOR3 pos);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static HRESULT Load(void);
	static void Unload(void);
	void HitDamage(int nValue);
	static bool GetbUse(void);
	static PLAYERSTATE GetState(void);
	static void SetState(PLAYERSTATE state);
	static D3DXVECTOR3 GetPos(void);
private:
	int m_nCntFrame;//�t���[�����̃J�E���g
	float m_fPlayerMove;//�v���C���[�̈ړ���
	static LPDIRECT3DTEXTURE9 m_pTexture; //���L�e�N�X�`���̃|�C���^
	static D3DXVECTOR3 m_pos;	//�ړ���
	D3DXVECTOR3 m_move;	//�ړ���
	static PLAYERSTATE m_Pstate;	//�v���C���[�̏��
	static int m_nLife;	//���C�t
	static bool m_bUse;	//�����Ă��邩�ǂ���
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
	int m_nCounterAnim;//�A�j���[�V�����J�E���^�[
	int m_nTex_Y;//�e�N�X�`���ړ��p
	int m_nAttack;//�U����
	int m_nSpread;//�g�U��
	int m_nSpeed;//�X�s�[�h
	static int m_nRespawn;
	static D3DXCOLOR m_PlayerCol;
};

#endif