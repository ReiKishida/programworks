////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �|�[�Y�̏��� [pause.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "gamepad.h"
#include "tutorial.h"
//******************************************************************************************************************************
// �\���̒�`
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ���S���W
	D3DXCOLOR col;				// �F
	PAUSETYPE nType;			// ���
	float fWidth;				// ��
	float fHeight;				// ����
} PauseInfo;
typedef struct
{
	char *pFileName;
}PauseTexture;
//******************************************************************************************************************************
// �}�N����`
//******************************************************************************************************************************
#define PAUSESELECT_MAX	(3)
//******************************************************************************************************************************
// �O���[�o���ϐ�
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSETYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
PAUSE g_aPause[PAUSETYPE_MAX];
int g_nSelect;
bool g_bSelect;
int g_nPressCounter;
PauseInfo g_aPauseInfo[] =
{
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), PAUSETYPE_BG,SCREEN_WIDTH,SCREEN_HEIGHT },			// �w�i
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSETYPE_PAUSE ,SCREEN_WIDTH ,SCREEN_HEIGHT },	// �|�[�Y�y��
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 320.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_CONTINUE ,290,70 },		// �ĊJ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 260, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_RETRY ,380,90 },				// ������x�x��
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_QUIT,380,90 },				// �^�C�g���֖߂�
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 353, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_PUNCH,90,130 },// �p���`->�p���`
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 177, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_PUNCH_PUNCH,90,130 },// �p���`->�p���`->�p���`
	{ D3DXVECTOR3(SCREEN_WIDTH / 2		, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_SPIN,90,130 },// �p���`->�X�s��
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 173, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_SPIN_JUMP,90,130 },// �p���`->�X�s��->�W�����v
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_KICK,90,130 },// �L�b�N->�L�b�N
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 176, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_KICK_KICK,90,130 },// �L�b�N->�L�b�N->�L�b�N
	{ D3DXVECTOR3(SCREEN_WIDTH / 2		, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_SPIN,90,130 },// �L�b�N->�X�s��
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 173, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_JUMP,90,130 },// �L�b�N->�W�����v
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 335, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_JUMP_PUNCH,90,130 },// �L�b�N->�W�����v->�p���`	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 355, SCREEN_HEIGHT / 2 + 82 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_PUNCH,90,130 },// �X�s��->�p���`
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 + 80 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_KICK,90,130 },// �X�s��->�L�b�N
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 5  , SCREEN_HEIGHT / 2 + 80 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_PUNCH_KICK,90,130 },// �X�s��->�p���`->�L�b�N	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_KICK,90,130 },// �W�����v->�L�b�N	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 173, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_SPIN,90,130 },// �W�����v->�X�s��
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 2, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_PUNCH,90,130 },// �W�����v->�p���`
};
PauseTexture g_aPauseTex[PAUSETYPE_MAX] =
{
	{ "data / TEXTURE / " },	// �ǂݍ��ރe�N�X�`��
	{ "data/TEXTURE/pauseBG.jpg" },	// �ǂݍ��ރe�N�X�`��1
	{ "data/TEXTURE/menu000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/menu001.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/menu002.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
	{ "data/TEXTURE/Lock000.png" },		// �ǂݍ��ރe�N�X�`��2
};
//==============================================================================================================================
// �����������i�|���S���j
//==============================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D*pVtx;				// ���_���ւ̃|�C���^
								// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nTex = 0; nTex < PAUSETYPE_MAX; nTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			g_aPauseTex[nTex].pFileName,
			&g_apTexturePause[nTex]);
	}
	// �w�i�̏��̏�����
	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �������W
		g_aPause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// �F
		g_aPause[nCntPause].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �w�i�̈ړ���
		g_aPause[nCntPause].nType = 0;								// �^�C�v
		g_aPause[nCntPause].nCounterAnim = 0;						// �J�E���^�[
		g_aPause[nCntPause].nPatternAnim = 0;						// �p�^�[��No
		g_aPause[nCntPause].fWidth = 0;								// ��
		g_aPause[nCntPause].fHeight = 0;							// ����
		g_aPause[nCntPause].bUse = false;							// �g�p����Ă��邩�ǂ���
	}
	g_nSelect = 0;
	g_bSelect = false;
	g_nPressCounter = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// rhw�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	// ���̃Z�b�g
	for (int nCntPause = 0; nCntPause < sizeof g_aPauseInfo / sizeof(PauseInfo); nCntPause++)
	{
		SetPause(g_aPauseInfo[nCntPause].pos, g_aPauseInfo[nCntPause].col, g_aPauseInfo[nCntPause].nType, g_aPauseInfo[nCntPause].fWidth, g_aPauseInfo[nCntPause].fHeight);
	}
}
//==============================================================================================================================
// �I�������i�|���S���j
//==============================================================================================================================
void UninitPause(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < PAUSETYPE_MAX; nCntTex++)
	{
		if (g_apTexturePause[nCntTex] != NULL)
		{
			g_apTexturePause[nCntTex]->Release();
			g_apTexturePause[nCntTex] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//==============================================================================================================================
// �X�V�����i�|���S���j
//==============================================================================================================================
void UpdatePause(void)
{
	VERTEX_2D*pVtx;
	FADE pFADE;
	pFADE = GetFade();

	if (pFADE == FADE_NONE)
	{
		if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_CONTINUE)
		{// continu
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// �Q�[���Ȃ�
					ReleaseGamePause();
				}
			
			}
		}
		else if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_RETRY)
		{// retry
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// �Q�[���Ȃ�
					SetFade(MODE_GAME);
				}
				if (GetMode() == MODE_TUTORIAL)
				{// �`���[�g���A���Ȃ�
					SetFade(MODE_TUTORIAL);
				}
			}
		}
		else if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_QUIT)
		{// quit
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{

				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// �Q�[���Ȃ�
					SetFade(MODE_TITLE);
					SetGameState(GAMESTATE_NONE);
				}
				
			}
		}
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetGamePadStickTrigger(ANALOG_STICK_DOWN) == true)
	{// ���Ɉړ�(�P�́j
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = (g_nSelect + 1) % PAUSESELECT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true || GetGamePadStickTrigger(ANALOG_STICK_UP) == true)
	{// ��Ɉړ�(�P��)
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = (g_nSelect + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;
	}
	if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true || GetGamePadStickPress(ANALOG_STICK_DOWN) == true)
	{// ���Ɉړ�(�A��)
		g_nPressCounter++;
		if (g_nPressCounter >= 30)
		{
			if (g_nPressCounter % 10 == 0)
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				g_nSelect = (g_nSelect + 1) % PAUSESELECT_MAX;
			}
		}
	}
	else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true || GetGamePadStickPress(ANALOG_STICK_UP) == true)
	{// ��Ɉړ�(�A��)
		g_nPressCounter++;
		if (g_nPressCounter >= 30)
		{
			if (g_nPressCounter % 10 == 0)
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				g_nSelect = (g_nSelect + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;
			}
		}
	}
	if (GetKeyboardTrigger(DIK_W) == true || GetGamePadStickTrigger(ANALOG_STICK_UP) == true)
	{// ��������J�E���^�[���Z�b�g
		g_nPressCounter = 0;
	}
	else if (GetKeyboardTrigger(DIK_S) == true || GetGamePadStickTrigger(ANALOG_STICK_DOWN) == true)
	{// ��������J�E���^�[���Z�b�g
		g_nPressCounter = 0;
	}
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(BUTTON_START) == true)
	{
		g_aPause[PAUSETYPE_QUIT].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_aPause[PAUSETYPE_RETRY].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_aPause[PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = 0;
	}

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================================
// �`�揈���i�|���S���j
//==============================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
								// �f�o�C�X���擾����
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		if (g_aPause[nCntPause].bUse == true)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexturePause[g_aPause[nCntPause].nType]);
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntPause * 4,
				2);
		}
	}
}
//==============================================================================================================================
// �|�[�Y�̐ݒ�
//==============================================================================================================================
void SetPause(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;

	// ���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		if (g_aPause[nCntPause].bUse == false)
		{// �w�i���g�p����Ă��Ȃ�
			g_aPause[nCntPause].pos = pos;
			g_aPause[nCntPause].Initpos = pos;
			g_aPause[nCntPause].col = col;
			g_aPause[nCntPause].nType = nType;
			g_aPause[nCntPause].fWidth = fWidth;
			g_aPause[nCntPause].fHeight = fHeight;

			// ���_���̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;

			g_aPause[nCntPause].bUse = true;	// �g�p���Ă����Ԃɂ���
			break;
		}
	}
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================================
// �|�[�Y�̊l��
//==============================================================================================================================
PAUSE *GetPauseData(void)
{
	return &g_aPause[0];
}
