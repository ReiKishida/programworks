////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ライフ処理 [life.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "life.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "game.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_LIFE_NUM	(7)
#define MAX_LIFE		(100.0f)
#define ANIM			(0.3333f)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Life g_aLife[MAX_LIFE_NUM];
LifeData g_aLifeData[LIFETEX_MAX] =
{
	{ NULL,"data/TEXTURE/soul001.png" },		// プレイヤーの背景の背景
	{ NULL,"data/TEXTURE/soul001.png" },		// プレイヤーの背景
	{ NULL,"data/TEXTURE/.png" },				// ライフの裏の黒
	{ NULL,"data/TEXTURE/.jpg" },				// ライフの赤色
	{ NULL,"data/TEXTURE/life002.jpg" },		// ライフ
	{ NULL,"data/TEXTURE/Life_frame02.png" },	// ライフのフレーム
};
int g_nDrawDownLife;// ライフを減らす変数
int g_nLife;
bool g_bDown;
bool b_col;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		g_aLife[nCntLife].pVtxBuff = NULL;										// 頂点バッファへのポインタ
		g_aLife[nCntLife].pIdxBuff = NULL;										// インデックスバッファへのポインタ
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		g_aLife[nCntLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// 色
		g_aLife[nCntLife].fWidth = 0;											// 幅
		g_aLife[nCntLife].fHeight = 0;											// 高さ
		g_aLife[nCntLife].nBlock_X = 0;											// ブロックX
		g_aLife[nCntLife].nBlock_Y = 0;											// ブロックY
		g_aLife[nCntLife].nPatternAnim = 0;										// パターン
		g_aLife[nCntLife].nCounterAnim = 0;										// カウンター
		g_aLife[nCntLife].nType = 0;											// 種類
		g_aLife[nCntLife].bUse = false;											// 使用されているかどうか
		g_aLife[nCntLife].bUseDown = false;										// ダウン中かどうか
		g_aLife[nCntLife].bDownRed = false;									// 背景の赤いライフ用
		g_aLife[nCntLife].nDownRedCounter = 0;								// 背景の赤いライフ用減少
		g_aLife[nCntLife].nDrawDown = 0;									// ポリゴン減算用
	}
	g_nDrawDownLife = 0;
	g_nLife = 3;
	g_bDown = false;
	b_col = false;
	for (int nCntTex = 0; nCntTex < LIFETEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			g_aLifeData[nCntTex].pFileName,						// ファイルの名前
			&g_aLifeData[nCntTex].pTexture);						// テクスチャへのポインタ
	}
	// 左を中心としての初期座標・回転・テクスチャ・ライフ分割数・いじらない・長さ横・縦
	SetLife(D3DXVECTOR3(1005, 455, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), LIFETEX_ICONBG, 1, 1, 250 + 10, 250 + 10, false);
	SetLife(D3DXVECTOR3(1005, 460, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.9f, 1.0f, 1.0f, 1.0f), LIFETEX_ICON, 1, 1, 250, 250, false);

	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), LIFETEX_BG2, 100, 1, 560, 20, false);
	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), LIFETEX_GAGERED, 100, 1, 560, 20, true);
	SetLife(D3DXVECTOR3(550, 625, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIFETEX_GAGE, 100, 1, 560, 20, true);
	SetLife(D3DXVECTOR3(545, 620, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIFETEX_BG, 100, 1, 560 + 10, 30, false);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitLife(void)
{
	// テクスチャの破棄
	for (int nCntLife = 0; nCntLife <LIFETEX_MAX; nCntLife++)
	{
		if (g_aLifeData[nCntLife].pTexture != NULL)
		{
			g_aLifeData[nCntLife].pTexture->Release();
			g_aLifeData[nCntLife].pTexture = NULL;
		}
	}
	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		// 頂点バッファの破棄
		if (g_aLife[nCntLife].pVtxBuff != NULL)
		{
			g_aLife[nCntLife].pVtxBuff->Release();
			g_aLife[nCntLife].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aLife[nCntLife].pIdxBuff != NULL)
		{
			g_aLife[nCntLife].pIdxBuff->Release();
			g_aLife[nCntLife].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得
	VERTEX_2D *pVtx;	// 頂点情報のポインタ


	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aLife[nCntLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			if (g_aLife[nCntLife].bUseDown == true)
			{
				if (g_aLife[nCntLife].nType == LIFETEX_GAGE)
				{
					if (g_bDown == true)
					{// 減少中
						g_aLife[nCntLife].nDrawDown++;
						if (g_aLife[nCntLife].nDrawDown >= 100 - ((float)g_nLife / MAX_LIFE) * 100)
						{// 100%中 X%減ったとこまで減らす
							g_bDown = false;
							g_aLife[nCntLife - 1].bDownRed = true;		// ライフの前が赤ライフなら-1
							g_aLife[nCntLife - 1].nDownRedCounter = 30; // 赤ライフ減少間隔保管
							if (g_nLife <= 0)
							{// 死んだら保管なし
								g_aLife[nCntLife - 1].nDownRedCounter = 0;
							}
						}
					}
				}
				if (g_aLife[nCntLife].nType == LIFETEX_GAGERED)
				{
					if (g_aLife[nCntLife].bDownRed == true)
					{
						g_aLife[nCntLife].nDownRedCounter--;
						if (g_aLife[nCntLife].nDownRedCounter <= 0)
						{
							g_aLife[nCntLife].nDownRedCounter = 0;
							g_aLife[nCntLife].nDrawDown++;
							if (g_aLife[nCntLife].nDrawDown >= 100 - ((float)g_nLife / MAX_LIFE) * 100)
							{// 100%中 X%減ったとこまで減らす
								g_aLife[nCntLife].bDownRed = false;
							}
						}
					}
				}
			}
			if (g_aLife[nCntLife].nType == LIFETEX_GAGE)
			{
				if (b_col == false)
				{
					g_aLife[nCntLife].col.a -= 0.01f;
					if (g_aLife[nCntLife].col.a <= 0.8f)
					{
						b_col = true;
					}
				}
				else
				{
					g_aLife[nCntLife].col.a += 0.01f;
					if (g_aLife[nCntLife].col.a >= 1.0f)
					{
						b_col = false;
					}
				}
				for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
				{
					pVtx[nCnt].col = g_aLife[nCntLife].col;
				}
				g_aLife[nCntLife].nCounterAnim++;
				if (g_aLife[nCntLife].nCounterAnim % 5 == 0)
				{
					g_aLife[nCntLife].nPatternAnim = (g_aLife[nCntLife].nPatternAnim + 1) % 3;
					for (int nCntV = 0; nCntV < g_aLife[nCntLife].nBlock_Y + 1; nCntV++)
					{// テクスチャの設定
						for (int nCntH = 0; nCntH < g_aLife[nCntLife].nBlock_X + 1; nCntH++)
						{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
						 // テクスチャの設定
							pVtx[nCntH + (g_aLife[nCntLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aLife[nCntLife].nBlock_X) / 100.0f, float(nCntV * 100 / g_aLife[nCntLife].nBlock_Y) / 100.0f + (ANIM*g_aLife[nCntLife].nPatternAnim));
						}
					}
				}
			}
			// 頂点バッファをアンロックする
			g_aLife[nCntLife].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

												// ワイヤーフレーム
												//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLife[nCntLife].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLife[nCntLife].rot.y, g_aLife[nCntLife].rot.x, g_aLife[nCntLife].rot.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLife[nCntLife].pos.x, g_aLife[nCntLife].pos.y, g_aLife[nCntLife].pos.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxTrans);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aLife[nCntLife].pVtxBuff, 0, sizeof(VERTEX_2D));
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aLife[nCntLife].pIdxBuff);
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_aLifeData[g_aLife[nCntLife].nType].pTexture);
			if (g_aLife[nCntLife].bUseDown == true)
			{
				// ポリゴンの描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aLife[nCntLife].nNumIndex, 0, g_aLife[nCntLife].nNumPolygon - g_aLife[nCntLife].nDrawDown * 2);
			}
			else
			{
				// ポリゴンの描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aLife[nCntLife].nNumIndex, 0, g_aLife[nCntLife].nNumPolygon);

			}
		}
	}

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報のポインタ
	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * g_aLife[nCntLife].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_aLife[nCntLife].pVtxBuff,
				NULL);


			// 頂点バッファをロックし頂点データへのポインタを取得

			g_aLife[nCntLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				pVtx[nCnt].pos = D3DXVECTOR3((g_aLife[nCntLife].fWidth / g_aLife[nCntLife].nBlock_X)*(nCnt % (g_aLife[nCntLife].nBlock_X + 1)),
					+(g_aLife[nCntLife].fHeight)*(nCnt / (g_aLife[nCntLife].nBlock_X + 1)), 0.0f) + g_aLife[nCntLife].pos;

			}
			// rhwの設定
			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aLife[nCntLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aLife[nCntLife].col;
			}

			// 全体テクスチャ
			for (int nCntV = 0; nCntV < g_aLife[nCntLife].nBlock_Y + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aLife[nCntLife].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aLife[nCntLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aLife[nCntLife].nBlock_X) / 100.0f, float(nCntV * 100 / g_aLife[nCntLife].nBlock_Y) / 100.0f);
				}
			}
			// 頂点バッファをアンロックする
			g_aLife[nCntLife].pVtxBuff->Unlock();
			WORD *pIdx;	// インデックスデータへのポインタ

						// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aLife[nCntLife].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aLife[nCntLife].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aLife[nCntLife].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aLife[nCntLife].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aLife[nCntLife].nBlock_X + 2) == g_aLife[nCntLife].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aLife[nCntLife].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aLife[nCntLife].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aLife[nCntLife].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetLife(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntLife = 0; nCntLife < MAX_LIFE_NUM; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == false)
		{
			g_aLife[nCntLife].pos = pos;				// 位置
			g_aLife[nCntLife].rot = rot;				// 向き
			g_aLife[nCntLife].col = col;				// 色
			g_aLife[nCntLife].fWidth = fWidth;			// 幅
			g_aLife[nCntLife].fHeight = fHeight;		// 奥行き
			g_aLife[nCntLife].nBlock_X = nBlock_X;		// ブロックX
			g_aLife[nCntLife].nBlock_Y = nBlock_Y;		// ブロックZ
			g_aLife[nCntLife].nType = nType;			// 種類
			g_aLife[nCntLife].bUseDown = bUseDown;
			g_aLife[nCntLife].nNumVertex = (g_aLife[nCntLife].nBlock_X + 1)*(g_aLife[nCntLife].nBlock_Y + 1);											// 頂点数
			g_aLife[nCntLife].nNumIndex = 2 * (g_aLife[nCntLife].nBlock_X*g_aLife[nCntLife].nBlock_Y) + 4 * (g_aLife[nCntLife].nBlock_Y - 1) + 2;	// インデックス数										// 
			g_aLife[nCntLife].nNumPolygon = 2 * (g_aLife[nCntLife].nBlock_X*g_aLife[nCntLife].nBlock_Y) + 4 * (g_aLife[nCntLife].nBlock_Y - 1);	// ポリゴン数

			MakeVertexLife(pDevice);
			g_aLife[nCntLife].bUse = true;										// 使用されているかどうか
			break;
		}
	}
}
//==============================================================================================================================
// ライフの獲得
//==============================================================================================================================
Life *GetLife(void)
{
	return &g_aLife[0];
}
LifeData *GetLifeData(void)
{
	return &g_aLifeData[0];
}
//==============================================================================================================================
// ライフの初期設定
//==============================================================================================================================
void SetPlayerLife(int nLife)
{
	g_nLife = nLife;
}
//==============================================================================================================================
// ライフの変更
//==============================================================================================================================
void GetPlayerLife(int nLife)
{
	g_nLife = nLife;
	g_bDown = true;
}