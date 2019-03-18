//=============================================================================
//
// �^�C�g�����S����[2D�|���S��] [titlelogo.cpp]
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
#include "resultlogo.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_WIDTH (250.0f)	//�^�C�g�����S�̕�
#define LOGO_HEIGHT (120.0f) //�^�C�g�����S��Y�̍���
#define PRESS_WIDTH (200.0f) //PressEnter�̕�
#define PRESS_HEIGHT (70.0f) //PressEnter�̍���

#define PATTERN_ANIM (5)	//�e�N�X�`���p�^�[����

#define ANIMATION_SPEED (4)	//�A�j���[�V�����X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (0.25f)//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResultLogo::m_apTexture[MAX_RESULT_TEXTURE] = {};	//���L�e�N�X�`���̃|�C���^
CScene2D *CResultLogo::m_apScene2D[MAX_RESULT_NUM] = {};				//�V�[��2D�̃|�C���^
D3DXCOLOR CResultLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F���
int CResultLogo::m_nJumpRankingFrame = 0;								//�����L���O��ʂ֑J�ڂ���Ƃ��̃t���[���J�E���^
bool CResultLogo::m_bEnable = false;
CResultLogo::RESULTSTATE CResultLogo::m_state[MAX_RESULT_NUM] = {};
CResultLogo::RESULTTYPE CResultLogo::m_type[MAX_RESULT_NUM] = {};
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CResultLogo::CResultLogo() : CScene(5)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CResultLogo::~CResultLogo()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CResultLogo::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/result.png", &m_apTexture[0]);			//�^�C�g�����S[�p�^�[��1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/press_enter2.png", &m_apTexture[1]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/rankbonus.png", &m_apTexture[2]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/score.png", &m_apTexture[3]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/time.png", &m_apTexture[4]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/combo.png", &m_apTexture[5]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/damage.png", &m_apTexture[6]);		//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui/total.png", &m_apTexture[7]);		//�^�C�g�����S[�p�^�[��2]

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CResultLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_RESULT_TEXTURE; nCntTex++)
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
CResultLogo *CResultLogo::Create(void)
{
	//�^�C�g�����S�̃C���X�^���X����
	CResultLogo *pTitle;
	pTitle = new CResultLogo;
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
HRESULT CResultLogo::Init(void)
{
	//�t���[���̏�����
	m_nJumpRankingFrame = 0;
	m_nCntFrame = 0;

	//�F���̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case RESULTTYPE_LOGO:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(210.0f, 65.0f, 0.0f), LOGO_WIDTH, LOGO_HEIGHT);
			//�^�C�g���̎�ނ̐ݒ�
			m_type[nCntTitle] = RESULTTYPE_LOGO;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);
			break;
		case RESULTTYPE_PRESS:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 480.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), PRESS_WIDTH, PRESS_HEIGHT);
			m_type[nCntTitle] = RESULTTYPE_PRESS;
			m_state[nCntTitle] = RESULTSTATE_NORMAL;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[1]);
			break;

		case RESULTTYPE_BONUS:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 280.0f, 80.0f, 0.0f), 700.0f, 150.0f);
			m_type[nCntTitle] = RESULTTYPE_BONUS;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[2]);
			break;

		case RESULTTYPE_SCORE:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 180.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_SCORE;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[3]);
			break;
		case RESULTTYPE_TIME:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 270.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_TIME;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[4]);
			break;
		case RESULTTYPE_COMBO:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 390.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_COMBO;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[5]);
			break;
		case RESULTTYPE_DAMAGE:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(75.0f, 510.0f, 0.0f), 70.0f, 40.0f);
			m_type[nCntTitle] = RESULTTYPE_DAMAGE;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[6]);
			break;
		case RESULTTYPE_TOTAL:
			//2D�|���S���̐���
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 90.0f, 0.0f), 120.0f, 60.0f);
			m_type[nCntTitle] = RESULTTYPE_TOTAL;
			m_state[nCntTitle] = RESULTSTATE_NONE;
			//�e�N�X�`���̊��蓖��
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[7]);
			break;
		}

		//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
		SetObjType(CScene::OBJTYPE_TITLELOGO);
	}

	m_bEnable = false;

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CResultLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
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
void CResultLogo::Update(void)
{




	//�����L���O�փW�����v����t���[���̃J�E���g
	m_nJumpRankingFrame++;

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posTitle[MAX_RESULT_NUM];//�e��ʒu���

 // �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();

	if (m_bEnable == true)
	{
		if (pFade == CFade::FADE_NONE)
		{
			if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_START) == true)
			{//RETURN�L�[�������ꂽ�Ƃ�
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_state[RESULTTYPE_PRESS] = RESULTSTATE_USE;//�g�p��Ԃ�
				CFade::SetFade(CManager::MODE_RANKING);//���j���[�ֈڍs
			}
		}
	}


	for (int nCntTitle = 0; nCntTitle < MAX_RESULT_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosPolygon();	//�ʒu���̎擾

		if (m_type[nCntTitle] == RESULTTYPE_PRESS)
		{
			if (m_state[nCntTitle] == RESULTSTATE_NORMAL)
			{
				if (m_state[RESULTTYPE_PRESS] == RESULTSTATE_NORMAL)
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
				}
				else if (m_state[RESULTTYPE_PRESS] == RESULTSTATE_USE)
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

				}

				//�F���̔��f
				m_apScene2D[nCntTitle]->SetColor(m_col);
			}
		}


		//�ʒu���̐ݒ�
		m_apScene2D[nCntTitle]->SetPosPolygon(posTitle[nCntTitle]);


	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CResultLogo::Draw(void)
{


}

void CResultLogo::SwitchEnable(void)
{
	m_bEnable = m_bEnable ? false : true;
}

