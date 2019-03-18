//=============================================================================
//
// �G����[2D�|���S��] [enemy.cpp]
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
#include "enemy.h"
#include "fade.h"
#include "bg.h"
#include "game.h"
#include "particle.h"
#include "score.h"
#include "sound.h"
#include "timer.h"
#include "log.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//�X�e�[�W�̍��[
#define STAGE_LIMIT_MIN_X (240)		//�X�e�[�W�̉E�[

#define ENEMY_WIDTH (40.0f)			//�|���S���̕�
#define ENEMY_HEIGHT (45.0f)		//�|���S����Y�̍���
#define ENEMY_MOVE (1.5f)			//�G�̈ړ���
#define NORMAL_ENEMY_HP (15)		//�m�[�}���^��HP
#define ATTACK_ENEMY_HP	(25)		//�U���^�̓G��HP
#define SPREAD_ENEMY_HP (25)		//�g�U�^�̓G��HP
#define SPEED_ENEMY_HP (10)			//���x�^�̓G��HP
#define DEFENSE_ENEMY_HP (80)		//��̓G��HP

#define ENEMY_LENGTH_LONG_R (700.0f)//����
#define ENEMY_LENGTH_MIDIUM_R (550.0f)//����

#define ENEMY_LENGTH_LONG_L (620.0f)//����
#define ENEMY_LENGTH_MIDIUM_L (450.0f)//����


#define THROUGH_MOVE_X (3.0f)		//�ʉߑ��x(X)
#define THROUGH_MOVE_UPPER (1.3f)		//�ʉߑ��x(��)
#define THROUGH_MOVE_UNDER (1.2f)		//�ʉߑ��x(��)
#define THROUGH_MOVE_UNDER_2 (1.5f)		//�ʉߑ��x(��)
#define THROUGH_MOVE_Y_2 (2.0f)		//�ʉߑ��x(Y)
#define	FAN_MOVE (0.0045f)			//���񑬓x
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[MAX_ENEMYTYPE] = {}; //���L�e�N�X�`���̃|�C���^
int  CEnemy::m_nNumEnemy = 0;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CScene2D(5)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CEnemy::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy.png", &m_pTexture[0]);//�m�[�}���ȓG
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_attack.png", &m_pTexture[1]);//�U���^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_spread.png", &m_pTexture[2]);//�g�U�^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_speed.png", &m_pTexture[3]);//���x�^�̓G
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy_defense.png", &m_pTexture[4]);//�h��^�̓G

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntEnemyTex = 0; nCntEnemyTex < MAX_ENEMYTYPE; nCntEnemyTex++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCntEnemyTex] != NULL)
		{
			m_pTexture[nCntEnemyTex]->Release();
			m_pTexture[nCntEnemyTex] = NULL;
		}
	}
}




//=============================================================================
//�G�̐���
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type, ENEMYMOVE movetype)
{
	//�G�̃C���X�^���X����
	CEnemy *pEnemy;
	pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		pEnemy->m_type = type;
		pEnemy->m_movetype = movetype;
		//���C�t�̐ݒ�
		switch (pEnemy->m_type)
		{
		case ENEMYTYPE_NORMAL:
			pEnemy->m_nLife = NORMAL_ENEMY_HP;
			break;
		case ENEMYTYPE_ATTACK:
			pEnemy->m_nLife = ATTACK_ENEMY_HP;
			break;
		case ENEMYTYPE_SPREAD:
			pEnemy->m_nLife = SPREAD_ENEMY_HP;
			break;
		case ENEMYTYPE_SPEED:
			pEnemy->m_nLife = SPEED_ENEMY_HP;
			break;
		case ENEMYTYPE_DEFENSE:
			pEnemy->m_nLife = DEFENSE_ENEMY_HP;
			break;
		}
		//�G�̏���������
		pEnemy->Init(pos);
		m_nNumEnemy++;
	}
	return pEnemy;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	//�G�̕��A�����ݒ�
	SetScene2DWidth(ENEMY_WIDTH);
	SetScene2DHeight(ENEMY_HEIGHT);
	//���C�t�̐ݒ�
	SetLife(m_nLife);



	//���
	m_state = ENEMYSTATE_NORMAL;

	//�e�N�X�`���̊��蓖��
	switch (m_type)
	{
	case ENEMYTYPE_NORMAL:
		BindTexture(m_pTexture[0]);
		break;
	case ENEMYTYPE_ATTACK:
		BindTexture(m_pTexture[1]);
		break;
	case ENEMYTYPE_SPREAD:
		BindTexture(m_pTexture[2]);
		break;
	case ENEMYTYPE_SPEED:
		BindTexture(m_pTexture[3]);
		break;
	case ENEMYTYPE_DEFENSE:
		BindTexture(m_pTexture[4]);
		break;
	}

	//�ړ��ʂ̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�t���[�����̃J�E���g
	m_nCntFrame = 0;

	//2D�V�[���̏���������
	CScene2D::Init(pos);

	//�G�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_ENEMY);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CEnemy::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CEnemy::Update(void)
{

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	m_pos =GetPosScene2D();	//�G�̈ʒu���
	D3DXVECTOR3 rotEnemy = GetRotScene2D();	//�G�̊p�x���
	D3DXVECTOR3 BulletMove;//�e�̈ړ���
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	CPlayer *pPlayer;//�v���C���[
	pPlayer = CManager::GetPlayer();//�v���C���[�̎擾
	playerPos = pPlayer->GetPos();//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 posFan = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 50.0f, 0.0f);


	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	//���C�t�̎擾
	m_nLife = GetLife();

	//�p�x���
	float fAngle;

	switch (m_state)
	{

	case ENEMYSTATE_NORMAL:
		//============================================================================================
		//										�U �� �p �^ �[ ��									//
		//============================================================================================
#if 1
		switch (m_type)
		{
		case ENEMYTYPE_NONE:
			break;
		case ENEMYTYPE_NORMAL:
			if (mode == CManager::MODE_GAME)
			{
				if (rand() % 80 == 0)
				{
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -7.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 110, 1);
					m_nCoolDown = 50;
					m_state = ENEMYSTATE_COOLDOWN;
				}
			}
			break;
		case ENEMYTYPE_ATTACK:
			if (rand() % 100 == 0)
			{
				CPlayer *pPlayer;
				pPlayer = CManager::GetPlayer();
				playerPos = pPlayer->GetPos();

				fAngle = atan2f(playerPos.x - m_pos.x, playerPos.y - m_pos.y);

				BulletMove.x = (sinf(fAngle) * 7.0f) * -1;
				BulletMove.y = (cosf(fAngle) * 7.0f) * -1;

				CBullet::Create(m_pos, BulletMove, CBullet::BULLETTYPE_ENEMY_BOMB, 45, 1);
				m_nCoolDown = 85;
				m_state = ENEMYSTATE_COOLDOWN;
			}
			break;
		case ENEMYTYPE_SPREAD:

			if (rand() % 100 == 0)
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
				m_nCoolDown = 100;
				m_state = ENEMYSTATE_COOLDOWN;

			}
			break;
		case ENEMYTYPE_SPEED:
			if (rand() % 150 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_R, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 1.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				CBullet::Create(m_pos, D3DXVECTOR3(-4.0f, 2.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_LASER_L, 200, 1);
				m_nCoolDown = 80;
				m_state = ENEMYSTATE_COOLDOWN;

			}

			break;
		case ENEMYTYPE_DEFENSE:
			if (rand() % 200 == 0)
			{
				CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -9.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 60, 1);
				m_nCoolDown = 75;
				m_state = ENEMYSTATE_COOLDOWN;
			}
			break;
		}//(��)�U���p�^�[��switch��
#endif
		break;

	case ENEMYSTATE_COOLDOWN:
		m_nCoolDown--;
		if (m_nCoolDown <= 0)
		{
			m_state = ENEMYSTATE_NORMAL;
		}
		break;
	}

	//============================================================================================
	//										�� �� �p �^ �[ ��									//
	//============================================================================================
	switch (m_movetype)
	{
	case ENEMYMOVE_NONE:
		break;
	case ENEMYMOVE_THROUGH_UPPER:
		m_move.y = THROUGH_MOVE_UPPER;//���ֈړ�
		break;
	case ENEMYMOVE_THROUGH_UNDER:
		m_move.y = THROUGH_MOVE_UNDER;//���ֈړ�
		break;
	case ENEMYMOVE_FAN_L_LONG://����]
		//�p�x���Z
		m_rot.x += FAN_MOVE;
		m_rot.y += FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * 0.5f)) * ENEMY_LENGTH_LONG_L + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  0.5f)) * ENEMY_LENGTH_LONG_L + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_L_MIDIUM://����]
							  //�p�x���Z
		m_rot.x += FAN_MOVE;
		m_rot.y += FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * 0.5f)) * ENEMY_LENGTH_MIDIUM_L + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  0.5f)) * ENEMY_LENGTH_MIDIUM_L + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_R_LONG://�E��]
		//�p�x���Z
		m_rot.x -= FAN_MOVE;
		m_rot.y -= FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * -0.5f)) * ENEMY_LENGTH_LONG_R + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  -0.5f)) * ENEMY_LENGTH_LONG_R + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_FAN_R_MIDIUM://�E��]
		 //�p�x���Z
		m_rot.x -= FAN_MOVE;
		m_rot.y -= FAN_MOVE;

		m_pos = (D3DXVECTOR3(sinf(m_rot.x + D3DX_PI - (D3DX_PI * -0.5f)) * ENEMY_LENGTH_MIDIUM_R + posFan.x
			, cosf(m_rot.y + D3DX_PI - (D3DX_PI *  -0.5f)) * ENEMY_LENGTH_MIDIUM_R + posFan.y
			, posFan.z));
		break;

	case ENEMYMOVE_STOP_R://��ʒ�������E�ɂ͂���
		m_move.y = THROUGH_MOVE_UNDER_2;//���ֈړ�
		if (m_pos.y > SCREEN_HEIGHT / 2 - 150.0f)
		{
			m_move.y = 0.7f;//���ֈړ�
			m_move.x = THROUGH_MOVE_X;//�E�ֈړ�
		}
		break;

	case ENEMYMOVE_STOP_L://��ʒ������獶�ɂ͂���
		m_move.y = THROUGH_MOVE_UNDER_2;//���ֈړ�
		if (m_pos.y > SCREEN_HEIGHT / 2 - 150.0f)
		{
			m_move.y = 0.7f;//���ֈړ�
			m_move.x = (THROUGH_MOVE_X * -1);//�E�ֈړ�
		}
		break;

	case ENEMYMOVE_STOP:
		m_move.y = THROUGH_MOVE_UNDER_2;//���ֈړ�
		if (m_pos.y > SCREEN_HEIGHT / 2 - 200.0f)
		{
			m_nCntFrame++;
			if (m_nCntFrame >= 240)
			{
				m_move.y = THROUGH_MOVE_UNDER_2;//���ֈړ�
			}
			else
			{
				m_move.y = 0.0f;//���ֈړ�
			}
		}
		break;
	}

	//�ړ��ʂ̑��
	m_pos += m_move;

	//�ʒu���̐ݒ�
	SetPosScene2D(m_pos);
	//�p�x���̐ݒ�
	SetRotScene2D(rotEnemy);

	//���C�t�̊m�F
	CheckLife(m_nLife);


	//============================================================================================
	//										�͈͂𒴂�����										//
	//============================================================================================
	switch (m_movetype)
	{
	case ENEMYMOVE_THROUGH_UPPER:
		if (m_pos.y <= 0)
		{
			Uninit();
		}
		break;
	case ENEMYMOVE_THROUGH_UNDER:
		if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	case ENEMYMOVE_FAN_L_LONG://����]
		if (m_pos.x <= STAGE_LIMIT_MIN_X - 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_L_MIDIUM://����]
		if (m_pos.x <= STAGE_LIMIT_MIN_X - 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_R_LONG://�E��]
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_FAN_R_MIDIUM://�E��]
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 50.0f)
		{
			Uninit();
		}
		break;

	case ENEMYMOVE_STOP_R:
		if (m_pos.x >= STAGE_LIMIT_MAX_X + 40.0f)
		{
			Uninit();
		}
		else if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	case ENEMYMOVE_STOP_L:

		if (m_pos.x <= STAGE_LIMIT_MIN_X -40.0f)
		{
			Uninit();
		}
		else if (m_pos.y >= SCREEN_HEIGHT + 50.0f)
		{
			Uninit();
		}

		break;
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	//2D�V�[���̕`�揈��
	CScene2D::Draw();
}



//=============================================================================
//�ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CEnemy::GetPosEnemy(void)
{
	return m_pos;
}

//=============================================================================
//���C�t�̊m�F
//=============================================================================
void CEnemy::CheckLife(int nLife)
{
	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_nLife <= 0)
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

		switch (m_type)
		{
		case ENEMYTYPE_NORMAL:
			CScore::AddScore(500);//�X�R�A���Z
			CTimer::AddSeconds(3);//�b�����Z
   			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH  + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_NORMAL);
			break;
		case ENEMYTYPE_ATTACK:
			CScore::AddScore(600);//�X�R�A���Z
			CTimer::AddSeconds(4);//�b�����Z
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_ATTACK);
			break;
		case ENEMYTYPE_SPREAD:
			CScore::AddScore(700);//�X�R�A���Z
			CTimer::AddSeconds(4);//�b�����Z
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_SPREAD);

			break;
		case ENEMYTYPE_SPEED:
			CScore::AddScore(700);//�X�R�A���Z
			CTimer::AddSeconds(4);//�b�����Z
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_SPEED);

			break;
		case ENEMYTYPE_DEFENSE:
			CScore::AddScore(1000);//�X�R�A���Z
			CTimer::AddSeconds(5);//�b����
			CLog::Create(D3DXVECTOR3(SCREEN_WIDTH + 150.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), CLog::LOGTYPE_DEFENSE);

			break;
		}
		//�I������
		Uninit();


		pSound->PlaySound(CSound::SOUND_LABEL_SE_ADDTIME);
	}
}

