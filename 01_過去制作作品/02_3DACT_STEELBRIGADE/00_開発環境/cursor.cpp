//=============================================================================
//
// カーソル処理 [Cursor.cpp]
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
// マクロ定義
//*****************************************************************************

#define CURSOR_X (15.0f)	//X軸半分
#define CURSOR_Y (30.0f)	//Y軸半分

#define LIMIT_MIN_X (30)	//画面端-X
#define LIMIT_MAX_X (1250)	//画面端X
#define LIMIT_MIN_Y (25)	//画面端-Y
#define LIMIT_MAX_Y (680)	//画面端Y

#define MOVE (1.5f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCursor = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffcursor = NULL;	//頂点バッファへのポインタ
CURSOR g_Cursor;	//カーソル
int nCoolTime;
int nWeapon_Select;	//武装
JoyState g_JoyStateCursur;

//=============================================================================
// カーソル生成処理
//=============================================================================
void InitCursor(void)
{

	InitTopics();

	//ゲームパッドの状態
	g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//デバイスの取得
	pDevice = GetDevice();

	g_Cursor.pos = D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT /2, 0.0f);//カーソル中心座標
	g_Cursor.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
	//g_Cursor.nIndex = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor.png", &g_pTextureCursor);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffcursor,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標

	pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x, g_Cursor.pos.y - CURSOR_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x, g_Cursor.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y, 0.0f);


	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffcursor->Unlock();

	g_Cursor.bUpdate = true;
}
//=============================================================================
// カーソル消去処理
//=============================================================================
void UnInitCursor(void)
{
	UnInitTopics();


	//テクスチャの破棄
	if (g_pTextureCursor != NULL)
	{
		g_pTextureCursor->Release();
		g_pTextureCursor = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffcursor != NULL)
	{
		g_pVtxBuffcursor->Release();
		g_pVtxBuffcursor = NULL;
	}
}
//=============================================================================
// カーソル更新処理
//=============================================================================
void UpdateCursor(void)
{


	UpdateTopics();

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();


	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (g_Cursor.bUpdate == true)
	{
		int nReload = 0;
		VERTEX_2D *pVtx;			//頂点情報へのポインタ


		MODE *pMode;
		pMode = GetMode();



		g_Cursor.posold = g_Cursor.pos;




		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);



		//=========================================
		//任意のキー(Aキー)が押されたかどうか    //
		//=========================================
		if (GetKeyboardPress(DIK_A) == true
			||(state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{

			if (GetKeyboardPress(DIK_W) == true
				|| (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//上キー(Wキー)が押された
				g_Cursor.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;

				if (g_Cursor.pos.y <= LIMIT_MIN_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true
				|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//下キー(Sキー)が押された
				g_Cursor.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;

				if (g_Cursor.pos.y >= LIMIT_MAX_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else
			{//左キー(Aキー)のみ
				g_Cursor.move.x += sinf(-D3DX_PI * 0.5f) * MOVE;

			}

			if (g_Cursor.pos.x <= LIMIT_MIN_X)
			{
				g_Cursor.move.x = 0;
			}
		}

		//=========================================
		//任意のキー(Dキー)が押されたかどうか    //
		//=========================================
		else if (GetKeyboardPress(DIK_D) == true
			|| (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{


			if (GetKeyboardPress(DIK_W) == true
				|| (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//上キー(Wキー)が押された
				g_Cursor.move.x += sinf(D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(D3DX_PI * 0.75f) * MOVE;

				if (g_Cursor.pos.y <= LIMIT_MIN_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true
				|| (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{//下キー(Sキー)が押された
				g_Cursor.move.x += sinf(D3DX_PI * 0.75f) * MOVE;
				g_Cursor.move.y += cosf(D3DX_PI * 0.25f) * MOVE;

				if (g_Cursor.pos.y >= LIMIT_MAX_Y)
				{
					g_Cursor.move.y = 0;
				}
			}
			else
			{//右キー(Dキー)のみ
				g_Cursor.move.x += sinf(D3DX_PI * 0.5f) * MOVE;

			}
			if (g_Cursor.pos.x >= LIMIT_MAX_X)
			{
				g_Cursor.move.x = 0;
			}
		}




		//=========================================
		//任意のキー(Wキー)が押されたかどうか    //
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
		//任意のキー(Sキー)が押されたかどうか    //
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
		//任意のキー(RETURN(Enter)キー)が押されたかどうか    //
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
						//モード選択
						SetFade(MODE_BRIEFING);
					}
				}
			}
			g_JoyStateCursur.nJoypadState = INPUT_JOYSTATE_PUSH;

		}


		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffcursor->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y - CURSOR_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Cursor.pos.x - CURSOR_X, g_Cursor.pos.y + CURSOR_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Cursor.pos.x + CURSOR_X, g_Cursor.pos.y + CURSOR_Y, 0.0f);


		//頂点バッファをアンロックする
		g_pVtxBuffcursor->Unlock();

	}//bUpdateがtrueの間
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
// カーソル描画処理
//=============================================================================
void DrawCursor(void)
{
	DrawTopics();

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

							  //デバイスを取得する
	pDevice = GetDevice();



		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffcursor, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャ
		pDevice->SetTexture(0, g_pTextureCursor);


		//カーソル描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);

}

CURSOR GetCursor000(void)
{
	return g_Cursor;
}



