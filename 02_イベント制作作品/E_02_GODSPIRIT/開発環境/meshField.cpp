////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ床処理 [meshField.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshField.h"
#include "input.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MESHFIELD	(100)
#define MAX_MESHFIELDTYPE	(24)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
MeshField g_aMeshField[MAX_MESHFIELD];
MeshFieldData g_aMeshFieldData[MAX_MESHFIELDTYPE];
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		g_aMeshField[nCntMeshField].pVtxBuff = NULL;									// 頂点バッファへのポインタ
		g_aMeshField[nCntMeshField].pIdxBuff = NULL;									// インデックスバッファへのポインタ
		g_aMeshField[nCntMeshField].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					// 位置
		g_aMeshField[nCntMeshField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
		g_aMeshField[nCntMeshField].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);				// 色
		g_aMeshField[nCntMeshField].fWidth = 0;											// 幅
		g_aMeshField[nCntMeshField].fDepth = 0;											// 奥行き
		g_aMeshField[nCntMeshField].nBlock_X = 0;										// ブロックX
		g_aMeshField[nCntMeshField].nBlock_Z = 0;										// ブロックZ
		g_aMeshField[nCntMeshField].fSlopeSin = 0;											// 斜め
		g_aMeshField[nCntMeshField].fSlopeSinMove = 0;										// 斜め計算
		g_aMeshField[nCntMeshField].fSlopeCos = 0;											// 斜め
		g_aMeshField[nCntMeshField].fSlopeCosMove = 0;										// 斜め計算
		g_aMeshField[nCntMeshField].nType = 0;		// 種類
		g_aMeshField[nCntMeshField].fMoveVtx = 0;									// 頂点ごとの高さ
		g_aMeshField[nCntMeshField].fTexmove = D3DXVECTOR2(0.0f,0.0f);									// テクスチャの移動速度
		g_aMeshField[nCntMeshField].fTexPos = D3DXVECTOR2(0.0f, 0.0f);								// テクスチャの移動
		g_aMeshField[nCntMeshField].nNumVertex =(g_aMeshField[nCntMeshField].nBlock_X + 1)*(g_aMeshField[nCntMeshField].nBlock_Z + 1);// 頂点数
		g_aMeshField[nCntMeshField].nNumIndex = 2*(g_aMeshField[nCntMeshField].nBlock_X*g_aMeshField[nCntMeshField].nBlock_Z)+4*(g_aMeshField[nCntMeshField].nBlock_Z - 1)+2;// インデックス数										// 
		g_aMeshField[nCntMeshField].nNumPolygon = 2*(g_aMeshField[nCntMeshField].nBlock_X*g_aMeshField[nCntMeshField].nBlock_Z)+4*(g_aMeshField[nCntMeshField].nBlock_Z-1);	// ポリゴン数
		g_aMeshField[nCntMeshField].bCull = false;										// カリング
		g_aMeshField[nCntMeshField].bCollision = true;									// あたり判定
		g_aMeshField[nCntMeshField].bUse = false;										// 使用されているかどうか
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitMeshField(void)
{
	// テクスチャの破棄
	for (int nCntMeshField = 0; nCntMeshField < g_aMeshFieldData[0].nNumTex; nCntMeshField++)
	{
		if (g_aMeshFieldData[nCntMeshField].pTexture != NULL)
		{
			g_aMeshFieldData[nCntMeshField].pTexture->Release();
			g_aMeshFieldData[nCntMeshField].pTexture = NULL;
		}
	}
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		// 頂点バッファの破棄
		if (g_aMeshField[nCntMeshField].pVtxBuff != NULL)
		{
			g_aMeshField[nCntMeshField].pVtxBuff->Release();
			g_aMeshField[nCntMeshField].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aMeshField[nCntMeshField].pIdxBuff != NULL)
		{
			g_aMeshField[nCntMeshField].pIdxBuff->Release();
			g_aMeshField[nCntMeshField].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateMeshField(void)
{

	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshField[nCntMeshField].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 波打ち（旗などに使用するとき）
#if 0
		//　波打ち
			if (g_aMeshField[nCntMeshField].nType == 3)
			{
				for (int nCntZ = 0; nCntZ < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntZ++)
				{
					g_aMeshField[nCntMeshField].fMoveVtx += 0.05f;
					for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nBlock_X + 1; nCnt++)
					{// メッシュ
						pVtx[nCntZ*(g_aMeshField[nCntMeshField].nBlock_X + 1)].pos.y = cosf(g_aMeshField[nCntMeshField].fMoveVtx) * 10;
					}
				}
				//for (int nCntZ = 0; nCntZ < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntZ++)
				//{
				//	g_aMeshField[nCntMeshField].fSlopeSinMove += 0.001f;
				//	for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nBlock_X + 1; nCnt++)
				//	{// メッシュ
				//		pVtx[nCntZ*(g_aMeshField[nCntMeshField].nBlock_X + 1) + nCnt].pos.y = cosf(g_aMeshField[nCntMeshField].fSlopeSinMove + float(nCntZ) / float(g_aMeshField[nCntMeshField].nBlock_Z)) * 10;
				//	}
				//}
			}
	
#endif

			// テクスチャ移動(海など）
#if 1
			g_aMeshField[nCntMeshField].fTexPos -= g_aMeshField[nCntMeshField].fTexmove;
			if (g_aMeshField[nCntMeshField].fTexPos.x <= -1.0f)
			{
				g_aMeshField[nCntMeshField].fTexPos.x = 0.0f;
			}
			if (g_aMeshField[nCntMeshField].fTexPos.y <= -1.0f)
			{
				g_aMeshField[nCntMeshField].fTexPos.y = 0.0f;
			}
			for (int nCntV = 0; nCntV < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aMeshField[nCntMeshField].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aMeshField[nCntMeshField].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshField[nCntMeshField].nBlock_X) / 100.0f + g_aMeshField[nCntMeshField].fTexPos.x, float(nCntV * 100 / g_aMeshField[nCntMeshField].nBlock_Z) / 100.0f + g_aMeshField[nCntMeshField].fTexPos.y);
				}
			}
			
#endif
			// 頂点バッファをアンロックする
			g_aMeshField[nCntMeshField].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse == true)
		{
			if (g_aMeshField[nCntMeshField].nType == MESHFIRLDTYPE_SEAPNG)
			{
				// Zバッファへの書き込み
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}
			if (g_aMeshField[nCntMeshField].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshField[nCntMeshField].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntMeshField].rot.y, g_aMeshField[nCntMeshField].rot.x, g_aMeshField[nCntMeshField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntMeshField].pos.x, g_aMeshField[nCntMeshField].pos.y, g_aMeshField[nCntMeshField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntMeshField].mtxWorld, &g_aMeshField[nCntMeshField].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntMeshField].mtxWorld);
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aMeshField[nCntMeshField].pVtxBuff, 0, sizeof(VERTEX_3D));
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aMeshField[nCntMeshField].pIdxBuff);
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_aMeshFieldData[g_aMeshField[nCntMeshField].nType].pTexture);
			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aMeshField[nCntMeshField].nNumIndex, 0, g_aMeshField[nCntMeshField].nNumPolygon);
			if (g_aMeshField[nCntMeshField].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			if (g_aMeshField[nCntMeshField].nType == MESHFIRLDTYPE_SEAPNG)
			{
				// Zバッファへの書き込み
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
		}
	}
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse == false)
		{

			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshField[nCntMeshField].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aMeshField[nCntMeshField].pVtxBuff,
				NULL);

			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshField[nCntMeshField].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//float PosX = g_aMeshField[nCntMeshField].fWidth * (g_aMeshField[nCntMeshField].nBlock_X * -0.5f);
			//float PosZ = g_aMeshField[nCntMeshField].fDepth * (g_aMeshField[nCntMeshField].nBlock_Z * 0.5f);
			// 頂点座標の設定
			for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nNumVertex; nCnt++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
	/*			if (nCnt % (g_aMeshField[nCntMeshField].nBlock_X + 1) == 0)
				{
					PosX = g_aMeshField[nCntMeshField].fWidth *(g_aMeshField[nCntMeshField].nBlock_X * -0.5f);
					if (nCnt != 0)
					{
						PosZ -= g_aMeshField[nCntMeshField].fDepth;
					}
				}
				pVtx[nCnt].pos = D3DXVECTOR3(PosX, 0.0f, PosZ);
				PosX += g_aMeshField[nCntMeshField].fWidth;*/

				//pVtx[nCnt].pos = D3DXVECTOR3(-(g_aMeshField[nCntMeshField].fWidth  * float(g_aMeshField[nCntMeshField].nBlock_X/2) ) + (g_aMeshField[nCntMeshField].fWidth)*(nCnt % (g_aMeshField[nCntMeshField].nBlock_X + 1)),
				//	0.0f, (g_aMeshField[nCntMeshField].fDepth * g_aMeshField[nCntMeshField].nBlock_Z /2) - (g_aMeshField[nCntMeshField].fDepth)*(nCnt / (g_aMeshField[nCntMeshField].nBlock_Z + 1)));
		
				// 頂点情報の設定	座標の直接指定はここでは基本的にはしない(g_pos~~等を足し引きはしないD3DXMatrixTranslationでやってる
				for (int nCntPos = 0; nCntPos < g_aMeshField[nCntMeshField].nNumVertex; nCntPos++)
				{
					pVtx[nCntPos].pos = D3DXVECTOR3(float(-((g_aMeshField[nCntMeshField].nBlock_X * g_aMeshField[nCntMeshField].fWidth) / 2) + g_aMeshField[nCntMeshField].fWidth * (nCntPos % (g_aMeshField[nCntMeshField].nBlock_X + 1))),
						0.0f,
						float((g_aMeshField[nCntMeshField].nBlock_Z * g_aMeshField[nCntMeshField].fDepth) / 2 - g_aMeshField[nCntMeshField].fDepth * (nCntPos / (g_aMeshField[nCntMeshField].nBlock_X + 1))));
				}
			}
			// 法線の設定
			for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aMeshField[nCntMeshField].col;
			}
			//// ブロックごとに一枚配置するテクスチャ
			//for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nNumVertex; nCnt++)
			//{
			//	// テクスチャの設定
			//	pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(g_aMeshField[nCntMeshField].nBlock_X+1)), 0.0f + (nCnt / (g_aMeshField[nCntMeshField].nBlock_X + 1)));
			//}

			if (g_aMeshField[nCntMeshField].fSlopeSin > 0 || g_aMeshField[nCntMeshField].fSlopeSin < 0)
			{
				for (int nCntZ = 0; nCntZ < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntZ++)
				{
					g_aMeshField[nCntMeshField].fSlopeSinMove += float(D3DX_PI / (g_aMeshField[nCntMeshField].nBlock_Z));
					for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nBlock_X + 1; nCnt++)
					{// メッシュ
						pVtx[nCntZ*(g_aMeshField[nCntMeshField].nBlock_X + 1) + nCnt].pos.y = sinf(g_aMeshField[nCntMeshField].fSlopeSinMove) * g_aMeshField[nCntMeshField].fSlopeSin;
					}
				}
			}
			if (g_aMeshField[nCntMeshField].fSlopeCos > 0 || g_aMeshField[nCntMeshField].fSlopeCos < 0)
			{
				for (int nCntZ = 0; nCntZ < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntZ++)
				{
					g_aMeshField[nCntMeshField].fSlopeCosMove += float(D3DX_PI / (g_aMeshField[nCntMeshField].nBlock_Z));
					for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nBlock_X + 1; nCnt++)
					{// メッシュ
						pVtx[nCntZ*(g_aMeshField[nCntMeshField].nBlock_X + 1) + nCnt].pos.y = cosf(g_aMeshField[nCntMeshField].fSlopeCosMove) * g_aMeshField[nCntMeshField].fSlopeCos;
					}
				}
			}
			for (int nCntV = 0; nCntV < g_aMeshField[nCntMeshField].nBlock_Z + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aMeshField[nCntMeshField].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aMeshField[nCntMeshField].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshField[nCntMeshField].nBlock_X) / 100.0f + g_aMeshField[nCntMeshField].fTexPos.x, float(nCntV * 100 / g_aMeshField[nCntMeshField].nBlock_Z) / 100.0f + g_aMeshField[nCntMeshField].fTexPos.y);
				}
			}
			// 頂点バッファをアンロックする
			g_aMeshField[nCntMeshField].pVtxBuff->Unlock();


			WORD *pIdx;	// インデックスデータへのポインタ

			// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aMeshField[nCntMeshField].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aMeshField[nCntMeshField].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aMeshField[nCntMeshField].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			//int nCnt = 0;
			//// 頂点座標の設定
			//for (int nCntX = 0; nCntX < g_aMeshField[nCntMeshField].nNumIndex; nCntX += 2)
			//{
			//	if (g_aMeshField[nCntMeshField].nBlock_X > 1 && nCntX % ((2 + 2 * g_aMeshField[nCntMeshField].nBlock_X) + 2) == (2 + 2 * g_aMeshField[nCntMeshField].nBlock_X))
			//	{// 重なり部分
			//		pIdx[nCntX] = pIdx[nCntX - 1];
			//		pIdx[nCntX + 1] = (g_aMeshField[nCntMeshField].nBlock_X + 1) + int(nCnt / 2);
			//	}
			//	else
			//	{// 通常部分
			//		pIdx[nCntX] = (g_aMeshField[nCntMeshField].nBlock_X + 1) + int(nCnt / 2);
			//		pIdx[nCntX + 1] = int(nCnt / 2);
			//		nCnt += 2;
			//	}
			//}

			//int nIdxX = g_aMeshField[nCntMeshField].nBlock_X + 1;
			//int nIdxZ = (g_aMeshField[nCntMeshField].nBlock_Z + 1) * 2;
			//for (int nCntMesh = 0; nCntMesh < g_aMeshField[nCntMeshField].nNumIndex; nCntMesh += 2)
			//{
			//	if (nCntMesh == nIdxZ)
			//	{
			//		pIdx[nCntMesh] = nIdxX - (g_aMeshField[nCntMeshField].nBlock_X + 2);
			//		pIdx[nCntMesh + 1] = nIdxX;
			//		nIdxZ += ((g_aMeshField[nCntMeshField].nBlock_X + 1) * 2) + 2;
			//	}
			//	else
			//	{
			//		pIdx[nCntMesh] = nIdxX;
			//		pIdx[nCntMesh + 1] = nIdxX - (g_aMeshField[nCntMeshField].nBlock_X + 1);
			//		nIdxX++;
			//	}
			//}

			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshField[nCntMeshField].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aMeshField[nCntMeshField].nBlock_X + 2) == g_aMeshField[nCntMeshField].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aMeshField[nCntMeshField].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aMeshField[nCntMeshField].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aMeshField[nCntMeshField].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// フィールドのあたり判定
//==============================================================================================================================
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight,MeshField **pMeshField)
{
	bool bLand = false;

// 頂点１つ１つのあたり判定
#if 1
	*pMeshField = NULL;
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshField[nCntMeshField].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			if (g_aMeshField[nCntMeshField].bCollision == true)
			{
				if (g_aMeshField[nCntMeshField].pos.x - g_aMeshField[nCntMeshField].fWidth  * g_aMeshField[nCntMeshField].nBlock_X / 2 < pPos->x + fRadiusWidth&&	// 左
					g_aMeshField[nCntMeshField].pos.x + g_aMeshField[nCntMeshField].fWidth  * g_aMeshField[nCntMeshField].nBlock_X / 2 > pPos->x - fRadiusWidth&&		// 右
					g_aMeshField[nCntMeshField].pos.z - g_aMeshField[nCntMeshField].fDepth  * g_aMeshField[nCntMeshField].nBlock_Z / 2 < pPos->z + fRadiusWidth&&	// 手前
					g_aMeshField[nCntMeshField].pos.z + g_aMeshField[nCntMeshField].fDepth  * g_aMeshField[nCntMeshField].nBlock_Z / 2 > pPos->z - fRadiusWidth)		// 奥
				{// X Z が範囲内
					for (int nCnt = 0; nCnt < g_aMeshField[nCntMeshField].nNumVertex; nCnt++)
					{
						if (g_aMeshField[nCntMeshField].pos.x + pVtx[nCnt].pos.x - g_aMeshField[nCntMeshField].fWidth / 2 <= pPos->x + fRadiusWidth&&	// 左
							g_aMeshField[nCntMeshField].pos.x + pVtx[nCnt].pos.x + g_aMeshField[nCntMeshField].fWidth / 2 >= pPos->x - fRadiusWidth&&		// 右
							g_aMeshField[nCntMeshField].pos.z + pVtx[nCnt].pos.z - g_aMeshField[nCntMeshField].fDepth / 2 <= pPos->z + fRadiusWidth&&	// 手前
							g_aMeshField[nCntMeshField].pos.z + pVtx[nCnt].pos.z + g_aMeshField[nCntMeshField].fDepth / 2 >= pPos->z - fRadiusWidth)		// 奥
						{
							if (g_aMeshField[nCntMeshField].rot.x == D3DX_PI)
							{// 天井なら逆向きに反射する
								if (g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y <= pPos->y + fRadiusHeight&&
									g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y  > pPosOld->y + fRadiusHeight)
								{// 下
									pPos->y = g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y - fRadiusHeight;
									pMove->y = 0;
								}
							}
							else
							{
								if (g_aMeshField[nCntMeshField].bCull == true)
								{
									if (g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y <= pPos->y + fRadiusHeight&&
										g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y  > pPosOld->y + fRadiusHeight)
									{// 下
										pPos->y = g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y - fRadiusHeight;

										pMove->y = 0;
									}
								}
								if (g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y > pPos->y&&
									g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y - 10 < pPosOld->y)
								{// 上
									pPos->y = g_aMeshField[nCntMeshField].pos.y + pVtx[nCnt].pos.y;
									pMove->y = 0;
									bLand = true; // 当たった					
									*pMeshField = &g_aMeshField[nCntMeshField];
								}
							}
						}
					}
				}
			}
			// 頂点バッファをアンロックする
			g_aMeshField[nCntMeshField].pVtxBuff->Unlock();
		}
	}
#endif
	return bLand;
}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType, int nBlock_X, int nBlock_Z, float fWidth, float fDepth,float fSlopeSin,bool bCull)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		if (g_aMeshField[nCntMeshField].bUse == false)
		{
			g_aMeshField[nCntMeshField].pos = pos;					// 位置
			g_aMeshField[nCntMeshField].rot = rot;				// 向き
			g_aMeshField[nCntMeshField].fWidth = fWidth;											// 幅
			g_aMeshField[nCntMeshField].fDepth = fDepth;											// 奥行き
			g_aMeshField[nCntMeshField].nBlock_X = nBlock_X;										// ブロックX
			g_aMeshField[nCntMeshField].nBlock_Z = nBlock_Z;										// ブロックZ
			g_aMeshField[nCntMeshField].fSlopeSin = fSlopeSin;											// 斜め
			g_aMeshField[nCntMeshField].nType = nType;		// 種類
			g_aMeshField[nCntMeshField].bCull = bCull;		// カリング
			g_aMeshField[nCntMeshField].nNumVertex = (g_aMeshField[nCntMeshField].nBlock_X + 1)*(g_aMeshField[nCntMeshField].nBlock_Z + 1);// 頂点数
			g_aMeshField[nCntMeshField].nNumIndex = 2 * (g_aMeshField[nCntMeshField].nBlock_X*g_aMeshField[nCntMeshField].nBlock_Z) + 4 * (g_aMeshField[nCntMeshField].nBlock_Z - 1) + 2;// インデックス数										// 
			g_aMeshField[nCntMeshField].nNumPolygon = 2 * (g_aMeshField[nCntMeshField].nBlock_X*g_aMeshField[nCntMeshField].nBlock_Z) + 4 * (g_aMeshField[nCntMeshField].nBlock_Z - 1);	// ポリゴン数		

			MakeVertexMeshField(pDevice);
			g_aMeshField[nCntMeshField].bUse = true;										// 使用されているかどうか

			break;
		}
	}
}
//==============================================================================================================================
// メッシュの獲得
//==============================================================================================================================
MeshField *GetMeshField(void)
{
	return &g_aMeshField[0];
}
MeshFieldData *GetMeshFieldData(void)
{
	return &g_aMeshFieldData[0];
}
