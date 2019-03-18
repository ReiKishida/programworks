////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// スピリット処理 [spirit.cpp]
// Author : 圷 和也
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "spirit.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "game.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_SPIRIT_NUM	(3)
#define MAX_SPIRIT		(100.0f)
#define SPIRIT_ANIM		(0.3333f)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexSpirit(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Spirit g_aSpirit[MAX_SPIRIT_NUM];
SpiritData g_aSpiritData[SPIRITTEX_MAX] =
{
	{ NULL,"data/TEXTURE/.png" },				// スピリットの裏の黒
	{ NULL,"data/TEXTURE/spirit002.jpg" },		// スピリット
	{ NULL,"data/TEXTURE/Spirit_frame02.png" },	// スピリットのフレーム
};
int g_nDrawDownSpirit;// スピリットを減らす変数
int g_nSpirit;
bool g_bSpiritDown;
bool g_bSpiritUp;
bool b_SpiritCol;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		g_aSpirit[nCntSpirit].pVtxBuff = NULL;										// 頂点バッファへのポインタ
		g_aSpirit[nCntSpirit].pIdxBuff = NULL;										// インデックスバッファへのポインタ
		g_aSpirit[nCntSpirit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
		g_aSpirit[nCntSpirit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		g_aSpirit[nCntSpirit].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// 色
		g_aSpirit[nCntSpirit].fWidth = 0;											// 幅
		g_aSpirit[nCntSpirit].fHeight = 0;											// 高さ
		g_aSpirit[nCntSpirit].nBlock_X = 0;											// ブロックX
		g_aSpirit[nCntSpirit].nBlock_Y = 0;											// ブロックY
		g_aSpirit[nCntSpirit].nPatternAnim = 0;										// パターン
		g_aSpirit[nCntSpirit].nCounterAnim = 0;										// カウンター
		g_aSpirit[nCntSpirit].nType = 0;											// 種類
		g_aSpirit[nCntSpirit].bUse = false;											// 使用されているかどうか
		g_aSpirit[nCntSpirit].bUseDown = false;										// ダウン中かどうか
		g_aSpirit[nCntSpirit].nDrawDown = 0;									// ポリゴン減算用
	}
	g_nDrawDownSpirit = 0;
	g_nSpirit = 100;
	g_bSpiritDown = false;
	g_bSpiritUp = false;
	b_SpiritCol = false;
	for (int nCntTex = 0; nCntTex < SPIRITTEX_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			g_aSpiritData[nCntTex].pFileName,						// ファイルの名前
			&g_aSpiritData[nCntTex].pTexture);						// テクスチャへのポインタ
	}
	// 左を中心としての初期座標・回転・テクスチャ・スピリット分割数・いじらない・長さ横・縦
	SetSpirit(D3DXVECTOR3(550, 655, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), SPIRITTEX_BG2, 100, 1, 560, 15, false);
	SetSpirit(D3DXVECTOR3(550, 655, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRITTEX_GAGE, 100, 1, 560, 15, true);
	SetSpirit(D3DXVECTOR3(545, 650, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SPIRITTEX_BG, 100, 1, 560 + 10, 25, false);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitSpirit(void)
{
	// テクスチャの破棄
	for (int nCntSpirit = 0; nCntSpirit <SPIRITTEX_MAX; nCntSpirit++)
	{
		if (g_aSpiritData[nCntSpirit].pTexture != NULL)
		{
			g_aSpiritData[nCntSpirit].pTexture->Release();
			g_aSpiritData[nCntSpirit].pTexture = NULL;
		}
	}
	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		// 頂点バッファの破棄
		if (g_aSpirit[nCntSpirit].pVtxBuff != NULL)
		{
			g_aSpirit[nCntSpirit].pVtxBuff->Release();
			g_aSpirit[nCntSpirit].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aSpirit[nCntSpirit].pIdxBuff != NULL)
		{
			g_aSpirit[nCntSpirit].pIdxBuff->Release();
			g_aSpirit[nCntSpirit].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aSpirit[nCntSpirit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			if (g_aSpirit[nCntSpirit].bUseDown == true)
			{
				if (g_aSpirit[nCntSpirit].nType == SPIRITTEX_GAGE)
				{
					if (g_aSpirit[nCntSpirit].nDrawDown > int(100 - ((float)g_nSpirit / MAX_SPIRIT) * 100))
					{// 100%中 X%減ったとこまで減らす
						g_aSpirit[nCntSpirit].nDrawDown--;
					}
					else if (g_aSpirit[nCntSpirit].nDrawDown < int(100 - ((float)g_nSpirit / MAX_SPIRIT) * 100))
					{
						g_aSpirit[nCntSpirit].nDrawDown++;
					}
				}
			}
			if (g_aSpirit[nCntSpirit].nType == SPIRITTEX_GAGE)
			{
				if (b_SpiritCol == false)
				{
					g_aSpirit[nCntSpirit].col.a -= 0.01f;
					if (g_aSpirit[nCntSpirit].col.a <= 0.8f)
					{
						b_SpiritCol = true;
					}
				}
				else
				{
					g_aSpirit[nCntSpirit].col.a += 0.01f;
					if (g_aSpirit[nCntSpirit].col.a >= 1.0f)
					{
						b_SpiritCol = false;
					}
				}
				for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
				{
					pVtx[nCnt].col = g_aSpirit[nCntSpirit].col;
				}
				g_aSpirit[nCntSpirit].nCounterAnim++;
				if (g_aSpirit[nCntSpirit].nCounterAnim % 5 == 0)
				{
					g_aSpirit[nCntSpirit].nPatternAnim = (g_aSpirit[nCntSpirit].nPatternAnim + 1) % 3;
					for (int nCntV = 0; nCntV < g_aSpirit[nCntSpirit].nBlock_Y + 1; nCntV++)
					{// テクスチャの設定
						for (int nCntH = 0; nCntH < g_aSpirit[nCntSpirit].nBlock_X + 1; nCntH++)
						{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
						 // テクスチャの設定
							pVtx[nCntH + (g_aSpirit[nCntSpirit].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aSpirit[nCntSpirit].nBlock_X) / 100.0f, float(nCntV * 100 / g_aSpirit[nCntSpirit].nBlock_Y) / 100.0f + (SPIRIT_ANIM*g_aSpirit[nCntSpirit].nPatternAnim));
						}
					}
				}
			}
			// 頂点バッファをアンロックする
			g_aSpirit[nCntSpirit].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawSpirit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

												// ワイヤーフレーム
												//	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSpirit[nCntSpirit].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSpirit[nCntSpirit].rot.y, g_aSpirit[nCntSpirit].rot.x, g_aSpirit[nCntSpirit].rot.z);
			D3DXMatrixMultiply(&g_aSpirit[nCntSpirit].mtxWorld, &g_aSpirit[nCntSpirit].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSpirit[nCntSpirit].pos.x, g_aSpirit[nCntSpirit].pos.y, g_aSpirit[nCntSpirit].pos.z);
			D3DXMatrixMultiply(&g_aSpirit[nCntSpirit].mtxWorld, &g_aSpirit[nCntSpirit].mtxWorld, &mtxTrans);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aSpirit[nCntSpirit].pVtxBuff, 0, sizeof(VERTEX_2D));
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aSpirit[nCntSpirit].pIdxBuff);
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_aSpiritData[g_aSpirit[nCntSpirit].nType].pTexture);
			if (g_aSpirit[nCntSpirit].bUseDown == true)
			{
				// ポリゴンの描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aSpirit[nCntSpirit].nNumIndex, 0, g_aSpirit[nCntSpirit].nNumPolygon - g_aSpirit[nCntSpirit].nDrawDown * 2);
			}
			else
			{
				// ポリゴンの描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aSpirit[nCntSpirit].nNumIndex, 0, g_aSpirit[nCntSpirit].nNumPolygon);

			}
		}
	}

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexSpirit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;	// 頂点情報のポインタ
	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == false)
		{
			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * g_aSpirit[nCntSpirit].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_aSpirit[nCntSpirit].pVtxBuff,
				NULL);


			// 頂点バッファをロックし頂点データへのポインタを取得

			g_aSpirit[nCntSpirit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				pVtx[nCnt].pos = D3DXVECTOR3((g_aSpirit[nCntSpirit].fWidth / g_aSpirit[nCntSpirit].nBlock_X)*(nCnt % (g_aSpirit[nCntSpirit].nBlock_X + 1)),
					+(g_aSpirit[nCntSpirit].fHeight)*(nCnt / (g_aSpirit[nCntSpirit].nBlock_X + 1)), 0.0f) + g_aSpirit[nCntSpirit].pos;

			}
			// rhwの設定
			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{
				pVtx[nCnt].rhw = 1.0f;
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aSpirit[nCntSpirit].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aSpirit[nCntSpirit].col;
			}

			// 全体テクスチャ
			for (int nCntV = 0; nCntV < g_aSpirit[nCntSpirit].nBlock_Y + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aSpirit[nCntSpirit].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aSpirit[nCntSpirit].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aSpirit[nCntSpirit].nBlock_X) / 100.0f, float(nCntV * 100 / g_aSpirit[nCntSpirit].nBlock_Y) / 100.0f);
				}
			}
			// 頂点バッファをアンロックする
			g_aSpirit[nCntSpirit].pVtxBuff->Unlock();
			WORD *pIdx;	// インデックスデータへのポインタ

						// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aSpirit[nCntSpirit].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aSpirit[nCntSpirit].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aSpirit[nCntSpirit].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aSpirit[nCntSpirit].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aSpirit[nCntSpirit].nBlock_X + 2) == g_aSpirit[nCntSpirit].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aSpirit[nCntSpirit].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aSpirit[nCntSpirit].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aSpirit[nCntSpirit].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetSpirit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntSpirit = 0; nCntSpirit < MAX_SPIRIT_NUM; nCntSpirit++)
	{
		if (g_aSpirit[nCntSpirit].bUse == false)
		{
			g_aSpirit[nCntSpirit].pos = pos;				// 位置
			g_aSpirit[nCntSpirit].rot = rot;				// 向き
			g_aSpirit[nCntSpirit].col = col;				// 色
			g_aSpirit[nCntSpirit].fWidth = fWidth;			// 幅
			g_aSpirit[nCntSpirit].fHeight = fHeight;		// 奥行き
			g_aSpirit[nCntSpirit].nBlock_X = nBlock_X;		// ブロックX
			g_aSpirit[nCntSpirit].nBlock_Y = nBlock_Y;		// ブロックZ
			g_aSpirit[nCntSpirit].nType = nType;			// 種類
			g_aSpirit[nCntSpirit].bUseDown = bUseDown;
			g_aSpirit[nCntSpirit].nNumVertex = (g_aSpirit[nCntSpirit].nBlock_X + 1)*(g_aSpirit[nCntSpirit].nBlock_Y + 1);											// 頂点数
			g_aSpirit[nCntSpirit].nNumIndex = 2 * (g_aSpirit[nCntSpirit].nBlock_X*g_aSpirit[nCntSpirit].nBlock_Y) + 4 * (g_aSpirit[nCntSpirit].nBlock_Y - 1) + 2;	// インデックス数										// 
			g_aSpirit[nCntSpirit].nNumPolygon = 2 * (g_aSpirit[nCntSpirit].nBlock_X*g_aSpirit[nCntSpirit].nBlock_Y) + 4 * (g_aSpirit[nCntSpirit].nBlock_Y - 1);	// ポリゴン数

			MakeVertexSpirit(pDevice);
			g_aSpirit[nCntSpirit].bUse = true;										// 使用されているかどうか
			break;
		}
	}
}
//==============================================================================================================================
// スピリットの獲得
//==============================================================================================================================
Spirit *GetSpirit(void)
{
	return &g_aSpirit[0];
}
SpiritData *GetSpiritData(void)
{
	return &g_aSpiritData[0];
}
//==============================================================================================================================
// スピリットの初期設定
//==============================================================================================================================
void SetPlayerSpirit(int nSpirit)
{
	g_nSpirit = nSpirit;
}
//==============================================================================================================================
// スピリットの変更
//==============================================================================================================================
void GetPlayerSpirit(int nSpirit)
{
	g_nSpirit = nSpirit;	
}