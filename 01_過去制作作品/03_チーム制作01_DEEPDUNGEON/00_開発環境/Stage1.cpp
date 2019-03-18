//=============================================================================
//
// ステージ1処理 [Stage1.cpp]
// Author :
//===========================================================================
#include "Stage1.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_TEXTUER "data/TEXTURE/Field000.jpg"
#define STAGE_SPLIT (16)	//ステージ1を構成するフィールドの分割数
#define STAGE1VTX_X (100.0f)
#define STAGE1VTX_Z (100.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStage1(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage1[STAGE_SPLIT] = {};	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureStage1 = NULL;			//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffStage1[STAGE_SPLIT] = {};		//インデックスバッファへのポインタ
Stage1 g_Stage1[STAGE_SPLIT];
StageVtx1 g_StageVtx[STAGE_SPLIT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitStage1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		g_Stage1[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Stage1[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Stage1[nCnt].bUse = false;
		g_Stage1[nCnt].Heidth = 0;
		g_Stage1[nCnt].Width = 0;
		g_StageVtx[nCnt].VertexStage = 0;
		g_StageVtx[nCnt].IndexStage = 0;
		g_StageVtx[nCnt].PolygonStage = 0;
	}
	SetStageMap1(8, 4, D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//1-a
	SetStageMap1(3, 8, D3DXVECTOR3(0.0f, 0.0f, 800.0f));	//1-b
	SetStageMap1(3, 3, D3DXVECTOR3(300.0f, 0.0f, 600.0f));	//1-c
	SetStageMap1(3, 5, D3DXVECTOR3(600.0f, 0.0f, 600.0f));	//1-d
	SetStageMap1(8, 8, D3DXVECTOR3(-100.0f, 0.0f, 1600.0f));//1-e
	SetStageMap1(3, 5, D3DXVECTOR3(900.0f, 0.0f, 1200.0f));	//1-f
	SetStageMap1(6, 3, D3DXVECTOR3(700.0f, 0.0f, 1500.0f));	//1-g
	SetStageMap1(3, 3, D3DXVECTOR3(1000.0f, 0.0f, 1700.0f));//1-h
	SetStageMap1(5, 5, D3DXVECTOR3(600.0f, 0.0f, 2300.0f));	//1-i
	SetStageMap1(8, 3, D3DXVECTOR3(-200.0f, 0.0f, 2200.0f));//1-j
	SetStageMap1(3, 3, D3DXVECTOR3(100.0f, 0.0f, 1900.0f));	//1-k
	SetStageMap1(6, 1, D3DXVECTOR3(-600.0f, 0.0f, 1000.0f));//1-l
	SetStageMap1(4, 2, D3DXVECTOR3(-400.0f, 0.0f, 1200.0f));//1-n
	SetStageMap1(3, 4, D3DXVECTOR3(-600.0f, 0.0f, 900.0f));	//1-m
	SetStageMap1(2, 2, D3DXVECTOR3(-600.0f, 0.0f, 1200.0f));//1-o
	SetStageMap1(3, 3, D3DXVECTOR3(-700.0f, 0.0f, 1500.0f));//1-p

	MakeVertexStage1(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStage1(void)
{
	//テクスチャの破棄
	if (g_pTextureStage1 != NULL)
	{
		g_pTextureStage1->Release();
		g_pTextureStage1 = NULL;
	}
	// 頂点バッファの開放
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_pVtxBuffStage1[nCnt] != NULL)
		{
			g_pVtxBuffStage1[nCnt]->Release();
			g_pVtxBuffStage1[nCnt] = NULL;
		}
		//インデックスバッファの破棄
		if (g_pIdxBuffStage1[nCnt] != NULL)
		{
			g_pIdxBuffStage1[nCnt]->Release();
			g_pIdxBuffStage1[nCnt] = NULL;
		}
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateStage1(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStage1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Stage1[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Stage1[nCnt].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Stage1[nCnt].rot.y, g_Stage1[nCnt].rot.x, g_Stage1[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Stage1[nCnt].mtxWorld, &g_Stage1[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Stage1[nCnt].pos.x, g_Stage1[nCnt].pos.y, g_Stage1[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Stage1[nCnt].mtxWorld, &g_Stage1[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Stage1[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffStage1[nCnt], 0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffStage1[nCnt]);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStage1);
			// ステージ1の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_StageVtx[nCnt].VertexStage,	  //頂点数
				0,
				g_StageVtx[nCnt].PolygonStage);//ポリゴン数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexStage1(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	WORD*pIdx;		//インデックスデータへのポインタ
	int nCntIdx;	//インデックスのカウンター

					//テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTUER, &g_pTextureStage1);
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_StageVtx[nCnt].VertexStage * STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffStage1[nCnt],
			NULL);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_StageVtx[nCnt].IndexStage*STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,		//16bit確保する
			D3DPOOL_MANAGED,
			&g_pIdxBuffStage1[nCnt],
			NULL);


		// 頂点情報の設定----------------------------------------
		//頂点をロックして頂点データへのポインタを取得
		g_pVtxBuffStage1[nCnt]->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntz = 0; nCntz <= g_Stage1[nCnt].Heidth; nCntz++)
		{//縦
			for (int nCntx = 0; nCntx <= g_Stage1[nCnt].Width; nCntx++)
			{//横
			 //頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(-STAGE1VTX_X + (STAGE1VTX_X * nCntx), 0.0f, STAGE1VTX_Z - (STAGE1VTX_Z * nCntz));

				//法線の設定（中の数字を全部足して1.0(-1.0)になる）
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラーを設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャの設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f * nCntx), 0.0f + (1.0f * nCntz));

				pVtx += 1;
			}
		}
		//頂点バッファのアンロック
		g_pVtxBuffStage1[nCnt]->Unlock();



		// インデックス情報の設定----------------------------------------
		//インデックスバッファをロックしてインデックスバッファデータへのポインタを取得
		g_pIdxBuffStage1[nCnt]->Lock(0, 0, (void**)&pIdx, 0);

		nCntIdx = 0;//インデックスカウンターの初期化

		for (int nCntIdx_Z = 0; nCntIdx_Z < g_Stage1[nCnt].Heidth; nCntIdx_Z++)
		{//Z軸
			for (int nCntIdx_X = 0; nCntIdx_X <= g_Stage1[nCnt].Width; nCntIdx_X++, nCntIdx++)
			{//X軸（Idxも一緒に数える）
				pIdx[0] = (g_Stage1[nCnt].Width + 1) + nCntIdx;		//(横幅+1)+Idxカウント
				pIdx[1] = nCntIdx;									//Idxカウントの数
				pIdx += 2;											//ニコイチで進める

																	//横幅の端まで到達した　&&　縦幅のカウントが縦幅-1より小さい
				if (nCntIdx_X == g_Stage1[nCnt].Width && nCntIdx_Z < g_Stage1[nCnt].Heidth - 1)
				{
					pIdx[0] = nCntIdx;									//Idxカウントの数
					pIdx[1] = (g_Stage1[nCnt].Width + 1) + nCntIdx + 1;	//(横幅+1)+Idxカウント+1
					pIdx += 2;											//ニコイチで進める
				}
			}
		}
		//インデックスバッファのアンロック
		g_pIdxBuffStage1[nCnt]->Unlock();




	}

}
//=============================================================================
// ステージ1の取得
//=============================================================================
Stage1*GetStage1(void)
{
	return &g_Stage1[0];
}

//=============================================================================
// ステージ1の配置
//=============================================================================
void SetStageMap1(int Width, int Heidth, D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Stage1[nCnt].bUse == false)
		{
			g_Stage1[nCnt].Heidth = Heidth;
			g_Stage1[nCnt].Width = Width;
			g_Stage1[nCnt].pos = pos;
			g_Stage1[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//Vtx,Index,Polygonの計算
			g_StageVtx[nCnt].VertexStage = (Width + 1) * (Heidth + 1);
			g_StageVtx[nCnt].IndexStage = (Width * Heidth) * 2 + (4 * (Heidth - 1)) + 2;
			g_StageVtx[nCnt].PolygonStage = (Width * Heidth * 2) + (4 * (Heidth - 1));
			g_Stage1[nCnt].bUse = true;
			break;
		}
	}

}