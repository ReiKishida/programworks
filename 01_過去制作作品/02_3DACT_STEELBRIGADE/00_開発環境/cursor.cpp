//=============================================================================
//
// �J�[�\������ [Cursor.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "cursor.h"
#include "input.h"
#include "game.h"
#include "effect.h"
#include "sound.h"
#include "topics.h"
#include "fade.h"
#include "inputx.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define CURSOR_X (15.0f)	//X������
#define CURSOR_Y (30.0f)	//Y������

#define LIMIT_MIN_X (30)	//��ʒ[-X
#define LIMIT_MAX_X (1250)	//��ʒ[X
#define LIMIT_MIN_Y (25)	//��ʒ[-Y
#define LIMIT_MAX_Y (680)	//��ʒ[Y

#define MOVE (1.5f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCursor = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffcursor = NULL;	//���_�o�b�t�@�ւ̃|�C���^
CURSOR g_Cursor;	//�J�[�\��
int nCoolTime;
int nWeapon_Select;	//����
JoyState g_JoyStateCursur;

//=============================================================================
// �J�[�\����������
//=============================================================================
void InitCursor(void)
{

	InitTopics();

	//�Q�[���p�b�h�̏��
	g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Cursor.pos = D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT /2, 0.0f);//�J�[�\�����S���W
	g_Cursor.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
	//g_Cursor.nIndex = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor.png", &g_pTextureCursor);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffcursor,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W

	pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x, g_Cursor.pos.y - CURSOR_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x, g_Cursor.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y, 0.0f);


	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffcursor->Unlock();

	g_Cursor.bUpdate = true;
}
//=============================================================================
// �J�[�\����������
//=============================================================================
void UnInitCursor(void)
{
	UnInitTopics();


	//�e�N�X�`���̔j��
	if (g_pTextureCursor != NULL)
	{
		g_pTextureCursor->Release();
		g_pTextureCursor = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffcursor != NULL)
	{
		g_pVtxBuffcursor->Release();
		g_pVtxBuffcursor = NULL;
	}
}
//=============================================================================
// �J�[�\���X�V����
//=============================================================================
void UpdateCursor(void)
{


	UpdateTopics();

	//�Q�[���p�b�h��Ԏ擾
	JoyState pJoyState = GetJoystate();


	//���
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (g_Cursor.bUpdate == true)
	{
		int nReload = 0;
		VERTEX_2D *pVtx;			//���_���ւ̃|�C���^


		MODE *pMode;
		pMode = GetMode();



		g_Cursor.posold = g_Cursor.pos;




		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);



		//=========================================
		//�C�ӂ̃L�[(A�L�[)�������ꂽ���ǂ���    //
		//=========================================
		if (GetKeyboardPress(DIK_A) == true
			||(state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{

			if (GetKeyboardPress(DIK_W) == true
				|| (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//��L�[(W�L�[)�������ꂽ
				g_Cursor.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;

				if (g_Cursor.pos.y <= LIMIT_MIN_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true
				|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//���L�[(S�L�[)�������ꂽ
				g_Cursor.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;

				if (g_Cursor.pos.y >= LIMIT_MAX_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else
			{//���L�[(A�L�[)�̂�
				g_Cursor.move.x += sinf(-D3DX_PI * 0.5f) * MOVE;

			}

			if (g_Cursor.pos.x <= LIMIT_MIN_X)
			{
				g_Cursor.move.x = 0;
			}
		}

		//=========================================
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (GetKeyboardPress(DIK_D) == true
			|| (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{


			if (GetKeyboardPress(DIK_W) == true
				|| (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//��L�[(W�L�[)�������ꂽ
				g_Cursor.move.x += sinf(D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(D3DX_PI * 0.75f) * MOVE;

				if (g_Cursor.pos.y <= LIMIT_MIN_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true
				|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//���L�[(S�L�[)�������ꂽ
				g_Cursor.move.x += sinf(D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(D3DX_PI * 0.25f) * MOVE;

				if (g_Cursor.pos.y >= LIMIT_MAX_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else
			{//�E�L�[(D�L�[)�̂�
				g_Cursor.move.x += sinf(D3DX_PI * 0.5f) * MOVE;

			}
			if (g_Cursor.pos.x >= LIMIT_MAX_X)
			{
				g_Cursor.move.x = 0;
			}
		}




		//=========================================
		//�C�ӂ̃L�[(W�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (GetKeyboardPress(DIK_W) == true
			||(state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{

			g_Cursor.move.y += cosf(D3DX_PI) * MOVE;
			if (g_Cursor.pos.y <= LIMIT_MIN_Y)
			{
				g_Cursor.move.x = 0;
				g_Cursor.move.y = 0;
			}


		}


		//=========================================
		//�C�ӂ̃L�[(S�L�[)�������ꂽ���ǂ���    //
		//=========================================
		else if (GetKeyboardPress(DIK_S) == true
			|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{

			g_Cursor.move.y += cosf(-D3DX_PI * 0) * MOVE;

			if (g_Cursor.pos.y >= LIMIT_MAX_Y)
			{
				g_Cursor.move.x = 0;
				g_Cursor.move.y = 0;
			}
		}


		g_Cursor.pos.x += g_Cursor.move.x;
		g_Cursor.pos.y += g_Cursor.move.y;


		g_Cursor.move.x += (0.0f - g_Cursor.move.x) * 0.18f;
		g_Cursor.move.y += (0.0f - g_Cursor.move.y) * 0.18f;

		FADE pFade;
		pFade = *GetFade();

		TOPICS pTopics;
		pTopics = *GetTopics();


		//=========================================
		//�C�ӂ̃L�[(RETURN(Enter)�L�[)�������ꂽ���ǂ���    //
		//=========================================
		if (GetKeyboardTrigger(DIK_RETURN) == TRUE && g_Cursor.bUpdate == true
			|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true) && g_Cursor.bUpdate == true)
		{
			if (g_JoyStateCursur.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				if (CollisionTopics(&g_Cursor.pos, &g_Cursor.posold) == TOPICSTYPE_MAIL_0)
				{

					SetTopics(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 20.0f, 0.0f), TOPICSTYPE_MAIL_0_1, 3, SCREEN_HEIGHT / 2 + 350.0f, SCREEN_WIDTH / 2);

					g_Cursor.bUpdate = false;

				}
				if (CollisionTopics(&g_Cursor.pos, &g_Cursor.posold) == TOPICSTYPE_MAIL_1)
				{

					SetTopics(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 20.0f, 0.0f), TOPICSTYPE_MAIL_1_1, 4, SCREEN_HEIGHT / 2 + 350.0f, SCREEN_WIDTH / 2);

					g_Cursor.bUpdate = false;

				}
				if (CollisionTopics(&g_Cursor.pos, &g_Cursor.posold) == TOPICSTYPE_NEWS_0)
				{

					SetTopics(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 20.0f, 0.0f), TOPICSTYPE_NEWS_0_1, 5, SCREEN_HEIGHT / 2 + 350.0f, SCREEN_WIDTH / 2);

					g_Cursor.bUpdate = false;

				}
				if (CollisionTopics(&g_Cursor.pos, &g_Cursor.posold) == TOPICSTYPE_MISSION_0)
				{
					if (pFade == FADE_NONE)
					{
						//���[�h�I��
						SetFade(MODE_BRIEFING);
					}
				}
			}
			g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_PUSH;

		}


		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_X, g_Cursor.pos.y + CURSOR_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y + CURSOR_Y, 0.0f);


		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffcursor->Unlock();

	}//bUpdate��true�̊�
	else
	{
		if (GetKeyboardTrigger(DIK_RETURN) == TRUE && g_Cursor.bUpdate == false
			|| (state.Gamepad.wButtons &XINPUT_GAMEPAD_B && pJoyState.bConnectionJoypad == true) && g_Cursor.bUpdate == false)
		{
			if (g_JoyStateCursur.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				g_Cursor.bUpdate = true;
			}
			g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}

	if (g_JoyStateCursur.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateCursur.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}


//=============================================================================
// �J�[�\���`�揈��
//=============================================================================
void DrawCursor(void)
{
	DrawTopics();

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

							  //�f�o�C�X���擾����
	pDevice = GetDevice();



		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffcursor, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`��
		pDevice->SetTexture(0, g_pTextureCursor);


		//�J�[�\���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);

}

CURSOR GetCursor000(void)
{
	return g_Cursor;
}



