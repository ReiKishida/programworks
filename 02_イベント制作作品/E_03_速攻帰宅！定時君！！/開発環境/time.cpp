//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// タイム処理 [time.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "time.h"
#include "main.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define	MAX_TIME				(6)							// タイムの最大桁数
#define TIME_WIDTH				(60)						// 一文字の幅
#define TIME_HEIGHT				(80)						// 一文字の高さ
#define TIME_TEXTURENAME	"data//TEXTURE//time000.png"		// 読み込むテクスチャのファイル名
#define ANIM_CUT				(10)							// アニメーションの分割数
#define ANIM_CUT_WIDTH			(10.0f)							// アニメーションの横の分割数
#define ANIM_CUT_HEIGHT			(1.0f)							// アニメーションの縦の分割数
#define ANIM_PATTERN_WIDTH		float(1 / ANIM_CUT_WIDTH)		// アニメーションの横パターンの加算数
#define ANIM_PATTERN_HEIGHT		float(1 / ANIM_CUT_HEIGHT)		// アニメーションの縦パターンの加算数
#define GAME_TIME				(17*60*60 + 29*60 + 50)			// ゲームの初期タイム(時+分数+秒数
#define GAME_START_TIME			(17*60*60 + 30*60 + 00)			// ゲームの初期タイム(時+分数+秒数
#define TIME_WIDTH_INTERVAL		(40.0f)							// 時分秒の間の幅

#define TIME_POS_X				(SCREEN_WIDTH / 2.0f + TIME_WIDTH*(MAX_TIME/2) +TIME_WIDTH_INTERVAL + TIME_WIDTH/2.0f)			// 位置X
#define TIME_POS_Y				(80.0f)							// 位置Y

#define TIME_RESULT_POS_X		(1130.0f)			// 位置X
#define TIME_RESULT_POS_Y		(500.0f)							// 位置Y


//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int nTime;			// タイム
	bool bUse;			// 使用しているかどうか
}Time;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
//*****************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	// 頂点バッファへのポインタ
Time					g_aTime[MAX_TIME];		// タイムの情報
int						g_nTime;				// 敵を倒したポイントを記録
bool					g_bStop;				// タイムが停止しているかどうか
bool					g_bGameStart;			// ゲームスタートするかどうか

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	g_bGameStart = false;
	// デバイスの取得
	pDevice = GetDevice();

	// タイムの情報の初期化
	g_bStop = false;
	for(int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTime[nCntTime].nTime = 0;
		g_aTime[nCntTime].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEXTURENAME,
		&g_pTextureTime);

	// 頂点情報の作成
	MakeVertexTime(pDevice);

	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_GAME:
		g_nTime = GAME_TIME;
		// タイムを表示
		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			SetTime(D3DXVECTOR3(TIME_POS_X - float(((TIME_WIDTH) * (1 + nCntTime))) - TIME_WIDTH_INTERVAL * (nCntTime / 2), TIME_POS_Y, 0.0f));
		}
		break;
	case MODE_RESULT:
		// タイムを表示
		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			SetTime(D3DXVECTOR3(TIME_RESULT_POS_X - float(((TIME_WIDTH) * (1 + nCntTime))) - TIME_WIDTH_INTERVAL * (nCntTime / 2), TIME_RESULT_POS_Y, 0.0f));
		}
		break;
	}

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitTime(void)
{
	// テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateTime()
{
	static int nFrameCnt = 0;
	nFrameCnt++;

	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_GAME:
		if (g_bStop == false)
		{
			if (nFrameCnt % 20 == 0)
			{
				g_nTime++;
			}
		}
		break;
	}

	// ゲームスタート処理
	if (GAME_START_TIME <= g_nTime)
	{
		g_bGameStart = true;
	}

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		if (g_aTime[nCntTime].bUse == true)	// タイムが使用されている
		{
			if (nCntTime >= 4)
			{ // 時間の処理
			  // 秒数を60で割って分数を算出してさらに60で割って時間を算出
				int nTimeHour = (g_nTime / 60) / 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
				}
				else
				{
					int nDigit2 = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else if (nCntTime >= 2)
			{ // 分数の処理
				// 秒数を60で割って分数を算出
				int nTimeMinutes = g_nTime / 60;
				nTimeMinutes = nTimeMinutes % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeMinutes % (int(powf(10.0f, float(nCntTime-2))) * 10) / int(powf(10.0f, float(nCntTime-2)));
				}
				else
				{
					int nDigit2 = nTimeMinutes % (int(powf(10.0f, float(nCntTime-2))) * 10) / int(powf(10.0f, float(nCntTime-2)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else
			{// 秒数の処理
				/*int nTimeSeconds = g_nTime / 60 / 60;
				nTimeSeconds = nTimeSeconds % 60;*/

				int nTimeSeconds = g_nTime % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
				}
				else
				{
					int nDigit2 = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawTime(void)
{
	int nCntTime;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画
	for(nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		if (g_aTime[nCntTime].bUse == true)	// タイムが使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// タイム設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTime(D3DXVECTOR3 pos)
{
	int nCntTime;
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		if (g_aTime[nCntTime].bUse == false)
		{ // タイムが使用されていない場合
		  // 頂点情報の設定
			g_aTime[nCntTime].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);

			if (nCntTime >= 4)
			{ // 時間の処理
			  // 秒数を60で割って分数を算出してさらに60で割って時間を算出
				int nTimeHour = (g_nTime / 60) / 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));
				}
				else
				{
					int nDigit2 = nTimeHour % (int(powf(10.0f, float(nCntTime - 4))) * 10) / int(powf(10.0f, float(nCntTime - 4)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else if (nCntTime >= 2)
			{ // 分数の処理
			  // 秒数を60で割って分数を算出
				int nTimeMinutes = g_nTime / 60;
				nTimeMinutes = nTimeMinutes % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));
				}
				else
				{
					int nDigit2 = nTimeMinutes % (int(powf(10.0f, float(nCntTime - 2))) * 10) / int(powf(10.0f, float(nCntTime - 2)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}
			else
			{// 秒数の処理
			 /*int nTimeSeconds = g_nTime / 60 / 60;
			 nTimeSeconds = nTimeSeconds % 60;*/

				int nTimeSeconds = g_nTime % 60;

				if (nCntTime % 2 == 0)
				{
					g_aTime[nCntTime].nTime = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));
				}
				else
				{
					int nDigit2 = nTimeSeconds % (int(powf(10.0f, float(nCntTime))) * 10) / int(powf(10.0f, float(nCntTime)));

					g_aTime[nCntTime].nTime = nDigit2 % 6;
				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aTime[nCntTime].nTime * ANIM_PATTERN_WIDTH + ANIM_PATTERN_WIDTH, 1.0f);

			g_aTime[nCntTime].bUse = true;	// 使用する状態にする
			break;	// このbreakはかなり大切
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y - TIME_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_WIDTH / 2, g_aTime[nCntTime].pos.y + TIME_HEIGHT / 2, 0.0f);
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
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// タイムの取得
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
int GetTime(void)
{
	return g_nTime;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// タイムの停止
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void StopTime(void)
{
	g_bStop = true;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// リザルトの時間設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTimeResult(int nTime)
{
	g_nTime = nTime;
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// ゲーム開始可能かどうかの処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
bool GetGameStartTime(void)
{
	return g_bGameStart;
}