//=============================================================================
//
// スコア処理 [score.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "callout.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_CALLOUT0		"data/TEXTURE/callout_00.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CALLOUT1		"data/TEXTURE/callout_01.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CALLOUT2		"data/TEXTURE/callout_02.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CALLOUT3		"data/TEXTURE/callout_03.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CALLOUT4		"data/TEXTURE/callout_04.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_CALLOUT5		"data/TEXTURE/callout_05.png"	// 読み込むテクスチャファイル名

#define MAX_TEXTURE	(2)

#define MAX_CALLOUT	(1)

#define CALLOUT_SIZE	(100.0f)	// 大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCallout(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCallout[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCallout = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffPresent = NULL;
CALLOUT g_aCallout[MAX_CALLOUT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitCallout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCallout = 0; nCntCallout < MAX_CALLOUT; nCntCallout++)
	{
		// 初期化
		g_aCallout[nCntCallout].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCallout[nCntCallout].nType = 0;
		g_aCallout[nCntCallout].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CALLOUT0, &g_pTextureCallout[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CALLOUT1, &g_pTextureCallout[1]);

	// 頂点情報の作成
	MakeVertexCallout(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCallout(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (g_pTextureCallout[nCntTex] != NULL)
		{
			g_pTextureCallout[nCntTex]->Release();
			g_pTextureCallout[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCallout != NULL)
	{
		g_pVtxBuffCallout->Release();
		g_pVtxBuffCallout = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCallout(void)
{
	for (int nCntCallout = 0; nCntCallout < MAX_CALLOUT; nCntCallout++)
	{
		if (g_aCallout[nCntCallout].bUse == true)
		{
			switch (g_aCallout[nCntCallout].type)
			{
			case CALLOUTTYPE_NONE:
				break;

			case CALLOUTTYPE_CHANGE:
				g_aCallout[nCntCallout].nType = 0;
				break;

			case CALLOUTTYPE_PRESENT1:
				g_aCallout[nCntCallout].nType = 1;
				break;

			case CALLOUTTYPE_PRESENT2:
				g_aCallout[nCntCallout].nType = 2;
				break;

			case CALLOUTTYPE_PRESENT3:
				g_aCallout[nCntCallout].nType = 3;
				break;

			case CALLOUTTYPE_PRESENT4:
				g_aCallout[nCntCallout].nType = 4;
				break;

			case CALLOUTTYPE_PRESENT5:
				g_aCallout[nCntCallout].nType = 5;
				break;

			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCallout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCallout, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCallout = 0; nCntCallout < MAX_CALLOUT; nCntCallout++)
	{
		if(g_aCallout[nCntCallout].bUse == true)
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCallout[g_aCallout[nCntCallout].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntCallout, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCallout(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntCallout;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CALLOUT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCallout,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCallout->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCallout = 0; nCntCallout < MAX_CALLOUT; nCntCallout++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x - CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y - CALLOUT_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x + CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y - CALLOUT_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x - CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y + CALLOUT_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x + CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y + CALLOUT_SIZE, 0.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCallout->Unlock();
}

//=============================================================================
// 吹き出しの設定
//=============================================================================
int SetCallout(D3DXVECTOR3 pos, CALLOUTTYPE type)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntCallout;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCallout->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCallout = 0; nCntCallout < MAX_CALLOUT; nCntCallout++)
	{
		if (g_aCallout[nCntCallout].bUse == false)
		{//ブロックが使用されていない
			g_aCallout[nCntCallout].pos = pos;
			g_aCallout[nCntCallout].type = type;
			g_aCallout[nCntCallout].bUse = true;	//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x - CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y - CALLOUT_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x + CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y - CALLOUT_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x - CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y + CALLOUT_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCallout[nCntCallout].pos.x + CALLOUT_SIZE, g_aCallout[nCntCallout].pos.y + CALLOUT_SIZE, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffCallout->Unlock();

	return nCntCallout;
}

//=============================================================================
// 吹き出しを消す
//=============================================================================
void DeleteCallout(int nIdxCallout)
{
	if (g_aCallout[nIdxCallout].bUse == true)
	{
		g_aCallout[nIdxCallout].bUse = false;
	}
}
