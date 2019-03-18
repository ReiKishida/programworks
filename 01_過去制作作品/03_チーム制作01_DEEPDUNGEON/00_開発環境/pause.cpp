//=========================================================================================================================
// 
// ポーズ処理 [pause.cpp]
// Author : TEAM_Fascinator
// 
//=========================================================================================================================
#include "pause.h"		
#include "fade.h"		
#include "input.h"
#include "inputx.h"
#include "game.h"

//=========================================================================================================================
// マクロ定義
//=========================================================================================================================
#define PAUSE_TEXTURE_NAME_1	"data/TEXTURE/pausebg.png"		// 読み込むテクスチャファイル1
#define PAUSE_TEXTURE_NAME_2	"data/TEXTURE/Continue.png"		// 読み込むテクスチャファイル2
#define PAUSE_TEXTURE_NAME_3	"data/TEXTURE/Quit.png"			// 読み込むテクスチャファイル4
#define MAX_PAUSE_TEXTURE		(3)								// ポーズテクスチャ最大数
#define PAUSE_POS_X				(0)								// ポーズの左上X座標
#define PAUSE_POS_Y				(0)								// ポーズの左上Y座標
#define PAUSE_WIDTH				(SCREEN_WIDTH)					// ポーズの幅
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					// ポーズの高さ
#define PAUSE_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define PAUSE_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲

//=========================================================================================================================
// グローバル変数
//=========================================================================================================================
LPDIRECT3DTEXTURE9			g_pTexturePause[MAX_PAUSE_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPause = NULL;			// 頂点バッファへのポインタ
D3DXVECTOR3					g_posPause;						// ポーズの位置
JoyState					g_JoyStatePause;				// ジョイパッドの状態
int							g_SelectNum;					// 選択番号
float						g_SelectColor;					// 選択色
int							g_nPauseCount = 0;				// 選択肢点滅用

//=========================================================================================================================
// ポーズ初期化処理
//=========================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得

	g_posPause = D3DXVECTOR3((PAUSE_POS_X + PAUSE_WIDTH) / 2, (PAUSE_POS_Y + PAUSE_HEIGHT) / 2, 0.0f);		// ポーズの中心値
	g_SelectColor = 1.0f;						// 選択カラー

	float fPosPause = -120.0f;						// ポーズ位置

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,PAUSE_TEXTURE_NAME_1,&g_pTexturePause[0]);	//pause
	D3DXCreateTextureFromFile(pDevice,PAUSE_TEXTURE_NAME_2,&g_pTexturePause[1]);	//continue
	D3DXCreateTextureFromFile(pDevice,PAUSE_TEXTURE_NAME_3,&g_pTexturePause[2]);	//quit

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEXTURE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPause,
								NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	g_pVtxBuffPause->Lock(0,0,(void**)&pVtx,0);	// 頂点バッファをロック

	for (int nCount = 0; nCount < MAX_PAUSE_TEXTURE; nCount++)
	{
		if (nCount == 0)
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) - 180, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) - 180, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + 180, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + 180, 0.0f);

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + fPosPause, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + fPosPause, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) - 180, (PAUSE_HEIGHT / 2) + fPosPause + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((PAUSE_WIDTH / 2) + 180, (PAUSE_HEIGHT / 2) + fPosPause + 100, 0.0f);

			if (nCount == 1)
			{	//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{	//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// 幅
			fPosPause += 150.0f;
		}

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PAUSE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, PAUSE_TEXTURE_UV_V);

		pVtx += 4;
	}

	g_pVtxBuffPause->Unlock();	// 頂点バッファをアンロック
}

//=========================================================================================================================
// ポーズ終了処理
//=========================================================================================================================
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_PAUSE_TEXTURE; nCntTex++)
	{
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=========================================================================================================================
// ポーズ更新処理
//=========================================================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	FADE fade = GetFade();

	//コントローラー関係
	JoyState pJoystate = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);	// 頂点バッファをロック

	if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true ||
		(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && pJoystate.bConnectionJoypad == true) || 
		(state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
	{//上
		if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			// 頂点カラー設定
			pVtx[g_SelectNum * 4 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_SelectNum = (g_SelectNum + 1) % 2;
			g_SelectColor = 1.0f;
		}
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true ||
		(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && pJoystate.bConnectionJoypad == true) ||
		(state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoystate.bConnectionJoypad == true))
	{//下
		if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			// 頂点カラー設定
			pVtx[g_SelectNum * 4 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_SelectNum = (g_SelectNum + 1) % 2;
			g_SelectColor = 1.0f;
		}
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;
	}

	if (g_nPauseCount == 0)
	{
		g_SelectColor -= 0.01f;	// 不透明度を下げていく
		if (g_SelectColor <= 0.5f)
		{// 暗くする
			g_nPauseCount = 1;
			g_SelectColor = 0.5f;
		}
	}
	else if (g_nPauseCount == 1)
	{
		g_SelectColor += 0.01f;	// 不透明度を上げていく
		if (g_SelectColor >= 1.0f)
		{// 明るくする
			g_nPauseCount = 0;
			g_SelectColor = 1.0f;
		}
	}

	// 頂点カラー
	pVtx[g_SelectNum * 4 + 4].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 1 + 4].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 2 + 4].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	pVtx[g_SelectNum * 4 + 3 + 4].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);

	g_pVtxBuffPause->Unlock();	// 頂点バッファをアンロック

	if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
	{// モード選択
		if (fade != FADE_OUT && g_JoyStatePause.nJoypadState == 0)
		{
			if (g_SelectNum == 0)
			{// continue
				SetPauseState(false);
			}
			else if (g_SelectNum == 1)
			{// quit
				SetFade(MODE_TITLE);
			}
		}
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	else if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A && pJoystate.bConnectionJoypad == true))
	{
		SetPauseState(false);
	}

	if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStatePause.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStatePause.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=========================================================================================================================
// ポーズ描画処理
//=========================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0,g_pVtxBuffPause,0,sizeof(VERTEX_2D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < MAX_PAUSE_TEXTURE; nCount++)
	{
		// テクスチャ
		pDevice->SetTexture(0, g_pTexturePause[nCount]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}