//=============================================================================
//
// �e�̏���[2D�|���S��] [bullet.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "Bullet.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "score.h"
#include "force.h"
#include "polygon.h"
#include "game.h"
#include "boss.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_LIMIT_MAX_X (1040)	//�X�e�[�W�̍��[
#define STAGE_LIMIT_MIN_X (240)		//�X�e�[�W�̉E�[
#define STAGE_LIMIT_MAX_Y (720)		//�X�e�[�W�̉��[
#define STAGE_LIMIT_MIN_Y (0)		//�X�e�[�W�̏�[

#define BULLET_WIDTH (10.0f)		//�e�̕�
#define BULLET_HEIGHT (10.0f)		//�e�̍���

#define LIMIT_HORMING (85)			//�z�[�~���O����
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
bool g_bDeleteBullet = false;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CBullet::CBullet() : CScene2D(5)
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
//�e�N�X�`���̃��[�h
//=============================================================================
HRESULT CBullet::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &m_pTexture);

	return S_OK;

}
//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CBullet::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
//�e�̐���
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, int nAttack)
{
	//�e�̐���
	CBullet *pBullet;
	pBullet = new CBullet;





	if (pBullet != NULL)
	{
		//�e�̎��
		pBullet->m_type = type;
		//�e�̈ړ��ʐݒ�
		pBullet->m_BulletMove = move;
		pBullet->m_nCntFrame = 0;
		pBullet->m_nLife = nLife;
		pBullet->m_nAttack = nAttack;
		//�e�̏�����
		pBullet->Init(pos);

	}

	return pBullet;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{


	//�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);

	//�|���S���̕��A�����ݒ�
	SetScene2DWidth(BULLET_WIDTH);
	SetScene2DHeight(BULLET_HEIGHT);
	m_nCntFrame = 0;
	//2D�V�[���̏���������
	CScene2D::Init(pos);

	//�e�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_BULLET);

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CBullet::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CBullet::Update(void)
{
	CPlayer *pPlayer;//�v���C���[�̃|�C���^
	pPlayer = CGame::GetPlayer();//�v���C���[�̎擾
	D3DXVECTOR3 playerPos;//�v���C���[�̈ʒu
	playerPos = pPlayer->GetPos();//�v���C���[�̈ʒu���擾

	CBoss *pBoss;
	pBoss = CGame::GetBoss();


	//�e�̏��
	D3DXVECTOR3 posBullet = GetPosScene2D();	//�e�̈ʒu���
	D3DXVECTOR3 rotBullet = GetRotScene2D();	//�e�̊p�x���
	D3DXVECTOR3 DestPos;	//�ړI�̈ʒu
	D3DXVECTOR3 BulletMove;	//�e�̈ړ���
	DestPos = playerPos - posBullet;

	float fAngle;
	float fShotAngle = D3DX_PI + (D3DX_PI / 2);
	int nNumScene = CScene::GetNumAll();	//�I�u�W�F�N�g�̐��̎擾

	//�t�H�[�X�̐��̎擾
	int nNumAttackForce = CForce::GetNumAttack();
	int nNumSpreadForce = CForce::GetNumSpread();
	int nNumSpeedForce = CForce::GetNumSpeed();
	int nNumDefenseForce = CForce::GetNumDefense();

	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	//�v���C���[�̏��
	CPlayer::PLAYERSTATE state = CPlayer::GetState();


	m_nCntFrame++;

	switch (m_type)
	{
	case BULLETTYPE_PLAYER:
		if (state == CPlayer::PLAYERSTATE_NORMAL || state == CPlayer::PLAYERSTATE_APPEAR)
		{
			CEffect::Create(posBullet, D3DXCOLOR((0.25f * nNumAttackForce) + (0.25f * nNumDefenseForce), (0.25f * nNumSpreadForce) + (0.15f * nNumDefenseForce), (0.25f * nNumSpeedForce), 1.0f), 20.0f, 20.0f, 15, CEffect::EFFECTTYPE_BULLET);
		}
		else if (state == CPlayer::PLAYERSTATE_HACK)
		{
			CEffect::Create(posBullet, D3DXCOLOR(((rand() % 10) * 0.1f), ((rand() % 10) * 0.1f), ((rand() % 10) * 0.1f), 1.0f), ((rand() % 20 + 10) * 1.0f), ((rand() % 20 + 10) * 1.0f), 10, CEffect::EFFECTTYPE_BULLET);
		}
		break;
	case BULLETTYPE_ENEMY_NORMAL:
		CEffect::Create(posBullet, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), 20.0f, 20.0f, 15, CEffect::EFFECTTYPE_BULLET);
		break;
	case BULLETTYPE_ENEMY_BOMB:
		CEffect::Create(posBullet, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f, 20, CEffect::EFFECTTYPE_BULLET);
		break;
	case BULLETTYPE_ENEMY_LASER:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 40, CEffect::EFFECTTYPE_BULLET);



		break;
	case BULLETTYPE_ENEMY_LASER_L:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 120, CEffect::EFFECTTYPE_BULLET);
		if (m_nLife > LIMIT_HORMING)
		{
			if (m_nCntFrame % 5 == 0)
			{
				DestPos = playerPos - posBullet;//�ړI�̈ʒu

				fAngle = atan2f(DestPos.x, DestPos.y);//�p�x
				//���ʂ̊p�x�𒴂�����
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				//�ړ��ʂ̐ݒ�
				BulletMove.x = sinf(fAngle);
				BulletMove.y = cosf(fAngle);
				BulletMove.z = 0.0f;

				//�ʒu���X�V
				m_BulletMove.x += BulletMove.x * -1.0f;
				m_BulletMove.y -= BulletMove.y * 0.6f;
			}
		}
		break;
	case BULLETTYPE_ENEMY_LASER_R:
		CEffect::Create(posBullet, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 15.0f, 15.0f, 120, CEffect::EFFECTTYPE_BULLET);
		if (m_nLife > LIMIT_HORMING)
		{
			if (m_nCntFrame % 5 == 0)
			{
				DestPos = playerPos - posBullet;//�ړI�̈ʒu

				fAngle = atan2f(DestPos.x, DestPos.y);
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				BulletMove.x = sinf(fAngle);
				BulletMove.y = cosf(fAngle);
				BulletMove.z = 0.0f;

				//�ʒu���X�V
				m_BulletMove.x -= BulletMove.x * 1.0f;
				m_BulletMove.y -= BulletMove.y * 0.6f;
			}
		}
		break;

	}


	//�ʒu�����X�V
	posBullet.y -= m_BulletMove.y;
	posBullet.x -= m_BulletMove.x;



	//�ʒu���̐ݒ�
	CScene2D::SetPosScene2D(posBullet);

	//�p�x���̐ݒ�
	CScene2D::SetRotScene2D(rotBullet);



	//���C�t�̌���
	m_nLife--;
	if (m_nLife <= 0)
	{
		if (m_type == BULLETTYPE_ENEMY_BOMB)
		{
			Create(posBullet, D3DXVECTOR3(0.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(4.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, 5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);

			Create(posBullet, D3DXVECTOR3(4.0f, 0.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, 0.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);


			Create(posBullet, D3DXVECTOR3(0.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(4.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);
			Create(posBullet, D3DXVECTOR3(-4.0f, -5.0f, 0.0f), CBullet::BULLETTYPE_ENEMY_NORMAL, 30, 1);


			//�����̐���
			CExplosion::Create(posBullet, 50.0f);
			//�I������
			Uninit();

		}
		else
		{
			//�I������
			Uninit();

			//�����̐���
			CExplosion::Create(posBullet, 50.0f);
		}
	}
	else if (posBullet.y <= STAGE_LIMIT_MIN_Y || posBullet.y >= STAGE_LIMIT_MAX_Y
		|| posBullet.x <= STAGE_LIMIT_MIN_X || posBullet.x >= STAGE_LIMIT_MAX_X)
	{
		//�I������
		Uninit();
	}
	else
	{///////////////////////////////////�����蔻��///////////////////////////////////
		for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
		{//�D�揇�ʂ�for��
			for (int nCntScene = 0; nCntScene < nNumScene; nCntScene++)
			{//nNumScene = �I�u�W�F�N�g�̑���
			 //�V�[���̎擾
				CScene2D *pScene2D;
				pScene2D = (CScene2D*)GetScene(nCntPriority, nCntScene);

				if (pScene2D != NULL)
				{//pScene2D���󂶂�Ȃ����
				 //�|���S���̏��
					D3DXVECTOR3 posScene2D = pScene2D->GetPosScene2D();	//�G�̈ʒu���

					int nForceType = pScene2D->GetForce();

					//�I�u�W�F�N�g�̎�ނ̎擾
					CScene::OBJTYPE type;
					type = pScene2D->GetObjType();

					if (m_type == BULLETTYPE_PLAYER)
					{//�v���C���[�̒e�Ȃ�
						if (type == CScene::OBJTYPE_ENEMY)
						{//if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
							if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
								&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
								&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
								&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
								)
							{//�͈͓��Ȃ�
								pScene2D->HitLife(m_nAttack);
								//pEnemy->HitEnemy(m_nAttack);
								//�X�R�A���Z
								CScore::AddScore(100);

								//�I������
								Uninit();
								break;
							}//(��)if:�͈͓��Ȃ�
						}//(��)if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
						if (type == CScene::OBJTYPE_BOSS)
						{//if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
							if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
								&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
								&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
								&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
								)
							{//�͈͓��Ȃ�
								//pBoss->HitDamage(m_nAttack);
								pScene2D->HitLife(m_nAttack);

								//�X�R�A���Z
								CScore::AddScore(100);

								//�I������
								Uninit();
								break;
							}//(��)if:�͈͓��Ȃ�
						}//(��)if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
					}//(��)if:�v���C���[�̒e�Ȃ�
					if (m_type == BULLETTYPE_ENEMY_NORMAL	||
						m_type == BULLETTYPE_ENEMY_BOMB		||
						m_type == BULLETTYPE_ENEMY_HORMING	||
						m_type == BULLETTYPE_ENEMY_LASER_R	||
						m_type == BULLETTYPE_ENEMY_LASER_L)
					{//�G�̒e�Ȃ�
						if (type == CScene::OBJTYPE_PLAYER)
						{//�I�u�W�F�N�g�̎�ނ��v���C���[�Ȃ�
							CPlayer::PLAYERSTATE state = CPlayer::GetState();
							if (state == CPlayer::PLAYERSTATE_NORMAL)
							{//�v���C���[�̏�Ԃ��m�[�}���Ȃ�
								if (posBullet.x >= (posScene2D.x - pScene2D->GetWidthScene2D()) + 2.0f
									&& posBullet.x <= (posScene2D.x + pScene2D->GetWidthScene2D()) -2.0f
									&& posBullet.y >= (posScene2D.y - pScene2D->GetHeightScene2D())+ 2.0f
									&& posBullet.y <= (posScene2D.y + pScene2D->GetHeightScene2D()) -2.0f
									)
								{//if:�͈͓��Ȃ�
								 //�����̐���
									//CExplosion::Create(posBullet, 50.0f);
									//�I������
									Uninit();
									pPlayer->HitDamage(1);
									break;
								}//(��)if:�͈͓��Ȃ�
							}//(��)�v���C���[�̏�Ԃ��m�[�}���Ȃ�
						}//(��)if:�I�u�W�F�N�g�̎�ނ��v���C���[�Ȃ�
						if (type == CScene::OBJTYPE_FORCE)
						{//�I�u�W�F�N�g�̎�ނ��t�H�[�X�Ȃ�
							if (nForceType == 3)
							{//�t�H�[�X�^�C�v���u��v�Ȃ�
								if (posBullet.x >= posScene2D.x - pScene2D->GetWidthScene2D()
									&& posBullet.x <= posScene2D.x + pScene2D->GetWidthScene2D()
									&& posBullet.y >= posScene2D.y - pScene2D->GetHeightScene2D()
									&& posBullet.y <= posScene2D.y + pScene2D->GetHeightScene2D()
									)
								{//�͈͓��Ȃ�
								 //�I������
									Uninit();
									break;
								}//(��)�͈͓��Ȃ�
							}//(��)�t�H�[�X�^�C�v���u��v�Ȃ�
						}//(��)�I�u�W�F�N�g�̎�ނ��t�H�[�X�Ȃ�
					}//(��)�e���G�Ȃ�
				}//(��)null����Ȃ��Ƃ�
			}//(��)�V�[����for��
		}//(��)�D�揇�ʂ�for��
	}//(��)else
}

//=============================================================================
//�`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	//2D�V�[���̕`�揈��
	CScene2D::Draw();
}
