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
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "ui.h"
#include "timer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (250)	//�|���S���̕�
#define BG_HEIGHT (720)	//�|���S����Y�̍���
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
CUi::CUi(int nPriority) : CScene(7)
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/L_70.jpg", &m_pTexture[0]);// UI���n
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/R_70.jpg", &m_pTexture[1]);// UI���n
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/highscore.png", &m_pTexture[2]);//�n�C�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score.png", &m_pTexture[3]);//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk0.png", &m_pTexture[4]);//�U�̃t�H�[�X��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr0.png", &m_pTexture[5]);//�g�̃t�H�[�X��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd0.png", &m_pTexture[6]);//���̃t�H�[�X��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def0.png", &m_pTexture[7]);//��̃t�H�[�X��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/time.png", &m_pTexture[8]);//�^�C���̕���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/colon.png", &m_pTexture[9]);//�R����
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/dot.png", &m_pTexture[10]);//�h�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/log.png", &m_pTexture[11]);//���O�̕���



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
CUi *CUi::Create(D3DXVECTOR3 pos, UITYPE type)
{
	//�w�i�̐���
	CUi *pUi;
	pUi = new CUi;

	if (pUi != NULL)
	{
		//��ސݒ�
		pUi->m_Type = type;
		//�w�i�̏���������
		pUi->Init(pos);
	}


	return pUi;



}


//=============================================================================
//����������
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos)
{
		//�F���ݒ�
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
		m_nCntColor = 0;
		//2D�|���S���̐���
		m_pScene2D = new CScene2D(6);
		if (m_pScene2D != NULL)
		{
			switch (m_Type)
			{
			case UI_TYPE_BG_L:// UI�̉��n
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(BG_WIDTH);
				m_pScene2D->SetScene2DHeight(BG_HEIGHT);
				//����������
				m_pScene2D->Init(pos);
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_BG_L]);
				break;
			case UI_TYPE_BG_R:// UI�̉��n
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(BG_WIDTH);
				m_pScene2D->SetScene2DHeight(BG_HEIGHT);
				//����������
				m_pScene2D->Init(pos);
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_BG_R]);
				break;
			case UI_TYPE_HIGHSCORE://�n�C�X�R�A
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(90.0f);
				m_pScene2D->SetScene2DHeight(35.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(70.0f, 25.0f,0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_HIGHSCORE]);
				break;

			case UI_TYPE_SCORE://�X�R�A
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(70.0f);
				m_pScene2D->SetScene2DHeight(35.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(60.0f, 140.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SCORE]);
				break;

			case UI_TYPE_ATK://�U�̃{�[�i�X��
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y - 30.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_ATK]);
				break;

			case UI_TYPE_SPR://�g�̃{�[�i�X��
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 30.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SPR]);
				break;

			case UI_TYPE_SPD://���̃{�[�i�X��
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 90.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_SPD]);
				break;

			case UI_TYPE_DEF://��̃{�[�i�X��
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(60.0f, pos.y + 150.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_DEF]);
				break;

			case UI_TYPE_TIME://�^�C���̕���
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(80.0f);
				m_pScene2D->SetScene2DHeight(30.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 480.0f, pos.y - 320.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_TIME]);
				break;

			case UI_TYPE_COLON://�R����
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(15.0f);
				m_pScene2D->SetScene2DHeight(40.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 480.0f, pos.y - 240.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_COLON]);
				break;

			case UI_TYPE_DOT://�h�b�g
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(10.0f);
				m_pScene2D->SetScene2DHeight(10.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 560.0f, pos.y - 220.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_DOT]);
				break;

			case UI_TYPE_LOG://���O�̕���
				//�w�i�̕��A�����ݒ�
				m_pScene2D->SetScene2DWidth(70.0f);
				m_pScene2D->SetScene2DHeight(30.0f);
				//����������
				m_pScene2D->Init(D3DXVECTOR3(pos.x + 520.0f, pos.y - 180.0f, 0.0f));
				//�e�N�X�`���̊��蓖��
				m_pScene2D->BindTexture(m_pTexture[UI_TYPE_LOG]);
				break;
			}


			//�w�i�̃I�u�W�F�N�g�̐ݒ�
			m_pScene2D->SetObjType(CScene::OBJTYPE_UI);
		}



	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CUi::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_TEXTURE_UI; nCntScene2D++)
	{
		if (m_pScene2D != NULL)
		{//�V�[��2D���󂶂�Ȃ��Ƃ�
			//2D�V�[���̏I������
			m_pScene2D->Uninit();
			m_pScene2D = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CUi::Update(void)
{

		if (m_pScene2D != NULL)
		{//�V�[��2D���󂶂�Ȃ��Ƃ�
			D3DXVECTOR3 posBg = m_pScene2D->GetPosScene2D();	//�v���C���[�̈ʒu���
			if (m_Type == UI_TYPE_DOT || m_Type == UI_TYPE_COLON)
			{//���Ԃ̋�؂�
				int nTime = CTimer::GetTimer();
				if (nTime >= 10000)
				{//1���ȏ�
					m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (nTime <= 5999)
				{//1���ȉ�
					m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
				}
				if (nTime <= 2999)
				{//�c��30�b�ȉ�
					m_nCntColor++;
					m_nCntColor = m_nCntColor % 18;
					if (m_nCntColor < 9)
					{
						m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					if (m_nCntColor >= 9)
					{
						m_Color = D3DXCOLOR(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}


			//�ʒu���̐ݒ�
			m_pScene2D->SetPosScene2D(posBg);

			//�ʒu���̐ݒ�
			m_pScene2D->SetColor(m_Color);

		}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CUi::Draw(void)
{

}

