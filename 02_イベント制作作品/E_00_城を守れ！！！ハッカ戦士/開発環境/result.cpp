//=============================================================================
//
// リザルト処理 [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Result_TEXTURENAME0	"DATA/TEXTURE/clear.jpg"	//タイトルのテクスチャ名
#define Result_TEXTURENAME1	"DATA/TEXTURE/gameover.jpg"	//タイトルのテクスチャ名


#define RESULT_TYPE (2)


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXCOLOR col;
}RESULT;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TYPE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ



//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	GAMESTATE pGame;
	pGame = *GetGameState();

	//デバイスの取得
	pDevice = GetDevice();


	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME0, &g_pTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME1, &g_pTextureResult[1]);





	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UnInitResult(void)
{
	for (int nCntResult = 0; nCntResult < RESULT_TYPE; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}

	}
	//テクスチャの破棄

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード選択
			//SetMode(MODE_TITLE);
			SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	GAMESTATE pGame;
	pGame = *GetGameState2();

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	if (pGame == GAMESTATE_CLEAR)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[0]);
	}
	else if (pGame == GAMESTATE_FAILED)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[1]);
	}
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}