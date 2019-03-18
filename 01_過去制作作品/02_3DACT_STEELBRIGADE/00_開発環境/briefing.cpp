//=============================================================================
//
// 情報画面処理 [briefing.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "briefing.h"
#include "fade.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BRIEFING (2)

#define BRIEFING_TEXTURENAME	"DATA/TEXTURE/btiefing.png"	//ブリーフィングのテクスチャ名
#define BRIEFING_TEXTURENAME_2	"DATA/TEXTURE/press_enter2.png"	//ブリーフィングのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBriefing[MAX_BRIEFING] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBriefing = NULL;	//頂点バッファへのポインタ

BRIEFING g_aBriefing[MAX_BRIEFING];

JoyState g_JoyStateBriefing;

//=============================================================================
// ブリーフィング初期化処理
//=============================================================================
void InitBriefing(void)
{
	LPDIRECT3DDEVICE9 pDevice;



	//デバイスの取得
	pDevice = GetDevice();


	//種類設定
	g_aBriefing[0].nType = 0;
	g_aBriefing[1].nType = 1;

	//位置初期化
	g_aBriefing[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBriefing[1].pos = D3DXVECTOR3(1050, 600, 0.0f);

	//色情報初期化
	g_aBriefing[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aBriefing[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BRIEFING_TEXTURENAME, &g_pTextureBriefing[0]);
	D3DXCreateTextureFromFile(pDevice, BRIEFING_TEXTURENAME_2, &g_pTextureBriefing[1]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BRIEFING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBriefing,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBriefing->Unlock();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aBriefing[1].pos.x - 150, g_aBriefing[1].pos.y - 45, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aBriefing[1].pos.x + 150, g_aBriefing[1].pos.y - 45, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aBriefing[1].pos.x - 150, g_aBriefing[1].pos.y + 45, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aBriefing[1].pos.x + 150, g_aBriefing[1].pos.y + 45, 0.0f);

	//1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBriefing->Unlock();

	//ゲームパッドの状態
	g_JoyStateBriefing.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


}

//=============================================================================
// ブリーフィング終了処理
//=============================================================================
void UnInitBriefing(void)
{


	for (int nCntBriefing = 0; nCntBriefing < MAX_BRIEFING; nCntBriefing++)
	{
		//テクスチャの破棄
		if (g_pTextureBriefing[nCntBriefing] != NULL)
		{
			g_pTextureBriefing[nCntBriefing]->Release();
			g_pTextureBriefing[nCntBriefing] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBriefing != NULL)
	{
		g_pVtxBuffBriefing->Release();
		g_pVtxBuffBriefing = NULL;
	}
}

//=============================================================================
// ブリーフィング更新処理
//=============================================================================
void UpdateBriefing(void)
{

	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();


	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{//Enterキーが押されたとき、スタートボタン押されたとき
			if (g_JoyStateBriefing.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				//決定音
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//ゲームへ移動
				SetFade(MODE_GAME);
			}
			g_JoyStateBriefing.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}

	// 頂点情報の作成
	VERTEX_2D *pVtx;



	//	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBriefing->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);


	//頂点カラー
	pVtx[4].col = g_aBriefing[1].col;
	pVtx[5].col = g_aBriefing[1].col;
	pVtx[6].col = g_aBriefing[1].col;
	pVtx[7].col = g_aBriefing[1].col;

	//頂点バッファをアンロック
	g_pVtxBuffBriefing->Unlock();



	switch (g_aBriefing[1].state)
	{
	case BRIEFINGSTATE_NONE://空の状態
		g_aBriefing[1].col.a -= 0.02f;
		if (g_aBriefing[1].col.a <= 0.0f)
		{
			g_aBriefing[1].col.a = 0.0f;
			g_aBriefing[1].state = BRIEFINGSTATE_USE;
		}
		break;

	case BRIEFINGSTATE_USE://使用しているとき
		g_aBriefing[1].col.a += 0.02f;
		if (g_aBriefing[1].col.a >= 1.0f)
		{
			g_aBriefing[1].col.a = 1.0f;
			g_aBriefing[1].state = BRIEFINGSTATE_NONE;
		}
		break;
	}

}

//=============================================================================
// ブリーフィング描画処理
//=============================================================================
void DrawBriefing(void)
{


	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

								//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBriefing, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBriefing = 0; nCntBriefing < MAX_BRIEFING; nCntBriefing++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBriefing[g_aBriefing[nCntBriefing].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBriefing * 4, 2);
	}

}
