//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Ryota Kinoshita
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// �C���N���[�h�֐�
#include <string.h>		// �X�g�����O�֐�
#include <stdlib.h>		// �����h�֐�

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "Texture.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "create.h"
#include "DebugProc.h"
#include "line.h"
#include "objCreate.h"

#include <time.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR (2024)
#define FAILE_TIME ("data/system.txt")
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;			// �����_���[
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// �L�[�{�[�h
CInputMouse *CManager::m_pInputMouse = NULL;		// �}�E�X
CInputGamePad *CManager::m_pInputGamePad = NULL;	// �Q�[���p�b�h
CSound *CManager::m_pSound = NULL;					// ��
CTexture *CManager::m_pTexture = NULL;				// �e�N�X�`��
CManager::MODE CManager::m_mode = MODE_TITLE;		// ���[�h
CTitle *CManager::m_pTitle = NULL;					// �^�C�g��
CGame *CManager::m_pGame = NULL;					// �Q�[��
CResult *CManager::m_pResult = NULL;				// ���U���g
CCreate *CManager::m_pCreate = NULL;				// �N���G�C�g
CDebugProc *CManager::m_pDebugProc = NULL;			// �f�o�b�O

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// ���[�h
//=============================================================================
void CManager::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	FILE *pFile;
	char cLine[MAX_CHAR];	// �s�S�ēǂݍ���
	char cData[MAX_CHAR];	// ��������
	pFile = fopen(FAILE_TIME, "r");		// �t�@�C���̖��O�̓ǂݍ���

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);		// �s�̓ǂݍ���
			if (strcmp(cData, "SETGAME") == 0)
			{// OBJECTSET�������ꍇ
				while (strcmp(cData, "END_SETGAME") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "MODE") == 0)
					{// ���[�h
						sscanf(&cLine[0], "%s %s %d ", &cData[0], &cData[0], &m_mode);
					}

				}
			}
		}
		fclose(pFile);// �I��
	}
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	srand(unsigned int(time(0)));
	if (m_pRenderer == NULL)
	{// �����_�����O�̐���
		m_pRenderer = new CRenderer;
	}
	if (m_pRenderer != NULL)
	{// �����_�����O����������Ă���
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{// ����������
			return -1;
		}
	}
	if (m_pInputKeyboard == NULL)
	{// �L�[�{�[�h�̐���
		m_pInputKeyboard = new CInputKeyboard;
	}
	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h����������Ă���
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{// �L�[�{�[�h�̏�����
			return -1;
		}
	}

	if (m_pInputMouse == NULL)
	{// �}�E�X�̐���
		m_pInputMouse = new CInputMouse;
	}
	if (m_pInputMouse != NULL)
	{// �}�E�X����������Ă���
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{// �Q�[���p�b�h�̏�����
			return -1;
		}
	}
	if (m_pInputGamePad == NULL)
	{// �Q�[���p�b�h����
		m_pInputGamePad = new CInputGamePad;
	}
	if (m_pInputGamePad != NULL)
	{// �Q�[���p�b�h����������Ă���
		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{// �Q�[���p�b�h�̏�����
			return -1;
		}
	}
	if (m_pSound == NULL)
	{// �T�E���h�̐���
		m_pSound = new CSound;
	}
	if (m_pSound != NULL)
	{// �T�E���h�̏�����
		if (FAILED(m_pSound->Init(hWnd)))
		{// �T�E���h�̏�����
			return -1;
		}
	}
	if (m_pDebugProc == NULL)
	{// �f�o�b�O�e�L�X�g�̐���
		m_pDebugProc = new CDebugProc;
	}
	if (m_pDebugProc != NULL)
	{// �f�o�b�O�e�L�X�g�̏�����
		if (FAILED(m_pDebugProc->Init()))
		{// �f�o�b�O�e�L�X�g�̏�����
			return -1;
		}
	}
	if (m_pTexture == NULL)
	{// �e�N�X�`���̐���
		m_pTexture = new CTexture;
	}
	if (m_pTexture != NULL)
	{// ���[�h
		m_pTexture->Load();
	}

	CManager::SetMode(m_mode);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �e��I������
	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g��
		if (m_pTitle != NULL)
		{// �^�C�g���̔j��
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;

	case MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// �Q�[���̔j��
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;

	case MODE_RESULT:	// ���U���g
		if (m_pResult != NULL)
		{// ���U���g�̔j��
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;
	}

	if (m_pDebugProc != NULL)
	{// �f�o�b�O�e�L�X�g�̔j��
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h�̔j��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputMouse != NULL)
	{// �}�E�X�̔j��
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pInputGamePad != NULL)
	{// �Q�[���p�b�h�̔j��
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}
	if (m_pSound != NULL)
	{// �T�E���h�̔j��
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pTexture != NULL)
	{// �j��
		m_pTexture->UnLoad();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̔j��
		m_pRenderer->Uninit(); // �I������
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	static bool bWireFrame = false;
#ifdef _DEBUG
	int nCntFPS = GetCntFPS();
	CDebugProc::print("%d\n",nCntFPS);
#endif
	CDebugProc::print("---------------------------\n");
	CDebugProc::print("���C���[�t���[���FF2\n");
	CDebugProc::print("����\��/��\���FF3\n");
	CDebugProc::print("����t���Ȃ����FF9\n");
	CDebugProc::print("---------------------------\n");

	if (m_pSound != NULL)
	{// �T�E���h�̍X�V
		m_pSound->Update();
	}
	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h�̍X�V
		m_pInputKeyboard->Update();
	}
	if (m_pInputMouse != NULL)
	{// �}�E�X�̍X�V
		m_pInputMouse->Update();
	}
	if (m_pInputGamePad != NULL)
	{// �Q�[���p�b�h�̍X�V
		m_pInputGamePad->Update();
	}
	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̍X�V
		m_pRenderer->Update();
		if (m_pInputKeyboard->GetTrigger(DIK_F2))
		{// ���C���[�t���[��
			bWireFrame = bWireFrame ? false : true;
			if (bWireFrame) { m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); }
			else { m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); }
		}
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3))
	{// ���C����\��
		CLine::SetDisp(CLine::GetDisp() ? false : true);// ���C��
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8))
	{// �f�[�^�̍X�V
		CObjCreate::SetScan();		// �t�@�C���ǂݍ���[ �I�u�W�F�N�g ]
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9))
	{// ���f���̔���f�[�^�̍X�V
		CSceneX::ScanCollision();
	}
	// �e��I������
	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g��
		if (m_pTitle != NULL)
		{// �^�C�g���̍X�V
			m_pTitle->Update();
		}
		break;

	case MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// �Q�[���̍X�V
			m_pGame->Update();
		}
		break;
	case MODE_CREATE:	// �N���G�C�g
		if (m_pCreate != NULL)
		{// �N���G�C�g�̍X�V
			m_pCreate->Update();
		}
		break;
	case MODE_RESULT:	// ���U���g
		if (m_pResult != NULL)
		{// ���U���g�̍X�V
			m_pResult->Update();
		}
		break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// �����_�����O�N���X�̕`��
		m_pRenderer->Draw();
	}

}
//=============================================================================
// ���[�h�ݒ�
//=============================================================================
HRESULT CManager::SetMode(CManager::MODE mode)
{
	// �e��I������
	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g��
		if (m_pTitle != NULL)
		{// �^�C�g���̔j��
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;

	case MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// �Q�[���̔j��
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;
	case MODE_CREATE:	// �N���G�C�g
		if (m_pCreate != NULL)
		{// �N���G�C�g�̔j��
			m_pCreate->Uninit();
			delete m_pCreate;
			m_pCreate = NULL;
		}
		break;

	case MODE_RESULT:	// ���U���g
		if (m_pResult != NULL)
		{// ���U���g�̔j��
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		if (m_pSound != NULL)
		{// �T�E���h�̒�~
			m_pSound->Stop();
		}
		break;
	}

	m_mode = mode;

	// �e�평��������
	switch (mode)
	{
	case MODE_TITLE:	// �^�C�g��
		if (m_pTitle == NULL)
		{// �^�C�g���̐���
			m_pTitle = CTitle::Create();
		}
		if (m_pSound != NULL)
		{// �T�E���h�J�n
			m_pSound->PlaySound(0,CSound::LABEL_BGM_TITLE);
		}
		break;

	case MODE_GAME:		// �Q�[��
		if (m_pGame == NULL)
		{// �Q�[���̐���
			m_pGame = CGame::Create();
		}
		if (m_pSound != NULL)
		{// �T�E���h�J�n
			m_pSound->PlaySound(0, CSound::LABEL_BGM_GAME);

		}
		break;
	case MODE_CREATE:	// �N���G�C�g
		if (m_pCreate == NULL)
		{// �N���G�C�g�̍X�V
			m_pCreate=CCreate::Create();
		}
		break;
	case MODE_RESULT:	// ���U���g
		if (m_pResult == NULL)
		{// ���U���g�̐���
			m_pResult = CResult::Create();
		}
		if (m_pSound != NULL)
		{// �T�E���h�J�n
			m_pSound->PlaySound(0, CSound::LABEL_BGM_RESULT);

		}
		break;
	}
	return S_OK;
}
//=============================================================================
// ���[�h�擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// �p�x�Z�[�u
//=============================================================================
void CManager::SaveAngle(float *pAngle)
{
	if (*pAngle > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		*pAngle += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// ��]�ʃZ�[�u
//=============================================================================
void CManager::SaveRot(D3DXVECTOR3 *rot)
{
	if (rot->x > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		rot->x -= D3DX_PI * 2.0f;
	}
	if (rot->x < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		rot->x += D3DX_PI * 2.0f;
	}
	if (rot->y > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		rot->y -= D3DX_PI * 2.0f;
	}
	if (rot->y < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		rot->y += D3DX_PI * 2.0f;
	}
	if (rot->z > D3DX_PI)
	{// ������3.14�𒴂���Ȃ�
		rot->z -= D3DX_PI * 2.0f;
	}
	if (rot->z < -D3DX_PI)
	{// ������-3.14�𒴂���Ȃ�
		rot->z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// int�^�̗ݏ�v�Z
//=============================================================================
int CManager::PowInt(const int nNum, const int nCnt)
{
	int nAnswer = 1;

	for (int nCntPow = 0; nCntPow < nCnt; nCntPow++)
	{
		nAnswer *= nNum;
	}

	return nAnswer;
}

//=============================================================================
// �����_���Ȋp�x�擾
//=============================================================================
float CManager::GetRandomPI(void)
{
	float fRot = 0;

	fRot = (rand() % 628 -314)/100.0f;

	return fRot;
}
//=============================================================================
// �����_�����擾[float]
//=============================================================================
float CManager::GetRandBetWeen(const float fMin, const float fMax)
{
	float fRange = 0;
	fRange = rand() % (int(fMax * 100) - int(fMin * 100) + 1) + (fMin * 100);
	fRange = fRange / 100.0f;
	return fRange;
}
//=============================================================================
// �����_�����擾[int]
//=============================================================================
int CManager::GetIntRandBetWeen(const int nMin, const int nMax)
{
	int nRange = 0;
	nRange = rand() % (nMax - nMin + 1) + nMin;
	return nRange;
}
//=============================================================================
// �ő�l�ŏ��l�����
//=============================================================================
void CManager::SaveMinMax(float *pSave, const float Min, const float Max)
{
	if (*pSave <= Min)
	{// �ŏ�
		*pSave = Min;
	}
	if (*pSave >= Max)
	{// �ő�
		*pSave = Max;
	}

 }

//=============================================================================
// �J�������[�h�擾
//=============================================================================
CCamera *CManager::GetCameraMode(void)
{
	CCamera *pCamera = NULL;
	// �e��I������
	switch (m_mode)
	{
	case MODE_TITLE:	// �^�C�g��
		pCamera = CTitle::GetCamera();

		break;
	case MODE_GAME:		// �Q�[��
		pCamera = CGame::GetCamera();
		break;
	case MODE_CREATE:		// �N���G�C�g
		pCamera = CCreate::GetCamera();
		break;
	case MODE_RESULT:	// ���U���g
		pCamera = CResult::GetCamera();
		break;
	}
	return pCamera;
}
//=============================================================================
// �����_�����O�N���X�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// �L�[�{�[�h�̎擾
//=============================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// �Q�[���p�b�h�̎擾
//=============================================================================
CInputGamePad *CManager::GetInputGamePad(void)
{
	return m_pInputGamePad;
}
//=============================================================================
// �}�E�X�̎擾
//=============================================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}
//=============================================================================
// �T�E���h�̎擾
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// �e�N�X�`���̎擾
//=============================================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

