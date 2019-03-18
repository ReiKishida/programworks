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
#include "finalscore.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	SCORE_GAME_WIDTH (30.0f)
#define	SCORE_GAME_HEIGHT (40.0f)
#define	SCORE_GAME_INTERVAL (40.0f)

#define	SCORE_RESULT_WIDTH (50.0f)
#define	SCORE_RESULT_HEIGHT (70.0f)
#define	SCORE_RESULT_INTERVAL (70.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CFinalScore::m_apNumber[SCORE_NUMBER] = {};
int CFinalScore::m_nScore = 0;
int CFinalScore::m_nDestScore = 0;
int CFinalScore::m_nNowScore = 0;

D3DXVECTOR3 CFinalScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CFinalScore::CFinalScore(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{//�N���A�Ȓl����
	m_nScore = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CFinalScore::~CFinalScore()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CFinalScore *CFinalScore::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CFinalScore *pScore;
	pScore = new CFinalScore;

	if (pScore != NULL)
	{
		//�X�R�A�̏���������
		m_pos = pos;
		pScore->m_nDestScore = 0;
		pScore->Init();
	}

	return pScore;



}


//=============================================================================
//����������
//=============================================================================
HRESULT CFinalScore::Init()
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{


		//�����̐���
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_RESULT_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_RESULT_WIDTH, SCORE_RESULT_HEIGHT);

		m_nScore = 0;


	}

	//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_SCORE);
	int nScore = CManager::GetScore();
	int nScorebonus = CManager::GetScoreBonus();
	int nTimerbonus = CManager::GetTimeBonus();
	int nCombobonus = CManager::GetComboGaugeMaxBonus();
	int nDamageBonus = CManager::GetDamageBonus();

	int nFinalScore = nScore +  nScorebonus + nTimerbonus + nCombobonus + nDamageBonus;
	CManager::SetFinalScore(nFinalScore);
	SetScoreNum(nFinalScore);


	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CFinalScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
	{
		if (m_apNumber[nCntNumber] != NULL)
		{
			//�����̏I������
			m_apNumber[nCntNumber]->Uninit();
			delete m_apNumber[nCntNumber];
			m_apNumber[nCntNumber] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CFinalScore::Update(void)
{
	//CManager::SetHighScore(m_nScore);
	AddScoreFix();
}

//�`�揈��
//=============================================================================
void CFinalScore::Draw(void)
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
void CFinalScore::SetScore(int nScore)
{
	m_nScore = nScore;



}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CFinalScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CFinalScore::AddScore(int nValue)
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

void CFinalScore::AddScoreFix()
{


}

void CFinalScore::SetScoreNum(int nValue)
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

