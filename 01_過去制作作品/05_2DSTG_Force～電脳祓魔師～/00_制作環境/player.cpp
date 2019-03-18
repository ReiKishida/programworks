//=============================================================================
//
// �v���C���[����[2D�|���S��] [player.cpp]
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
#include "effect.h"
#include "sound.h"
#include "force.h"
#include "followforce.h"
#include "explosion.h"
#include "fade.h"
#include "remaining.h"
#include "bg.h"
#include "score.h"
#include "particle.h"
#include "game.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_WIDTH (50.0f)	//�|���S���̕�
#define PLAYER_HEIGHT (50.0f)	//�|���S����Y�̍���
#define PLAYER_MOVE (0.8f)	//�v���C���[�̈ړ���
#define SCREEN_LIMIT_MAX_X (1070.0f - PLAYER_WIDTH)	//��ʒ[(�E�[)
#define SCREEN_LIMIT_MIN_X (210.0f + PLAYER_WIDTH)			//���(���[)

#define SCREEN_LIMIT_MAX_Y ((SCREEN_HEIGHT + 15.0f) - PLAYER_WIDTH)	//���(���[)
#define SCREEN_LIMIT_MIN_Y (-15.0f + PLAYER_WIDTH)			//���(��[)

#define BULLET_LIFE (70)//�e�̎���
#define BULLET_VEROCITY (18.0f)//�e�̈ړ���
#define PATTERN_ANIM (4)	//�e�N�X�`���p�^�[����

#define ANIMATION_SPEED (10)	//�A�j���[�V�����X�s�[�h
#define TEX_X (0.25f)	//�e�N�X�`�����W_X
#define TEX_Y (0.34f)//�e�N�X�`�����W_Y


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL; //���L�e�N�X�`���̃|�C���^
int CPlayer::m_nLife = 0; //���C�t
int CPlayer::m_nRespawn = 0; //���X�|�[������
D3DXVECTOR3 CPlayer::m_pos = {};//�ʒu���
D3DXCOLOR CPlayer::m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�v���C���[�̐F���
bool CPlayer::m_bUse = true;//�g�p���Ă��邩
CPlayer::PLAYERSTATE CPlayer::m_Pstate = PLAYERSTATE_NONE;//�v���C���[�̏��
JoyState g_JoyStatePlayer;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene2D(6)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_Pstate = PLAYERSTATE_NONE;
	m_nRespawn = 120;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//�����_���[�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);//�v���C���[

	return S_OK;
}

//=============================================================================
//�e�N�X�`���̔j��
//=============================================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}



//=============================================================================
//�v���C���[�̐���
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	//�v���C���[�̃C���X�^���X����
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		//���C�t
		pPlayer->m_nLife = 1;
		//�v���C���[�̈ړ���
		pPlayer->m_fPlayerMove = 0.3f;

		//�v���C���[�̏���������
		pPlayer->Init(pos);

	}

	return pPlayer;
}


//=============================================================================
//����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	//�����Ă����Ԃ�
	m_bUse = true;
	m_Pstate = PLAYERSTATE_NORMAL;

	//�A�j���[�V�����J�E���^�[
	m_nCounterAnim = 0;
	m_nTex_Y = 0;
	m_nRespawn = 120;

	//�v���C���[�̕��A�����ݒ�
	SetScene2DWidth(PLAYER_WIDTH);
	SetScene2DHeight(PLAYER_HEIGHT);

	//�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);
	SetLife(m_nLife);
	m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//���[�u�ʏ�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//2D�V�[���̏���������
	CScene2D::Init(pos);
	SetTexture(TEX_X, TEX_Y);
	//�v���C���[�̃I�u�W�F�N�g�̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	//�Q�[���p�b�h�̏��
	g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	m_nCntFrame = 0;
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//2D�V�[���̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�X�V����
//=============================================================================
void CPlayer::Update(void)
{

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();

	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	D3DXVECTOR3 movePlayer; //�v���C���[�̈ړ���
	m_pos = GetPosScene2D();	//�v���C���[�̈ʒu���
	D3DXVECTOR3 rotPlayer = CScene2D::GetRotScene2D();	//�v���C���[�̊p�x���

	int nNumSpeedForce; //���̃t�H�[�X��
	int nNumSpreadForce; //�g�̃t�H�[�X��
	int nNumAttackForce;//�U�̃t�H�[�X��
	float fAddMove;//�ړ��ʂ̑�����

	nNumSpeedForce = CForce::GetNumSpeed();	//���̃t�H�[�X��
	nNumSpreadForce = CForce::GetNumSpread();//�g�̃t�H�[�X��
	nNumAttackForce = CForce::GetNumAttack();//�U�̃t�H�[�X��
	int nNumScene = CScene::GetNumAll();	//�I�u�W�F�N�g�̐��̎擾


	switch (m_Pstate)
	{
	case PLAYERSTATE_NORMAL:
		m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_bUse = true;

		m_nAttack = 1 + nNumAttackForce * 2;
		fAddMove = (0.17f * nNumSpeedForce); //�ړ��ʂ̉��Z��
		m_nSpread = nNumSpreadForce;

		break;


	case PLAYERSTATE_HACK:
		m_nAttack = 5;//�U����
		fAddMove = 0.8f; //�ړ��ʂ̉��Z��
		nNumSpreadForce = 4;//�g�U��
		nNumSpeedForce = 3;//���̃t�H�[�X��
		m_nRespawn--;
		CScore::AddScore(10);
		m_PlayerCol = D3DXCOLOR(0.7f, 0.0f, 1.0f, 1.0f);

		if (m_nRespawn <= 0)
		{
			m_Pstate = PLAYERSTATE_NORMAL;
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CBg::SetFade(CBg::BGTYPE_PLAYERNORMAL);

			m_nRespawn = 0;
		}
		break;

	case PLAYERSTATE_APPEAR:
		m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		m_bUse = true;
		m_nAttack = 1 + nNumAttackForce * 2;
		fAddMove = (0.17f * nNumSpeedForce); //�ړ��ʂ̉��Z��
		m_nSpread = nNumSpreadForce;
		m_nRespawn--;
		//if (pInputKeyboard->GetKeyboardTrigger(DIK_Y) == true)
		//{//R�L�[�������ꂽ�Ƃ�
		//	CBg::SetFade(CBg::BGTYPE_HACK_PLAYER);
		//	m_nRespawn = 600;
		//}
		if (m_nRespawn <= 0)
		{
			m_Pstate = PLAYERSTATE_NORMAL;
			m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			m_nRespawn = 0;
		}
		break;


	case PLAYERSTATE_DEATH:
		m_nRespawn--;//���X�|�[�����Ԍ���
		fAddMove = 0.0f;
		if (m_nRespawn <= 0)
		{
			m_bUse = true;
			m_Pstate = PLAYERSTATE_APPEAR;
			m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 300.0f, 0.0f);
			m_PlayerCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			m_nRespawn = 180;
		}
		break;
	}

	if (m_Pstate == PLAYERSTATE_APPEAR || m_Pstate == PLAYERSTATE_NORMAL || m_Pstate == PLAYERSTATE_HACK)
	{
		//===========================================================================
		//�v���C���[�̈ړ�
		//===========================================================================
		//=========================================
		//�C�ӂ̃L�[(A�L�[)�������ꂽ���ǂ���    //
		//=========================================
		if (pInputKeyboard->GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//A�L�[�������ꂽ�Ƃ�
			m_nTex_Y = 1;//���ړ�
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//��L�[(W�L�[)�������ꂽ
				m_move.x += sinf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//���L�[(S�L�[)�������ꂽ
				m_move.x += sinf(-D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(-D3DX_PI * 0.25f) * (m_fPlayerMove + fAddMove);
			}
			else
			{//���L�[(A�L�[)�̂�
				m_move.x += sinf(-D3DX_PI * 0.5f) * (m_fPlayerMove + fAddMove);
			}
		}
		//=========================================
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{
			m_nTex_Y = 2;//���ړ�
			if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//��L�[(W�L�[)�������ꂽ
				m_move.x += sinf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);

			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//���L�[(S�L�[)�������ꂽ
				m_move.x += sinf(D3DX_PI * 0.75f) * (m_fPlayerMove + fAddMove);
				m_move.y += cosf(D3DX_PI * 0.25f) * (m_fPlayerMove + fAddMove);

			}
			else
			{//�E�L�[(D�L�[)�̂�
				m_move.x += sinf(D3DX_PI * 0.5f) * (m_fPlayerMove + fAddMove);
			}
		}
		//=========================================
		//�C�ӂ̃L�[(W�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//W�L�[�������ꂽ�Ƃ�
			m_nTex_Y = 0;//�O�ړ�
			m_move.y += cosf(D3DX_PI) * (m_fPlayerMove + fAddMove);
		}
		//=========================================
		//�C�ӂ̃L�[(S�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{//S�L�[�������ꂽ�Ƃ�
			m_nTex_Y = 0;//���ړ�
			m_move.y += cosf(-D3DX_PI * 0) * (m_fPlayerMove + fAddMove);
		}
		else
		{
			m_nTex_Y = 0;//���ړ�
		}
		//========================================================================
		//�e�̐���
		//========================================================================
		if (pInputKeyboard->GetKeyboardPress(DIK_SPACE) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_SHOULDER && pJoyState.bConnectionJoypad == true))
		{
			m_nCntFrame++;
			if (m_nCntFrame % (16 - (nNumSpeedForce * 2)) == 0)
			{//���̃t�H�[�X���ɉ����Ĕ��˂܂ł̊Ԋu
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);

				switch (nNumSpreadForce)
				{//�g�̃t�H�[�X���ɉ�����
				case 0://�g�U�̃t�H�[�X��:0
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;

				case 1://�g�U�̃t�H�[�X��:1
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					break;
				case 2://�g�U�̃t�H�[�X��:2
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(4.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-4.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					break;

				case 3://�g�U�̃t�H�[�X��:3
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(3.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-3.5f, BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(D3DXVECTOR3(m_pos.x + 15.0f, m_pos.y, 0.0f), D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(D3DXVECTOR3(m_pos.x - 15.0f, m_pos.y, 0.0f), D3DXVECTOR3(0.0f, -BULLET_VEROCITY, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;

				case 4://�g�U�̃t�H�[�X��:4
					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(3.5f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-3.5f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(8.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-8.0f, 14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);

					CBullet::Create(m_pos, D3DXVECTOR3(0.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(5.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					CBullet::Create(m_pos, D3DXVECTOR3(-5.0f, -14.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, BULLET_LIFE, m_nAttack);
					break;
				}//(��)�g�̃t�H�[�X���ɉ�����
			}//(��)���̃t�H�[�X���ɉ����Ĕ��˂܂ł̊Ԋu
		}
	}

	if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}


	//�ʒu������
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;


	//���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	//=============================================================================
	//�G�Ƃ̓����蔻��
	//=============================================================================
	if (mode == CManager::MODE_GAME)
	{//���[�h���Q�[���Ȃ�
		for (int nCntPriority = 0; nCntPriority < PRIORITY_NUMBER; nCntPriority++)
		{//�D�揇�ʂ�for��
			for (int nCntScene = 0; nCntScene < nNumScene; nCntScene++)
			{//nNumScene = �I�u�W�F�N�g�̑���
			 //�V�[���̎擾
				CScene2D *pScene2D;
				pScene2D = (CScene2D*)GetScene(nCntPriority, nCntScene);

				if (pScene2D != NULL)
				{//pScene2D���󂶂�Ȃ����
				 //�|���S���̏��
					D3DXVECTOR3 posScene2D = pScene2D->GetPosScene2D();	//�G�̈ʒu���
					//�I�u�W�F�N�g�̎�ނ̎擾
					CScene::OBJTYPE type;
					type = pScene2D->GetObjType();
					if (type == CScene::OBJTYPE_ENEMY)
					{//if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
						if (m_pos.x >= posScene2D.x - pScene2D->GetWidthScene2D()
							&& m_pos.x <= posScene2D.x + pScene2D->GetWidthScene2D()
							&& m_pos.y >= posScene2D.y - pScene2D->GetHeightScene2D()
							&& m_pos.y <= posScene2D.y + pScene2D->GetHeightScene2D()
							)
						{//�͈͓��Ȃ�
							HitDamage(1);
						}//(��)�͈͓��Ȃ�
					}//(��)if:�I�u�W�F�N�g�̎�ނ��G�Ȃ�
				}//(��)pScene2D���󂶂�Ȃ����
			}//(��)nNumScene = �I�u�W�F�N�g�̑���
		}//(��)�D�揇�ʂ�for��
	} //(��)���[�h���Q�[���Ȃ�

	//�����ړ�
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;


	//=============================================================================
	//��ʒ[�̏���
	//=============================================================================
	if (m_pos.x >= SCREEN_LIMIT_MAX_X)
	{//��ʉE�[
		m_pos.x = SCREEN_LIMIT_MAX_X;
	}
	if (m_pos.x <= SCREEN_LIMIT_MIN_X)
	{//��ʍ��[
		m_pos.x = SCREEN_LIMIT_MIN_X;
	}
	if (m_pos.y >= SCREEN_LIMIT_MAX_Y)
	{//��ʉ��[
		m_pos.y = SCREEN_LIMIT_MAX_Y;
	}
	if (m_pos.y <= SCREEN_LIMIT_MIN_Y)
	{//��ʏ�[
		m_pos.y = SCREEN_LIMIT_MIN_Y;
	}


	m_nCounterAnim++;//�A�j���[�V�����J�E���^�[�̉��Z
	if (m_nCounterAnim % ANIMATION_SPEED == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//�p�^�[��No.�X�V

		//�e�N�X�`���A�j���[�V����
		CScene2D::SetTextureAnimY(TEX_X, TEX_Y, m_nPatternAnim, m_nTex_Y);
	}

	//�ʒu���̐ݒ�
	CScene2D::SetPosScene2D(m_pos);

	//�p�x���̐ݒ�
	CScene2D::SetRotScene2D(rotPlayer);

	//�F���̐ݒ�
	CScene2D::SetColor(m_PlayerCol);
}

//=============================================================================
//�`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_bUse == true)
	{
		//2D�V�[���̕`�揈��
		CScene2D::Draw();
	}
}
//=============================================================================
//�_���[�W����
//=============================================================================
void CPlayer::HitDamage(int nValue)
{
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	if (m_Pstate == PLAYERSTATE_NORMAL)
	{
		//�p�[�e�B�N���̐���
		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 15);
			CParticle::Create(m_pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), CParticle::PARITCLETYPE_EXPLOSION, 15);
		}
		m_bUse = false;
		m_Pstate = PLAYERSTATE_DEATH;
		m_nRespawn = 180;
		CRemaining::DecreseRemaining(-1);
		int nRemaining = CRemaining::GetRemaining();
		if (nRemaining < 0)
		{
			if (pFade == CFade::FADE_NONE)
			{


				CGame::SetGameState(CGame::GAMESTATE_FAILED);
			}
		}

	}
}

//=============================================================================
//�g�p���Ă��邩���擾
//=============================================================================
bool CPlayer::GetbUse()
{
	return m_bUse;
}

//=============================================================================
//�v���C���[�̏�Ԏ擾
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetState(void)
{
	return m_Pstate;
}

//=============================================================================
//�v���C���[�̏�Ԑݒ�
//=============================================================================
void CPlayer::SetState(PLAYERSTATE state)
{
	m_Pstate = state;
}

//=============================================================================
//�v���C���[�̈ʒu�擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
