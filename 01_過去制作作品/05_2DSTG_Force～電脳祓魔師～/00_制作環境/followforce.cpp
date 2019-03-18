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
#include "followforce.h"
#include "polygon.h"
#include "input.h"
#include "effect.h"
#include "game.h"
#include "tutorial.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FORCE_WIDTH (150)	//�|���S���̕�
#define FORCE_HEIGHT (150)	//�|���S����Y�̍���
#define FORCE_RADIUS (67.0f)//�t�H�[�X�̔��a
#define FORCE_ROTATE (0.1f)//�t�H�[�X�̉�]��
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFollowForce::m_apTexture[MAX_FOLLOWFORCE] = {}; //���L�e�N�X�`���̃|�C���^
CScene2D *CFollowForce::m_apPolygon[MAX_FOLLOWFORCE] = {};	//�|���S���̃|�C���^
int CFollowForce::m_nNumAttack = 0;	//�U�̃t�H�[�X��
int CFollowForce::m_nNumSpread = 0;	//�g�̃t�H�[�X��
int CFollowForce::m_nNumSpeed = 0;	//���̃t�H�[�X��
int CFollowForce::m_nNumDefense = 0;	//��̃t�H�[�X��
bool CFollowForce::m_bUse = true;//�g�p���Ă��邩
JoyState g_JoyStateFollowForce;


//=============================================================================
//�R���X�g���N�^
//=============================================================================
CFollowForce::CFollowForce() : CScene(5)
{//�N���A�Ȓl����
	m_nCnt = 0;
	m_ForceRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	{
		fLengthForce[nCntNumber] = 0.0f;
	}
	fAngleForce = 0.0f;
	m_bUse = true;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CFollowForce::~CFollowForce()
{


}
//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CFollowForce::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/atk.png", &m_apTexture[FOLLOWFORCETYPE_ATTACK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spr.png", &m_apTexture[FOLLOWFORCETYPE_SPREAD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spd.png", &m_apTexture[FOLLOWFORCETYPE_SPEED]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/def.png", &m_apTexture[FOLLOWFORCETYPE_DEFENSE]);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CFollowForce::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_FOLLOWFORCE; nCntTex++)
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
CFollowForce *CFollowForce::Create(D3DXVECTOR3 pos)
{
	//�X�R�A�̐���
	CFollowForce *pScore;
	pScore = new CFollowForce;

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
HRESULT CFollowForce::Init(D3DXVECTOR3 pos)
{
	m_nCnt = 0;
	m_ForceRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = true;
	//�Q�[���p�b�h�̏��
	g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	for (int nCntScene2D = 0; nCntScene2D < MAX_FOLLOWFORCE; nCntScene2D++)
	{
		m_aForceStruct[nCntScene2D].type = FOLLOWFORCETYPE_NONE;

		m_aForceStruct[nCntScene2D].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//2D�|���S���̐���
		m_apPolygon[nCntScene2D] = new CScene2D;
		if (m_apPolygon[nCntScene2D] != NULL)
		{
			//�w�i�̕��A�����ݒ�
			m_apPolygon[nCntScene2D]->SetScene2DWidth(34.0f);
			m_apPolygon[nCntScene2D]->SetScene2DHeight(34.0f);


			//�e�N�X�`���̊��蓖��
			m_apPolygon[nCntScene2D]->BindTexture(m_apTexture[FOLLOWFORCETYPE_ATTACK]);

			//����������
			m_apPolygon[nCntScene2D]->Init(D3DXVECTOR3(sinf(m_ForceRot.x + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntScene2D) * FORCE_RADIUS + pos.x
												     , cosf(m_ForceRot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntScene2D) * FORCE_RADIUS + pos.y
													 , pos.z));

			//�e�N�X�`�����W�ݒ�
			m_apPolygon[nCntScene2D]->SetTexture(1.0f, 1.0f);

			//�t�H�[�X�̐ݒ�
			SetForce(nCntScene2D, FOLLOWFORCETYPE_ATTACK);

			//�I�u�W�F�N�g�̃^�C�v�ݒ�
			m_apPolygon[nCntScene2D]->SetObjType(CScene::OBJTYPE_FORCE);


		}
	}
	return S_OK;
}


//=============================================================================
//�I������
//=============================================================================
void CFollowForce::Uninit(void)
{



	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	{
		if (m_apPolygon[nCntNumber] != NULL)
		{
			//2D�V�[���̏I������
			m_apPolygon[nCntNumber]->Uninit();
			m_apPolygon[nCntNumber] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�X�V����
//=============================================================================
void CFollowForce::Update(void)
{
	CManager::MODE mode;
	mode = CManager::GetMode();

#if 1
	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();



	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);




	//�v���C���[�̎擾
	CPlayer *pPlayer;
	D3DXVECTOR3 PosPlayer;
	if (mode == CManager::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		PosPlayer = pPlayer->GetPosScene2D();

	}
	if (mode == CManager::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		PosPlayer = pPlayer->GetPosScene2D();
	}

	D3DXVECTOR3 posForce[MAX_FOLLOWFORCE];
	m_bUse = CPlayer::GetbUse();
	CPlayer::PLAYERSTATE PlayerState = CPlayer::GetState();

	//�G�t�F�N�g�̐F�ݒ�
	for (int nCntForce = 0; nCntForce < MAX_FOLLOWFORCE; nCntForce++)
	{
		switch (m_aForceStruct[nCntForce].type)
		{
		case FOLLOWFORCETYPE_ATTACK:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_SPREAD:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_SPEED:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case FOLLOWFORCETYPE_DEFENSE:
			m_aForceStruct[nCntForce].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f);
			break;
		}

		//�t�H�[�X�̈ʒu�擾
		posForce[nCntForce] = m_apPolygon[nCntForce]->GetPosScene2D();

		if (PlayerState == CPlayer::PLAYERSTATE_APPEAR || PlayerState == CPlayer::PLAYERSTATE_NORMAL)
		{
			//�G�t�F�N�g�̐���
			CEffect::Create(posForce[nCntForce], m_aForceStruct[nCntForce].col, 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
		}
		else if (PlayerState == CPlayer::PLAYERSTATE_HACK)
		{
			//�G�t�F�N�g�̐���
			CEffect::Create(posForce[0], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[1], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[2], D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);
			CEffect::Create(posForce[3], D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f), 24.0f, 24.0f, 7, CEffect::EFFECTTYPE_BULLET);

		}
		if (m_bUse == true)
		{
			//�v���C���[�̈ʒu���
			posForce[nCntForce] = (D3DXVECTOR3(sinf(m_ForceRot.x + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntForce) * FORCE_RADIUS + PosPlayer.x
				, cosf(m_ForceRot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_FOLLOWFORCE) * nCntForce) * FORCE_RADIUS + PosPlayer.y
				, PosPlayer.z));
		}
		else
		{
			posForce[nCntForce] = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
		}
	}
	float fForceRotate = 0.1f + (0.014f * m_nNumDefense);
	//�t�H�[�X�̉�]
	m_ForceRot.x += fForceRotate;
	m_ForceRot.y += fForceRotate;

	//�J�E���g�̃��Z�b�g
	m_nCnt = m_nCnt % MAX_FOLLOWFORCE;
#if 1
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true))
	{//�U�̃t�H�[�X�̐ݒu
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_ATTACK);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
	{//�g�̃t�H�[�X�ݒu
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_SPREAD);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_X && pJoyState.bConnectionJoypad == true))
	{//���̃t�H�[�X�ݒu
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_SPEED);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_Y && pJoyState.bConnectionJoypad == true))
	{//��̃t�H�[�X�ݒu
		if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetForce(m_nCnt, FOLLOWFORCETYPE_DEFENSE);
			m_nCnt++;
		}
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_PUSH;

	}
#endif
#if 0
	if (pInputKeyboard->GetKeyboardTrigger(DIK_1) == true )
	{//�U�̃t�H�[�X�̐ݒu
		SetForce(m_nCnt, FOLLOWFORCETYPE_ATTACK);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true )
	{//�g�̃t�H�[�X�ݒu

		SetForce(m_nCnt, FOLLOWFORCETYPE_SPREAD);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_3) == true )
	{//���̃t�H�[�X�ݒu
		SetForce(m_nCnt, FOLLOWFORCETYPE_SPEED);
		m_nCnt++;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_4) == true )
	{//��̃t�H�[�X�ݒu
		SetForce(m_nCnt, FOLLOWFORCETYPE_DEFENSE);
		m_nCnt++;
	}
#endif
	if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateFollowForce.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateFollowForce.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
	for (int nCntForce = 0; nCntForce < MAX_FOLLOWFORCE; nCntForce++)
	{
		m_apPolygon[nCntForce]->SetPosScene2D(posForce[nCntForce]);
	}
#endif
}

//=============================================================================
//�`�揈��
//=============================================================================
void CFollowForce::Draw(void)
{
	//if (m_bUse == true)
	//{
	//	for (int nCntNumber = 0; nCntNumber < MAX_FOLLOWFORCE; nCntNumber++)
	//	{
	//		if (m_apPolygon[nCntNumber] != NULL)
	//		{
	//			m_apPolygon[nCntNumber]->Draw();
	//		}
	//	}
	//}

}

//=============================================================================
//�X�R�A�̐ݒ�
//=============================================================================
void CFollowForce::SetForce(int nCntForce, FOLLOWFORCETYPE type)
{
	m_apPolygon[nCntForce]->BindTexture(m_apTexture[type]);

	if (m_aForceStruct[nCntForce].type != type)
	{
		switch (m_aForceStruct[nCntForce].type)
		{
		case FOLLOWFORCETYPE_ATTACK://�U�̃t�H�[�X
			if (0 < m_nNumAttack)m_nNumAttack--;
			break;
		case FOLLOWFORCETYPE_SPREAD://�g�̃t�H�[�X
			if (0 < m_nNumSpread)m_nNumSpread--;
			break;
		case FOLLOWFORCETYPE_SPEED://���̃t�H�[�X
			if (0 < m_nNumSpeed)m_nNumSpeed--;
			break;
		case FOLLOWFORCETYPE_DEFENSE://��̃t�H�[�X
			if (0 < m_nNumDefense)m_nNumDefense--;
			break;
		}

		switch (type)
		{
		case FOLLOWFORCETYPE_ATTACK://�U�̃t�H�[�X
			if (m_nNumAttack < 4)m_nNumAttack++;
			break;
		case FOLLOWFORCETYPE_SPREAD://�g�̃t�H�[�X
			if (m_nNumSpread < 4)m_nNumSpread++;
			break;
		case FOLLOWFORCETYPE_SPEED://���̃t�H�[�X
			if (m_nNumSpeed < 4)m_nNumSpeed++;
			break;
		case FOLLOWFORCETYPE_DEFENSE://��̃t�H�[�X
			if (m_nNumDefense < 4)m_nNumDefense++;
			break;
		}

		m_aForceStruct[nCntForce].type = type;
		m_apPolygon[nCntForce]->SetForce(type);
	}





}

