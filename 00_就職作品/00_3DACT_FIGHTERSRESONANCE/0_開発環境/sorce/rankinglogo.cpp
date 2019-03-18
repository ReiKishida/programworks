//=============================================================================
//
// �����L���O���S����[2D�|���S��] [rinkinglogo.cpp]
// Author :�ݓc��(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "rankinglogo.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_WIDTH (230.0f)	//�^�C�g�����S�̕�
#define LOGO_HEIGHT (110.0f) //�^�C�g�����S��Y�̍���
#define PRESS_WIDTH (200.0f) //PressEnter�̕�
#define PRESS_HEIGHT (70.0f) //PressEnter�̍���

#define PATTERN_ANIM (5)	//�e�N�X�`���p�^�[����

#define ANIMATION_SPEED (4)	//�A�j���[�V�����X�s�[�h
#define TEX_X (0.25f)		//�e�N�X�`�����W_X
#define TEX_Y (0.25f)//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRankingLogo::m_apTexture[MAX_RANKING_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CRankingLogo::m_apScene2D[MAX_RANKING_NUM] = {};//�V�[��2D�̃|�C���^
D3DXCOLOR CRankingLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F���
int CRankingLogo::m_nJumpTitleFrame = 0;//�����L���O��ʂ֑J�ڂ���Ƃ��̃t���[���J�E���^

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CRankingLogo::CRankingLogo(int nPriority, OBJTYPE type) : CScene(nPriority , type)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/ranking.png", &m_apTexture[0]);		//�^�C�g�����S[�p�^�[��1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/press_enter2.png", &m_apTexture[1]);	//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/flame.png", &m_apTexture[2]);			//�^�C�g�����S[�p�^�[��2]

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CRankingLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RANKING_TEXTURE; nCntTex++)
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
//�^�C�g�����S�̐���
//=============================================================================
CRankingLogo *CRankingLogo::Create(void)
{
	//�^�C�g�����S�̃C���X�^���X����
	CRankingLogo *pTitle;
	pTitle = new CRankingLogo;
	if (pTitle != NULL)
	{
		//�^�C�g�����S�̏���������
		pTitle->Init();
	}

	return pTitle;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CRankingLogo::Init(void)
{
	//�t���[���̏�����
	m_nJumpTitleFrame = 0;
	m_nCntFrame = 0;
	//�F���̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case RANKINGTYPE_FLAME:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2, 0.0f), 955.0f, 550.0f);
			//�^�C�g���̎�ނ̐ݒ�
			m_type = RANKINGTYPE_FLAME;
			m_state = RANKINGSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[2]);
			break;
		case RANKINGTYPE_LOGO:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(170.0f, 65.0f, 0.0f), LOGO_WIDTH, LOGO_HEIGHT);
			//�^�C�g���̎�ނ̐ݒ�
			m_type = RANKINGTYPE_LOGO;
			m_state = RANKINGSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);
			break;
		case RANKINGTYPE_PRESS:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 480.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), PRESS_WIDTH, PRESS_HEIGHT);
			m_type = RANKINGTYPE_PRESS;
			m_state = RANKINGSTATE_NORMAL;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[1]);
			break;
		}

		//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
		SetObjType(CScene::OBJTYPE_TITLELOGO);
	}
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CRankingLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		if (m_apScene2D[nCntTitle] != NULL)
		{
			//2D�V�[���̏I������
			m_apScene2D[nCntTitle]->Uninit();
			m_apScene2D[nCntTitle] = NULL;

		}

	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CRankingLogo::Update(void)
{
	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();



	//�����L���O�փW�����v����t���[���̃J�E���g
	m_nJumpTitleFrame++;

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posTitle[MAX_RANKING_NUM];//�e��ʒu���


	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
		{//RETURN�L�[�������ꂽ�Ƃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			m_state = RANKINGSTATE_USE;//�g�p��Ԃ�
			CFade::SetFade(CManager::MODE_TITLE);//���j���[�ֈڍs
		}
		if (m_nJumpTitleFrame % 500 == 0)
		{
			CManager::SetTimer(9, 9, 9, 9, 9, 9);
			CFade::SetFade(CManager::MODE_TITLE);//�����L���O�ֈڍs
		}
	}

	for (int nCntTitle = 0; nCntTitle < MAX_RANKING_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosPolygon();	//�ʒu���̎擾

		switch (m_type)
		{
		case RANKINGTYPE_FLAME:
			break;
		case RANKINGTYPE_LOGO:
		break;
		case RANKINGTYPE_PRESS:
			if (m_state == RANKINGSTATE_NORMAL)
			{//�ʏ��Ԃ̂Ƃ�
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 280;
				if (m_nCntFrame < 140)
				{
					m_col.a -= 0.01f;
				}
				if (m_nCntFrame > 140)
				{
					m_col.a += 0.01f;
				}
				//�F���̔��f
				m_apScene2D[2]->SetColor(m_col);
			}
			else if (m_state == RANKINGSTATE_USE)
			{//�{�^���������ꂽ�Ƃ�
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 6;
				if (m_nCntFrame < 3)
				{
					m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
				}
				if (m_nCntFrame > 3)
				{
					m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�F��Ԃ�
				}
				//�F���̔��f
				m_apScene2D[2]->SetColor(m_col);
			}
			break;
		}

		//�ʒu���̐ݒ�
		m_apScene2D[nCntTitle]->SetPosPolygon(posTitle[nCntTitle]);

	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CRankingLogo::Draw(void)
{


}

