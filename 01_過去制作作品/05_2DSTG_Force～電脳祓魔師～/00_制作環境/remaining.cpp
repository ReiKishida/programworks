//=============================================================================
//
// �c�@�̏���[2D�|���S��] [remaining.cpp]
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
#include "remaining.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define REMAINING_WIDTH (50)	//�|���S���̕�
#define REMAINING_HEIGHT (50)	//�|���S����Y�̍���
#define REMAINING_NUM (2)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		CRemaining::m_pTexture = NULL;
CScene2D *CRemaining::m_apRemaining[REMAINING_NUMBER] = {};
int CRemaining::m_nRemaining = REMAINING_NUM;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CRemaining::CRemaining() :CScene(7)
{//�N���A�Ȓl����
	m_nRemaining = REMAINING_NUM;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRemaining::~CRemaining()
{


}



//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CRemaining::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CRemaining::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//=============================================================================
//�X�R�A�̐���
//=============================================================================
CRemaining *CRemaining::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CRemaining *pScore;
	pScore = new CRemaining;

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
HRESULT CRemaining::Init(D3DXVECTOR3 pos)
{
	for (int nCntRemaining = 0; nCntRemaining < REMAINING_NUMBER; nCntRemaining++)
	{
		//�����̐���
		m_apRemaining[nCntRemaining] = new CScene2D(6);
		if (m_apRemaining[nCntRemaining] != NULL)
		{


			//����������
			m_apRemaining[nCntRemaining]->SetScene2DWidth(REMAINING_WIDTH);
			m_apRemaining[nCntRemaining]->SetScene2DHeight(REMAINING_HEIGHT);
			//�e�N�X�`���̊��蓖��
			m_apRemaining[nCntRemaining]->BindTexture(m_pTexture);

			m_apRemaining[nCntRemaining]->Init(D3DXVECTOR3(pos.x + (80.0f * nCntRemaining), pos.y, pos.z));
			m_apRemaining[nCntRemaining]->SetTexture(0.25f, 0.33f);

			m_nRemaining = REMAINING_NUM;

			//�X�R�A�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_REMAINING);
		}
	}

	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CRemaining::Uninit(void)
{
	for (int nCntRemaining = 0; nCntRemaining < REMAINING_NUMBER; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			//2D�V�[���̏I������
			m_apRemaining[nCntRemaining]->Uninit();
			m_apRemaining[nCntRemaining] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CRemaining::Update(void)
{
	D3DXVECTOR3 pos;
	for (int nCntRemaining = 0; nCntRemaining < m_nRemaining; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			pos = m_apRemaining[nCntRemaining]->GetPosScene2D();

			m_apRemaining[nCntRemaining]->SetPosScene2D(pos);
		}
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CRemaining::Draw(void)
{
	for (int nCntRemaining = 0; nCntRemaining < m_nRemaining; nCntRemaining++)
	{
		if (m_apRemaining[nCntRemaining] != NULL)
		{
			m_apRemaining[nCntRemaining]->Draw();
		}
	}
}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CRemaining::SetScore(int nScore)
{
	m_nRemaining = nScore;



}

//=============================================================================
//�X�R�A�̎擾
//=============================================================================
int CRemaining::GetRemaining(void)
{
	return m_nRemaining;
}

//=============================================================================
//�X�R�A�̉��Z
//=============================================================================
void CRemaining::DecreseRemaining(int nValue)
{
	//�X�R�A�̉��Z
	m_nRemaining += nValue;
	if (m_nRemaining >= 0)
	{
		if (m_apRemaining[m_nRemaining] != NULL)
		{
			m_apRemaining[m_nRemaining]->Uninit();
		}
	}

}

