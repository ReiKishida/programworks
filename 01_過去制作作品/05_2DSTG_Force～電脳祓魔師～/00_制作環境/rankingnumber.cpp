//=============================================================================
//
// �����L���O�����̏���[2D�|���S��] [rankingnumber.cpp]
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
#include "rankingnumber.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
#define	RANKING_WIDTH (50.0f)	//�����̕�
#define	RANKING_HEIGHT (70.0f)	//�����̍���
#define	RANKING_INTERVAL_WIDTH (85.0f)	//���Ԋu
#define	RANKING_INTERVAL_HEIGHT (120.0f)	//�����Ԋu

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CRankingScore::m_apNumber[MAX_RANKING][SCORE_NUMBER] = {};
int CRankingScore::m_nRanking[MAX_RANKING] = {40000 ,30000, 20000, 10000, 5000};
int CRankingScore::m_nRankingPlayer = 0;
bool CRankingScore::m_bPlayer = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CRankingScore::CRankingScore() :CScene(6)
{//�N���A�Ȓl����
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRankingScore::~CRankingScore()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CRankingScore *CRankingScore::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CRankingScore *pScore;
	pScore = new CRankingScore;

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
HRESULT CRankingScore::Init(D3DXVECTOR3 pos)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			//�����̐���
			m_apNumber[nCntRank][nCntNumber] = new CNumber;
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{

				//����������
				m_apNumber[nCntRank][nCntNumber]->SetNumberWidth(RANKING_WIDTH);
				m_apNumber[nCntRank][nCntNumber]->SetNumberHeight(RANKING_HEIGHT);
				m_apNumber[nCntRank][nCntNumber]->Init(D3DXVECTOR3(pos.x + (RANKING_INTERVAL_WIDTH * nCntNumber), pos.y + (RANKING_INTERVAL_HEIGHT * nCntRank), pos.z));


				//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_RANKINGNUNBER);
			}
		}
	}


	SetRankingNum(CManager::GetScoreNum());


	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CRankingScore::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{
				//2D�V�[���̏I������
				m_apNumber[nCntRank][nCntNumber]->Uninit();
				m_apNumber[nCntRank][nCntNumber] = NULL;
			}
		}
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CRankingScore::Update(void)
{
	SetRankingScore();

}

//=============================================================================
//�`�揈��
//=============================================================================
void CRankingScore::Draw(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_NUMBER; nCntNumber++)
		{
			if (m_apNumber[nCntRank][nCntNumber] != NULL)
			{
				m_apNumber[nCntRank][nCntNumber]->Draw();
			}
		}
	}
}


//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CRankingScore::SetRankingScore(void)
{

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{


		m_apNumber[nCntRank][0]->SetNumber(m_nRanking[nCntRank] / 100000000);
		m_apNumber[nCntRank][1]->SetNumber(m_nRanking[nCntRank] % 10000000 / 1000000);
		m_apNumber[nCntRank][2]->SetNumber(m_nRanking[nCntRank] % 1000000 / 100000);
		m_apNumber[nCntRank][3]->SetNumber(m_nRanking[nCntRank] % 100000 / 10000);
		m_apNumber[nCntRank][4]->SetNumber(m_nRanking[nCntRank] % 10000 / 1000);
		m_apNumber[nCntRank][5]->SetNumber(m_nRanking[nCntRank] % 1000 / 100);
		m_apNumber[nCntRank][6]->SetNumber(m_nRanking[nCntRank] % 100 / 10);
		m_apNumber[nCntRank][7]->SetNumber(m_nRanking[nCntRank] % 10);
	}
}

//=============================================================================
//�����L���O�̐ݒ�
//=============================================================================
void CRankingScore::SetRankingNum(int nValue)
{
	int nRank;
	int aEmptyData[6];
	aEmptyData[5] = nValue;

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = m_nRanking[nCntNum];

	}

	for (int nCntNum = 1; nCntNum < 6; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nRank = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nRank;
			}
		}
	}


	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}
	m_bPlayer = true;

	m_nRankingPlayer = nValue;
}

