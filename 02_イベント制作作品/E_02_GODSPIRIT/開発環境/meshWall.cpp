////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッシュ壁処理 [meshwall.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "meshWall.h"
#include "line.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_MESHWALL	(100)
#define MAX_MESHWALLTYPE	(24)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
MeshWall g_aMeshWall[MAX_MESHWALL];
MeshWallData g_aMeshWallData[MAX_MESHWALLTYPE];

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		g_aMeshWall[nCntMeshWall].pVtxBuff = NULL;										// 頂点バッファへのポインタ
		g_aMeshWall[nCntMeshWall].pIdxBuff = NULL;										// インデックスバッファへのポインタ
		g_aMeshWall[nCntMeshWall].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);			// 位置
		g_aMeshWall[nCntMeshWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aMeshWall[nCntMeshWall].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色
		g_aMeshWall[nCntMeshWall].fWidth = 0;		// 幅
		g_aMeshWall[nCntMeshWall].fHeight = 0;	// 高さ
		g_aMeshWall[nCntMeshWall].nBlock_X = 1;	// ブロックX
		g_aMeshWall[nCntMeshWall].nBlock_Y = 1;	// ブロックY
		g_aMeshWall[nCntMeshWall].nType = 0;		// 種類
		g_aMeshWall[nCntMeshWall].fMoveVtx = 0;										// 頂点ごとの高さ
		g_aMeshWall[nCntMeshWall].fTexmoveMesh = 0;									// テクスチャの移動速度
		g_aMeshWall[nCntMeshWall].bCull = false;										// カリングするかしないか
		g_aMeshWall[nCntMeshWall].bCollision = true;									// 判定あるかないか
		g_aMeshWall[nCntMeshWall].bUse = false;											// 使用されているかどうか

	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitMeshWall(void)
{
	// テクスチャの破棄
	for (int nCntMeshWall = 0; nCntMeshWall < g_aMeshWallData[0].nNumTex; nCntMeshWall++)
	{
		if (g_aMeshWallData[nCntMeshWall].pTexture!= NULL)
		{
			g_aMeshWallData[nCntMeshWall].pTexture->Release();
			g_aMeshWallData[nCntMeshWall].pTexture = NULL;
		}
	}
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		// 頂点バッファの破棄
		if (g_aMeshWall[nCntMeshWall].pVtxBuff != NULL)
		{
			g_aMeshWall[nCntMeshWall].pVtxBuff->Release();
			g_aMeshWall[nCntMeshWall].pVtxBuff = NULL;
		}
		// インデックスバッファバッファの破棄
		if (g_aMeshWall[nCntMeshWall].pIdxBuff != NULL)
		{
			g_aMeshWall[nCntMeshWall].pIdxBuff->Release();
			g_aMeshWall[nCntMeshWall].pIdxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateMeshWall(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshWall[nCntMeshWall].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 波打ち（旗などに使用するとき）
#if 0
		//　波打ち
			g_aMeshWall[nCntMeshWall].fMoveVtx += 0.1f;
			for (int nCntZ = 0; nCntZ < BLOCK_Z + 1; nCntZ++)
			{
				for (int nCnt = 0; nCnt < BLOCK_X + 1; nCnt++)
				{// メッシュ
					pVtx[0 + nCnt + nCntZ*(BLOCK_Z + 1)].pos.y = cosf(g_aMeshWall[nCntMeshWall].fMoveVtx + float(nCntZ) / 10.0f) * 10;
				}
			}
#endif
			// テクスチャ移動(海など）
#if 0
			g_aMeshWall[0].fTexmoveMesh -= 0.0003f;
			g_aMeshWall[1].fTexmoveMesh -= 0.0005f;
			if (g_aMeshWall[nCntMeshWall].fTexmoveMesh <= -0.5f)
			{
				g_aMeshWall[nCntMeshWall].fTexmoveMesh = 0.0f;
			}
			for (int nCntV = 0; nCntV < g_aMeshWall[nCntMeshWall].nBlock_Y + 1; nCntV++)
			{// テクスチャの設定
				for (int nCntH = 0; nCntH < g_aMeshWall[nCntMeshWall].nBlock_X + 1; nCntH++)
				{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				 // テクスチャの設定
					pVtx[nCntH + (g_aMeshWall[nCntMeshWall].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshWall[nCntMeshWall].nBlock_X) / 100.0f, float(nCntV * 100 / g_aMeshWall[nCntMeshWall].nBlock_Y) / 100.0f + g_aMeshWall[nCntMeshWall].fTexmoveMesh);
				}
			}
#endif
			// 頂点バッファをアンロックする
			g_aMeshWall[nCntMeshWall].pVtxBuff->Unlock();
		}
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	//// αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);



	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			if (g_aMeshWall[nCntMeshWall].nType == 4)
			{
				// Zバッファへの書き込み
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}
			if (g_aMeshWall[nCntMeshWall].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
		
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aMeshWall[nCntMeshWall].pVtxBuff, 0, sizeof(VERTEX_3D));
			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aMeshWall[nCntMeshWall].pIdxBuff);
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_aMeshWallData[g_aMeshWall[nCntMeshWall].nType].pTexture);
			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aMeshWall[nCntMeshWall].nNumIndex, 0, g_aMeshWall[nCntMeshWall].nNumPolygon);
			if (g_aMeshWall[nCntMeshWall].bCull == true)
			{// カリングオフ
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			if (g_aMeshWall[nCntMeshWall].nType == 4)
			{
				// Zバッファへの書き込み
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
		}
	}
	//// αブレンディングを元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == false)
		{
			// 頂点情報の作成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshWall[nCntMeshWall].nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aMeshWall[nCntMeshWall].pVtxBuff,
				NULL);

			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshWall[nCntMeshWall].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
			for (int nCnt = 0; nCnt < g_aMeshWall[nCntMeshWall].nNumVertex; nCnt++)
			{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
				pVtx[nCnt].pos = D3DXVECTOR3(-(g_aMeshWall[nCntMeshWall].fWidth  * g_aMeshWall[nCntMeshWall].nBlock_X / 2) + (g_aMeshWall[nCntMeshWall].fWidth )*(nCnt % (g_aMeshWall[nCntMeshWall].nBlock_X + 1)),
					(g_aMeshWall[nCntMeshWall].fHeight * g_aMeshWall[nCntMeshWall].nBlock_Y) - (g_aMeshWall[nCntMeshWall].fHeight)*(nCnt / (g_aMeshWall[nCntMeshWall].nBlock_X + 1)), 0.0f);
			}

			// 法線の設定
			for (int nCnt = 0; nCnt < g_aMeshWall[nCntMeshWall].nNumVertex; nCnt++)
			{
				pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			}
			// 頂点カラーの設定
			for (int nCnt = 0; nCnt < g_aMeshWall[nCntMeshWall].nNumVertex; nCnt++)
			{
				pVtx[nCnt].col = g_aMeshWall[nCntMeshWall].col;
			}
			// ブロックごとに一枚配置するテクスチャ
			for (int nCnt = 0; nCnt < g_aMeshWall[nCntMeshWall].nNumVertex; nCnt++)
			{
				// テクスチャの設定
				pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (nCnt%(g_aMeshWall[nCntMeshWall].nBlock_X+1)), 0.0f + (nCnt / (g_aMeshWall[nCntMeshWall].nBlock_X + 1)));
			}

			//// 動くテクスチャ
			//for (int nCntV = 0; nCntV < g_aMeshWall[nCntMeshWall].nBlock_Y + 1; nCntV++)
			//{// テクスチャの設定
			//	for (int nCntH = 0; nCntH < g_aMeshWall[nCntMeshWall].nBlock_X + 1; nCntH++)
			//	{// "X"は元に戻すから％　"Z"は減少または増加し続けるので÷
			//	 // テクスチャの設定
			//		pVtx[nCntH + (g_aMeshWall[nCntMeshWall].nBlock_X + 1)*nCntV].tex = D3DXVECTOR2(float(nCntH * 100 / g_aMeshWall[nCntMeshWall].nBlock_X) / 100.0f + g_aMeshWall[nCntMeshWall].fTexmoveMesh, float(nCntV * 100 / g_aMeshWall[nCntMeshWall].nBlock_Y) / 100.0f);
			//	}
			//}
			// 頂点バッファをアンロックする
			g_aMeshWall[nCntMeshWall].pVtxBuff->Unlock();

			WORD *pIdx;	// インデックスデータへのポインタ

				// インデックスバッファの作成
			pDevice->CreateIndexBuffer(sizeof(WORD)*g_aMeshWall[nCntMeshWall].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aMeshWall[nCntMeshWall].pIdxBuff,
				NULL);

			// インデックスバッファをロックし頂点データへのポインタを取得
			g_aMeshWall[nCntMeshWall].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			// インデックスの設定
			for (int nCntIdx = 0, nCntPos = 0; nCntIdx < g_aMeshWall[nCntMeshWall].nNumIndex; nCntIdx += 2)
			{
				if ((nCntIdx / 2) % (g_aMeshWall[nCntMeshWall].nBlock_X + 2) == g_aMeshWall[nCntMeshWall].nBlock_X + 1)
				{
					pIdx[nCntIdx] = nCntPos - 1;
					pIdx[nCntIdx + 1] = nCntPos + g_aMeshWall[nCntMeshWall].nBlock_X + 1;
				}
				else
				{
					pIdx[nCntIdx] = nCntPos + g_aMeshWall[nCntMeshWall].nBlock_X + 1;
					pIdx[nCntIdx + 1] = nCntPos;

					nCntPos++;
				}
			}
			// 頂点バッファをアンロックする
			g_aMeshWall[nCntMeshWall].pIdxBuff->Unlock();
			break;
		}
	}
}
//==============================================================================================================================
// 壁のあたり判定
//==============================================================================================================================
bool CollisionMeshWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight)
{
	bool bCollision = false;

	// 通常のあたり判定
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			if (g_aMeshWall[nCntMeshWall].bCollision == true)
			{
				if (g_aMeshWall[nCntMeshWall].rot.y == 0.0f || g_aMeshWall[nCntMeshWall].rot.y == D3DX_PI)
				{
					if (g_aMeshWall[nCntMeshWall].pos.x - g_aMeshWall[nCntMeshWall].fWidth * g_aMeshWall[nCntMeshWall].nBlock_X / 2 <= pPos->x + fRadiusWidth&&	// 左
						g_aMeshWall[nCntMeshWall].pos.x + g_aMeshWall[nCntMeshWall].fWidth * g_aMeshWall[nCntMeshWall].nBlock_X / 2 >= pPos->x - fRadiusWidth&&	// 右
						g_aMeshWall[nCntMeshWall].pos.y + g_aMeshWall[nCntMeshWall].fHeight * g_aMeshWall[nCntMeshWall].nBlock_Y > pPos->y&&// 上
						g_aMeshWall[nCntMeshWall].pos.y <= pPos->y + fRadiusHeight)	// 下
					{// X Y が範囲内
						if (g_aMeshWall[nCntMeshWall].rot.y == 0.0f)
						{
							if (g_aMeshWall[nCntMeshWall].pos.z < pPos->z + fRadiusWidth&&
								g_aMeshWall[nCntMeshWall].pos.z >= pPosOld->z + fRadiusWidth)
							{//	前
								pPos->z = g_aMeshWall[nCntMeshWall].pos.z - fRadiusWidth;
								pMove->z = 0;
								bCollision = true; // 当たった
							}
						}
						if (g_aMeshWall[nCntMeshWall].rot.y == D3DX_PI)
						{// 180度回転した壁
							if (g_aMeshWall[nCntMeshWall].pos.z > pPos->z - fRadiusWidth&&
								g_aMeshWall[nCntMeshWall].pos.z <= pPosOld->z - fRadiusWidth)
							{//	後ろ
								pPos->z = g_aMeshWall[nCntMeshWall].pos.z + fRadiusWidth;
								pMove->z = 0;
								bCollision = true; // 当たった
							}
						}
					}
				}
				else
				{
					if (g_aMeshWall[nCntMeshWall].pos.z - g_aMeshWall[nCntMeshWall].fWidth * g_aMeshWall[nCntMeshWall].nBlock_X / 2 <= pPos->z + fRadiusWidth&&	// 左
						g_aMeshWall[nCntMeshWall].pos.z + g_aMeshWall[nCntMeshWall].fWidth * g_aMeshWall[nCntMeshWall].nBlock_X / 2 >= pPos->z - fRadiusWidth&&	// 右
						g_aMeshWall[nCntMeshWall].pos.y + g_aMeshWall[nCntMeshWall].fHeight * g_aMeshWall[nCntMeshWall].nBlock_Y > pPos->y&&// 上
						g_aMeshWall[nCntMeshWall].pos.y <= pPos->y + fRadiusHeight)	// 下
					{
						if (g_aMeshWall[nCntMeshWall].rot.y == D3DX_PI / 2)
						{// 90度回転した壁
							if (g_aMeshWall[nCntMeshWall].pos.x < pPos->x + fRadiusWidth&&
								g_aMeshWall[nCntMeshWall].pos.x >= pPosOld->x + fRadiusWidth)
							{//	後ろ
								pPos->x = g_aMeshWall[nCntMeshWall].pos.x - fRadiusWidth;
								pMove->x = 0;
								bCollision = true; // 当たった
							}
						}
						if (g_aMeshWall[nCntMeshWall].rot.y == -D3DX_PI / 2)
						{// -90度回転した壁
							if (g_aMeshWall[nCntMeshWall].pos.x > pPos->x - fRadiusWidth&&
								g_aMeshWall[nCntMeshWall].pos.x <= pPosOld->x - fRadiusWidth)
							{//	後ろ
								pPos->x = g_aMeshWall[nCntMeshWall].pos.x + fRadiusWidth;
								pMove->x = 0;
								bCollision = true; // 当たった
							}
						}
					}
				}
			}
		}
	}
	return bCollision;
}
bool CollisionVecMeshWall(D3DXVECTOR3 pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	bool bCollision = false;
	D3DXVECTOR3 MeshWallvec[4];
	D3DXVECTOR3 vec[4];
	D3DXVECTOR3 vecOld[4];

	D3DXVECTOR3 pos = pPos;
	D3DXVECTOR3 PosOld = posOld;
	D3DXVECTOR3 vecMove;
	D3DXVECTOR3 vecA;
	D3DXVECTOR3 vecB;
	float vecC;
	
	// 通常のあたり判定
	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == true)
		{
			// 頂点バッファをロックし頂点データへのポインタを取得
			g_aMeshWall[nCntMeshWall].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			if (g_aMeshWall[nCntMeshWall].pos.x - g_aMeshWall[nCntMeshWall].fWidth  * g_aMeshWall[nCntMeshWall].nBlock_X / 2 <= pPos.x + fRadiusWidth&&	// 左
				g_aMeshWall[nCntMeshWall].pos.x + g_aMeshWall[nCntMeshWall].fWidth  * g_aMeshWall[nCntMeshWall].nBlock_X / 2 >= pPos.x - fRadiusWidth&&	// 右
				g_aMeshWall[nCntMeshWall].pos.y + g_aMeshWall[nCntMeshWall].fHeight * g_aMeshWall[nCntMeshWall].nBlock_Y > pPos.y&&// 上
				g_aMeshWall[nCntMeshWall].pos.y <= pPos.y + fRadiusHeight)	// 下
			{// X Y が範囲内

				vecMove = D3DXVECTOR3((pos.x - PosOld.x), (pos.y - PosOld.y), pos.z - PosOld.z);
				vecA = D3DXVECTOR3(-(pos.x - PosOld.x), -(pos.y - PosOld.y), -(pos.z - PosOld.z));
				vecB = pVtx[0].nor;
				vecC = ((vecA.x *vecB.x) + (vecA.z *vecB.z));

				MeshWallvec[0] = ((pVtx[1].pos) - (pVtx[0].pos));
				vec[0] = pPos - (pVtx[0].pos + g_aMeshWall[nCntMeshWall].pos);
				vecOld[0] = posOld - (pVtx[0].pos + g_aMeshWall[nCntMeshWall].pos);
				if (g_aMeshWall[nCntMeshWall].rot.y == D3DX_PI)
				{
					vecB = -pVtx[0].nor;
					MeshWallvec[0] = ((pVtx[0].pos) - (pVtx[1].pos));
					vec[0] = pPos - (pVtx[1].pos + g_aMeshWall[nCntMeshWall].pos);
					vecOld[0] = posOld - (pVtx[1].pos + g_aMeshWall[nCntMeshWall].pos);
				}

				/*		MeshWallvec[1] = ((pVtx[3].pos) - (pVtx[1].pos));
						vec[1] = pPos - (pVtx[1].pos + g_aMeshWall[nCntMeshWall].pos);
						vecOld[1] = posOld - (pVtx[1].pos + g_aMeshWall[nCntMeshWall].pos);

						MeshWallvec[2] = ((pVtx[2].pos) - (pVtx[3].pos));
						vec[2] = pPos - (pVtx[3].pos + g_aMeshWall[nCntMeshWall].pos);
						vecOld[2] = posOld - (pVtx[3].pos + g_aMeshWall[nCntMeshWall].pos);

						MeshWallvec[3] = ((pVtx[0].pos) - (pVtx[2].pos));
						vec[3] = pPos - (pVtx[2].pos + g_aMeshWall[nCntMeshWall].pos);
						vecOld[3] = posOld - (pVtx[2].pos + g_aMeshWall[nCntMeshWall].pos);

						if ((MeshWallvec[0].z*vecOld[0].x) - (MeshWallvec[0].x*vecOld[0].z) > 0 &&
							(MeshWallvec[1].y*vecOld[1].x) - (MeshWallvec[1].x*vecOld[1].y) > 0 &&
							(MeshWallvec[2].z*vecOld[2].x) - (MeshWallvec[2].x*vecOld[2].z) < 0 &&
							(MeshWallvec[3].y*vecOld[3].x) - (MeshWallvec[3].x*vecOld[3].y) > 0)
						{
							if ((MeshWallvec[0].z*vec[0].x) - (MeshWallvec[0].x*vec[0].z) < 0 &&
								(MeshWallvec[1].y*vec[1].x) - (MeshWallvec[1].x*vec[1].y) > 0 &&
								(MeshWallvec[2].z*vec[2].x) - (MeshWallvec[2].x*vec[2].z) > 0 &&
								(MeshWallvec[3].y*vec[3].x) - (MeshWallvec[3].x*vec[3].y) > 0)
							{
								bCollision = true;
							}
						}*/
				if ((MeshWallvec[0].z*vecOld[0].x) - (MeshWallvec[0].x*vecOld[0].z) > 0)
				{
					if (((MeshWallvec[0].z*vec[0].x) - (MeshWallvec[0].x*vec[0].z)) < 0)
					{
						pMove->x = (vecMove.x + (vecC*2.0f))*pVtx[0].nor.x;
						pMove->y = vecMove.y + (vecC*2.0f)*pVtx[0].nor.y;
						pMove->z = vecMove.z + (vecC*2.0f)*pVtx[0].nor.z;

						bCollision = true;
					}
				}
			}
	
			// 頂点バッファをアンロックする
			g_aMeshWall[nCntMeshWall].pVtxBuff->Unlock();
		}
	}
	return bCollision;

}
//==============================================================================================================================
// メッシュの設定
//==============================================================================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nBlock_X, int nBlock_Y, float fWidth, float fHeight, bool bCull)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMeshWall = 0; nCntMeshWall < MAX_MESHWALL; nCntMeshWall++)
	{
		if (g_aMeshWall[nCntMeshWall].bUse == false)
		{
			g_aMeshWall[nCntMeshWall].pos = pos;					// 位置
			g_aMeshWall[nCntMeshWall].rot = rot;				// 向き
			g_aMeshWall[nCntMeshWall].fWidth = fWidth;											// 幅
			g_aMeshWall[nCntMeshWall].fHeight = fHeight;											// 奥行き
			g_aMeshWall[nCntMeshWall].nBlock_X = nBlock_X;										// ブロックX
			g_aMeshWall[nCntMeshWall].nBlock_Y = nBlock_Y;										// ブロックZ
			g_aMeshWall[nCntMeshWall].nType = nType;		// 種類
			g_aMeshWall[nCntMeshWall].bCull = bCull;		// カリング
			g_aMeshWall[nCntMeshWall].nNumVertex = (g_aMeshWall[nCntMeshWall].nBlock_X + 1)*(g_aMeshWall[nCntMeshWall].nBlock_Y + 1);											// 頂点数
			g_aMeshWall[nCntMeshWall].nNumIndex = 2 * (g_aMeshWall[nCntMeshWall].nBlock_X*g_aMeshWall[nCntMeshWall].nBlock_Y) + 4 * (g_aMeshWall[nCntMeshWall].nBlock_Y - 1) + 2;	// インデックス数										// 
			g_aMeshWall[nCntMeshWall].nNumPolygon = 2 * (g_aMeshWall[nCntMeshWall].nBlock_X*g_aMeshWall[nCntMeshWall].nBlock_Y) + 4 * (g_aMeshWall[nCntMeshWall].nBlock_Y - 1);	// ポリゴン数

			MakeVertexMeshWall(pDevice);
			g_aMeshWall[nCntMeshWall].bUse = true;										// 使用されているかどうか
			break;
		}
	}
}
//==============================================================================================================================
// メッシュの獲得
//==============================================================================================================================
MeshWall *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}
MeshWallData *GetMeshWallData(void)
{
	return &g_aMeshWallData[0];
}
