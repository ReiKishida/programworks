//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Kishida Rei
//=============================================================================
#include "main.h"
#include "manager.h"
#include "pause.h"
#include "polygon.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_apTexture[MAX_PAUSE_TEXTURE] = {}; //���L�e�N�X�`���̃|�C���^
CPolygon *CPause::m_apPolygon[MAX_PAUSE] = {};//�V�[��2D
int CPause::m_nSelectNum = PAUSEMENU_CONTINUE;
int CPause::m_nCntFrame = 0;
CPause::PAUSESTATE CPause::m_state[MAX_PAUSE] = {};//�|�[�Y���j���[�̍���

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FLAME_WIDTH (500.0f)		//�g�̕�
#define FLAME_HEIGHT (580.0f)		//�g�̍���

#define MENU_WIDTH (350.0f)			//�e�탁�j���[�̕�
#define MENU_HEIGHT (150.0f)		//�e�탁�j���[�̍���

#define GUID_WIDTH (600.0f)			//���ڐ����̕�
#define GUID_HEIGHT (350.0f)		//���ڐ����̍���

#define FLASHING_NUM (80)
#define FLASHING_NUM_HALF (40)

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPause::CPause()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
//����������
//=============================================================================
HRESULT CPause::Init(void)
{

	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();


	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause00.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause01.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause02.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/pause03.png", &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/mastertuto.png", &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo/tuto_key.png", &m_apTexture[5]);



	//�I������Ă��鐔��
	m_nSelectNum = PAUSEMENU_CONTINUE;
	//�t���[�����̃J�E���g
	m_nCntFrame = 0;


	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		//�|���S���̐���
		m_apPolygon[nCntPolygon] = new CPolygon;
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			switch (nCntPolygon)
			{
			case PAUSETYPE_BACK:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(SCREEN_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(SCREEN_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT /2, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(NULL);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.9f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			case PAUSETYPE_FLAME:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(FLAME_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(FLAME_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 , 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[0]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;

				break;
			case PAUSETYPE_CONTINUE:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 -80.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[1]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_RETRY:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2  + 30.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[2]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_QUIT:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(MENU_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(MENU_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[3]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NORMAL;
				break;
			case PAUSETYPE_GUID_ARCADE:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(GUID_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(GUID_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 + 170.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[4]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			case PAUSETYPE_GUID_KEYBOARD:
				//�|�[�Y�̕��A�����ݒ�
				m_apPolygon[nCntPolygon]->SetPolygonWidth(GUID_WIDTH);
				m_apPolygon[nCntPolygon]->SetPolygonHeight(GUID_HEIGHT);
				//2D�V�[���̏���������
				m_apPolygon[nCntPolygon]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 170.0f, 0.0f));
				m_apPolygon[nCntPolygon]->BindTexture(m_apTexture[5]);
				m_apPolygon[nCntPolygon]->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_state[nCntPolygon] = PAUSESTATE_NONE;
				break;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CPause::Uninit(void)
{
	//�e�N�X�`���̂̔j��
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE_TEXTURE; nCntPolygon++)
	{
		if (m_apTexture[nCntPolygon] != NULL)
		{
			m_apTexture[nCntPolygon]->Release();
			m_apTexture[nCntPolygon] = NULL;
		}
	}

	//�|���S���̔j��
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}
}

//=============================================================================
//�X�V����
//=============================================================================
void CPause::Update(void)
{
	// �R���g���[���[�擾
	CInputGamePad *pGamePad = CManager::GetInputGamePad();
	CInputGamePad::GamePad *pGamePadStick = CManager::GetInputGamePad()->GetgamePadStick();


	//�t�F�[�h�̎擾
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posPause[MAX_PAUSE];	//�ʒu���
	D3DXCOLOR colPause[MAX_PAUSE];		//�F���



	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		posPause[nCntPolygon] = m_apPolygon[nCntPolygon]->GetPosPolygon();		//�ʒu���̎擾
		colPause[nCntPolygon] = m_apPolygon[nCntPolygon]->GetPolygonColor();	//�F���̎擾
	}



	//===============================================================================
	//	�|�[�Y�̉�ʑJ�ڐ�
	//===============================================================================
	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(0, BUTTON_A) == true)
		{//RETURN�L�[�������ꂽ�Ƃ�

				switch (m_nSelectNum)
				{//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
				case PAUSEMENU_CONTINUE://�R���e�B�j���[�I�𒆂Ȃ�
					CRenderer::SetPause();//�|�[�Y����
					break;
				case PAUSEMENU_RETRY://���g���C�I�𒆂Ȃ�
					CRenderer::SetPause();//�|�[�Y����
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//���艹
					CFade::SetFade(CManager::MODE_GAME);//�Q�[���ֈڍs
					break;
				case PAUSEMENU_QUIT://�N�C�b�g�I�𒆂Ȃ�
					CRenderer::SetPause();//�|�[�Y����
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);//���艹
					CFade::SetFade(CManager::MODE_TITLE);//�^�C�g���ֈڍs
					break;
				}

		}
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_V) == true)
	{//W�L�[�������ꂽ�Ƃ�
		CCamera::SetFree();
	}

	//===============================================================================
	//	���ڑI��
	//===============================================================================
	if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pGamePadStick[0].aGamePad.lY < -GAMEPAD_DEADZONE)
	{//W�L�[�������ꂽ�Ƃ�
	 //�F���̔��f

			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
			m_nSelectNum = (m_nSelectNum + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;//�I����Ԃ���グ��
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_S) == true || pGamePadStick[0].aGamePad.lY > GAMEPAD_DEADZONE)
	{//S�L�[�������ꂽ�Ƃ�


			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
			m_nSelectNum = (m_nSelectNum + 1) % PAUSEMENU_MAX;//�I����Ԃ��������

	}


	//===============================================================================
	//	�_�ŏ���
	//===============================================================================
	m_nCntFrame++;
	m_nCntFrame = m_nCntFrame % FLASHING_NUM;
	if (m_nCntFrame < FLASHING_NUM_HALF)
	{
		colPause[m_nSelectNum + 2].a -= 0.015f;
	}
	if (m_nCntFrame > FLASHING_NUM_HALF)
	{
		colPause[m_nSelectNum + 2].a += 0.015f;
	}

	if (m_state[m_nSelectNum] == PAUSESTATE_USE)
	{//�{�^���������ꂽ�Ƃ�
		m_nCntFrame = m_nCntFrame % 6;
		if (m_nCntFrame < 3)
		{
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�F�𔒂�
		}
		if (m_nCntFrame > 3)
		{
			colPause[m_nSelectNum + 2] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�F��Ԃ�
		}
	}

	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->SetPosPolygon(posPause[nCntPolygon]);	//�ʒu����ݒ�
		m_apPolygon[nCntPolygon]->SetPolygonColor(colPause[nCntPolygon]);//�F���̐ݒ�
	}

}

//=============================================================================
//�`�揈��
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_PAUSE; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon]->Draw();
	}

}

//=============================================================================
//�I������Ă��鐔���̐ݒ�
//=============================================================================
void CPause::SetSelect(void)
{
	m_nSelectNum = PAUSEMENU_CONTINUE;
}
