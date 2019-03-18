//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// タイム処理 [rankRankTime.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "rankTime.h"
#include "main.h"
#include "ranking.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define	MAX_RANKTIME				(6)							// タイムの最大桁数
#define RANKTIME_WIDTH				(60)						// 一文字の幅
#define RANKTIME_HEIGHT				(80)						// 一文字の高さ
#define RANKTIME_TEXTURENAME		"data//TEXTURE//time000.png"		// 読み込むテクスチャのファイル名
#define ANIM_CUT					(10)							// アニメーションの分割数
#define ANIM_CUT_WIDTH				(10.0f)							// アニメーションの横の分割数
#define ANIM_CUT_HEIGHT				(1.0f)							// アニメーションの縦の分割数
#define ANIM_PATTERN_WIDTH			float(1 / ANIM_CUT_WIDTH)		// アニメーションの横パターンの加算数
#define ANIM_PATTERN_HEIGHT			float(1 / ANIM_CUT_HEIGHT)		// アニメーションの縦パターンの加算数
#define GAME_RANKTIME				(17*60*60 + 29*60 + 50)			// ゲームの初期タイム(時+分数+秒数

#define RANKTIME_WIDTH_INTERVAL		(40.0f)							// 時分秒の間の幅

#define RANKTIME_POS_X				(SCREEN_WIDTH / 2.0f + RANKTIME_WIDTH*(MAX_RANKTIME/2) +RANKTIME_WIDTH_INTERVAL + RANKTIME_WIDTH/2.0f)			// 位置X
#define RANKTIME_POS_Y				(200.0f)				// 位置Y

#define RANKTIME_RESULT_POS_X		(1200.0f)			// 位置X
#define RANKTIME_RESULT_POS_Y		(600.0f)			// 位置Y

#define RANK_HEIGHT_INTERVAL		(100.0f)			// Yの間隔


//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nRankTime;			// タイム
	bool bUse;			// 使用しているかどうか
}RankTime;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexRankTime(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureRankTime = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankTime = NULL;				// 頂点バッファへのポインタ
RankTime				g_aRankTime[MAX_RANKING][MAX_RANKTIME];	// タイムの情報
int						g_nRankTime[MAX_RANKING];				// ランキングの時間を

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitRankTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			g_aRankTime[nCntRank][nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aRankTime[nCntRank][nCntTime].nRankTime = 0;
			g_aRankTime[nCntRank][nCntTime].bUse = false;
		}
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RANKTIME_TEXTURENAME,
		&g_pTextureRankTime);

	// 頂点情報の作成
	MakeVertexRankTime(pDevice);

	MODE mode = GetMode();


	// タイムを表示
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			SetRankTime(D3DXVECTOR3(RANKTIME_POS_X - float(((RANKTIME_WIDTH) * (1 + nCntTime))) - RANKTIME_WIDTH_INTERVAL * (nCntTime / 2), RANKTIME_POS_Y + RANK_HEIGHT_INTERVAL * nCntRank, 0.0f));
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitRankTime(void)
{
	// テクスチャの開放
	if (g_pTextureRankTime != NULL)
	{
		g_pTextureRankTime->Release();
		g_pTextureRankTime = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRankTime != NULL)
	{
		g_pVtxBuffRankTime->Release();
		g_pVtxBuffRankTime = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateRankTime()
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == true)	// タイムが使用されている
			{
				if (nCntTime >= 4)
				{ // 時間の処理
				  // 秒数を60で割って分数を算出してさらに60で割って時間を算出
					int nRankTimeHour = (g_nRankTime[nCntRank] / 60) / 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
					}
					else
					{
						int nDigit2 = nRankTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}
				else if (nCntTime >= 2)
				{ // 分数の処理
					// 秒数を60で割って分数を算出
					int nRankTimeMinutes = g_nRankTime[nCntRank] / 60;
					nRankTimeMinutes = nRankTimeMinutes % 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));
					}
					else
					{
						int nDigit2 = nRankTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}
				else
				{// 秒数の処理
					/*int nRankTimeSeconds = g_nRankTime / 60 / 60;
					nRankTimeSeconds = nRankTimeSeconds % 60;*/

					int nRankTimeSeconds = g_nRankTime[nCntRank] % 60;

					if (nCntTime % 2 == 0)
					{
						g_aRankTime[nCntRank][nCntTime].nRankTime = nRankTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
					}
					else
					{
						int nDigit2 = nRankTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

						g_aRankTime[nCntRank][nCntTime].nRankTime = nDigit2 % 6;
					}
				}

				pVtx[0].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aRankTime[nCntRank][nCntTime].nRankTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRankTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawRankTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffRankTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankTime);

	// ポリゴンの描画
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == true)	// タイムが使用されている
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntRank * MAX_RANKTIME)*4 + nCntTime * 4, 2);
			}
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// タイム設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetRankTime(D3DXVECTOR3 pos)
{
	int nCntTime;
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	bool bBreak = false;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			if (g_aRankTime[nCntRank][nCntTime].bUse == false)
			{ // タイムが使用されていない場合
			  // 頂点情報の設定
				g_aRankTime[nCntRank][nCntTime].pos = pos;
				pVtx[0].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);

				g_aRankTime[nCntRank][nCntTime].bUse = true;	// 使用する状態にする
				bBreak = true;
				break;	// このbreakはかなり大切
			}
		}
		if (bBreak == true)
		{
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRankTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexRankTime(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKTIME * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankTime,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRankTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < MAX_RANKTIME; nCntTime++, pVtx += 4)
		{
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y - RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x - RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankTime[nCntRank][nCntTime].pos.x + RANKTIME_WIDTH / 2, g_aRankTime[nCntRank][nCntTime].pos.y + RANKTIME_HEIGHT / 2, 0.0f);
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, ANIM_PATTERN_HEIGHT);
			pVtx[3].tex = D3DXVECTOR2(ANIM_PATTERN_WIDTH, ANIM_PATTERN_HEIGHT);
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRankTime->Unlock();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// タイムの取得
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
int GetRankTime(void)
{
	return g_nRankTime[0];
}



//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルトの時間設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetRankTimeResult(int nRank,int nRankTime)
{
	g_nRankTime[nRank] = nRankTime;
}