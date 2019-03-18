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
#include "menu.h"
#include "bg.h"
#include "polygon.h"
#include "menuscene.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_WIDTH (130.0f)		//�e�탁�j���[�̕�
#define MENU_HEIGHT (130.0f)		//�e�탁�j���[�̍���

#define WINDOW_WIDTH (1000.0f)	//�E�B���h�E�̕�
#define WINDOW_HEIGHT (280.0f)	//�E�B���h�E�̍���

#define HUMAN_WIDTH (320.0f)	//�l�̕�
#define  HUMAN_HEIGHT (520.0f)	//�l�̍���
#define MAX_MENU_NUM (3)	//���j���[�̍��ڐ�
//#define MAX_MENU_NUM (2)	//�E�B���h�E�ƃq�g
#define MAX_STATE_NUM (2)	//��Ԃ̐�
#define MENU_RADIUS (130.0f)  //���ڂ̋���
#define ROT_SPEED (0.05f)
#define FLASHING_NUM (100)
#define FLASHING_NUM_HALF (50)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMenuScene::m_apTexture[MAX_MENUTEXTURE_NUM] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CMenuScene::m_apScene2D[MAX_MENUPOLYGON_NUM] = {};
bool CMenuScene::m_bRotate = false;	//��]���Ă邩
int CMenuScene::m_nSelectMenu = 0;	//�I�𒆂̔ԍ�
int CMenuScene::m_nSelectState = 0;	//�I�𒆂̔ԍ�
JoyState g_JoyStateMenuScene;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CMenuScene::CMenuScene()
{
	//�A�j���[�V�����J�E���^�[
	m_nTex_Y = 0;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CMenuScene::~CMenuScene()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CMenuScene::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game mode.png", &m_apTexture[MENUTYPE_GAMEMODE]);		//�Q�[�����[�h
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_apTexture[MENUTYPE_RANKING]);		//�����L���O
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit.png", &m_apTexture[MENUTYPE_QUIT]);			//�N�C�b�g
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_human.png", &m_apTexture[MENUTYPE_HUMAN]);		//�l
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_game.png", &m_apTexture[MENUTYPE_WINDOW_GAME]);			//�E�B���h�E
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_ranking.png", &m_apTexture[MENUTYPE_WINDOW_RANKING]);			//�E�B���h�E
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_quit.png", &m_apTexture[MENUTYPE_WINDOW_QUIT]);			//�E�B���h�E

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CMenuScene::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_MENUTEXTURE_NUM; nCntTex++)
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
//���j���[����
//=============================================================================
CMenuScene *CMenuScene::Create(D3DXVECTOR3 pos)
{
	//���j���[�C���X�^���X����
	CMenuScene *pTitle;
	pTitle = new CMenuScene;
	if (pTitle != NULL)
	{
		//���j���[����������
		pTitle->Init(pos);
	}

	return pTitle;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CMenuScene::Init(D3DXVECTOR3 pos)
{
	//�Q�[���p�b�h�̏��
	g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//�F���̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nSelectMenu = MENUTYPE_GAMEMODE;
	m_state = MENUSTATE_NONE;
	m_nCntFrame = 0;
	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		//2D�|���S���̐���
		m_apScene2D[nCntMenu] = new CScene2D(7);
		if (m_apScene2D[nCntMenu] != NULL)
		{
			switch (nCntMenu)
			{
			case MENUTYPE_GAMEMODE:
				//�^�C�g�����S�̕��A�����ݒ�
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_GAMEMODE]);
				//2D�V�[���̏���������
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//�ʏ�̏�Ԃ�
				//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_RANKING:
				//�^�C�g�����S�̕��A�����ݒ�
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_RANKING]);
				//2D�V�[���̏���������
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(420.0f, 300.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//�ʏ�̏�Ԃ�
				//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_QUIT:
				//�^�C�g�����S�̕��A�����ݒ�
				m_apScene2D[nCntMenu]->SetScene2DWidth(MENU_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(MENU_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_QUIT]);
				//2D�V�[���̏���������
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(180.0f, 300.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//�ʏ�̏�Ԃ�
				//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_MENU);
				break;
			case MENUTYPE_HUMAN:
				//���j���[���A�����ݒ�
				m_apScene2D[nCntMenu]->SetScene2DWidth(HUMAN_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(HUMAN_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_HUMAN]);
				//2D�V�[���̏���������
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 320.0f, SCREEN_HEIGHT / 2, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(0.25, 1.0f);
				m_state = MENUSTATE_NORMAL;//��̏�Ԃ�
				//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_MENU);

				break;
			case MENUTYPE_WINDOW_GAME:
				//���j���[���A�����ݒ�
				m_apScene2D[nCntMenu]->SetScene2DWidth(WINDOW_WIDTH);
				m_apScene2D[nCntMenu]->SetScene2DHeight(WINDOW_HEIGHT);
				//�e�N�X�`���̊��蓖��
				m_apScene2D[nCntMenu]->BindTexture(m_apTexture[MENUTYPE_WINDOW_GAME]);
				//2D�V�[���̏���������
				m_apScene2D[nCntMenu]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 + 200.0f, 0.0f));
				m_apScene2D[nCntMenu]->SetTexture(1.0f, 1.0f);
				m_state = MENUSTATE_NORMAL;//��̏�Ԃ�
				//�^�C�g�����S�̃I�u�W�F�N�g�̐ݒ�
				SetObjType(CScene::OBJTYPE_MENU);

				break;
			}
		}
	}
	return S_OK;
}
//=============================================================================
//�I������
//=============================================================================
void CMenuScene::Uninit(void)
{
	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		if (m_apScene2D[nCntMenu] != NULL)
		{
			//2D�V�[���̏I������
			m_apScene2D[nCntMenu]->Uninit();
			m_apScene2D[nCntMenu] = NULL;
		}
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CMenuScene::Update(void)
{
	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();
	m_MenuPos = D3DXVECTOR3(300.0f, 230.0f, 0.0f);

	D3DXVECTOR3 posMenu[MAX_MENUPOLYGON_NUM];//�e��ʒu���
	D3DXCOLOR colMenu[MAX_MENUPOLYGON_NUM];//�e��ʒu���

	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		posMenu[nCntMenu] = m_apScene2D[nCntMenu]->GetPosScene2D();	//�ʒu���̎擾
		colMenu[nCntMenu] = m_apScene2D[nCntMenu]->GetColor();	//�ʒu���̎擾
	}

	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
		{//RETURN�L�[�������ꂽ�Ƃ�
			m_state = MENUSTATE_USE;//�g�p��Ԃ�
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//���艹

			switch (m_nSelectMenu)
			{//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
			case MENUTYPE_GAMEMODE://�Q�[���I�𒆂Ȃ�
				CFade::SetFade(CManager::MODE_TUTORIAL);//�Q�[���ֈڍs
				break;
			case MENUTYPE_QUIT://�ݒ�I�𒆂Ȃ�
				CFade::SetFade(CManager::MODE_TITLE);//�^�C�g���ֈڍs
				break;
			case MENUTYPE_RANKING://�ݒ�I�𒆂Ȃ�
				CFade::SetFade(CManager::MODE_MENURANKING);//�����L���O�ֈڍs
				break;
			}
		}
	}

	switch (m_nSelectMenu)
	{//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
	case MENUTYPE_GAMEMODE://�Q�[���I�𒆂Ȃ�
		m_apScene2D[4]->BindTexture(m_apTexture[MENUTYPE_WINDOW_GAME]);//�E�B���h�E�e�N�X�`�����W�̐ݒ�
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.25f, 0.0f, 0.0f, 1.0f);//�E�B���h�E�e�N�X�`�����W�̐ݒ�
		break;
	case MENUTYPE_RANKING://�ݒ�I�𒆂Ȃ�
		m_apScene2D[4]->BindTexture(m_apTexture[MENUTYPE_WINDOW_RANKING]);//�E�B���h�E�e�N�X�`�����W
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.5f, 0.25f, 0.0f, 1.0f);//�E�B���h�E�e�N�X�`�����W�̐ݒ�
		break;
	case MENUTYPE_QUIT://�ݒ�I�𒆂Ȃ�
		m_apScene2D[4]-> BindTexture(m_apTexture[MENUTYPE_WINDOW_QUIT]);//�E�B���h�E�e�N�X�`��
		m_apScene2D[MENUTYPE_HUMAN]->SetTexture_All(0.75f, 0.5f, 0.0f, 1.0f);//�E�B���h�E�e�N�X�`�����W�̐ݒ�
		break;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pInputKeyboard->GetKeyboardTrigger(DIK_A) == true
		 || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)
		 || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//W�L�[�������ꂽ�Ƃ�
		if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//�J�[�\��
		 //m_bRotate = true;
		 //�F���̔��f
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
			m_nSelectMenu = (m_nSelectMenu + (MAX_MENU_NUM - 1)) % MAX_MENU_NUM;//�I����Ԃ���グ��
		}
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_S) == true || pInputKeyboard->GetKeyboardTrigger(DIK_D) == true
		|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)
		|| (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//S�L�[�������ꂽ�Ƃ�
		if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);//�J�[�\��
		 //m_bRotate= true;
		 //�F���̔��f
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
			m_nSelectMenu = (m_nSelectMenu + 1) % MAX_MENU_NUM;//�I����Ԃ��������
		}
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_PUSH;

	}


	m_nCntFrame++;
	m_nCntFrame = m_nCntFrame % FLASHING_NUM;
	if (m_nCntFrame < FLASHING_NUM_HALF)
	{
		colMenu[m_nSelectMenu].a -= 0.02f;
	}
	if (m_nCntFrame > FLASHING_NUM_HALF)
	{
		colMenu[m_nSelectMenu].a += 0.02f;
	}

	if (m_state == MENUSTATE_USE)
	{//�{�^���������ꂽ�Ƃ�
		m_nCntFrame++;
		m_nCntFrame = m_nCntFrame % 6;
		if (m_nCntFrame < 3)
		{
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
		}
		if (m_nCntFrame > 3)
		{
			colMenu[m_nSelectMenu] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�F��Ԃ�
		}
	}


	for (int nCntMenu = 0; nCntMenu < MAX_MENUPOLYGON_NUM; nCntMenu++)
	{
		m_apScene2D[nCntMenu]->SetColor(colMenu[nCntMenu]);//�F���̐ݒ�
		m_apScene2D[nCntMenu]->SetPosScene2D(posMenu[nCntMenu]);//�ʒu���̐ݒ�
	}

	if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateMenuScene.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateMenuScene.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CMenuScene::Draw(void)
{


}

