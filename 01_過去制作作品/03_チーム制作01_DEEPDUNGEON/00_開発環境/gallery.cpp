//===========================================================================
//
// ギャラリー処理 [Gallery.cpp]
// Author : TEAM_Fascinator
//
//===========================================================================
#include "main.h"
#include "gallery.h"
#include "input.h"
#include "inputx.h"
#include "fade.h"
#include "sound.h"

//===========================================================================
// マクロ定義(Gallery)
//===========================================================================
#define TITLE_TEXTURE_NAME		"data/TEXTURE/Gallery.jpg"	// テクスチャ
#define TITLE_POS_X				(0.0f)						// 左上X座標
#define TITLE_POS_Y				(0.0f)						// 左上Y座標
#define TITLE_WIDTH				(SCREEN_WIDTH)				// 幅
#define TITLE_HEIGHT			(SCREEN_HEIGHT)				// 高さ
#define TITLE_TEXTURE_UV_U		(1.0f)						// アニメーションU範囲
#define TITLE_TEXTURE_UV_V		(1.0f)						// アニメーションV範囲
#define TITLE_SPEED				(0.003f)					// スピード

//===========================================================================
// グローバル変数(Gallery)
//===========================================================================
LPDIRECT3DTEXTURE9 g_pTextureGallery = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGallery = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3 g_posGallery;								// 位置
D3DXVECTOR3 g_moveGallery;								// 移動
JoyState	g_JoyStateGallery;							// ジョイパッドの状態

//===========================================================================
// 初期化処理(Gallery)
//===========================================================================
void InitGallery(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posGallery = D3DXVECTOR3((TITLE_POS_X + TITLE_WIDTH) / 2, (TITLE_POS_Y + TITLE_HEIGHT) / 2, 0.0f);	// 中心値
	g_moveGallery = D3DXVECTOR3(0.0f, TITLE_SPEED, 0.0f);													// 移動値
	g_JoyStateGallery.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_NAME, &g_pTextureGallery);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED,
		&g_pVtxBuffGallery, 
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffGallery->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TITLE_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, TITLE_TEXTURE_UV_V);

	pVtx += 4;	// ポインタ進行(4)

	// 頂点バッファをアンロック
	g_pVtxBuffGallery->Unlock();
}

//===========================================================================
// 終了処理(Gallery)
//===========================================================================
void UninitGallery(void)
{
	// テクスチャ破棄
	if (g_pTextureGallery != NULL)
	{
		g_pTextureGallery->Release();
		g_pTextureGallery = NULL;
	}
	// 頂点バッファ破棄
	if (g_pVtxBuffGallery != NULL)
	{ 
		g_pVtxBuffGallery->Release(); 
		g_pVtxBuffGallery = NULL;
	}
}

//===========================================================================
// 更新処理(Gallery)
//===========================================================================
void UpdateGallery(void)
{
	JoyState pJoystate = GetJoystate();		//ジョイパッドの接続情報確認
	FADE fade = GetFade();

	//ジョイパッドの入力情報取得
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
	{
		if (g_JoyStateGallery.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{//ジョイパッドの連打防止
			if (fade != FADE_OUT)
			{
				// モード設定
				SetFade(MODE_TITLE);
				// 決定音
				StopSound();
				PlaySound(SOUND_LABEL_SE_DECISION);
				PlaySound(SOUND_LABEL_BGM000);
			}
		}
		g_JoyStateGallery.nJoypadState = INPUT_JOYSTATE_PUSH;  //ジョイパッドを押下状態にする
	}

	//ジョイパッドの状態更新
	if (g_JoyStateGallery.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateGallery.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateGallery.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateGallery.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//===========================================================================
// 描画処理(Gallery)
//===========================================================================
void DrawGallery(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGallery, 0, sizeof(VERTEX_2D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureGallery);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}