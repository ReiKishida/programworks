//=============================================================================
//
// リザルト処理 [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/number002.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_HIGHSCORE	"data/TEXTURE/succed.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_HIGHSCORE2	"data/TEXTURE/failed.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexResult2(LPDIRECT3DDEVICE9 pDevice);

#define MAX_RESULT (8)	//桁数
#define MAX_TYPE_RESUT (2)
#define RESULT_X (30.0f) //Xの大きさ
#define RESULT_Y (60.0f)//Yの大きさ

#define HIGH_X (SCREEN_WIDTH /2) //Xの大きさ
#define HIGH_Y (SCREEN_HEIGHT /2)//Yの大きさ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
int						g_nResult = 0;					// スコア
int						g_aRResultNumber[MAX_RESULT];	//桁数
D3DXVECTOR3				g_Resultpos;//スコア中心座標


LPDIRECT3DTEXTURE9		g_pTextureResult2[MAX_TYPE_RESUT] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult2 = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3				g_HighgResult;//スコア中心座標
JoyState g_JoyStateResult;


//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコア初期化
	//g_nResult = 8000;

	g_Resultpos = D3DXVECTOR3(380, SCREEN_HEIGHT / 2 + 250.0f, 0.0f);

	g_HighgResult = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &g_pTextureResult);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE, &g_pTextureResult2[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HIGHSCORE2, &g_pTextureResult2[1]);

	// 頂点情報の作成
	MakeVertexResult2(pDevice);

	MakeVertexResult(pDevice);
	//ゲームパッドの状態
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResult(void)
{

	// テクスチャの開放
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}


	for (int nCnt = 0; nCnt < MAX_TYPE_RESUT; nCnt++)
	{
		// テクスチャの開放
		if (g_pTextureResult2[nCnt] != NULL)
		{
			g_pTextureResult2[nCnt]->Release();
			g_pTextureResult2[nCnt] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffResult2 != NULL)
	{
		g_pVtxBuffResult2->Release();
		g_pVtxBuffResult2 = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;
	pFade = *GetFade();


	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);


	// 頂点情報の作成
	VERTEX_2D *pVtx;

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_START && pJoyState.bConnectionJoypad == true))
		{
			if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				//モード選択
				//SetMode(MODE_TITLE);
				SetFade(MODE_RANKING);
			}
			g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;

		}
	}


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RESULT; nCntScore++)
	{
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aRResultNumber[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntScore], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aRResultNumber[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntScore], 1.0f);
		pVtx += 4;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	int nCntResult;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	GAMESTATE pGame;
	pGame = *GetGameState2();





	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (pGame == GAMESTATE_CLEAR)
	{
		//テクスチャ
		pDevice->SetTexture(0, g_pTextureResult2[0]);
	}
	if (pGame == GAMESTATE_FAILED)
	{
		//テクスチャ
		pDevice->SetTexture(0, g_pTextureResult2[1]);
	}
	//プレイヤー描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * 4, 2);
	}

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResult2(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);



	// 頂点情報を設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_Resultpos.x - RESULT_X, g_Resultpos.y - RESULT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Resultpos.x + RESULT_X, g_Resultpos.y - RESULT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Resultpos.x - RESULT_X, g_Resultpos.y + RESULT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Resultpos.x + RESULT_X, g_Resultpos.y + RESULT_Y, 0.0f);


		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);


		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_aRResultNumber[nCntResult], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntResult], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f *g_aRResultNumber[nCntResult], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f *g_aRResultNumber[nCntResult], 1.0f);



		pVtx += 4;

		g_Resultpos.x += 60;

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult2,
		NULL);


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_HighgResult.x - HIGH_X, g_HighgResult.y - HIGH_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_HighgResult.x + HIGH_X, g_HighgResult.y - HIGH_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_HighgResult.x - HIGH_X, g_HighgResult.y + HIGH_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_HighgResult.x + HIGH_X, g_HighgResult.y + HIGH_Y, 0.0f);


	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	// 頂点バッファをアンロックする
	g_pVtxBuffResult2->Unlock();
}

//=============================================================================
// スコアのセット
//=============================================================================
void SetResult(int nScore)
{
	if (nScore > g_nResult)
	{
		// 頂点情報の作成
		//		VERTEX_2D *pVtx;


		g_nResult = nScore;

		//g_nResult += g_nResult;


		g_aRResultNumber[0] = g_nResult / 100000000;
		g_aRResultNumber[1] = g_nResult % 10000000 / 1000000;
		g_aRResultNumber[2] = g_nResult % 1000000 / 100000;
		g_aRResultNumber[3] = g_nResult % 100000 / 10000;
		g_aRResultNumber[4] = g_nResult % 10000 / 1000;
		g_aRResultNumber[5] = g_nResult % 1000 / 100;
		g_aRResultNumber[6] = g_nResult % 100 / 10;
		g_aRResultNumber[7] = g_nResult % 10;




	}
}