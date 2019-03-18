//=============================================================================
//
// タイトル画面処理 [Result.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Result.h"
#include "effect.h"
#include "fade.h"
#include "bg.h"
#include "block.h"
#include "item.h"
#include "highscore.h"
#include "sound.h"
#include "resultitem.h"
#include "resultarey.h"
#include "resultvodka.h"
#include "resultsatsu.h"
#include "resultitemall.h"
#include "fg.h"
#include "fg2.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Result_TEXTURENAME000	"DATA/TEXTURE/result1.png"	//タイトルのテクスチャ名
#define Result_TEXTURENAME001	"DATA/TEXTURE/result01-1.png"	//タイトルのテクスチャ名


#define MAX_RESULT_TYPE (2)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TYPE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ

int g_nCntNextScene; //ランキングへの遷移

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitResult(void)
{
	//背景初期化
	InitBG();

	//ブロックの初期化
	InitBlock();

	//アイテムの初期化
	InitItem();

	//ハイスコア初期化
	InitHighScore();

	//アイテムの清算初期化
	InitResultItem();

	//アレイの清算初期化
	InitResultArey();

	//ウォッカの清算初期化
	InitResultVodka();

	//札の清算初期化
	InitResultSatsu();

	//リザルト時のアイテムスコア
	InitResultItemAll();


	//前景
	InitFg();

	//前景
	InitFg2();

	g_nCntNextScene = 0;


	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);


	SetItem(D3DXVECTOR3(180, 100, 0.0f), ITEMTYPE_H_COIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(180, 200, 0.0f), ITEMTYPE_H_VODKA, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetItem(D3DXVECTOR3(180, 300, 0.0f), ITEMTYPE_H_PAPER, D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	SetItem(D3DXVECTOR3(180, 400, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetItem(D3DXVECTOR3(180, 400, 0.0f), ITEMTYPE_H_AREY, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//草
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);


	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2 + 10, 588, 0.0f), 0, 30, 470);

	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME000, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, Result_TEXTURENAME001, &g_pTextureResult[1]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT_TYPE,
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
	pVtx[1].pos = D3DXVECTOR3(400, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 150, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(400, 150, 0.0f);

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
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点の座標
	pVtx[4].pos = D3DXVECTOR3(50, 0, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(50, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラー
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UnInitResult(void)
{
	//背景終了
	UnInitBG();

	//ブロック終了
	UnInitBlock();

	//アイテムの終了
	UnInitItem();

	//ハイスコア終了
	UnInitHighScore();

	//アイテムの清算終了
	UnInitResultItem();

	//アレイの清算初期化
	UnInitResultArey();

	//ウォッカの清算終了
	UnInitResultVodka();

	//ウォッカの清算終了
	UnInitResultSatsu();

	//リザルト時のアイテムスコア
	UnInitResultItemAll();

	//前景
	UnInitFg();

	//前景
	UnInitFg2();

	for (int nCntResult = 0; nCntResult <= MAX_RESULT_TYPE; nCntResult++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

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
	//背景更新
	UpdateBG();

	//ブロックの更新
	UpdateBlock();

	

	//アイテムの更新
	UpdateItem();

	//ハイスコア終了
	UpdateHighScore();

	//アイテムの清算更新
	UpdateResultItem();

	//アレイの清算初期化
	UpdateResultArey();

	//ウォッカの清算更新
	UpdateResultVodka();

	//ウォッカの清算更新
	UpdateResultSatsu();



	//リザルト時のアイテムスコア
	UpdateResultItemAll();

	//前景
	UpdateFg();

	//前景
	UpdateFg2();

	FADE pFade;
	pFade = *GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//モード選択
			//SetMode(MODE_TITLE);
			SetFade(MODE_RANKING);
			PlaySound(SOUND_LABEL_SE_DECIDE);
		}
	}

	g_nCntNextScene++;

	if (g_nCntNextScene % 680 == 0)
	{
		if (pFade == FADE_NONE)
		{

			//モード選択
			SetFade(MODE_RANKING);

		}
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawResult(void)
{
	//背景描画
	DrawBG();



	//ブロック描画
	DrawBlock();

	//前景
	DrawFg();

	//前景
	DrawFg2();

	//アイテム描画
	DrawItem();

	//ハイスコア描画
	DrawHighScore();

	//アイテムの清算描画
	DrawResultItem();

	//アレイの清算初期化
	DrawResultArey();

	//ウォッカの清算描画
	DrawResultVodka();

	//ウォッカの清算描画
	DrawResultSatsu();

	//リザルト時のアイテムスコア
	DrawResultItemAll();



	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

								//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TYPE; nCntResult++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * 4, 2);
	}
}