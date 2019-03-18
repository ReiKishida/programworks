//=============================================================================
//
// �v���C���[����[2D�|���S��] [player.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "sound.h"
#include "force.h"
#include "followforce.h"
#include "explosion.h"
#include "fade.h"
#include "remaining.h"
#include "bg.h"
#include "boss.h"
#include "particle.h"
#include "score.h"
#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_WIDTH (50.0f)	//�|���S���̕�
#define BOSS_HEIGHT (80.0f)	//�|���S����Y�̍���
#define SCREEN_LIMIT_MAX_X (1050.0f - BOSS_WIDTH)	//��ʒ[(�E�[)
#define SCREEN_LIMIT_MIN_X (250.0f + BOSS_WIDTH)			//���(���[)

#define SCREEN_LIMIT_MAX_Y ((SCREEN_HEIGHT + 15.0f) - BOSS_WIDTH)	//���(���[)
#define SCREEN_LIMIT_MIN_Y (-15.0f + BOSS_WIDTH)			//���(��[)

#define BULLET_LIFE (70)//�e�̎���

#define TEX_X (0.5f)	//�e�N�X�`�����W_X
#define TEX_Y (1.0f)//�e�N�X�`�����W_Y

#define BOSS_LIFE (300)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL; //���L�e�N�X�`���̃|�C���^
//int CBoss::m_nLife = 0; //���C�t
int CBoss::m_nHackTime = 0; //�n�b�N�^�C��
int CBoss::m_nMoveTime = 0;//�ړ��p������
int CBoss::m_nCntWait = 0; //�ҋ@����
int CBoss::m_MoveState = 0;	//�ړ����
int CBoss::m_nCntCoolTime = 0;	//�N�[���^�C��
float CBoss::m_fAngle = 0.0f;//�p�x

int CBoss::m_nMovePattern = 0;//�ړ��p�^�[��
bool CBoss::m_bWait = false;//�ҋ@���Ă��邩
D3DXVECTOR3 CBoss::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu���
D3DXVECTOR3 CBoss::m_posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu���
D3DXVECTOR3 CBoss::m_Destpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu���
D3DXVECTOR3 CBoss::m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
bool CBoss::m_bUse = true;
bool CBoss::m_bSwitch = true;
CBoss::BOSSSTATE CBoss::m_state = BOSSSTATE_NONE;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CBoss::CBoss() : CScene2D(5)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_state = BOSSSTATE_NONE;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CBoss::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CBoss::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}



//=============================================================================
//�{�X�̐���
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos)
{
	//�{�X�̃C���X�^���X����
	CBoss *pBoss;
	pBoss = new CBoss;
	if (pBoss != NULL)
	{
		//���C�t
		pBoss->m_nLife = BOSS_LIFE;
		//�{�X�̈ړ���
		pBoss->m_fBossMove = 0.5f;

		//�{�X�̏���������
		pBoss->Init(pos);

	}

	return pBoss;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos)
{
	//�����Ă����Ԃ�
	m_bUse = true;
	m_bWait = false;
	m_bSwitch = true;
	m_state = BOSSSTATE_NORMAL;

	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	//�e�N�X�`���̏�����
	m_nTex_Y = 0;
	//�p�x���̏�����
	m_fAngle = 0.0f;
	//�ړ����̏�����
	m_MoveState = 0;
	m_nMovePattern = 0;


	//�{�X�̕��A�����ݒ�
	SetScene2DWidth(BOSS_WIDTH);
	SetScene2DHeight(BOSS_HEIGHT);

	//�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);
	m_nLife = BOSS_LIFE;
	SetLife(m_nLife);

	//�ړ��ʏ�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//2D�V�[���̏���������
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);
	//�{�X�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_BOSS);

	m_nCntFlame = 0;
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CBoss::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CBoss::Update(void)
{


	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	D3DXVECTOR3 movePlayer; //�{�X�̈ړ���
	m_pos = GetPosScene2D();	//�{�X�̈ʒu���
	D3DXVECTOR3 rotPlayer = GetRotScene2D();	//�{�X�̊p�x���

	m_nLife = GetLife();//���C�t�̎擾
	switch (m_state)
	{
	case BOSSSTATE_NORMAL:
		switch (m_nMovePattern)
		{
		case 0:
			MovePattern0();
			break;
		case 1:
			MovePattern1();
			break;
		case 2:
			MovePattern2();
			break;
		}

		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));



		break;


	case BOSSSTATE_HACK:
		m_nHackTime--;
		SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

		if (m_nHackTime <= 0)
		{
			m_state = BOSSSTATE_NORMAL;
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CBg::SetFade(CBg::BGTYPE_BOSSNORMAL);

			m_nHackTime = 0;
		}
		break;

	case BOSSSTATE_APPEAR:
		break;

	case BOSSSTATE_DEATH:

		break;
	}



	//�ʒu������
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;


	//�����ړ�
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;

	//��ʒ[�̏���
	if (m_pos.x >= SCREEN_LIMIT_MAX_X)
	{//��ʉE�[
		m_pos.x = SCREEN_LIMIT_MAX_X;
	}
	if (m_pos.x <= SCREEN_LIMIT_MIN_X)
	{//��ʍ��[
		m_pos.x = SCREEN_LIMIT_MIN_X;
	}
	if (m_pos.y >= SCREEN_LIMIT_MAX_Y)
	{//��ʉ��[
		m_pos.y = SCREEN_LIMIT_MAX_Y;
	}
	//if (m_pos.y <= SCREEN_LIMIT_MIN_Y)
	//{//��ʏ�[
	//	m_pos.y = SCREEN_LIMIT_MIN_Y;
	//}


	//�ʒu���̐ݒ�
	CScene2D::SetPosScene2D(m_pos);

	//�p�x���̐ݒ�
	CScene2D::SetRotScene2D(rotPlayer);

	HitDamage(m_nLife);

}

//=============================================================================
//�`�揈��
//=============================================================================
void CBoss::Draw(void)
{
	if (m_bUse == true)
	{
		//2D�V�[���̕`�揈��
		CScene2D::Draw();
	}
}
//=============================================================================
//�_���[�W����
//=============================================================================
void CBoss::HitDamage(int nValue)
{
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();
	//m_nLife = GetLife();


	if (nValue <= 0)
	{
		//�p�[�e�B�N���̐���
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 80);
		}
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION_R, 80);
		}
		//�I������
		Uninit();
		CScore::AddScore(5000);//�X�R�A���Z
		CGame::SetGameState(CGame::GAMESTATE_CLEAR);

	}


}


//=============================================================================
//�{�X�̏�Ԏ擾
//=============================================================================
CBoss::BOSSSTATE CBoss::GetState(void)
{
	return m_state;
}
//=============================================================================
//�{�X�̏�Ԑݒ�
//=============================================================================
void CBoss::SetState(BOSSSTATE state)
{
	m_state = state;
}

//=============================================================================
//�ړ��p�^�[��(1:�o����)
//=============================================================================
void CBoss::MovePattern0(void)
{
	m_pos.y += 1.5f;

	if (m_pos.y >= 120.0f)
	{
		//MoveInit(SCREEN_WIDTH / 2 - 250.0f, 120.0f, 1);
		m_nMovePattern = 1;
	}
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}
}

//=============================================================================
//�ړ��p�^�[��(2:���E�ړ�)
//=============================================================================
void CBoss::MovePattern1(void)
{
	if (m_bSwitch == true)
	{
		m_move.x = 1.5f;
	}
	if (m_bSwitch == false)
	{
		m_move.x = -1.5f;
	}

	if (m_bWait == false)
	{
		m_pos.x += m_move.x;

		if (m_pos.x >= SCREEN_LIMIT_MAX_X)
		{

			m_bSwitch = false;

		}
		else if (m_pos.x <= SCREEN_LIMIT_MIN_X)
		{
			m_bSwitch = true;
		}
	}

	if (m_nLife < 220)
	{
		//�U���p�^�[��2
		AttackPattern1();
	}
	else
	{
		//�U���p�^�[��1
		AttackPattern0();
	}

	//�N�[���^�C������
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}


	if (m_nLife <= 100)
	{
		MoveInit(SCREEN_WIDTH / 2 - 250.0f, 120.0f, 1);
		m_nMovePattern = 2;
	}


}

//=============================================================================
//�ړ��p�^�[��(3:�O�p�ړ�)
//=============================================================================
void CBoss::MovePattern2(void)
{
	float fAngle;//�p�x�v�Z�p
	m_fAngle += 1.0;

	fAngle = sinf(m_fAngle * D3DX_PI / 180);


	float fMove_X = m_posold.x + fAngle * m_Destpos.x;
	float fMove_Y = m_posold.y + fAngle * m_Destpos.y;


	m_pos.x = fMove_X;
	m_pos.y = fMove_Y;


	if (m_fAngle == 90.0f)
	{

		if (m_MoveState == 0)
		{
			MoveInit(SCREEN_WIDTH/2 -250.0f, 120.0f, 1);
		}
		else if (m_MoveState == 1)
		{
			MoveInit(SCREEN_WIDTH / 2, 300.0f, 2);
		}
		else
		{
			MoveInit(SCREEN_WIDTH / 2 + 250.0f, 120.0f, 0);
		}
	}

	//�U���p�^�[��3
	AttackPattern2();

	//�N�[���^�C������
	m_nCntCoolTime--;
	if (m_nCntCoolTime <= 0)
	{
		m_nCntCoolTime = 0;
	}


}

//=============================================================================
//�U���p�^�[��(1:�ʏ�U���A���[�U�[)
//=============================================================================
void CBoss::AttackPattern0(void)
{
	D3DXVECTOR3 BulletMove;//�e�̈ړ���
	int nAttackType;//�U���p�^�[��
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	float fAngle;//�p�x���

	if (m_nCntCoolTime <= 0)
	{
		nAttackType = rand() % 2;

		switch (nAttackType)
		{
		case 0:
			if (rand() % 50 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
				m_nCntCoolTime = 15;
			}
			break;

		case 1:
			if (rand() % 100 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCntCoolTime = 180;

			}
			break;
		}
	}
}


//=============================================================================
//�U���p�^�[��(2:�ʏ�U���A�g�U�e)
//=============================================================================
void CBoss::AttackPattern1(void)
{
	D3DXVECTOR3 BulletMove;//�e�̈ړ���
	int nAttackType;//�U���p�^�[��
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	float fAngle;//�p�x���


	if (m_nCntCoolTime <= 0)
	{

			nAttackType = rand() % 2;

			switch (nAttackType)
			{
			case 0:
				if (rand() % 50 == 0)
				{
					CPlayer *pPlayer;
					pPlayer = CManager::GetPlayer();
					playerPos = pPlayer->GetPos();

					fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

					BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
					BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

					CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
					m_nCntCoolTime = 15;
				}
				break;

			case 1:
				if (rand() % 150 == 0)
				{
					int nNumloop = 0;
					float fRad = atan2f(m_pos.y - playerPos.y, m_pos.x - playerPos.x);
					while (1)
					{

						float fSpeed = 7.0f;
						float fRadius = fRad + nNumloop*((360 / 20)* D3DX_PI / 180);

						BulletMove.x = sinf(fRadius) * 7.0f;
						BulletMove.y = cosf(fRadius) * 7.0f;


						CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
						++nNumloop;
						if (nNumloop == 20)
						{
							break;
						}
					}
					m_nCntCoolTime = 180;

				}
				break;
			}

	}
}


//=============================================================================
//�U���p�^�[��(3:�S��)
//=============================================================================
void CBoss::AttackPattern2(void)
{
	D3DXVECTOR3 BulletMove;//�e�̈ړ���
	int nAttackType;//�U���p�^�[��
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	float fAngle;//�p�x���


	if (m_nCntCoolTime <= 0)
	{

		nAttackType = rand() % 3;

		switch (nAttackType)
		{
		case 0:
			if (rand() % 50 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
				m_nCntCoolTime = 15;
			}
			break;

		case 1:
			if (rand() % 150 == 0)
			{//�g�U�e
				int nNumloop = 0;
				float fRad = atan2f(m_pos.y - playerPos.y, m_pos.x - playerPos.x);
				while (1)
				{

					float fSpeed = 7.0f;
					float fRadius = fRad + nNumloop*((360 / 20)* D3DX_PI / 180);

					BulletMove.x = sinf(fRadius) * 7.0f;
					BulletMove.y = cosf(fRadius) * 7.0f;


					CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_NORMAL, 120, 1);
					++nNumloop;
					if (nNumloop == 20)
					{
						break;
					}
				}
				m_nCntCoolTime = 180;

			}
			break;

		case 2:
			if (rand() % 100 == 0)
			{//�z�[�~���O
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.5f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.5f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCntCoolTime = 180;

			}
			break;
		}

	}
}


//=============================================================================
//�ړ�(�ړI�n�̐ݒ�)
//=============================================================================
void CBoss::MoveInit(float dest_x, float dest_y, int state)
{
	//�ߋ��̈ʒu���
	m_posold.x = m_pos.x;
	m_posold.y = m_pos.y;

	//�ړI�̈ʒu
	m_Destpos.x = dest_x - m_pos.x;
	m_Destpos.y = dest_y - m_pos.y;

	m_fAngle = 0;

	m_MoveState = state;
}
