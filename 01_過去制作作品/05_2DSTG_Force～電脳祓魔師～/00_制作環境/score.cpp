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
#include "score.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	SCORE_GAME_WIDTH (15.0f)
#define	SCORE_GAME_HEIGHT (25.0f)
#define	SCORE_GAME_INTERVAL (25.0f)

#define	SCORE_RESULT_WIDTH (70.0f)
#define	SCORE_RESULT_HEIGHT (90.0f)
#define	SCORE_RESULT_INTERVAL (90.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CScore::m_apNumber[SCORE_NUMBER] = {};
int CScore::m_nScore = 0;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScore::CScore() :CScene(6)
{//�N���A�Ȓl����
	m_nScore = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScore::~CScore()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, SCORETYPE type)
{
	//�X�R�A�̐���
	CScore *pScore;
	pScore = new CScore;

	if (pScore != NULL)
	{
		//�X�R�A�̏���������
		pScore->m_type = type;
		pScore->Init(pos);
	}

	return pScore;



}


//=============================================================================
//����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{
		//�����̐���
		m_apNumber[nCntNumber] = new CNumber;
		if (m_apNumber[nCntNumber] != NULL)
		{
			switch (m_type)
			{
			case SCORETYPE_GAME:
				//����������
				m_apNumber[nCntNumber]->SetNumberWidth(SCORE_GAME_WIDTH);
				m_apNumber[nCntNumber]->SetNumberHeight(SCORE_GAME_HEIGHT);
				m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (SCORE_GAME_INTERVAL * nCntNumber), pos.y, pos.z));
				break;

			case SCORETYPE_RESULT:
				//����������
				m_apNumber[nCntNumber]->SetNumberWidth(SCORE_RESULT_WIDTH);
				m_apNumber[nCntNumber]->SetNumberHeight(SCORE_RESULT_HEIGHT);
				m_apNumber[nCntNumber]->Init(D3DXVECTOR3(pos.x + (SCORE_RESULT_INTERVAL * nCntNumber), pos.y, pos.z));
				break;
			}


			m_nScore = 0;

			//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_SCORE);
		}
	}


	//���[�h�����U���g�̂Ƃ��X�R�A��
	if (m_type == SCORETYPE_RESULT)
	{
		SetScoreNum(CManager::GetScoreNum());
	}

	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
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
void CScore::Update(void)
{
	CManager::SetHighScore(m_nScore);
}

//�`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
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
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;



}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	//�X�R�A�̉��Z
	m_nScore += nValue;

	m_apNumber[0]->SetNumber(m_nScore / 100000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10);
}

void CScore::SetScoreNum(int nValue)
{
	//�X�R�A�̉��Z
	m_nScore = nValue;

	m_apNumber[0]->SetNumber(m_nScore / 100000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10);
}

