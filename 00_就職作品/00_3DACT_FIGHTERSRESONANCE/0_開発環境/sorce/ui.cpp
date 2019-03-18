//=============================================================================
//
// UI�̏���[2D�|���S��] [ui.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "ui.h"
#include "timer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_WIDTH (80.0f)				//�X�R�A�|���S���̕�
#define SCORE_HEIGHT (40.0f)				//�X�R�A�|���S����Y�̍���


#define TIME_WIDTH (70.0f)			//�^�C���|���S���̕�
#define TIME_HEIGHT (30.0f)			//�^�C���|���S����Y�̍���


#define COLON_WIDTH (11.5f)			//�R�����|���S���̕�
#define COLON_HEIGHT (35.0f)			//�R�����|���S����Y�̍���

#define DOT_WIDTH (9.0f)			//����|���S���̕�
#define DOT_HEIGHT (9.0f)			//����|���S����Y�̍���

#define CONTROLL_WIDTH (350.0f)				//����|���S���̕�
#define CONTROLL_HEIGHT (200.0f)				//����|���S����Y�̍���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUi::m_pTexture[MAX_TEXTURE_UI] = {}; //���L�e�N�X�`���̃|�C���^
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CUi::CUi(int nPriority, OBJTYPE type) : CScene2D(nPriority, type)
{//�N���A�Ȓl����

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CUi::~CUi()
{


}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CUi::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/score.png", &m_pTexture[0]);		//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/time.png", &m_pTexture[1]);			//�^�C���̕���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/colon.png", &m_pTexture[2]);		//�R����
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/dot.png", &m_pTexture[3]);			//�h�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/mastertuto.png", &m_pTexture[4]);		//�h�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/tuto_key.png", &m_pTexture[5]);			//�h�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/gaugege.png", &m_pTexture[6]);			//�h�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/endtuto.png", &m_pTexture[7]);			//�h�b�g


	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CUi::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_UI; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
//�w�i�̐���
//=============================================================================
CUi *CUi::Create(CUi::UIMODE mode)
{
	//�w�i�̐���
	CUi *pUi;
	pUi = new CUi;

	if (pUi != NULL)
	{
		pUi->m_mode = mode;

		//�w�i�̏���������
		pUi->Init();
	}

	return pUi;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CUi::Init(void)
{
	//�F���ݒ�
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;

	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			switch (nCntScene2D)
			{
			case UITYPE_SCORE:// �^�C��
							  //2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(880.0f, 30.0f, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SCORE]);
				break;
			case UITYPE_TIME:// �^�C��
							 //2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(980.0f, 120.0f, 0.0f), TIME_WIDTH, TIME_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_TIME]);
				break;
			case UITYPE_COLON:// �R����
							  //2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1125.0f, 125.0f, 0.0f), COLON_WIDTH, COLON_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_COLON]);
				break;

			case UITYPE_DOT://�h�b�g
							//2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1205.0f, 140.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_DOT]);
				break;

			case UITYPE_CONTROLL_ARCADE://����
				//2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(210.0f, 600.0f, 0.0f), CONTROLL_WIDTH, CONTROLL_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_CONTROLL_ARCADE]);
				break;
			case UITYPE_CONTROLL_KEYBOARD://����
										//2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(210.0f, 350.0f, 0.0f), CONTROLL_WIDTH, CONTROLL_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_CONTROLL_KEYBOARD]);
				break;
			case UITYPE_SKILL://����
				//2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(300.0f, 140.0f, 0.0f), 450.0f, 60.0f);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SKILL]);
				break;
			case UITYPE_END://����
				//2D�|���S���̐���
				m_pScene2D_Tuto[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1150.0f, 650.0f, 0.0f), 150.0f, 100.0f);
				//�e�N�X�`���̊��蓖��
				m_pScene2D_Tuto[nCntScene2D]->BindTexture(m_pTexture[UITYPE_END]);
				break;
			}
			m_pScene2D_Tuto[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
		}

		break;
	case CUi::UIMODE_GAME:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			switch (nCntScene2D)
			{
			case UITYPE_SCORE:// �^�C��
							  //2D�|���S���̐���
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(880.0f, 30.0f, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_SCORE]);
				break;
			case UITYPE_TIME:// �^�C��
							 //2D�|���S���̐���
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(980.0f, 120.0f, 0.0f), TIME_WIDTH, TIME_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_TIME]);
				break;
			case UITYPE_COLON:// �R����
							  //2D�|���S���̐���
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1125.0f, 125.0f, 0.0f), COLON_WIDTH, COLON_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_COLON]);
				break;

			case UITYPE_DOT://�h�b�g
							//2D�|���S���̐���
				m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(1205.0f, 140.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITYPE_DOT]);
				break;

			}
			m_pScene2D[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
		}

		break;
	}


	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CUi::Uninit(void)
{
	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			if (m_pScene2D_Tuto[nCntScene2D] != NULL)
			{//�V�[��2D���󂶂�Ȃ��Ƃ�
			 //2D�V�[���̏I������
				m_pScene2D_Tuto[nCntScene2D]->Uninit();
				m_pScene2D_Tuto[nCntScene2D] = NULL;
			}
		}
		break;
	case CUi::UIMODE_GAME:

		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			if (m_pScene2D[nCntScene2D] != NULL)
			{//�V�[��2D���󂶂�Ȃ��Ƃ�
			 //2D�V�[���̏I������
				m_pScene2D[nCntScene2D]->Uninit();
				m_pScene2D[nCntScene2D] = NULL;
			}
		}
		break;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CUi::Update(void)
{
	m_nCntColor++;
	m_nCntColor = m_nCntColor % 140;
	if (m_nCntColor < 70)
	{
		m_Color.a -= 0.01f;
	}
	if (m_nCntColor > 70)
	{
		m_Color.a += 0.01f;
	}
	switch (m_mode)
	{
	case CUi::UIMODE_TUTORIAL:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI_TUTORIAL; nCntScene2D++)
		{
			if (m_pScene2D_Tuto[nCntScene2D] != NULL)
			{//�V�[��2D���󂶂�Ȃ��Ƃ�
				D3DXVECTOR3 posBg = m_pScene2D_Tuto[nCntScene2D]->GetPosPolygon();	//�v���C���[�̈ʒu���




				//�F���̐ݒ�
				m_pScene2D_Tuto[UITYPE_END]->SetColor(m_Color);
				m_pScene2D_Tuto[UITYPE_SKILL]->SetColor(m_Color);

				//�ʒu���̐ݒ�
				m_pScene2D_Tuto[nCntScene2D]->SetPosPolygon(posBg);
			}
		}
		break;
	case CUi::UIMODE_GAME:
		for (int nCntScene2D = 0; nCntScene2D < MAX_UI; nCntScene2D++)
		{
			if (m_pScene2D[nCntScene2D] != NULL)
			{//�V�[��2D���󂶂�Ȃ��Ƃ�
				D3DXVECTOR3 posBg = m_pScene2D[nCntScene2D]->GetPosPolygon();	//�v���C���[�̈ʒu���

				//�ʒu���̐ݒ�
				m_pScene2D[nCntScene2D]->SetPosPolygon(posBg);
			}
		}
		break;
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CUi::Draw(void)
{

}

