//=============================================================================
//
// UI�̏���[2D�|���S��] [ui.cpp]
// Author :�ݓc��(KishidaRei)
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
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WIDTH (900)	//�|���S���̕�
#define BG_HEIGHT (700)	//�|���S����Y�̍���
#define FORCE_RADIUS (40.0f)
#define TEX_X (1.0f)
#define TEX_Y (1.0f)
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CForce::m_apTexture[MAX_FORCE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CForce::m_apScene2D[MAX_FORCE] = {};	//�|���S���̃|�C���^
int CForce::m_aForce[MAX_FORCE] = {};	//�t�H�[�X�̃|�C���^
int CForce::m_nNumAttack = 0;	//�U�̃t�H�[�X��
int CForce::m_nNumSpread = 0;	//�g�̃t�H�[�X��
int CForce::m_nNumSpeed = 0;	//���̃t�H�[�X��
int CForce::m_nNumDefense = 0;	//��̃t�H�[�X��

int CForce::m_nCntDPS = 0;
int CForce::m_nDPS = 0;
int CForce::m_nDamage = 0;
JoyState g_JoyStateForce;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CForce::CForce() :CScene(7)
{//�N���A�Ȓl����
	m_nCnt = 0;
	m_nNumAttack = 0;	//�U�̃t�H�[�X��
	m_nNumSpread = 0;	//�g�̃t�H�[�X��
	m_nNumSpeed = 0;	//���̃t�H�[�X��
	m_nNumDefense = 0;	//��̃t�H�[�X��

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CForce::~CForce()
{


}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CForce::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk.png", &m_apTexture[FORCETYPE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr.png", &m_apTexture[FORCETYPE_SPREAD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd.png", &m_apTexture[FORCETYPE_SPEED]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def.png", &m_apTexture[FORCETYPE_DEFENSE]);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CForce::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_FORCE; nCntTex++)
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
//�X�R�A�̐���
//=============================================================================
CForce *CForce::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CForce *pScore;
	pScore = new CForce;

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
HRESULT CForce::Init(D3DXVECTOR3 pos)
{
	m_nCnt = 0;
	m_nCntDPS = 0;
	//�Q�[���p�b�h�̏��
	g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
	{
		//�����̐���
		m_apScene2D[nCntNumber] = new CScene2D(7);
		if (m_apScene2D[nCntNumber] != NULL)
		{
			m_type[nCntNumber] = FORCETYPE_NONE;

			//����������
			m_apScene2D[nCntNumber]->SetScene2DWidth(FORCE_RADIUS);
			m_apScene2D[nCntNumber]->SetScene2DHeight(FORCE_RADIUS);
			m_apScene2D[nCntNumber]->Init(D3DXVECTOR3(pos.x + (58.0f * nCntNumber), pos.y, pos.z));
			m_apScene2D[nCntNumber]->BindTexture(m_apTexture[FORCETYPE_ATTACK]);

			SetForce(nCntNumber, FORCETYPE_ATTACK);

			//�t�H�[�X�̃I�u�W�F�N�g�̐ݒ�
			SetObjType(CScene::OBJTYPE_FORCE);

		}
	}



	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CForce::Uninit(void)
{


	for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
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
//�X�V����
//=============================================================================
void CForce::Update(void)
{
	m_nCntDPS++;

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

	//�t�H�[�X�̈ʒu
	D3DXVECTOR3 posForce[MAX_FORCE];


	for (int nCntForce = 0; nCntForce < MAX_FORCE; nCntForce++)
	{
		posForce[nCntForce] = m_apScene2D[nCntForce]->GetPosScene2D();
	}

	//�J�E���g�̃��Z�b�g
	m_nCnt = m_nCnt % MAX_FORCE;

#if 1
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
	{//�U�̃t�H�[�X�̐ݒu
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_ATTACK);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
	{//�g�̃t�H�[�X�ݒu
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_SPREAD);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_X && pJoyState.bConnectionJoypad == true))
	{//���̃t�H�[�X�ݒu
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_SPEED);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_Y && pJoyState.bConnectionJoypad == true))
	{//��̃t�H�[�X�ݒu
		if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

			SetForce(m_nCnt, FORCETYPE_DEFENSE);
			m_nCnt++;
		}
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
#endif
#if 0
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true )
	{//�U�̃t�H�[�X�̐ݒu

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_ATTACK);
		m_nCnt++;


	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true )
	{//�g�̃t�H�[�X�ݒu

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_SPREAD);
		m_nCnt++;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true )
	{//���̃t�H�[�X�ݒu

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_SPEED);
		m_nCnt++;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true )
	{//��̃t�H�[�X�ݒu

		pSound->PlaySound(CSound::SOUND_LABEL_FORCE);

		SetForce(m_nCnt, FORCETYPE_DEFENSE);
		m_nCnt++;

	}
#endif

	if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateForce.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

	for (int nCntForce = 0; nCntForce < MAX_FORCE; nCntForce++)
	{
		m_apScene2D[nCntForce]->SetPosScene2D(posForce[nCntForce]);
	}
}

//=============================================================================
//�`�揈��
//=============================================================================
void CForce::Draw(void)
{
	//for (int nCntNumber = 0; nCntNumber < MAX_FORCE; nCntNumber++)
	//{
	//	if (m_apScene2D[nCntNumber] != NULL)
	//	{
	//		m_apScene2D[nCntNumber]->Draw();
	//	}
	//}

}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CForce::SetForce(int nCntForce, FORCETYPE type)
{
	m_apScene2D[nCntForce]->BindTexture(m_apTexture[type]);

	if (m_type[nCntForce] != type)
	{
		switch (m_type[nCntForce])
		{
		case FORCETYPE_ATTACK:
			if (0 < m_nNumAttack)m_nNumAttack--;
			break;
		case FORCETYPE_SPREAD:
			if (0 < m_nNumSpread)m_nNumSpread--;
			break;
		case FORCETYPE_SPEED:
			if (0 < m_nNumSpeed)m_nNumSpeed--;
			break;
		case FORCETYPE_DEFENSE:
			if (0 < m_nNumDefense)m_nNumDefense--;
			break;
		}

		switch (type)
		{
		case FORCETYPE_ATTACK:
			if (m_nNumAttack < 4)m_nNumAttack++;
			break;
		case FORCETYPE_SPREAD:
			if (m_nNumSpread < 4)m_nNumSpread++;
			break;
		case FORCETYPE_SPEED:
			if (m_nNumSpeed < 4)m_nNumSpeed++;
			break;
		case FORCETYPE_DEFENSE:
			if (m_nNumDefense < 4)m_nNumDefense++;
			break;
		}

		m_type[nCntForce] = type;
	}
}

//=============================================================================
//�U�t�H�[�X�̐��̎擾
//=============================================================================
int  CForce::GetNumAttack(void)
{
	return m_nNumAttack;
}

//=============================================================================
//�g�t�H�[�X�̐��̎擾
//=============================================================================
int  CForce::GetNumSpread(void)
{
	return m_nNumSpread;

}

//=============================================================================
//���t�H�[�X�̐��̎擾
//=============================================================================
int  CForce::GetNumSpeed(void)
{
	return m_nNumSpeed;

}

//=============================================================================
//��t�H�[�X�̐��̎擾
//=============================================================================
int  CForce::GetNumDefense(void)
{
	return m_nNumDefense;

}


//============================================================================================
//DPS�`�F�b�N
//============================================================================================
void CForce::CheckDamage(int nDamage)
{

	m_nDamage = nDamage;

	if (m_nDamage >= 1)
	{
		m_nDPS = m_nDamage;
	}
	if (m_nDamage <= 0)
	{
		m_nDPS = 0;
	}


}


//============================================================================================
//DPS�擾
//============================================================================================
int CForce::GetnCnt()
{
	return m_nCntDPS;
}


