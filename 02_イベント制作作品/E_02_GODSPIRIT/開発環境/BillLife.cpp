////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ビルボードライフ処理 [BillLife.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "BIllLife.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_BILLLIFE	(520)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexBillLife(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
BillLife g_aBillLife[MAX_BILLLIFE];
BillLifeData g_aBillLifeData[BILLLIFETYPE_MAX] = 
{
	{ NULL,"data/TEXTURE/.jpeg" },
	{ NULL,"data/TEXTURE/.jpeg" },
	{ NULL,"data/TEXTURE/life.jpeg" },
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		g_aBillLife[nCntBillLife].pVtxBuff = NULL;									// 頂点バッファへのポインタ
		g_aBillLife[nCntBillLife].pIdxBuff = NULL;									// インデックスバッファへのポインタ
		g_aBillLife[nCntBillLife].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);				// 位置
		g_aBillLife[nCntBillLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
		g_aBillLife[nCntBillLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色
		g_aBillLife[nCntBillLife].fWidth = 0;										// 幅
		g_aBillLife[nCntBillLife].fHeight = 0;										// 高さ
		g_aBillLife[nCntBillLife].nBlock_X = 1;										// ブロックX
		g_aBillLife[nCntBillLife].nBlock_Y = 1;										// ブロックY
		g_aBillLife[nCntBillLife].nType = BILLLIFETYPE_ENEMY;						// 種類
		g_aBillLife[nCntBillLife].fMoveVtx = 0;										// 頂点ごとの高さ
		g_aBillLife[nCntBillLife].fTexmoveMesh = 0;									// テクスチャの移動速度
		g_aBillLife[nCntBillLife].nLife = 0;										// ライフ
		g_aBillLife[nCntBillLife].nInitLife = 0;									// 初期ライフ
		g_aBillLife[nCntBillLife].bDown = false;									// ダウン中かどうか
		g_aBillLife[nCntBillLife].bUseDown = false;									// ダウンするかどうか
		g_aBillLife[nCntBillLife].bDownRed = false;									// 背景の赤いライフ用
		g_aBillLife[nCntBillLife].nDownRedCounter = 0;								// 背景の赤いライフ用減少
		g_aBillLife[nCntBillLife].nDrawDown = 0;									// ポリゴン減算用
		g_aBillLife[nCntBillLife].nIdy = 0;											// 番号
		g_aBillLife[nCntBillLife].fOffset = 0;										// オフセット
		g_aBillLife[nCntBillLife].nCntTime = 0;										// 死亡したときライフが消えるタイミング
		g_aBillLife[nCntBillLife].bUse = false;										// 使用されているかどうか
	}
	for (int nCntTex = 0; nCntTex < BILLLIFETYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			g_aBillLifeData[nCntTex].pFileName,					// ファイルの名前
			&g_aBillLifeData[nCntTex].pTexture);				// テクスチャへのポインタ
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitBillLife(void)
{
	// テクスチャの破棄
	for (int nCntBillLife = 0; nCntBillLife < BILLLIFETYPE_MAX; nCntBillLife++)
	{
		if (g_aBillLifeData[nCntBillLife].pTexture!= NULL)
		{
			g_aBillLifeData[nCntBillLife].pTexture->Release();
			g_aBillLifeData[nCntBillLife].pTexture = NULL;
		}
	}
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		// 頂点バッファの破棄
		if (g_aBillLife[nCntBillLife].pVtxBuff != NULL)
		{
			g_aBillLife[nCntBillLife].pVtxBuff->Release();
			g_aBillLife[nCntBillLife].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aBillLife[nCntBillLife].pIdxBuff != NULL)
		{
			g_aBillLife[nCntBillLife].pIdxBuff->Release();
			g_aBillLife[nCntBillLife].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得

	for (int nCntLife = 0; nCntLife < MAX_BILLLIFE; nCntLife++)
	{
		if (g_aBillLife[nCntLife].bUse == true)
		{
			if (g_aBillLife[nCntLife].bUseDown == true)
			{
				if (g_aBillLife[nCntLife].nType == BILLLIFETYPE_ENEMY)
				{
					if (g_aBillLife[nCntLife].bDown == true)
					{// 減少中
						if (g_aBillLife[nCntLife].nInitLife <= 5)
						{// ライフが５以下の場合速度を速める			
							g_aBillLife[nCntLife].nDrawDown += 3;
						}
						else
						{
							g_aBillLife[nCntLife].nDrawDown++;
						}
						if (g_aBillLife[nCntLife].nDrawDown >= 100 - ((float)g_aBillLife[nCntLife].nLife / g_aBillLife[nCntLife].nInitLife) * 100)
						{// 100%中 X%減ったとこまで減らす
							g_aBillLife[nCntLife].bDown = false;
							g_aBillLife[nCntLife - 1].bDownRed = true;		// ライフの前が赤ライフなら-1
							g_aBillLife[nCntLife - 1].nDownRedCounter = 30; // 赤ライフ減少間隔保管
							if (g_aBillLife[nCntLife].nLife <= 0)
							{// 死んだら保管なし
								g_aBillLife[nCntLife - 1].nDownRedCounter = 0;
							}
						}
					}
				}
				if (g_aBillLife[nCntLife].nType == BILLLIFETYPE_ENEMYBGRED)
				{
					if (g_aBillLife[nCntLife].bDownRed == true)
					{
						g_aBillLife[nCntLife].nDownRedCounter--;
						if (g_aBillLife[nCntLife].nDownRedCounter <= 0)
						{
							g_aBillLife[nCntLife].nDownRedCounter = 0;
							if (g_aBillLife[nCntLife].nInitLife <= 5)
							{// ライフが５以下の場合速度を速める
								g_aBillLife[nCntLife].nDrawDown += 3;
							}
							else
							{
								g_aBillLife[nCntLife].nDrawDown++;
							}
							if (g_aBillLife[nCntLife].nDrawDown >= 100 - ((float)g_aBillLife[nCntLife].nLife / g_aBillLife[nCntLife].nInitLife) * 100)
							{// 100%中 X%減ったとこまで減らす
								g_aBillLife[nCntLife].bDown = false;
								g_aBillLife[nCntLife].bDownRed = false;
							}
						}
					}
				}
			}
			if (g_aBillLife[nCntLife].nLife <= 0)
			{
				g_aBillLife[nCntLife].nCntTime++;
				if (g_aBillLife[nCntLife].nCntTime >= 30)
				{
					g_aBillLife[nCntLife].bUse = false;
					g_aBillLife[nCntLife].bUse = false;
				}
			}
		}
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawBillLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビルボード用マトリックス
	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);				// αブレンドを行う

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nLife != g_aBillLife[nCntBillLife].nInitLife)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aBillLife[nCntBillLife].mtxWorld);
				// ビルボードの設定
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);
				// 逆行列の設定
				g_aBillLife[nCntBillLife].mtxWorld._11 = mtxView._11;
				g_aBillLife[nCntBillLife].mtxWorld._12 = mtxView._21;
				g_aBillLife[nCntBillLife].mtxWorld._13 = mtxView._31;
				g_aBillLife[nCntBillLife].mtxWorld._21 = mtxView._12;
				g_aBillLife[nCntBillLife].mtxWorld._22 = mtxView._22;
				g_aBillLife[nCntBillLife].mtxWorld._23 = mtxView._32;
				g_aBillLife[nCntBillLife].mtxWorld._31 = mtxView._13;
				g_aBillLife[nCntBillLife].mtxWorld._32 = mtxView._23;
				g_aBillLife[nCntBillLife].mtxWorld._33 = mtxView._33;
				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aBillLife[nCntBillLife].pos.x, g_aBillLife[nCntBillLife].pos.y, g_aBillLife[nCntBillLife].pos.z);
				D3DXMatrixMultiply(&g_aBillLife[nCntBillLife].mtxWorld, &g_aBillLife[nCntBillLife].mtxWorld, &mtxTrans);
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aBillLife[nCntBillLife].mtxWorld);
				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_aBillLife[nCntBillLife].pVtxBuff, 0, sizeof(VERTEX_3D));
				// インデックスバッファをデータストリームに設定
				pDevice->SetIndices(g_aBillLife[nCntBillLife].pIdxBuff);
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_aBillLifeData[g_aBillLife[nCntBillLife].nType].pTexture);
				// ポリゴンの描画
				if (g_aBillLife[nCntBillLife].bUseDown == true)
				{
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aBillLife[nCntBillLife].nNumIndex, 0, g_aBillLife[nCntBillLife].nNumPolygon - g_aBillLife[nCntBillLife].nDrawDown * 2);
				}
				else
				{
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aBillLife[nCntBillLife].nNumIndex, 0, g_aBillLife[nCntBillLife].nNumPolygon);
				}
			}
		}
	}
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化
																// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Zバッファへの書き込み
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexBillLife(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == false)
		{
			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aBillLife[nCntBillLife].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aBillLife[nCntBillLife].pVtxBuff,
				NULL);

			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aBillLife[nCntBillLife].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
			//for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			//{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			//	pVtx[nCnt].pos = D3DXVECTOR3(-(g_aBillLife[nCntBillLife].fWidth  * g_aBillLife[nCntBillLife].nBlock_X / 2) + (g_aBillLife[nCntBillLife].fWidth )*(nCnt % (g_aBillLife[nCntBillLife].nBlock_X + 1)),
			//		(g_aBillLife[nCntBillLife].fHeight * g_aBillLife[nCntBillLife].nBlock_Y) - (g_aBillLife[nCntBillLife].fHeight)*(nCnt / (g_aBillLife[nCntBillLife].nBlock_X + 1)), 0.0f);
			//}

			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				pVtx[nCnt].pos = D3DXVECTOR3(-(g_aBillLife[nCntBillLife].fWidth/2) + (g_aBillLife[nCntBillLife].fWidth / g_aBillLife[nCntBillLife].nBlock_X)*(nCnt % (g_aBillLife[nCntBillLife].nBlock_X + 1)),
					(g_aBillLife[nCntBillLife].fHeight / 2) -(g_aBillLife[nCntBillLife].fHeight)*(nCnt / (g_aBillLife[nCntBillLife].nBlock_X + 1)), 0.0f) ;
			}
			// 法線の設定
			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aBillLife[nCntBillLife].col;
			}
			// ブロックごとに一枚配置するテクスチャ
			//for (int nCnt = 0; nCnt < g_aBillLife[nCntBillLife].nNumVertex; nCnt++)
			//{
			//	// テクスチャの設定
			//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(BLOCK_X+1)), 0.0f + (nCnt / (BLOCK_X + 1)));
			//}

			// 動くテクスチャ
			for (int nCntV = 0; nCntV < g_aBillLife[nCntBillLife].nBlock_Y + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aBillLife[nCntBillLife].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aBillLife[nCntBillLife].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aBillLife[nCntBillLife].nBlock_X) / 100.0f + g_aBillLife[nCntBillLife].fTexmoveMesh, float(nCntV * 100 / g_aBillLife[nCntBillLife].nBlock_Y) / 100.0f);
				}
			}
			// 頂点バッファをアンロックする
			g_aBillLife[nCntBillLife].pVtxBuff->Unlock();

			WORD *pIdx;	// インデックスデータへのポインタ

				// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aBillLife[nCntBillLife].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aBillLife[nCntBillLife].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aBillLife[nCntBillLife].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aBillLife[nCntBillLife].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aBillLife[nCntBillLife].nBlock_X + 2) == g_aBillLife[nCntBillLife].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aBillLife[nCntBillLife].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aBillLife[nCntBillLife].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aBillLife[nCntBillLife].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetBillLife(int nIdy, D3DXVECTOR3 pos, int nLife, D3DXCOLOR col,BILLLIFETYPE nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, float fOffset, bool bUseDown)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == false)
		{
			g_aBillLife[nCntBillLife].fOffset = fOffset;			// オフセット
			g_aBillLife[nCntBillLife].pos = pos;					// 位置
			g_aBillLife[nCntBillLife].pos.y += g_aBillLife[nCntBillLife].fOffset;	// 位置
			g_aBillLife[nCntBillLife].col = col;					// 色
			g_aBillLife[nCntBillLife].nLife = nLife;				// ライフ
			g_aBillLife[nCntBillLife].nInitLife = nLife;			// 初期ライフ
			g_aBillLife[nCntBillLife].fWidth = fWidth;				// 幅
			g_aBillLife[nCntBillLife].fHeight = fHeight;			// 奥行き
			g_aBillLife[nCntBillLife].nBlock_X = nBlock_X;			// ブロックX
			g_aBillLife[nCntBillLife].nBlock_Y = nBlock_Y;			// ブロックZ
			g_aBillLife[nCntBillLife].nType = nType;				// 種類
			g_aBillLife[nCntBillLife].nNumVertex = (g_aBillLife[nCntBillLife].nBlock_X + 1)*(g_aBillLife[nCntBillLife].nBlock_Y + 1);											// 頂点数
			g_aBillLife[nCntBillLife].nNumIndex = 2 * (g_aBillLife[nCntBillLife].nBlock_X*g_aBillLife[nCntBillLife].nBlock_Y) + 4 * (g_aBillLife[nCntBillLife].nBlock_Y - 1) + 2;	// インデックス数										// 
			g_aBillLife[nCntBillLife].nNumPolygon = 2 * (g_aBillLife[nCntBillLife].nBlock_X*g_aBillLife[nCntBillLife].nBlock_Y) + 4 * (g_aBillLife[nCntBillLife].nBlock_Y - 1);	// ポリゴン数
			g_aBillLife[nCntBillLife].bUseDown = bUseDown;
			g_aBillLife[nCntBillLife].nIdy = nIdy;
			g_aBillLife[nCntBillLife].nCntTime = 0;
			g_aBillLife[nCntBillLife].nDrawDown = 0;
			g_aBillLife[nCntBillLife].bDown = false;				// ダウン中かどうか
			g_aBillLife[nCntBillLife].bDownRed = false;				// 赤いライフダウン中かどうか
			g_aBillLife[nCntBillLife].nDownRedCounter = 0;			// 赤いライフ減少間隔保管カウンター
			MakeVertexBillLife(pDevice);
			g_aBillLife[nCntBillLife].bUse = true;										// 使用されているかどうか
			break;
		}
	}
}
//==============================================================================================================================
// ライフの獲得
//==============================================================================================================================
BillLife *GetBillLife(void)
{
	return &g_aBillLife[0];
}
BillLifeData *GetBillLifeData(void)
{
	return &g_aBillLifeData[0];
}

//==============================================================================================================================
// ライフの変更
//==============================================================================================================================
void GetBillLife(int nIdy,int nLife)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].nLife = nLife;
				g_aBillLife[nCntBillLife].bDown = true;
			}
		}
	}
}
//==============================================================================================================================
// ポジション移動
//==============================================================================================================================
void SetPositionBillLife(int nIdy, D3DXVECTOR3 pos)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].pos = D3DXVECTOR3(pos.x,pos.y + g_aBillLife[nCntBillLife].fOffset,pos.z);
			}
		}
	}
}
//==============================================================================================================================
// 削除
//==============================================================================================================================
void DeleteBillLife(int nIdy)
{
	for (int nCntBillLife = 0; nCntBillLife < MAX_BILLLIFE; nCntBillLife++)
	{
		if (g_aBillLife[nCntBillLife].bUse == true)
		{
			if (g_aBillLife[nCntBillLife].nIdy == nIdy)
			{
				g_aBillLife[nCntBillLife].bUse = false;
			}
		}
	}
}
