//=============================================================================
//
// 情報画面処理 [tips.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "tips.h"
#include "fade.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TIPS (2)

#define Tips_TEXTURENAME	"DATA/TEXTURE/sousa00.png"	//タイトルのテクスチャ名
#define Tips_TEXTURENAME_2	"DATA/TEXTURE/press_enter2.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTips[MAX_TIPS] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTips = NULL;	//頂点バッファへのポインタ

TIPS g_aTips[MAX_TIPS];

JoyState g_JoyStateTips;

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTips(void)
{
	LPDIRECT3DDEVICE9 pDevice;



	//デバイスの取得
	pDevice = GetDevice();



	g_aTips[0].nType = 0;
	g_aTips[1].nType = 1;

	g_aTips[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTips[1].pos = D3DXVECTOR3(1050, 670, 0.0f);

	g_aTips[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTips[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, Tips_TEXTURENAME, &g_pTextureTips[0]);
	D3DXCreateTextureFromFile(pDevice, Tips_TEXTURENAME_2, &g_pTextureTips[1]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIPS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTips,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTips->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffTips->Unlock();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTips->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(g_aTips[1].pos.x - 150, g_aTips[1].pos.y - 45, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aTips[1].pos.x + 150, g_aTips[1].pos.y - 45, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aTips[1].pos.x - 150, g_aTips[1].pos.y + 45, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aTips[1].pos.x + 150, g_aTips[1].pos.y + 45, 0.0f);

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
	g_pVtxBuffTips->Unlock();

	//ゲームパッドの状態
	g_JoyStateTips.nJoypadState = INPUT_JOYSTATE_NOTPUSH;


}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UnInitTips(void)
{


	for (int nCntTips = 0; nCntTips < MAX_TIPS; nCntTips++)
	{
		//テクスチャの破棄
		if (g_pTextureTips[nCntTips] != NULL)
		{
			g_pTextureTips[nCntTips]->Release();
			g_pTextureTips[nCntTips] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTips != NULL)
	{
		g_pVtxBuffTips->Release();
		g_pVtxBuffTips = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTips(void)
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

		{

		}
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{
			if (g_JoyStateTips.nJoypadState == INPUT_JOYSTATE_NOTPUSH )
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);

				SetFade(MODE_WORLD);
			}
			g_JoyStateTips.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}

	// 頂点情報の作成
	VERTEX_2D *pVtx;



//	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTips->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);


	//頂点カラー
	pVtx[4].col = g_aTips[1].col;
	pVtx[5].col = g_aTips[1].col;
	pVtx[6].col = g_aTips[1].col;
	pVtx[7].col = g_aTips[1].col;

	//頂点バッファをアンロック
	g_pVtxBuffTips->Unlock();



	switch (g_aTips[1].state)
	{
	case TIPSSTATE_NONE:
		g_aTips[1].col.a -= 0.02f;
		if (g_aTips[1].col.a <= 0.0f)
		{
			g_aTips[1].col.a = 0.0f;
			g_aTips[1].state = TIPSSTATE_USE;
		}
		break;

	case TIPSSTATE_USE:
		g_aTips[1].col.a += 0.02f;
		if (g_aTips[1].col.a >= 1.0f)
		{
			g_aTips[1].col.a = 1.0f;
			g_aTips[1].state = TIPSSTATE_NONE;
		}
		break;
	}

}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTips(void)
{


	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

								//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTips, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTips = 0; nCntTips < MAX_TIPS; nCntTips++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTips[g_aTips[nCntTips].nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTips * 4, 2);
	}

}
