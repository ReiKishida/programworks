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
#include "bullet.h"
#include "enemy.h"
#include "titlelogo.h"
#include "fade.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_WIDTH (650.0f)	//�^�C�g�����S�̕�
#define LOGO_HEIGHT (250.0f) //�^�C�g�����S��Y�̍���
#define PRESS_WIDTH (350.0f) //PressEnter�̕�
#define PRESS_HEIGHT (120.0f) //PressEnter�̍���

#define PATTERN_ANIM (5)	//�e�N�X�`���p�^�[����

#define ANIMATION_SPEED (4)	//�A�j���[�V�����X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (0.25f)//�e�N�X�`�����W_Y

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitleLogo::m_apTexture[MAX_TITLE_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CTitleLogo::m_apScene2D[MAX_TITLE_NUM] = {};//�V�[��2D�̃|�C���^
D3DXCOLOR CTitleLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F���
JoyState g_JoyStateTitle;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo() : CScene(5)
{
	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_0.png", &m_apTexture[0]);//�^�C�g�����S[�p�^�[��1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_1.png", &m_apTexture[1]);//�^�C�g�����S[�p�^�[��2]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_2.png", &m_apTexture[2]);//�^�C�g�����S[�p�^�[��3]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_3.png", &m_apTexture[3]);//�^�C�g�����S[�p�^�[��4]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_4.png", &m_apTexture[4]);//�^�C�g�����S[�p�^�[��5]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_5.png", &m_apTexture[5]);//�^�C�g�����S[�p�^�[��6]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_6.png", &m_apTexture[6]);//�^�C�g�����S[�p�^�[��7]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_7.png", &m_apTexture[7]);//�^�C�g�����S[�p�^�[��8]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_8.png", &m_apTexture[8]);//�^�C�g�����S[�p�^�[��9]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_9.png", &m_apTexture[9]);//�^�C�g�����S[�p�^�[��10]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter2.png", &m_apTexture[10]);//PressEnter

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CTitleLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TITLE_TEXTURE; nCntTex++)
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
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos)
{
	//�^�C�g�����S�̃C���X�^���X����
	CTitleLogo *pTitle;
	pTitle = new CTitleLogo;
	if (pTitle != NULL)
	{
		//�^�C�g�����S�̏���������
		pTitle->Init(pos);


	}

	return pTitle;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_nCntFrame = 0;
	//�Q�[���p�b�h�̏��
	g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//�F���̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		//2D�|���S���̐���
		m_apScene2D[nCntTitle] = new CScene2D(5);
		if (m_apScene2D[nCntTitle] != NULL)
		{
			if (nCntTitle == TITLETYPE_LOGO)
			{
				//�^�C�g�����S�̕��A�����ݒ�
				m_apScene2D[nCntTitle]->SetScene2DWidth(LOGO_WIDTH);
				m_apScene2D[nCntTitle]->SetScene2DHeight(LOGO_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);

			}
			else if (nCntTitle == TITLETYPE_PRESS)
			{
				//�^�C�g�����S�̕��A�����ݒ�
				m_apScene2D[nCntTitle]->SetScene2DWidth(PRESS_WIDTH);
				m_apScene2D[nCntTitle]->SetScene2DHeight(PRESS_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntTitle]->BindTexture(m_apTexture[10]);
			}




			if (nCntTitle == TITLETYPE_LOGO)
			{
				//2D�V�[���̏���������
				m_apScene2D[nCntTitle]->Init(pos);
				m_apScene2D[nCntTitle]->SetTexture(1.0f, 1.0f);
				m_state = TITLESTATE_NONE;//��̏�Ԃ�

			}
			else if (nCntTitle == TITLETYPE_PRESS)
			{
				//2D�V�[���̏���������
				m_apScene2D[nCntTitle]->Init(D3DXVECTOR3(pos.x, pos.y + 350.0f, pos.z));
				m_apScene2D[nCntTitle]->SetTexture(1.0f, 1.0f);
				m_state = TITLESTATE_NORMAL;//�ʏ��Ԃ�
			}
		}
			//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_TITLELOGO);
	}
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
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
void CTitleLogo::Update(void)
{
	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	D3DXVECTOR3 posTitle[MAX_TITLE_NUM];//�e��ʒu���


	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//RETURN�L�[�������ꂽ�Ƃ�
			if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				m_state = TITLESTATE_USE;//�g�p��Ԃ�
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//���艹
				CFade::SetFade(CManager::MODE_MENU);//���j���[�ֈڍs
				//CFade::SetFade(CManager::MODE_GAME);//�Q�[���ֈڍs
			}
			g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosScene2D();	//�ʒu���̎擾

		if (nCntTitle == TITLETYPE_LOGO)
		{

			if (rand() % 12 == 0)
			{


				if (m_TexNum = rand() % 10)
				{
					m_apScene2D[nCntTitle]->BindTexture(m_apTexture[m_TexNum]);
				}
			}
		}
		if (nCntTitle == TITLETYPE_PRESS)
		{
			if (m_state == TITLESTATE_NORMAL)
			{//�ʏ��Ԃ̂Ƃ�
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 160;
				if (m_nCntFrame < 80)
				{
					m_col.a -= 0.015f;
				}
				if (m_nCntFrame > 80)
				{
					m_col.a += 0.015f;
				}
				//�F���̔��f
				m_apScene2D[1]->SetColor(m_col);
			}
			else if(m_state == TITLESTATE_USE)
			{//�{�^���������ꂽ�Ƃ�
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 6;
				if (m_nCntFrame < 3)
				{
					m_col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);//�F�𔒂�
				}
				if (m_nCntFrame > 3)
				{
					m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�F��Ԃ�
				}
				//�F���̔��f
				m_apScene2D[1]->SetColor(m_col);

			}
		}

		//�ʒu���̐ݒ�
		m_apScene2D[nCntTitle]->SetPosScene2D(posTitle[nCntTitle]);

	}


	if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTitle.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTitle.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}


}

//=============================================================================
//�`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{


}

