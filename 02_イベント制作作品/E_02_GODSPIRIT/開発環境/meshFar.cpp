////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 円の処理 [meshFar.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshFar.h"
#include "input.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MESHFAR	(1)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexMeshFar(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
MeshFar g_aMeshFar[MAX_MESHFAR];
MeshFarData g_aMeshFarData[24];
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitMeshFar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		g_aMeshFar[nCntMeshFar].pVtxBuff = NULL;									// 頂点バッファへのポインタ
		g_aMeshFar[nCntMeshFar].pIdxBuff = NULL;									// インデックスバッファへのポインタ
		g_aMeshFar[nCntMeshFar].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					// 位置
		g_aMeshFar[nCntMeshFar].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
		g_aMeshFar[nCntMeshFar].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);				// 色
		g_aMeshFar[nCntMeshFar].fDepth = 0;											// 奥行き
		g_aMeshFar[nCntMeshFar].nBlock_X = 0;										// ブロックX
		g_aMeshFar[nCntMeshFar].nBlock_Z = 0;										// ブロックZ
		g_aMeshFar[nCntMeshFar].nType = 0;		// 種類
		g_aMeshFar[nCntMeshFar].fMoveVtx = 0;									// 頂点ごとの高さ
		g_aMeshFar[nCntMeshFar].fTexmove = 0;									// テクスチャの移動速度
		g_aMeshFar[nCntMeshFar].fTexPos = 0;								// テクスチャの移動
		g_aMeshFar[nCntMeshFar].nNumVertex =(g_aMeshFar[nCntMeshFar].nBlock_X + 1)*(g_aMeshFar[nCntMeshFar].nBlock_Z + 1);// 頂点数
		g_aMeshFar[nCntMeshFar].nNumIndex = 2*(g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z)+4*(g_aMeshFar[nCntMeshFar].nBlock_Z - 1)+2;// インデックス数										// 
		g_aMeshFar[nCntMeshFar].nNumPolygon = 2*(g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z)+4*(g_aMeshFar[nCntMeshFar].nBlock_Z-1);	// ポリゴン数
		g_aMeshFar[nCntMeshFar].bCull = false;										// カリング
		g_aMeshFar[nCntMeshFar].bUse = false;										// 使用されているかどうか
	}
	g_aMeshFarData[0].pFileName = "data/TEXTURE/cloud.jpg";

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		g_aMeshFarData[0].pFileName,						// ファイルの名前
		&g_aMeshFarData[0].pTexture);						// テクスチャへのポインタ
	SetMeshFar(D3DXVECTOR3(0,5550,0), D3DXVECTOR3(0, 0, 0), 0, 30, 1,3000, true);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitMeshFar(void)
{
	// テクスチャの破棄
	for (int nCntMeshFar = 0; nCntMeshFar < g_aMeshFarData[0].nNumTex; nCntMeshFar++)
	{
		if (g_aMeshFarData[nCntMeshFar].pTexture != NULL)
		{
			g_aMeshFarData[nCntMeshFar].pTexture->Release();
			g_aMeshFarData[nCntMeshFar].pTexture = NULL;
		}
	}
	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		// 頂点バッファの破棄
		if (g_aMeshFar[nCntMeshFar].pVtxBuff != NULL)
		{
			g_aMeshFar[nCntMeshFar].pVtxBuff->Release();
			g_aMeshFar[nCntMeshFar].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aMeshFar[nCntMeshFar].pIdxBuff != NULL)
		{
			g_aMeshFar[nCntMeshFar].pIdxBuff->Release();
			g_aMeshFar[nCntMeshFar].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateMeshFar(void)
{

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawMeshFar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == true)
		{
			if (g_aMeshFar[nCntMeshFar].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshFar[nCntMeshFar].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshFar[nCntMeshFar].rot.y, g_aMeshFar[nCntMeshFar].rot.x, g_aMeshFar[nCntMeshFar].rot.z);
			D3DXMatrixMultiply(&g_aMeshFar[nCntMeshFar].mtxWorld, &g_aMeshFar[nCntMeshFar].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshFar[nCntMeshFar].pos.x, g_aMeshFar[nCntMeshFar].pos.y, g_aMeshFar[nCntMeshFar].pos.z);
			D3DXMatrixMultiply(&g_aMeshFar[nCntMeshFar].mtxWorld, &g_aMeshFar[nCntMeshFar].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshFar[nCntMeshFar].mtxWorld);
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aMeshFar[nCntMeshFar].pVtxBuff, 0, sizeof(VERTEX_3D));
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aMeshFar[nCntMeshFar].pIdxBuff);
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_aMeshFarData[g_aMeshFar[nCntMeshFar].nType].pTexture);
			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, g_aMeshFar[nCntMeshFar].nNumIndex, 0, g_aMeshFar[nCntMeshFar].nNumPolygon);
			if (g_aMeshFar[nCntMeshFar].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
		}
	}

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexMeshFar(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == false)
		{

			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshFar[nCntMeshFar].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aMeshFar[nCntMeshFar].pVtxBuff,
				NULL);

			float fAngle=0, fLength=0;
			fLength = g_aMeshFar[nCntMeshFar].fDepth;
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshFar[nCntMeshFar].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
			for (int nCntZ = 0; nCntZ < g_aMeshFar[nCntMeshFar].nBlock_Z + 1; nCntZ++)
			{
				
				for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nBlock_X + 1; nCnt++)
				{// メッシュ
					fAngle = (D3DX_PI * 2 / (g_aMeshFar[nCntMeshFar].nBlock_X -1))*nCnt;

					if (nCnt == 0)
					{
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].tex = D3DXVECTOR2(1.0f, 1.0f);
					}
					else
					{
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].pos = D3DXVECTOR3(sinf(fAngle)*fLength,0.0f,cosf(fAngle)*fLength);
						pVtx[nCntZ*(g_aMeshFar[nCntMeshFar].nBlock_X + 1) + nCnt].tex = D3DXVECTOR2((1.0f / (g_aMeshFar[nCntMeshFar].nBlock_X - 1))*nCnt, (1.0f / (g_aMeshFar[nCntMeshFar].nBlock_X - 1))*nCnt);

					}
				}
			}
			// 法線の設定
			for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aMeshFar[nCntMeshFar].col;
			}
			//// ブロックごとに一枚配置するテクスチャ
			//for (int nCnt = 0; nCnt < g_aMeshFar[nCntMeshFar].nNumVertex; nCnt++)
			//{
			//	// テクスチャの設定
			//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(g_aMeshFar[nCntMeshFar].nBlock_X+1)), 0.0f + (nCnt / (g_aMeshFar[nCntMeshFar].nBlock_X + 1)));
			//}

			// 動くテクスチャ
			//for (int nCntV = 0; nCntV < g_aMeshFar[nCntMeshFar].nBlock_Z + 1; nCntV++)
			//{// テクスチャの設定
			//	for (int nCntH = 0; nCntH < g_aMeshFar[nCntMeshFar].nBlock_X + 1; nCntH++)
			//	{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			//	 // テクスチャの設定
			//		pVtx[nCntH + (g_aMeshFar[nCntMeshFar].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshFar[nCntMeshFar].nBlock_X) / 100.0f + g_aMeshFar[nCntMeshFar].fTexPos, float(nCntV * 100 / g_aMeshFar[nCntMeshFar].nBlock_Z) / 100.0f);
			//	}
			//}
			// 頂点バッファをアンロックする
			g_aMeshFar[nCntMeshFar].pVtxBuff->Unlock();


			WORD *pIdx;	// インデックスデータへのポインタ

			// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aMeshFar[nCntMeshFar].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aMeshFar[nCntMeshFar].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aMeshFar[nCntMeshFar].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	

			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshFar[nCntMeshFar].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aMeshFar[nCntMeshFar].nBlock_X + 2) == g_aMeshFar[nCntMeshFar].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aMeshFar[nCntMeshFar].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aMeshFar[nCntMeshFar].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aMeshFar[nCntMeshFar].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetMeshFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType, int nBlock_X, int nBlock_Z, float fDepth,bool bCull)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshFar = 0; nCntMeshFar < MAX_MESHFAR; nCntMeshFar++)
	{
		if (g_aMeshFar[nCntMeshFar].bUse == false)
		{
			g_aMeshFar[nCntMeshFar].pos = pos;					// 位置
			g_aMeshFar[nCntMeshFar].rot = rot;				// 向き
			g_aMeshFar[nCntMeshFar].fDepth = fDepth;											// 奥行き
			g_aMeshFar[nCntMeshFar].nBlock_X = nBlock_X;										// ブロックX
			g_aMeshFar[nCntMeshFar].nBlock_Z = nBlock_Z;										// ブロックZ
			g_aMeshFar[nCntMeshFar].nType = nType;		// 種類
			g_aMeshFar[nCntMeshFar].bCull = bCull;		// カリング
			g_aMeshFar[nCntMeshFar].nNumVertex = (g_aMeshFar[nCntMeshFar].nBlock_X + 1)*(g_aMeshFar[nCntMeshFar].nBlock_Z + 1);// 頂点数
			g_aMeshFar[nCntMeshFar].nNumIndex = 2 * (g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z) + 4 * (g_aMeshFar[nCntMeshFar].nBlock_Z - 1) + 2;// インデックス数										// 
			g_aMeshFar[nCntMeshFar].nNumPolygon = 2 * (g_aMeshFar[nCntMeshFar].nBlock_X*g_aMeshFar[nCntMeshFar].nBlock_Z) + 4 * (g_aMeshFar[nCntMeshFar].nBlock_Z - 1);	// ポリゴン数		


			MakeVertexMeshFar(pDevice);
			g_aMeshFar[nCntMeshFar].bUse = true;										// 使用されているかどうか

			break;
		}
	}
}
//==============================================================================================================================
// メッシュの獲得
//==============================================================================================================================
MeshFar *GetMeshFar(void)
{
	return &g_aMeshFar[0];
}
MeshFarData *GetMeshFarData(void)
{
	return &g_aMeshFarData[0];
}
