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
#include "tutoriallogo.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���

#define RANKINGLOGO_WIDTH (780.0f)//�`���[�g���A���\���̕�
#define RANKINGLOGO_HEIGHT (230.0f)//�`���[�g���A���\���̍���

#define END_RANKINGLOGO_WIDTH (100.0f)//�`���[�g���A���\���̕�
#define END_RANKINGLOGO_HEIGHT (120.0f)//�`���[�g���A���\���̍���

#define TEX_X (1.0f)
#define TEX_Y (1.0f)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorialLogo::m_apTexture[MAX_TUTORIAL_LOGO_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CTutorialLogo::m_apScene2D[MAX_TUTORIAL_LOGO] = {};	//�|���S���̃|�C���^


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorialLogo::CTutorialLogo()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorialLogo::~CTutorialLogo()
{

}

//=============================================================================
// �����L���O�p�|���S���̃e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CTutorialLogo::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/endtutorial.png", &m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// �����L���O�p�|���S���̃e�N�X�`���̔j��
//=============================================================================
void CTutorialLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TUTORIAL_LOGO_TEXTURE; nCntTex++)
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
CTutorialLogo * CTutorialLogo::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CTutorialLogo *pRankingLogo;
	pRankingLogo = new CTutorialLogo;

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
HRESULT CTutorialLogo::Init(D3DXVECTOR3 pos)
{
	for (int nCntRankingLogo = 0; nCntRankingLogo < MAX_TUTORIAL_LOGO; nCntRankingLogo++)
	{
		//�����̐���
		m_apScene2D[nCntRankingLogo] = new CScene2D(7);
		if (m_apScene2D[nCntRankingLogo] != NULL)
		{
			switch (nCntRankingLogo)
			{
			case 0:
				//����������
				m_apScene2D[nCntRankingLogo]->SetScene2DWidth(RANKINGLOGO_WIDTH);
				m_apScene2D[nCntRankingLogo]->SetScene2DHeight(RANKINGLOGO_HEIGHT);
				m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(pos.x, pos.y, pos.z));
				m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);
				break;

			case 1:
				//����������
				m_apScene2D[nCntRankingLogo]->SetScene2DWidth(END_RANKINGLOGO_WIDTH);
				m_apScene2D[nCntRankingLogo]->SetScene2DHeight(END_RANKINGLOGO_HEIGHT);
				m_apScene2D[nCntRankingLogo]->Init(D3DXVECTOR3(SCREEN_WIDTH /2 + 300.0f, SCREEN_HEIGHT / 2 - 250.0f, pos.z));
				m_apScene2D[nCntRankingLogo]->BindTexture(m_apTexture[nCntRankingLogo]);
				break;
			}


			//�t�H�[�X�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_TUTORIALLOGO);

		}
	}

	return S_OK;
}

//=============================================================================
// �����L���O�p�|���S���̏I��
//=============================================================================
void CTutorialLogo::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < MAX_TUTORIAL_LOGO; nCntNumber++)
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
void CTutorialLogo::Update(void)
{
	//�t�H�[�X�̈ʒu
	D3DXVECTOR3 posRankingLogo[MAX_TUTORIAL_LOGO];


	for (int nCntForce = 0; nCntForce < MAX_TUTORIAL_LOGO; nCntForce++)
	{
		posRankingLogo[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}
	for (int nCntForce = 0; nCntForce < MAX_TUTORIAL_LOGO; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posRankingLogo[nCntForce]);
	}
}

//=============================================================================
// �����L���O�p�|���S���̕`��
//=============================================================================
void CTutorialLogo::Draw(void)
{
}

