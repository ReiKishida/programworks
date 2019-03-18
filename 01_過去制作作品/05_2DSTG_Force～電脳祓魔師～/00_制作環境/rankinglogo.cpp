//=============================================================================
//
// �����L���O�p�|���S������ [rankinglogo.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "number.h"
#include "force.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "sound.h"
#include "rankinglogo.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define RANKINGLOGO_RADIUS (90.0f)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRankingLogo::m_apTexture[MAX_RANKING_LOGO_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CRankingLogo::m_apScene2D[MAX_RANKING_LOGO] = {};	//�|���S���̃|�C���^


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRankingLogo::CRankingLogo()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}

//=============================================================================
// �����L���O�p�|���S���̃e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1st.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2nd.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3rd.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4th.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5th.png", &m_apTexture[4]);

	return S_OK;
}

//=============================================================================
// �����L���O�p�|���S���̃e�N�X�`���̔j��
//=============================================================================
void CRankingLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RANKING_LOGO_TEXTURE; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
// �����L���O�p�|���S���̐���
//=============================================================================
CRankingLogo * CRankingLogo::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CRankingLogo *pRankingLogo;
	pRankingLogo = new CRankingLogo;

	if (pRankingLogo != NULL)
	{
		//�X�R�A�̏���������
		pRankingLogo->Init(pos);
	}

	return pRankingLogo;
}

//=============================================================================
// �����L���O�p�|���S���̏�����
//=============================================================================
HRESULT CRankingLogo::Init(D3DXVECTOR3 pos)
{
	for (int nCntRankingLogo = 0; nCntRankingLogo < MAX_RANKING_LOGO; nCntRankingLogo++)
	{
		//�����̐���
		m_apScene2D[nCntRankingLogo] = new CScene2D(7);
		if (m_apScene2D[nCntRankingLogo] != NULL)
		{
			//����������
			m_apScene2D[nCntRankingLogo]->SetScene2DWidth(RANKINGLOGO_RADIUS);
			m_apScene2D[nCntRankingLogo]->SetScene2DHeight(RANKINGLOGO_RADIUS);
			m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(pos.x , pos.y + (120.0f * nCntRankingLogo), pos.z));
			m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);

			//�t�H�[�X�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_RANKINGLOOG);

		}
	}

	return S_OK;
}

//=============================================================================
// �����L���O�p�|���S���̏I��
//=============================================================================
void CRankingLogo::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_RANKING_LOGO; nCntNumber++)
	{
		if (m_apScene2D[nCntNumber] != NULL)
		{
			//2D�V�[���̏I������
			m_apScene2D[nCntNumber]->Uninit();
			m_apScene2D[nCntNumber] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �����L���O�p�|���S���̍X�V
//=============================================================================
void CRankingLogo::Update(void)
{
	//�t�H�[�X�̈ʒu
	D3DXVECTOR3 posRankingLogo[MAX_RANKING_LOGO];


	for (int nCntForce = 0; nCntForce < MAX_RANKING_LOGO; nCntForce++)
	{
		posRankingLogo[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}
	for (int nCntForce = 0; nCntForce < MAX_RANKING_LOGO; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posRankingLogo[nCntForce]);
	}
}

//=============================================================================
// �����L���O�p�|���S���̕`��
//=============================================================================
void CRankingLogo::Draw(void)
{
}

