//=============================================================================
//
// �J�������� [camera.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "scene.h"
#include "boss.h"
#include "tutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_SPEED (1.0f)
#define CAMERA_HEIGHT (150.0f)
#define CAMERA_MOVE (3.5f)
#define ROT_CAMERASPEED (0.01f)
#define ROT_SPEED (0.1f)

#define CAMERA_ANGLE (12.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager *CCamera::m_pManager = NULL;
bool CCamera::m_bFree = false;
//=============================================================================
// �֐����F�R���X�g���N�^
// �֐��̊T�v�F�������ꂽ�Ƃ��ɌĂяo�����B�l�̃N���A
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	m_fLength = 0.0f;
	m_fHeight_V = 0.0f;
	m_fHeight_R = 0.0f;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);
	m_pManager = NULL;
}

//=============================================================================
// �֐����F�f�X�g���N�^
// �֐��̊T�v�F�Ō�ɌĂяo�����
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// �֐����F�J�����̏���������
// �֐��̊T�v�F�J�����̈ʒu��A������ݒ肷��B
//=============================================================================
HRESULT CCamera::Init(void)
{
	//m_type = type;

	//�J�����̈ʒu���̏�����
	m_posV = D3DXVECTOR3(450.0f, 250.0f, -1200.0f);
	m_posR = D3DXVECTOR3(450.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	m_fLength = 700.0f;		// �����_����̋���
	m_fHeight_V = 270.0f;		// ����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);	// �p�x���
	m_bFree = false;

	return S_OK;
}

//=============================================================================
// �֐����F�J�����̏I��
// �֐��̊T�v�F
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �֐����F�J�����̍X�V
// �֐��̊T�v�F�Ǐ]�J�����̐ݒ�
//=============================================================================
void CCamera::Update(void)
{

#if 1

																//���
	XINPUT_STATE state;
	XInputGetState(0, &state);


	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	CPlayer *pPlayer = NULL;
	CBoss *pBoss = NULL;

	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		pBoss = CGame::GetBoss();
		break;

	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}



	if (mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
	{
		D3DXVECTOR3 playerpos = pPlayer->GetPos();
		D3DXVECTOR3 playerrot = pPlayer->GetRot();
		D3DXVECTOR3 playermove = pPlayer->GetMove();


		if (CScene::GetBossStop() == true)
		{//�{�X�ȊO���~�܂��Ă���Ƃ�
		 //�ړI�̊p�x�ݒ�
			if (pBoss != NULL)
			{
				D3DXVECTOR3 bosspos = pBoss->GetPos();
				D3DXVECTOR3 bossrot = pBoss->GetRot();
				D3DXVECTOR3 bossmove = pBoss->GetMove();

				if (bosspos.z < -400.0f)
				{
					if (bossrot.y > -1.6 && bossrot.y < 1.6)
					{
						m_fDestAngle = 2.0f;
					}
					else if (bossrot.y < -1.6 && bossrot.y > 1.6)
					{
						m_fDestAngle = -2.0f;
					}
				}

				//�ړI�̊p�x�܂�
				m_fAngle = m_fDestAngle - m_rot.y;
				//�p�x����
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//�p�x���
				m_rot.y += m_fAngle * 0.035f;
				//�ړI�̋����ݒ�
				m_fdestLength = 80.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;

				//�ړI�̍����ݒ�(V)
				m_fdestHeight_V = 70.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;

				//�ړI�̍����ݒ�(R)
				m_fdestHeight_R = 80.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//�ړI�̎��_�ݒ�(V)
				m_posVDest.x = bosspos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = bosspos.z + cosf(m_rot.y) * -m_fLength;

				//�ړI�̎��_�ݒ�(R)
				m_posRDest.x = bosspos.x - sinf(bossrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = bosspos.z - cosf(bossrot.y) * 1.0f;

				//���݂̎��_�̐ݒ�(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.1f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.1f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.1f;

				//���݂̎��_�̐ݒ�(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;
			}
		}
		if (CScene::GetStop() == true && CScene::GetBossStop() == false)
		{//�v���C���[�ȊO���~�܂��Ă���Ƃ�


		 //�ړI�̊p�x�ݒ�
			if (playerpos.z < -400.0f)
			{
				if (playerrot.y > -1.6 && playerrot.y < 1.6)
				{
					m_fDestAngle = 0.9f;
				}
				else if (playerrot.y < -1.6 && playerrot.y > 1.6)
				{
					m_fDestAngle = -0.9f;
				}
			}
			else
			{
				if (playerrot.y > 0)
				{
					m_fDestAngle = 0.5f;
				}
				else if (playerrot.y < 0)
				{
					m_fDestAngle = -0.5f;
				}
				if (playerrot.y == 0)
				{
					m_fDestAngle = 0.0f;
				}
			}
			//�ړI�̊p�x�܂�
			m_fAngle = m_fDestAngle - m_rot.y;
			//�p�x����
			if (m_fAngle > D3DX_PI)
			{
				m_fAngle -= D3DX_PI * 2;
			}

			if (m_fAngle < -D3DX_PI)
			{
				m_fAngle += D3DX_PI * 2;
			}
			//�p�x���
			m_rot.y += m_fAngle * 0.035f;
			//�ړI�̋����ݒ�
			m_fdestLength = 80.0f;
			m_fLength += (m_fdestLength - m_fLength) * 0.1f;

			//�ړI�̍����ݒ�(V)
			m_fdestHeight_V = 50.0f;
			m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;

			//�ړI�̍����ݒ�(R)
			m_fdestHeight_R = 60.0f;
			m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

			//�ړI�̎��_�ݒ�(V)
			m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
			m_posVDest.y = m_fHeight_V;
			m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;

			//�ړI�̎��_�ݒ�(R)
			m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
			m_posRDest.y = m_fHeight_R;
			m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

			//���݂̎��_�̐ݒ�(V)
			m_posV.x += (m_posVDest.x - m_posV.x) * 0.1f;
			m_posV.y += (m_posVDest.y - m_posV.y) * 0.1f;
			m_posV.z += (m_posVDest.z - m_posV.z) * 0.1f;

			//���݂̎��_�̐ݒ�(R)
			m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
			m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
			m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;
		}
		else if(CScene::GetStop() == false && CScene::GetBossStop() == false)
		{//�ʏ펞
			if (playerpos.z > -35.0f)
			{//�d�Ԃ̒�
			 //�ړI�̊p�x�ݒ�
				m_fDestAngle = 0.0f;
				//�ړI�̊p�x�܂�
				m_fAngle = m_fDestAngle - m_rot.y;
				//�p�x����
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//�p�x���
				m_rot.y += m_fAngle * 0.035f;
				//�ړI�̋����ݒ�
				m_fdestLength = 280.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;
				//�ړI�̍����ݒ�(V)
				m_fdestHeight_V = 180.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
				//�ړI�̍����ݒ�(R)
				m_fdestHeight_R = 100.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//�ړI�̎��_�ݒ�(V)
				m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = -m_fLength;
				//�ړI�̎��_�ݒ�(R)
				m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = 0.0f;

				//���݂̎��_�̐ݒ�(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;

				//���݂̎��_�̐ݒ�(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
			else if (playerpos.z < -400.0f)
			{//���D��̒ʘH
			 //�ړI�̊p�x�ݒ�
				m_fDestAngle = D3DX_PI * 0.5f;
				//�ړI�̊p�x�܂�
				m_fAngle = m_fDestAngle - m_rot.y;
				//�p�x����
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//�p�x���
				m_rot.y += m_fAngle * 0.035f;
				//�ړI�̋����ݒ�
				m_fdestLength = 300.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.2f;
				//�ړI�̍����ݒ�(V)
				m_fdestHeight_V = 220.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.2f;
				//�ړI�̍����ݒ�(R)
				m_fdestHeight_R = 80.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.2f;


				//�ړI�̎��_�ݒ�(V)
				m_posVDest.x = 3850.0f + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;
				//�ړI�̎��_�ݒ�(R)
				m_posRDest.x = 3850.0f - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

				//���݂̎��_�̐ݒ�(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
				//���݂̎��_�̐ݒ�(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
			else
			{//����ȊO�̂Ƃ���
			 //�ړI�̊p�x�ݒ�
				m_fDestAngle = 0.0f;
				//�ړI�̊p�x�܂�
				m_fAngle = m_fDestAngle - m_rot.y;
				//�p�x����
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//�p�x���
				m_rot.y += m_fAngle * 0.035f;
				//�ړI�̋����ݒ�
				m_fdestLength = 580.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;
				//�ړI�̍����ݒ�(V)
				m_fdestHeight_V = 200.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
				//�ړI�̍����ݒ�(R)
				m_fdestHeight_R = -10.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//�ړI�̎��_�ݒ�(V)
				m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = cosf(m_rot.y) * -m_fLength;
				//�ړI�̎��_�ݒ�(R)
				m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = cosf(playerrot.y) * 1.0f;

				//���݂̎��_�̐ݒ�(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
				//���݂̎��_�̐ݒ�(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
		}
	}//�Q�[�����[�h�̂Ƃ�

	if (mode == CManager::MODE_RANKING || mode == CManager::MODE_TITLE)
	{

		//�p�x����
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2;
		}

		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2;
		}
		//�p�x���
		m_rot.y += 0.0f;

		//�ړI�̋����ݒ�
		m_fdestLength = 663.0f;
		m_fLength += (m_fdestLength - m_fLength) * 0.1f;
		//�ړI�̍����ݒ�(V)
		m_fdestHeight_V = 240.0f;
		m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
		//�ړI�̍����ݒ�(R)
		m_fdestHeight_R = 0.0f;
		m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

		//�ړI�̎��_�ݒ�(V)
		m_posVDest.x = 0.0f;
		m_posVDest.y = m_fHeight_V;
		m_posVDest.z = -m_fLength;
		//�ړI�̎��_�ݒ�(R)
		m_posRDest.x = 0.0f;
		m_posRDest.y = m_fHeight_R;
		m_posRDest.z = 0.0f;

		//���݂̎��_�̐ݒ�(V)
		m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
		m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
		m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
		//���݂̎��_�̐ݒ�(R)
		m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
		m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;



	}

#ifdef _DEBUG

	////�J�����̈ʒu���
	//CDebugProc::Print("PosV(X,Y,Z):%.1f, %.1f, %.1f\n", m_posV.x, m_posV.y, m_posV.z);
	//CDebugProc::Print("Rot(X,Y,Z):%.1f, %.2f, %.1f\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("PosR(X,Y,Z):%.1f, %.1f, %.1f\n", m_posR.x, m_posR.y, m_posR.z);
#endif


	OverView();
#endif

}

//=============================================================================
// �֐����F�J�����̐ݒ菈��
// �֐��̊T�v�F�J������ݒu����
//=============================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		900.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,//�r���[�}�g���b�N�X
		&m_posV,//���_
		&m_posR,//�����_
		&m_vecU);//������x�N�g��

				 // �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �֐����F�J�����̒���
// �֐��̊T�v�F�J�����̊p�x������߂����Ƃ��̒���
//=============================================================================
void CCamera::OverView()
{
	if (m_rot.y > D3DX_PI)
	{

		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{

		m_rot.y += D3DX_PI * 2;
	}


	if (m_rot.z > D3DX_PI)
	{

		m_rot.z -= D3DX_PI * 2;
	}

	if (m_rot.z < -D3DX_PI)
	{

		m_rot.z += D3DX_PI * 2;
	}

	if (m_rot.x > D3DX_PI)
	{

		m_rot.x -= D3DX_PI * 2;
	}

	if (m_rot.x < -D3DX_PI)
	{

		m_rot.x += D3DX_PI * 2;
	}


}

//=============================================================================
// �֐����F�J�����̃}�g���b�N�X�̎擾
// �֐��̊T�v�F�J�����̃}�g���b�N�X��Ԃ�
//=============================================================================
D3DXMATRIX CCamera::GetmtxView(void)
{
	return m_mtxView;
}

D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

void CCamera::SetFree(void)
{
	m_bFree = m_bFree ? false : true;

}

