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
#include "damagescore.h"
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

#define	SCORE_RESULT_WIDTH (60.0f)
#define	SCORE_RESULT_HEIGHT (80.0f)
#define	SCORE_RESULT_INTERVAL (80.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CNumber *CDamageScore::m_apNumber[DAMAGE_SCORE_NUMBER] = {};
int CDamageScore::m_nScore = 0;
int CDamageScore::m_nDestScore = 0;
int CDamageScore::m_nNowScore = 0;

D3DXVECTOR3 CDamageScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CDamageScore::CDamageScore(int nPriority, OBJTYPE type) :CScene(nPriority, type)
{//�N���A�Ȓl����
	m_nScore = 0;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CDamageScore::~CDamageScore()
{


}



//=============================================================================
//�X�R�A�̐���
//=============================================================================
CDamageScore *CDamageScore::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CDamageScore *pScore;
	pScore = new CDamageScore;

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
HRESULT CDamageScore::Init()
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
	{



		//�����̐���
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + (SCORE_GAME_INTERVAL * nCntNumber), m_pos.y, m_pos.z), SCORE_GAME_WIDTH, SCORE_GAME_WIDTH);
	}


	m_nScore = 0;

	//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_SCORE);



	int nEmpty;	//���int�^
	nEmpty = (int)CManager::GetDamage();

	SetScoreNum(nEmpty);


	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CDamageScore::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
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
void CDamageScore::Update(void)
{

	AddScoreFix();
}

//�`�揈��
//=============================================================================
void CDamageScore::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < DAMAGE_SCORE_NUMBER; nCntNumber++)
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
void CDamageScore::SetScore(int nScore)
{
	m_nScore = nScore;

}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CDamageScore::GetScore(void)
{
	return m_nScore;
}

void CDamageScore::AddScoreFix()
{


}

void CDamageScore::SetScoreNum(int nValue)
{
	//�X�R�A�̉��Z
	m_nScore = nValue;

	m_apNumber[0]->SetNumber(m_nScore / 10000);
	m_apNumber[1]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[2]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[3]->SetNumber(m_nScore % 10 / 1);
}


