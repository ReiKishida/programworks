//===========================================================================
//
// オプション処理 [option.cpp]
// Author : TEAM_Fascinator
//
//===========================================================================
#include "main.h"
#include "input.h"
#include "inputx.h"
#include "option.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//===========================================================================
// マクロ定義(Option)
//===========================================================================
#define OPTION_TEXTURE_NAME		"data/TEXTURE/credit.png"	// テクスチャ
#define OPTION_POS_X			(0.0f)						// 左上X座標
#define OPTION_POS_Y			(0.0f)						// 左上Y座標
#define OPTION_WIDTH			(SCREEN_WIDTH)				// 幅
#define OPTION_HEIGHT			(SCREEN_HEIGHT)				// 高さ
#define OPTION_MAX				(2)							// 高さ
#define OPTION_TEXTURE_UV_U		(1.0f)						// アニメーションU範囲
#define OPTION_TEXTURE_UV_V		(1.0f)						// アニメーションV範囲
#define OPTION_SPEED			(0.003f)					// スピード

//===========================================================================
// グローバル変数(Option)
//===========================================================================
LPDIRECT3DTEXTURE9			g_pTextureOption = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffOption = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3					g_posOption;					// 位置
D3DXVECTOR3					g_moveOption;					// 移動
JoyState					g_JoyStateOption;				// ジョイパッドの状態

//===========================================================================
// 初期化処理(Option)
//===========================================================================
void InitOption(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posOption = D3DXVECTOR3((OPTION_POS_X + OPTION_WIDTH) / 2, (OPTION_POS_Y + OPTION_HEIGHT) / 2, 0.0f);	// 中心値
	g_moveOption = D3DXVECTOR3(0.0f, OPTION_SPEED, 0.0f);													// 移動値
	g_JoyStateOption.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, OPTION_TEXTURE_NAME, &g_pTextureOption);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * OPTION_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOption,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(OPTION_POS_X, OPTION_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(OPTION_WIDTH, OPTION_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(OPTION_POS_X, OPTION_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(OPTION_WIDTH, OPTION_HEIGHT, 0.0f);

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(OPTION_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, OPTION_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(OPTION_TEXTURE_UV_U, OPTION_TEXTURE_UV_V);

	pVtx += 4;	// ポインタ進行(4)

	// 頂点バッファをアンロック
	g_pVtxBuffOption->Unlock();
}

//===========================================================================
// 終了処理(Option)
//===========================================================================
void UninitOption(void)
{
	// テクスチャ破棄
	if (g_pTextureOption != NULL)
	{ 
		g_pTextureOption->Release(); g_pTextureOption = NULL;
	}

	// 頂点バッファ破棄
	if (g_pVtxBuffOption != NULL)
	{ 
		g_pVtxBuffOption->Release(); g_pVtxBuffOption = NULL;
	}
}

//===========================================================================
// 更新処理(Option)
//===========================================================================
void UpdateOption(void)
{
	FADE fade = GetFade();
	JoyState pJoystate = GetJoystate();		//ジョイパッドの接続情報確認

	//ジョイパッドの入力情報取得
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
	{
		if (g_JoyStateOption.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{//ジョイパッドの連打防止

			if (fade != FADE_OUT)
			{
				// モード設定
				SetFade(MODE_TITLE);
				// 決定音
				PlaySound(SOUND_LABEL_SE_DECISION);
			}
		}
		g_JoyStateOption.nJoypadState = INPUT_JOYSTATE_PUSH;  //ジョイパッドを押下状態にする
	}

	//ジョイパッドの状態更新
	if (g_JoyStateOption.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateOption.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateOption.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateOption.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//===========================================================================
// 描画処理(Option)
//===========================================================================
void DrawOption(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffOption, 0, sizeof(VERTEX_2D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureOption);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}