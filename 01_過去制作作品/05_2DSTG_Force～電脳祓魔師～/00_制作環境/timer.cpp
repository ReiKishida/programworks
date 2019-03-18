//=============================================================================
//
// �^�C�}�[�̏���[2D�|���S��] [timer.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "number.h"
#include "timer.h"
#include "fade.h"
#include "game.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CTimer::m_apNumber[TIMER_NUMBER] = {};	//�����̃|�C���^
int CTimer::m_nTimer = 0;	//�^�C�}�[
int CTimer::m_nMinutes = 0;	//��
int CTimer::m_nTenSeconds = 0;	//10�b��
int CTimer::m_nOneSeconds = 0;	//1�b��
int CTimer::m_nTenComma = 0;	//�R���}�b1�b��
int CTimer::m_nOneComma = 0;	//�R���}�b10�b��
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MINUTES_WIDTH (30.0f)
#define MINUTES_HEIGHT (50.0f)

#define SECONDS_WIDTH (20.0f)
#define SECONDS_HEIGHT (40.0f)

#define COMMA_WIDTH (15.0f)
#define COMMA_HEIGHT (25.0f)

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CTimer::CTimer() :CScene(7)
{//�N���A�Ȓl����
	m_nMinutes = 0;
	m_nTenSeconds = 0;
	m_nOneSeconds = 0;
	m_nTenComma = 0;
	m_nOneComma = 0;
	m_nCntFrame = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{
	bUse = true;

}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CTimer *pScore;
	pScore = new CTimer;

	if (pScore != NULL)
	{
		//�X�R�A�̏���������
		pScore->Init(pos);
	}

	return pScore;



}


//=============================================================================
//����������
//=============================================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos)
{
	//float fSpace;
	m_nCntFrame = 0;
	bUse = true;

	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		//�����̐���
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{
			switch (nCntNumber)
			{
			case TIMERTYPE_MINUTE:
				//��
				m_apNumber[0]->SetNumberWidth(MINUTES_WIDTH);
				m_apNumber[0]->SetNumberHeight(MINUTES_HEIGHT);
				m_apNumber[0]->Init(D3DXVECTOR3(pos.x, pos.y, pos.z));
				break;
			case TIMERTYPE_TENSECONDS:
				//10�b��
				m_apNumber[1]->SetNumberWidth(SECONDS_WIDTH);
				m_apNumber[1]->SetNumberHeight(SECONDS_HEIGHT);
				m_apNumber[1]->Init(D3DXVECTOR3(pos.x + 60.0f, pos.y + 5.0f, pos.z));
				break;
			case TIMERTYPE_ONESECONDS:
				//1�b��
				m_apNumber[2]->SetNumberWidth(SECONDS_WIDTH);
				m_apNumber[2]->SetNumberHeight(SECONDS_HEIGHT);
				m_apNumber[2]->Init(D3DXVECTOR3(pos.x + 90.0f, pos.y + 5.0f, pos.z));
				break;
			case TIMERTYPE_TENCOMMASECONDS:
				//10�R���}�b
				m_apNumber[3]->SetNumberWidth(COMMA_WIDTH);
				m_apNumber[3]->SetNumberHeight(COMMA_HEIGHT);
				m_apNumber[3]->Init(D3DXVECTOR3(pos.x + 140.0f, pos.y + 15.0f, pos.z));
				break;
			case TIMERTYPE_ONECOMMASECONDS:
				//1�R���}�b
				m_apNumber[4]->SetNumberWidth(COMMA_WIDTH);
				m_apNumber[4]->SetNumberHeight(COMMA_HEIGHT);
				m_apNumber[4]->Init(D3DXVECTOR3(pos.x + 160.0f, pos.y + 15.0f, pos.z));
				break;
			}




			//====================================
			//�^�C�}�[�̐ݒ�
			//====================================
			SetTimer(1, 0, 3, 0, 0);

			//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}

	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//2D�V�[���̏I������
			m_apNumber[nCntNumber]->Uninit();
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CTimer::Update(void)
{
	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();
	//�J�E���g�p�t���[���̉��Z
	m_nCntFrame++;

	if (bUse == true)
	{
		m_nOneComma--;
	}
	//�P�O��R���}�b������
	if (m_nCntFrame % 6 == 0)
	{//6�t���[���ōX�V
			m_nTenComma--;
	}

		if (m_nOneComma < 0)
		{//�R���}�b

			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0 && m_nTenComma <= 0 && m_nOneComma < 0)
			{
				m_nOneComma = 0;
				bUse = false;
				if (pFade == CFade::FADE_NONE)
				{
					CGame::SetGameState(CGame::GAMESTATE_FAILED);
					//CFade::SetFade(CManager::MODE_RESULT);

				}
			}
			else
			{
				m_nOneComma = 9;

			}
		}
		//1��R���}�b������
		if (m_nTenComma < 0)
		{//�R���}�b
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0 && m_nTenComma <= 0)
			{
				m_nTenComma = 0;
				m_nOneComma = 0;
			}
			else
			{
				m_nTenComma = 9;
				m_nOneSeconds--;

			}
		}

		if (m_nOneSeconds < 0)
		{//�b
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0 && m_nOneSeconds <= 0)
			{//1����10�b���؂��Ă���Ƃ�
				m_nOneSeconds = 0;
			}
			else
			{
				m_nOneSeconds = 9;
				m_nTenSeconds--;

			}
		}
		if (m_nTenSeconds < 0)
		{//�b
			if (m_nMinutes <= 0 && m_nTenSeconds <= 0)
			{//1����؂��Ă���Ƃ�
				m_nTenSeconds = 0;
			}
			else
			{
				m_nTenSeconds = 5;
				m_nMinutes--;

			}

		}
		if (m_nMinutes >= 1)
		{//�c��1��
			m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		}
		if (m_nMinutes <= 0)
		{//�c��1��
			m_nMinutes = 0;

			m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
			m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

		}
		if (m_nMinutes <= 0 && m_nTenSeconds < 3)
		{//�c��30�b
			m_nCntColor++;
			m_nCntColor = m_nCntColor % 18;
			if (m_nCntColor < 9)
			{
				m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			if (m_nCntColor >= 9)
			{
				m_apNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apNumber[4]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

	//=======================================================
	//�ԍ��̃e�N�X�`���ݒ�
	//=======================================================
	//��
	m_apNumber[0]->SetNumber(m_nMinutes);

	//�b
	m_apNumber[1]->SetNumber(m_nTenSeconds);
	m_apNumber[2]->SetNumber(m_nOneSeconds);

	//�R���}�b
	m_apNumber[3]->SetNumber(m_nTenComma);
	m_apNumber[4]->SetNumber(m_nOneComma);

}

//=============================================================================
//�`�揈��
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CTimer::SetTimer(int nMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma)
{
	m_nMinutes = nMinutes;
	m_nTenSeconds = nTenSeconds;
	m_nOneSeconds = nOneSeconds;
	m_nTenComma = nTenComma;
	m_nOneComma = nOneComma;

}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CTimer::GetTimer(void)
{
	int nMinutes = m_nMinutes;
	int nTenSeconds = m_nTenSeconds;
	int nOneSeconds = m_nOneSeconds;
	int nTenComma = m_nTenComma;
	int nOneComma = m_nOneComma;

	nMinutes = nMinutes * 10000;
	nTenSeconds = nTenSeconds * 1000;
	nOneSeconds = nOneSeconds * 100;
	nTenComma = nTenComma * 10;
	nOneComma = nOneComma;

	m_nTimer = nMinutes + nTenSeconds + nOneSeconds + nTenComma + nOneComma;

	return m_nTimer;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CTimer::AddSeconds(int nSeconds)
{
	int nOneSecondsAmari;
	int nTenSecondsAmari;

	//�X�R�A�̉��Z
	m_nOneSeconds += nSeconds;


	if (m_nOneSeconds >= 10)
	{
		m_nTenSeconds++;
		nOneSecondsAmari = m_nOneSeconds % 10;
		m_nOneSeconds = 0;
		m_nOneSeconds = m_nOneSeconds + nOneSecondsAmari;
	}



	if (m_nTenSeconds >= 6)
	{
		m_nMinutes++;
		nTenSecondsAmari = m_nTenSeconds % 6;

		m_nTenSeconds = 0;

		m_nTenSeconds = m_nTenSeconds + nTenSecondsAmari;

	}



}

