////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�������� [camera.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "camera.h"
#include "input.h"
#include "main.h"
#include "sound.h"
#include "manager.h"
#include "Shader.h"
#include "game.h"
#include "renderer.h"
#include "player.h"
#include "DebugProc.h"
#include "meshWall.h"
#include "fade.h"
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define MAX_DISTANCE	(350.0f)		// ����
#define MAX_HEIGHT		(150)			// �����̋���
#define MAX_SPEED		(10.0f)			// �J�����̃X�s�[�h
#define MAX_ROLL		(0.03f)			// ��]��
#define MAX_DISTANCE_R	(10)			// �v���C���[�ƒ����_�̋���
#define MAX_HEIGHT_R	(70)			// �����_�̍���

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
bool CCamera::m_bPause = false;

//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CCamera::CCamera()
{
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CCamera::~CCamera()
{
}
//==============================================================================================================================
// �J�����̏���������
//==============================================================================================================================
HRESULT CCamera::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		m_aCamera[nCnt].m_posV = D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE);
		m_aCamera[nCnt].m_posVOld = D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE);
		m_aCamera[nCnt].m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_aCamera[nCnt].m_rot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_ViewRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_Diffrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_Destrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[nCnt].m_nState = CAMERASTATE_NONE;
		m_aCamera[nCnt].m_nStateCounter = 0;
		m_aCamera[nCnt].m_fLength = MAX_DISTANCE;
		m_aCamera[nCnt].m_fHeight = MAX_HEIGHT;
		m_aCamera[nCnt].m_fLengthR = MAX_DISTANCE_R;
		m_aCamera[nCnt].m_fHeightR = MAX_HEIGHT_R;
		m_aCamera[nCnt].m_viewpoart.X = 0;
		m_aCamera[nCnt].m_viewpoart.Y = 0;
		m_aCamera[nCnt].m_viewpoart.Width = SCREEN_WIDTH;
		m_aCamera[nCnt].m_viewpoart.Height = SCREEN_HEIGHT;
		m_aCamera[nCnt].m_viewpoart.MinZ = 0.0f;
		m_aCamera[nCnt].m_viewpoart.MaxZ = 1.0f;
	}

	// �~�j�}�b�v�J����
	m_aCamera[1].m_posV = D3DXVECTOR3(300.0f, 15000, -0.05f);
	m_aCamera[1].m_posR = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
	m_aCamera[1].m_rot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
	m_aCamera[1].m_viewpoart.X = SCREEN_WIDTH-300;
	m_aCamera[1].m_viewpoart.Y = 0;
	m_aCamera[1].m_viewpoart.Width = 300;
	m_aCamera[1].m_viewpoart.Height = 400;
	if (CManager::GetMode() == CManager::MODE::MODE_TITLE)
	{// �^�C�g���̎�
		m_aCamera[0].m_posV = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
		m_aCamera[0].m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[0].m_fLength = -2200.0f;
		m_aCamera[0].m_rot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[0].m_Diffrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[0].m_Destrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);

	}
	else if (CManager::GetMode() == CManager::MODE::MODE_GAME)
	{
		m_aCamera[0].m_posV = D3DXVECTOR3(0.0f, 12000.0f + 500, -MAX_DISTANCE);
		m_aCamera[0].m_posR = D3DXVECTOR3(0.0f, 12000.0f, 0.0f);
		m_aCamera[0].m_rot =	 D3DXVECTOR3(D3DX_PI/2, 0.0f, 0.0f);
		m_aCamera[0].m_Diffrot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
		m_aCamera[0].m_Destrot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);

		m_aCamera[0].m_fLength = 20;
		m_aCamera[0].m_fHeight = 500;
		m_aCamera[0].m_fLengthR = 0;
		m_aCamera[0].m_fHeightR = 0;

	}
	else if (CManager::GetMode() == CManager::MODE::MODE_RESULT)
	{// ���U���g�̎�
		m_aCamera[0].m_posV = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
		m_aCamera[0].m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera[0].m_fLength = -2200.0f;
		m_aCamera[0].m_rot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[0].m_Diffrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
		m_aCamera[0].m_Destrot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);

	}

	// �r���[�|�[�g
	m_aCamera[2].m_posV = D3DXVECTOR3(300.0f, 1000, 1.0f);
	m_aCamera[2].m_posR = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
	m_aCamera[2].m_rot = D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f);
	m_aCamera[2].m_viewpoart.X = SCREEN_WIDTH - 300;
	m_aCamera[2].m_viewpoart.Y = SCREEN_HEIGHT-300;
	m_aCamera[2].m_viewpoart.Width = 300;
	m_aCamera[2].m_viewpoart.Height = 300;
	m_aCamera[2].m_fLength = 100;
	m_aCamera[2].m_fHeight = MAX_HEIGHT / 7;
	m_aCamera[2].m_fHeightR = 55;
	m_PauseOldposV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PauseOldposR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = 0.0f;
	m_nCntPos = 0;
	m_nCntMove = 0;
	m_nCntRot = 0;
	m_TitleRot = 0.0f;
	m_Rotplus = D3DX_PI / 150;
	m_bPos = false;
	m_bRot = false;
	m_bSlow = false;
	m_bGrid = false;
	m_bPause = false;
	m_bReverse = false;

	return S_OK;
}

//==============================================================================================================================
// �J�����̏I������
//==============================================================================================================================
void CCamera::Uninit(void)
{
}

//==============================================================================================================================
// �J�����̍X�V����
//==============================================================================================================================
void CCamera::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	CInputGamePad::GamePad *pGamePad = CManager::GetInputGamePad()->GetgamePadStick();
	CManager::MODE pManager = CManager::GetMode();

	if (pManager == CManager::MODE::MODE_TITLE)
	{// �^�C�g�����[�h�̎�
		//== �J�����̌����ړ��̏��� ==//


		//*** �f�o�b�N ***//
		CDebugProc::print("m_rot  [ %.1f %.1f %.1f ]\n", m_aCamera[0].m_rot.x, m_aCamera[0].m_rot.y, m_aCamera[0].m_rot.z);
		CDebugProc::print("m_posV [ %.1f %.1f %.1f ]\n", m_aCamera[0].m_posV.x, m_aCamera[0].m_posV.y, m_aCamera[0].m_posV.z);
		CDebugProc::print("m_posR [ %.1f %.1f %.1f ]\n", m_aCamera[0].m_posR.x, m_aCamera[0].m_posR.y, m_aCamera[0].m_posR.z);
	}
	else if (pManager == CManager::MODE::MODE_GAME)
	{// �Q�[�����[�h�̎�
		for (int nCamera = 0; nCamera < MAX_CAMERA; nCamera++)
		{
			if (nCamera == 0)
			{
				if (m_aCamera[nCamera].m_nState == CAMERASTATE_NONE)
				{



					CPlayer *pPlayer = CGame::GetPlayer();
					bool bReverseOld = m_bReverse;

					m_aCamera[nCamera].m_posVOld = m_aCamera[nCamera].m_posV;
					if (m_bPause)
					{
						if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y += MAX_SPEED;
							m_aCamera[nCamera].m_posR.y += MAX_SPEED;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y -= MAX_SPEED;
							m_aCamera[nCamera].m_posR.y -= MAX_SPEED;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
						{// I��������
							m_aCamera[nCamera].m_posV.y += 10;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
						{// K��������
							m_aCamera[nCamera].m_posV.y += -10;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_Z))
						{// ���_������
								m_aCamera[nCamera].m_rot.y += MAX_ROLL;
								m_aCamera[nCamera].m_Destrot.y += MAX_ROLL;
								m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
								m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}
						else if (CManager::GetInputKeyboard()->GetPress(DIK_C))
						{// ���_�E����
								m_aCamera[nCamera].m_rot.y += -MAX_ROLL;
								m_aCamera[nCamera].m_Destrot.y += -MAX_ROLL;
								m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
								m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}
						for (int nIdy = 0; nIdy < MAX_GAMEPAD; nIdy++)
						{// �Q�[���p�b�h
							//if (pGamePad[nIdy].pDevGamePad != NULL)
							//{
							//	if (pGamePad[nIdy].aGamePad.lX < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lX > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY> GAMEPAD_DEADZONE)
							//	{
							//		float fGamePagStickAngle = atan2f(float(-pGamePad[nIdy].aGamePad.lX), float(pGamePad[nIdy].aGamePad.lY));
							//		m_aCamera[nCamera].m_posV.x += sinf(fGamePagStickAngle - D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
							//		m_aCamera[nCamera].m_posV.z += cosf(fGamePagStickAngle - D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
							//		m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							//		m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							//	}
							//	if (pGamePad[nIdy].aGamePad.lZ < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lZ > GAMEPAD_DEADZONE)
							//	{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							//	 // �p�x���v�Z����
							//		m_aCamera[nCamera].m_rot.y += float(pGamePad[nIdy].aGamePad.lZ / (MAX_ZONE*20.0f));
							//		m_aCamera[nCamera].m_Destrot.y += float(pGamePad[nIdy].aGamePad.lZ / (MAX_ZONE*20.0f));
							//		m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;
							//		m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;
							//	}
							//	if (pGamePad[nIdy].aGamePad.lRz > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lRz < -GAMEPAD_DEADZONE)
							//	{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							//	 // �p�x���v�Z����
							//		m_aCamera[nCamera].m_rot.x += float(pGamePad[nIdy].aGamePad.lRz / (MAX_ZONE*50.0f));
							//		m_aCamera[nCamera].m_Destrot.x += float(pGamePad[nIdy].aGamePad.lRz / (MAX_ZONE*50.0f));
							//		m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + cosf(m_aCamera[nCamera].m_rot.x) * m_aCamera[nCamera].m_fHeight;
							//	}
							//}
							//if (CManager::GetInputGamePad()->GetGamePadPress(nIdy, BUTTON_Y))
							//{// �㏸
							//	m_aCamera[nCamera].m_posV.y += MAX_SPEED;
							//	m_aCamera[nCamera].m_posR.y += MAX_SPEED;
							//}
							//if (CManager::GetInputGamePad()->GetGamePadPress(nIdy, BUTTON_X))
							//{// ���~
							//	m_aCamera[nCamera].m_posV.y -= MAX_SPEED;
							//	m_aCamera[nCamera].m_posR.y -= MAX_SPEED;
							//}
						}

					}
					else
					{

						m_bReverse = false;
						if (CManager::GetInputGamePad()->GetGamePadPress(0, BUTTON_Y))
						{// ���]
							m_bReverse = true;
						}

						if (pGamePad[0].aGamePad.lRz > MAX_ZONE / 2|| CManager::GetInputKeyboard()->GetPress(DIK_R))
						{// �㏸
							m_aCamera[nCamera].m_fHeight += 5;
							CManager::SaveMinMax(&m_aCamera[nCamera].m_fHeight, MAX_HEIGHT - 100, MAX_HEIGHT + 50);
						}
						if (pGamePad[0].aGamePad.lRz < -MAX_ZONE / 2 || CManager::GetInputKeyboard()->GetPress(DIK_F))
						{// ���~
							m_aCamera[nCamera].m_fHeight += -5;
							CManager::SaveMinMax(&m_aCamera[nCamera].m_fHeight, MAX_HEIGHT - 100, MAX_HEIGHT + 50);
						}
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_Z))
					{// ���_������
						m_aCamera[nCamera].m_rot.y += MAX_ROLL;
						m_aCamera[nCamera].m_Destrot.y += MAX_ROLL;
						m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_ViewRot.y += 0.1f;
					}
					else if (CManager::GetInputKeyboard()->GetPress(DIK_C))
					{// ���_�E����
						m_aCamera[nCamera].m_rot.y += -MAX_ROLL;
						m_aCamera[nCamera].m_Destrot.y += -MAX_ROLL;
						m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_ViewRot.y += -0.1f;
					}
					else if (pGamePad[0].aGamePad.lZ > GAMEPAD_DEADZONE || pGamePad[0].aGamePad.lZ < -GAMEPAD_DEADZONE)
					{// �Q�[���p�b�h
						m_aCamera[0].m_rot.y += float(pGamePad[0].aGamePad.lZ / (MAX_ZONE*20.0f));
						m_aCamera[0].m_Destrot.y += float(pGamePad[0].aGamePad.lZ / (MAX_ZONE*20.0f));
						m_aCamera[0].m_posV.x = m_aCamera[0].m_posR.x + sinf(m_aCamera[0].m_rot.y - D3DX_PI) * m_aCamera[0].m_fLength;
						m_aCamera[0].m_posV.z = m_aCamera[0].m_posR.z + cosf(m_aCamera[0].m_rot.y - D3DX_PI) * m_aCamera[0].m_fLength;
						float fGamePagStickAngle = fabsf(float(pGamePad[0].aGamePad.lZ)) / float(pGamePad[0].aGamePad.lZ);
						m_aCamera[nCamera].m_ViewRot.y = D3DX_PI / 2 * fGamePagStickAngle;
					}
					else
					{
						m_aCamera[nCamera].m_ViewRot.y = 0.0f;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_LEFT) == true)
					{// ���ړ�
						m_aCamera[nCamera].m_posV.x += sinf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posV.z += cosf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_RIGHT) == true)
					{// �E�ړ�
						m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_UP) == true)
					{// �O�ړ�
						m_aCamera[nCamera].m_posV.x += sinf(0 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posV.z += cosf(0 + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_DOWN) == true)
					{// ���ړ�
						m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
						m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
					}

					if (!m_bPause)
					{
						m_PauseOldposV = m_aCamera[nCamera].m_posV;
						m_PauseOldposR = m_aCamera[nCamera].m_posR;
						m_PauseOldrot = m_aCamera[nCamera].m_rot;
					}
					if (m_bPause && !CGame::GetPause())
					{
						m_aCamera[nCamera].m_posVDest = m_PauseOldposV;
						m_aCamera[nCamera].m_posRDest = m_PauseOldposR;
						m_aCamera[nCamera].m_rot = m_PauseOldrot;
					}
					m_bPause = CGame::GetPause();
					if (!m_bPause)
					{// �|�[�Y���Ă��Ȃ�

						CManager::SaveRot(&m_aCamera[nCamera].m_Destrot);			// ��]�Z�[�u
						m_aCamera[nCamera].m_Diffrot = m_aCamera[nCamera].m_Destrot - m_aCamera[nCamera].m_rot;// �ړI-����
						CManager::SaveRot(&m_aCamera[nCamera].m_Diffrot);			// ��]�Z�[�u
						m_aCamera[nCamera].m_rot.x += m_aCamera[nCamera].m_Diffrot.x;	// �����v�Z
						m_aCamera[nCamera].m_rot.y += m_aCamera[nCamera].m_Diffrot.y*0.03f;	// �����v�Z
						m_aCamera[nCamera].m_rot.z += m_aCamera[nCamera].m_Diffrot.z;	// �����v�Z
						CManager::SaveRot(&m_aCamera[nCamera].m_rot);

						m_aCamera[nCamera].m_posR.y -= 5.0f;
						if (CGame::GetStage() == CGame::STAGE_EARTH || CGame::GetStage() == CGame::STAGE_HELL)
						{
							m_aCamera[nCamera].m_posR.y -= 3.0f;
						}

						m_aCamera[nCamera].m_posV.x = 0;
						m_aCamera[nCamera].m_posV.z = -10;
						m_aCamera[nCamera].m_posR.x = 0;
						m_aCamera[nCamera].m_posR.z = 0;
						m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + (m_aCamera[nCamera].m_fHeight);			// �p�x�̉��Z

					}

				}
				else if (m_aCamera[nCamera].m_nState == CAMERASTATE_EVENT)
				{
				}
				else if (m_aCamera[nCamera].m_nState == CAMERASTATE_START)
				{// �J�n��
					m_aCamera[nCamera].m_nStateCounter++;
					m_aCamera[nCamera].m_posR.x += sinf(m_aCamera[nCamera].m_rot.y)*4;
					m_aCamera[nCamera].m_posR.z += cosf(m_aCamera[nCamera].m_rot.y) * 4;
					m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
					m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + 100;
					m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;

					if (m_aCamera[nCamera].m_nStateCounter == 170)
					{
						CFade::SetCloss();

					}
					if (m_aCamera[nCamera].m_nStateCounter >= 270)
					{
						m_aCamera[nCamera].m_nState = CAMERASTATE_NONE;
					}
				}
			}
			else if (nCamera == 2)
			{
				//CPlayer *pPlayer = CGame::GetPlayer(CManager::GetMainPlayerID());

				//m_aCamera[nCamera].m_Destrot.y = pPlayer->GetRot().y - D3DX_PI / 4;

				//CManager::SaveRot(&m_aCamera[nCamera].m_Destrot);			// ��]�Z�[�u
				//m_aCamera[nCamera].m_Diffrot = m_aCamera[nCamera].m_Destrot - m_aCamera[nCamera].m_rot;// �ړI-����
				//CManager::SaveRot(&m_aCamera[nCamera].m_Diffrot);			// ��]�Z�[�u
				//m_aCamera[nCamera].m_rot += m_aCamera[nCamera].m_Diffrot;	// �����v�Z
				//CManager::SaveRot(&m_aCamera[nCamera].m_rot);
				//D3DXVECTOR3 pos = pPlayer->GetPos() + pPlayer->GetParentPartsPos() + D3DXVECTOR3(0.0f, -25.0f, 0.0f);

				//float posYDest = pos.y + (m_aCamera[nCamera].m_fHeight);
				//m_aCamera[nCamera].m_posV.x = pos.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * (m_aCamera[nCamera].m_fLength);
				//m_aCamera[nCamera].m_posV.y +=( posYDest- m_aCamera[nCamera].m_posV.y);			// �p�x�̉��Z
				//m_aCamera[nCamera].m_posV.z = pos.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * (m_aCamera[nCamera].m_fLength);		// �p�x�̉��Z																																										// �v���C���[�̏����O�̒����_�ǔ�
				//m_aCamera[nCamera].m_posRDest.x = pos.x + sinf(pPlayer->GetRot().y - D3DX_PI) * m_aCamera[nCamera].m_fLengthR;
				//m_aCamera[nCamera].m_posRDest.y = pos.y + m_aCamera[nCamera].m_fHeightR;
				//m_aCamera[nCamera].m_posRDest.z = pos.z + cosf(pPlayer->GetRot().y - D3DX_PI) * m_aCamera[nCamera].m_fLengthR;
				//m_aCamera[nCamera].m_posRDiff = m_aCamera[nCamera].m_posRDest - m_aCamera[nCamera].m_posR;// ���݂̊p�x�ƖړI�̊p�x�̍���
				//m_aCamera[nCamera].m_posR.x += m_aCamera[nCamera].m_posRDiff.x;				// �p�x�̉��Z
				//m_aCamera[nCamera].m_posR.y = m_aCamera[nCamera].m_posRDest.y;				// �p�x�̉��Z
				//m_aCamera[nCamera].m_posR.z += m_aCamera[nCamera].m_posRDiff.z;				// �p�x�̉��Z
			}
		}
	}
		else if (pManager == CManager::MODE::MODE_CREATE)
		{// �N���G�C�g���[�h�̎�
			for (int nCamera = 0; nCamera < MAX_CAMERA; nCamera++)
			{
				if (nCamera == 0)
				{
					if (m_bSlow == false)
					{
						m_move = MAX_SPEED;
					}
					else
					{
						m_move = MAX_SPEED / 4;
					}
					CDebugProc::print("�����_�㉺�ړ��F�E/���N���b�N\n");
					CDebugProc::print("�����_�����ύX�F�}�E�X�z�C�[��\n");
					if (CManager::GetInputMouse()->GetWheel() > 0 || CManager::GetInputMouse()->GetWheel() < 0)
					{// �����L�΂�
						m_aCamera[nCamera].m_fLength += CManager::GetInputMouse()->GetWheel();
					}
					if (CManager::GetInputMouse()->GetPress(VK_LBUTTON))
					{// ��
						m_aCamera[nCamera].m_fHeight += 3.0f;
					}
					if (CManager::GetInputMouse()->GetPress(VK_RBUTTON))
					{// �E
						m_aCamera[nCamera].m_fHeight += -3.0f;
					}
					m_aCamera[nCamera].m_posVOld = m_aCamera[nCamera].m_posV;
					for (int nIdy = 0; nIdy < MAX_GAMEPAD; nIdy++)
					{// �Q�[���p�b�h
						if (pGamePad[nIdy].pDevGamePad != NULL)
						{
							if (pGamePad[nIdy].aGamePad.lX < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lX > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY> GAMEPAD_DEADZONE)
							{
								float fGamePagStickAngle = atan2f(float(-pGamePad[nIdy].aGamePad.lX), float(pGamePad[nIdy].aGamePad.lY));
								m_aCamera[nCamera].m_posV.x += sinf(fGamePagStickAngle - D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
								m_aCamera[nCamera].m_posV.z += cosf(fGamePagStickAngle - D3DX_PI + m_aCamera[nCamera].m_rot.y)*MAX_SPEED;
								m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
								m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							}
							if (pGamePad[nIdy].aGamePad.lZ < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lZ > GAMEPAD_DEADZONE)
							{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							 // �p�x���v�Z����
								m_aCamera[nCamera].m_rot.y += float(pGamePad[nIdy].aGamePad.lZ / (MAX_ZONE*20.0f));
								m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;
								m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;
							}
							if (pGamePad[nIdy].aGamePad.lRz > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lRz < -GAMEPAD_DEADZONE)
							{// �A�i���O�X�e�B�b�N�̕���[�p�x]�Ɉړ�
							 // �p�x���v�Z����
								m_aCamera[nCamera].m_rot.x += float(pGamePad[nIdy].aGamePad.lRz / (MAX_ZONE*50.0f));
								m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + cosf(m_aCamera[nCamera].m_rot.x) * m_aCamera[nCamera].m_fHeight;
							}
						}
					}

					if (m_bGrid == false)
					{// �O���b�h���[�h [ �I�t ]

					 //== ���񏈗� ==//
						if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
						{// ���_������
							m_aCamera[nCamera].m_rot.y += MAX_ROLL;
							m_aCamera[nCamera].m_Destrot.y += MAX_ROLL;
							m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
						{// ���_�E����
							m_aCamera[nCamera].m_rot.y += -MAX_ROLL;
							m_aCamera[nCamera].m_Destrot.y += -MAX_ROLL;
							m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}

						//== �ړ����� ==//
						if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
						{// ���ړ�
							m_aCamera[nCamera].m_posV.x += sinf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posV.z += cosf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
						{// �E�ړ�
							m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
						{// �O�ړ�
							m_aCamera[nCamera].m_posV.x += sinf(0 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posV.z += cosf(0 + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
						{// ���ړ�
							m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*m_move;
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y += m_move;
							m_aCamera[nCamera].m_posR.y += m_move;
						}
						if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y -= m_move;
							m_aCamera[nCamera].m_posR.y -= m_move;
						}

					}
					else
					{// �O���b�h���[�h [ �I�� ]

					 //== ���񏈗� ==//
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) == true)
						{// ���_������
							m_aCamera[nCamera].m_rot.y += D3DX_PI / 4;
							m_aCamera[nCamera].m_Destrot.y += D3DX_PI / 4;
							m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) == true)
						{// ���_�E����
							m_aCamera[nCamera].m_rot.y += -D3DX_PI / 4;
							m_aCamera[nCamera].m_Destrot.y += -D3DX_PI / 4;
							m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						}

						//== �ړ����� ==//
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
						{// ���ړ�
							m_aCamera[nCamera].m_posV.x += sinf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*(m_move * 10);
							m_aCamera[nCamera].m_posV.z += cosf(-D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*(m_move * 10);
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) *m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
						{// �E�ړ�
							m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*(m_move * 10);
							m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI / 2 + m_aCamera[nCamera].m_rot.y)*(m_move * 10);
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
						{// �O�ړ�
							m_aCamera[nCamera].m_posV.x += sinf(0 + m_aCamera[nCamera].m_rot.y)*(m_move * 50);
							m_aCamera[nCamera].m_posV.z += cosf(0 + m_aCamera[nCamera].m_rot.y)*(m_move * 50);
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
						{// ���ړ�
							m_aCamera[nCamera].m_posV.x += sinf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*(m_move * 50);
							m_aCamera[nCamera].m_posV.z += cosf(D3DX_PI + m_aCamera[nCamera].m_rot.y)*(m_move * 50);
							m_aCamera[nCamera].m_posR.x = m_aCamera[nCamera].m_posV.x + sinf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
							m_aCamera[nCamera].m_posR.z = m_aCamera[nCamera].m_posV.z + cosf(m_aCamera[nCamera].m_rot.y) * m_aCamera[nCamera].m_fLength;
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y += (m_move * 20);
							m_aCamera[nCamera].m_posR.y += (m_move * 20);
						}
						if (CManager::GetInputKeyboard()->GetTrigger(DIK_F) == true)
						{// ��
							m_aCamera[nCamera].m_posV.y -= (m_move * 20);
							m_aCamera[nCamera].m_posR.y -= (m_move * 20);
						}
					}

					//== ���Z�b�g ==//
					if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
					{// �����̃��Z�b�g
						m_aCamera[nCamera].m_rot.y = 0;
						m_aCamera[nCamera].m_Destrot.y = 0;
						m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_Z) == true)
					{// ���ړ�
						m_aCamera[nCamera].m_posV.x = 0;
						m_aCamera[nCamera].m_posV.z = 0;
						m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;
						m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) *m_aCamera[nCamera].m_fLength;


					}
					for (int nIdy = 0; nIdy < MAX_GAMEPAD; nIdy++)
					{// �Q�[���p�b�h
						if (CManager::GetInputGamePad()->GetGamePadPress(nIdy, BUTTON_Y))
						{// �㏸
							m_aCamera[nCamera].m_posV.y += m_move;
							m_aCamera[nCamera].m_posR.y += m_move;
						}
						if (CManager::GetInputGamePad()->GetGamePadPress(nIdy, BUTTON_X))
						{// ���~
							m_aCamera[nCamera].m_posV.y -= m_move;
							m_aCamera[nCamera].m_posR.y -= m_move;
						}
					}

					if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
					{// I��������
					 // �p�x���v�Z����
						m_aCamera[nCamera].m_rot.z += -0.02f;
						m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + cosf(m_aCamera[nCamera].m_rot.z + m_aCamera[nCamera].m_rot.x) * m_aCamera[nCamera].m_fHeight;
					}
					if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
					{// K��������
					 // �p�x���v�Z����
						m_aCamera[nCamera].m_rot.z += 0.02f;
						m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + cosf(m_aCamera[nCamera].m_rot.z + m_aCamera[nCamera].m_rot.x) * m_aCamera[nCamera].m_fHeight;
					}

					m_bPause = true;

					CManager::SaveRot(&m_aCamera[nCamera].m_Destrot);			// ��]�Z�[�u
					m_aCamera[nCamera].m_Diffrot = m_aCamera[nCamera].m_Destrot - m_aCamera[nCamera].m_rot;// �ړI-����
					CManager::SaveRot(&m_aCamera[nCamera].m_Diffrot);			// ��]�Z�[�u
					m_aCamera[nCamera].m_rot += m_aCamera[nCamera].m_Diffrot;	// �����v�Z
					CManager::SaveRot(&m_aCamera[nCamera].m_rot);

					m_aCamera[nCamera].m_posV.x = m_aCamera[nCamera].m_posR.x + sinf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;
					m_aCamera[nCamera].m_posV.y = m_aCamera[nCamera].m_posR.y + cosf(m_aCamera[nCamera].m_rot.z + m_aCamera[nCamera].m_rot.x) * m_aCamera[nCamera].m_fHeight;
					m_aCamera[nCamera].m_posV.z = m_aCamera[nCamera].m_posR.z + cosf(m_aCamera[nCamera].m_rot.y - D3DX_PI) * m_aCamera[nCamera].m_fLength;

				}
			}
		}

}

//==============================================================================================================================
// �J�����̐ݒ菈��
//==============================================================================================================================
void CCamera::Set(int nCamera)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	// �r���[�|�[�g
	pDevice->SetViewport(&m_aCamera[nCamera].m_viewpoart);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_aCamera[nCamera].m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_aCamera[nCamera].m_mtxProjection, D3DXToRadian(45.0f),
								(float)m_aCamera[nCamera].m_viewpoart.Width / (float)m_aCamera[nCamera].m_viewpoart.Height,
								10.0f,			// �ŏ�
								50000.0f);		// �ő�

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_aCamera[nCamera].m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_aCamera[nCamera].m_mtxView);
	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_aCamera[nCamera].m_mtxView,
		&m_aCamera[nCamera].m_posV, &m_aCamera[nCamera].m_posR, &m_aCamera[nCamera].m_vecU);
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera[nCamera].m_mtxView);

}
void CCamera::SetPosV(const int nCamera, const D3DXVECTOR3 pos)
{
	m_aCamera[nCamera].m_posV = pos;
}
void CCamera::SetPosR(const int nCamera, const D3DXVECTOR3 pos)
{
	m_aCamera[nCamera].m_posR = pos;

}
void CCamera::SetSlow(bool slow)
{
	m_bSlow = slow;
}
void CCamera::SetGrid(bool Grid)
{
	m_bGrid = Grid;
}
void CCamera::SetState(int nCamera, CAMERASTATE state)
{
	m_aCamera[nCamera].m_nState = state;
}
