//=============================================================================
//
// ステージ1処理 [Stage1.cpp]
// Author : 後閑茜
//
//=============================================================================
#include "Stage2.h"
#include "Wall2.h"
#include "Player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_TEXTUER "data/TEXTURE/Field000.jpg"
#define STAGE_SPLIT (19)	//ステージ1を構成するフィールドの分割数
#define STAGE2VTX_X (100.0f)
#define STAGE2VTX_Y (100.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexStage2(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage2[STAGE_SPLIT] = {};	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureStage2 = NULL;			//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffStage2[STAGE_SPLIT] = {};		//インデックスバッファへのポインタ
Stage2 g_Stage2[STAGE_SPLIT];
StageVtx2 g_StageVtx[STAGE_SPLIT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitStage2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	PLAYER *pPlayer = GetPlayer();

	/*転送魔法陣の位置は　D3DXVECTOR3(1500.0f, 0.0f, 700.0f)　あたりです*/

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		g_Stage2[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Stage2[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Stage2[nCnt].bUse = false;
		g_Stage2[nCnt].Heidth = 0;
		g_Stage2[nCnt].Width = 0;
		g_StageVtx[nCnt].VertexStage = 0;
		g_StageVtx[nCnt].IndexStage = 0;
		g_StageVtx[nCnt].PolygonStage = 0;
	}

	SetStageMap2(4, 5, D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//2-a
	SetStageMap2(3, 3, D3DXVECTOR3(400.0f, 0.0f, -100.0f));	//2-b
	SetStageMap2(6, 4, D3DXVECTOR3(700.0f, 0.0f, -100.0f));	//2-c
	SetStageMap2(4, 4, D3DXVECTOR3(900.0f, 0.0f, 300.0f));	//2-b
	SetStageMap2(3, 3, D3DXVECTOR3(600.0f, 0.0f, 300.0f));	//2-e
	SetStageMap2(7, 3, D3DXVECTOR3(200.0f, 0.0f, 600.0f));	//2-f
	SetStageMap2(4, 7, D3DXVECTOR3(-200.0f, 0.0f, 800.0f));	//2-g
	SetStageMap2(7, 3, D3DXVECTOR3(-200.0f, 0.0f, 1100.0f));//2-h
	SetStageMap2(4, 5, D3DXVECTOR3(500.0f, 0.0f, 1200.0f));	//2-i
	SetStageMap2(3, 4, D3DXVECTOR3(-200.0f, 0.0f, 1400.0f));//2-j
	SetStageMap2(8, 4, D3DXVECTOR3(-800.0f, 0.0f, 1800.0f));//2-k
	SetStageMap2(9, 6, D3DXVECTOR3(-800.0f, 0.0f, 2400.0f));//2-l
	SetStageMap2(5, 3, D3DXVECTOR3(100.0f, 0.0f, 2200.0f));	//2-n
	SetStageMap2(5, 7, D3DXVECTOR3(600.0f, 0.0f, 2500.0f));	//2-m
	SetStageMap2(3, 4, D3DXVECTOR3(700.0f, 0.0f, 1800.0f));	//2-o
	SetStageMap2(7, 3, D3DXVECTOR3(0.0f, 0.0f, 1700.0f));	//2-p
	SetStageMap2(6, 3, D3DXVECTOR3(1100.0f, 0.0f, 2400.0f));//2-q
	SetStageMap2(3, 10, D3DXVECTOR3(1400.0f, 0.0f, 2200.0f));//2-r
	SetStageMap2(9, 7, D3DXVECTOR3(1100.0f, 0.0f, 1200.0f));//2-s






	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeVertexStage2(pDevice);

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitStage2(void)
{
	//テクスチャの破棄
	if (g_pTextureStage2 != NULL)
	{
		g_pTextureStage2->Release();
		g_pTextureStage2 = NULL;
	}
	// 頂点バッファの開放
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_pVtxBuffStage2[nCnt] != NULL)
		{
			g_pVtxBuffStage2[nCnt]->Release();
			g_pVtxBuffStage2[nCnt] = NULL;
		}
		//インデックスバッファの破棄
		if (g_pIdxBuffStage2[nCnt] != NULL)
		{
			g_pIdxBuffStage2[nCnt]->Release();
			g_pIdxBuffStage2[nCnt] = NULL;
		}
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateStage2(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStage2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Stage2[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Stage2[nCnt].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Stage2[nCnt].rot.y, g_Stage2[nCnt].rot.x, g_Stage2[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Stage2[nCnt].mtxWorld, &g_Stage2[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Stage2[nCnt].pos.x, g_Stage2[nCnt].pos.y, g_Stage2[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Stage2[nCnt].mtxWorld, &g_Stage2[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Stage2[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffStage2[nCnt], 0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffStage2[nCnt]);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStage2);

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
void MakeVertexStage2(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	WORD*pIdx;		//インデックスデータへのポインタ
	int nCntIdx;	//インデックスのカウンター

					//テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTUER, &g_pTextureStage2);
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_StageVtx[nCnt].VertexStage*STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffStage2[nCnt],
			NULL);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_StageVtx[nCnt].IndexStage*STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,		//16bit確保する
			D3DPOOL_MANAGED,
			&g_pIdxBuffStage2[nCnt],
			NULL);


		// 頂点情報の設定----------------------------------------
		//頂点をロックして頂点データへのポインタを取得
		g_pVtxBuffStage2[nCnt]->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntz = 0; nCntz <= g_Stage2[nCnt].Heidth; nCntz++)
		{//縦
			for (int nCntx = 0; nCntx <= g_Stage2[nCnt].Width; nCntx++)
			{//横
			 //頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(-STAGE2VTX_X + (STAGE2VTX_X * nCntx), 0.0f, STAGE2VTX_X - (STAGE2VTX_X * nCntz));

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
		g_pVtxBuffStage2[nCnt]->Unlock();

		////頂点座標を設定
		//pVtx[0].pos = D3DXVECTOR3(-50.0f, 0.0f, 50.0f);
		//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		//pVtx[2].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);

		//pVtx[3].pos = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
		//pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//pVtx[5].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		//pVtx[6].pos = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);
		//pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		//pVtx[8].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

		////頂点座標を設定
		//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(0.0f, 1.0f);
		//pVtx[2].pos = D3DXVECTOR3(0.0f, 2.0f);

		//pVtx[3].pos = D3DXVECTOR3(1.0f, 0.0f);
		//pVtx[4].pos = D3DXVECTOR3(1.0f, 1.0f);
		//pVtx[5].pos = D3DXVECTOR3(1.0f, 2.0f);

		//pVtx[6].pos = D3DXVECTOR3(2.0f, 0.0f);
		//pVtx[7].pos = D3DXVECTOR3(2.0f, 1.0);
		//pVtx[8].pos = D3DXVECTOR3(2.0f, 2.0f);

		// インデックス情報の設定----------------------------------------
		//インデックスバッファをロックしてインデックスバッファデータへのポインタを取得
		g_pIdxBuffStage2[nCnt]->Lock(0, 0, (void**)&pIdx, 0);

		nCntIdx = 0;//インデックスカウンターの初期化

		for (int nCntIdx_Z = 0; nCntIdx_Z < g_Stage2[nCnt].Heidth; nCntIdx_Z++)
		{//Z軸
			for (int nCntIdx_X = 0; nCntIdx_X <= g_Stage2[nCnt].Width; nCntIdx_X++, nCntIdx++)
			{//X軸（Idxも一緒に数える）
				pIdx[0] = (g_Stage2[nCnt].Width + 1) + nCntIdx;		//(横幅+1)+Idxカウント
				pIdx[1] = nCntIdx;						//Idxカウントの数
				pIdx += 2;								//ニコイチで進める

														//横幅の端まで到達した　&&　縦幅のカウントが縦幅-1より小さい
				if (nCntIdx_X == g_Stage2[nCnt].Width && nCntIdx_Z < g_Stage2[nCnt].Heidth - 1)
				{
					pIdx[0] = nCntIdx;						//Idxカウントの数
					pIdx[1] = (g_Stage2[nCnt].Width + 1) + nCntIdx + 1;	//(横幅+1)+Idxカウント+1
					pIdx += 2;								//ニコイチで進める
				}
			}
		}
		//インデックスバッファのアンロック
		g_pIdxBuffStage2[nCnt]->Unlock();


		////インデックスの設定
		//pIdx[0] = 3;
		//pIdx[1] = 0;

		//pIdx[2] = 4;
		//pIdx[3] = 1;
		//
		//pIdx[4] = 5;
		//pIdx[5] = 2;
		//
		//pIdx[6] = 2;
		//pIdx[7] = 6;
		//
		//pIdx[8] = 6;
		//pIdx[9] = 3;
		//
		//pIdx[10] = 7;
		//pIdx[11] = 4;
		//
		//pIdx[12] = 8;
		//pIdx[13] = 5;

	}

}
//=============================================================================
// ステージ1の取得
//=============================================================================
Stage2*GetStage2(void)
{
	return &g_Stage2[0];
}

//=============================================================================
// ステージ1の配置
//=============================================================================
void SetStageMap2(int Width, int Heidth, D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Stage2[nCnt].bUse == false)
		{
			g_Stage2[nCnt].Heidth = Heidth;
			g_Stage2[nCnt].Width = Width;
			g_Stage2[nCnt].pos = pos;
			g_Stage2[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//Vtx,Index,Polygonの計算
			g_StageVtx[nCnt].VertexStage = (Width + 1) * (Heidth + 1);
			g_StageVtx[nCnt].IndexStage = (Width * Heidth) * 2 + (4 * (Heidth - 1)) + 2;
			g_StageVtx[nCnt].PolygonStage = (Width * Heidth * 2) + (4 * (Heidth - 1));
			g_Stage2[nCnt].bUse = true;
			break;
		}
	}

}