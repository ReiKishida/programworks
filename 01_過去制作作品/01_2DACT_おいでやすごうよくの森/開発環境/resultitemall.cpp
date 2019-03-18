//=============================================================================
//
// リザルト時のアイテムスコア処理 [ResultItemAll.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "resultitemall.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "block.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULTITEMALL		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
//#define	TEXTURE_RESULTITEMALL_CHARACTER	"data/TEXTURE/ResultItemAll.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexResultItemAll(LPDIRECT3DDEVICE9 pDevice);

#define MAX_TEXTURE_TYPE (1) //テクスチャ数
#define ResultItemAll_DIGIT (8)	//桁数
#define MAX_RESULTITEMALL (4)	//リザルト時のアイテムスコア数
#define CHANGE_ResultItemAll (6)//入れ替え用
#define MAX_NUMER (40) // 数字の総数


#define RESULTITEMALL_X (45.0f) //数字の大きさ_X
#define RESULTITEMALL_Y (65.0f)//数字の大きさ_Y
//#define RANK_CHARACTER_X	(280)
//#define RANK_CHARACTER_Y	(250)


#define RESULTITEMALL_POS0 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //リザルト時のコインスコアの位置
#define RESULTITEMALL_POS1 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //リザルト時のウォッカスコアの位置
#define RESULTITEMALL_POS2 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 280.0f, 100, 0.0f))) //リザルト時のおさつスコアの位置

#define RESULTITEMALL_POS3 ((D3DXVECTOR3(SCREEN_WIDTH/2 + RESULTITEMALL_X * 4 + 335.0f, 120, 0.0f))) //リザルト時のアレイスコアの位置



//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResultItemAll = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultItemAll = NULL;		// 頂点バッファへのポインタ
static int				g_nResultItemAll[MAX_RESULTITEMALL] = { 0, 0, 0, 0 };					// スコア
//int						g_aResultItemAllNumber[ResultItemAll_DIGIT];



//=============================================================================
// 初期化処理
//=============================================================================
void InitResultItemAll(void)
{
	

	for (int nCntBlock = 0; nCntBlock < 15; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(nCntBlock * 100.0f, 690, 0), 100.0f, 50.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);
	}



	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// テクスチャの読み込み(数字)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULTITEMALL, &g_pTextureResultItemAll);
	// テクスチャの読み込み(リザルト時のアイテムスコアモジ)

	// 頂点情報の作成
	MakeVertexResultItemAll(pDevice);




}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitResultItemAll(void)
{
	

	// テクスチャの開放
	if (g_pTextureResultItemAll != NULL)
	{
		g_pTextureResultItemAll->Release();
		g_pTextureResultItemAll = NULL;
	}
	


	// 頂点バッファの開放
	if (g_pVtxBuffResultItemAll != NULL)
	{
		g_pVtxBuffResultItemAll->Release();
		g_pVtxBuffResultItemAll = NULL;
	}





}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResultItemAll(void)
{
	

	// 頂点情報の作成
	VERTEX_2D *pVtx;
	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	//VERTEX_2D *pVtx;

	

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResultItemAll = 0; nCntResultItemAll < MAX_RESULTITEMALL; nCntResultItemAll++)
	{
		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
		}
	}





	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResultItemAll(void)
{
	


	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultItemAll, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);





	//=============================================================//
	//					リザルト時のアイテムスコア数字			   //
	//=============================================================//
	// テクスチャの設定

	for (int nCntResultItemAll = 0; nCntResultItemAll < (8 * MAX_RESULTITEMALL); nCntResultItemAll++)
	{
		pDevice->SetTexture(0, g_pTextureResultItemAll);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntResultItemAll, 2);

	}




}



//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexResultItemAll(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8 * MAX_RESULTITEMALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultItemAll,
		NULL);



	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultItemAll->Lock(0, 0, (void**)&pVtx, 0);

	//=============================================================================//
	// リザルト時のアイテムスコアのスコア(鉄アレイまで)						       //
	//=============================================================================//
	for (int nCntResultItemAll = 0; nCntResultItemAll < 4; nCntResultItemAll++)
	{

		for (int nCntDigit = 0; nCntDigit < 8; nCntDigit++, pVtx += 4)
		{
			if (nCntResultItemAll == 0)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS0;
			}
			if (nCntResultItemAll == 1)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS1;
			}

			if (nCntResultItemAll == 2)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS2;
			}if (nCntResultItemAll == 3)
			{
				pVtx[0].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[1].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), -(RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[2].pos = D3DXVECTOR3(-(RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
				pVtx[3].pos = D3DXVECTOR3((RESULTITEMALL_X / 2) - (RESULTITEMALL_X * nCntDigit), (RESULTITEMALL_Y / 2) + ((30 + RESULTITEMALL_Y) * nCntResultItemAll), 0.0f) + RESULTITEMALL_POS3;
			}

			//1.0fで固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

		
			if (nCntResultItemAll == 0)
			{//コインのスコアカラー
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f);
			}
			else if (nCntResultItemAll == 1)
			{//ウォッカのスコアカラー
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (nCntResultItemAll == 2)
			{//札のスコアカラー
			 //頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{//アレイのスコアカラー
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nResultItemAll[nCntResultItemAll] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);


		}

	}

	
}


//=============================================================================
// スコアのセット
//=============================================================================
void SetResultItemAll(int nNumCoin, int nNumVodka , int nNumSatsu, int nNumArey)
{
	g_nResultItemAll[0] = 2500 * nNumCoin;
	g_nResultItemAll[1] = 5000 * nNumVodka;
	g_nResultItemAll[2] = 10000 * nNumSatsu;
	g_nResultItemAll[3] = 1000 * nNumArey;

}
