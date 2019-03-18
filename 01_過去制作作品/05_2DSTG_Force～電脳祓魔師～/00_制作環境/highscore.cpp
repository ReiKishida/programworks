//=============================================================================
//
// �X�R�A�̏���[2D�|���S��] [score.cpp]
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
#include "highscore.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define HIGHSCORE (100)//�n�C�X�R�A
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CHighScore::m_apNumber[HIGHSCORE_NUMBER] = {};
int CHighScore::m_nHighScore = 0;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CHighScore::CHighScore() :CScene(6)
{//�N���A�Ȓl����
	m_nHighScore = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CHighScore::~CHighScore()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CHighScore *CHighScore::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CHighScore *pScore;
	pScore = new CHighScore;

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
HRESULT CHighScore::Init(D3DXVECTOR3 pos)
{

	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		//�����̐���
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{


			//����������
			m_apNumber[nCntNumber]->SetNumberWidth(15.0f);
			m_apNumber[nCntNumber]->SetNumberHeight(25.0f);
			m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (25.0f * nCntNumber), pos.y, pos.z));
			m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}
	SetHighScore(CManager::GetHighScoreNum());

	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CHighScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
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
void CHighScore::Update(void)
{
	SetScore(CManager::GetHighScoreNum());

	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CHighScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < HIGHSCORE_NUMBER; nCntNumber++)
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
void CHighScore::SetScore(int nScore)
{
	m_nHighScore = nScore;

}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CHighScore::GetScore(void)
{
	return m_nHighScore;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CHighScore::AddScore(int nValue)
{
	//�X�R�A�̉��Z
	m_nHighScore += nValue;

	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[1]->SetNumber(m_nHighScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nHighScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nHighScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nHighScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nHighScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nHighScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nHighScore % 10);
}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CHighScore::SetHighScore(int nValue)
{
	//�X�R�A�̉��Z
	m_nHighScore = nValue;

	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[0]->SetNumber(m_nHighScore / 100000000);
	m_apNumber[1]->SetNumber(m_nHighScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nHighScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nHighScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nHighScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nHighScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nHighScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nHighScore % 10);
}

