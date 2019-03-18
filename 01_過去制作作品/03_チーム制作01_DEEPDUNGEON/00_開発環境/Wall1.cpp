//=============================================================================
//
// ステージ1処理 [Wall1.cpp]
// Author : 後閑茜
//
//=============================================================================
#include "Wall1.h"
#include "player.h"
#include "enemy.h"
#include "enemy0.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy4.h"
#include "enemy5.h"
#include "enemy6.h"
#include "enemy7.h"
#include "enemy8.h"

#include "arrow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_TEXTUER "data/TEXTURE/Wall000.jpg"
#define STAGE_SPLIT (55)		//ステージ1を構成するフィールドの分割数(50)
#define STAGEVTX_X (100.0f)		//頂点を置く横間隔
#define STAGEVTX_Y (100.0f)		//頂点を置く高さ間隔
#define STAGEPOS_Y (200.0f)		//壁の位置（高さ）

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexWall1(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall1[STAGE_SPLIT] = {};	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall1 = NULL;			//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall1[STAGE_SPLIT] = {};		//インデックスバッファへのポインタ
Wall1 g_Wall1[STAGE_SPLIT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitWall1(void)
{
	/*フロア移動判定はいってるモデルの位置を
	D3DXVECTOR3(200.0f, 0.0f, 1300.0f);　
	に変更お願いします*/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		g_Wall1[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall1[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall1[nCnt].bUse = false;
		g_Wall1[nCnt].Heidth = 0;
		g_Wall1[nCnt].Width = 0;
		g_Wall1[nCnt].VertexStage = 0;
		g_Wall1[nCnt].IndexStage = 0;
		g_Wall1[nCnt].PolygonStage = 0;
	}
	//1-a周り---------------------------------
	SetWall1(5, 3, D3DXVECTOR3(300.0f, STAGEPOS_Y, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(600.0f, STAGEPOS_Y, -300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(700.0f, STAGEPOS_Y, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(-100.0f, STAGEPOS_Y, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-b周り---------------------------------
	SetWall1(2, 3, D3DXVECTOR3(200.0f, STAGEPOS_Y, 800.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(200.0f, STAGEPOS_Y, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(8, 3, D3DXVECTOR3(-100.0f, STAGEPOS_Y, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-c周り---------------------------------
	SetWall1(6, 3, D3DXVECTOR3(300.0f, STAGEPOS_Y, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(400.0f, STAGEPOS_Y, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-d周り---------------------------------
	SetWall1(5, 3, D3DXVECTOR3(800.0f, STAGEPOS_Y, 600.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(2, 3, D3DXVECTOR3(500.0f, STAGEPOS_Y, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(700.0f, STAGEPOS_Y, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-e周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(400.0f, STAGEPOS_Y, 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(2, 3, D3DXVECTOR3(-100.0f, STAGEPOS_Y, 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(500.0f, STAGEPOS_Y, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(-200.0f, STAGEPOS_Y, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(600.0f, STAGEPOS_Y, 1600.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(600.0f, STAGEPOS_Y, 1200.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(-200.0f, STAGEPOS_Y, 1000.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(4, 3, D3DXVECTOR3(-200.0f, STAGEPOS_Y, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-e内側---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(350.0f, STAGEPOS_Y, 1350.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(50.0f, STAGEPOS_Y, 1350.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(350.0f, STAGEPOS_Y, 1250.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(50.0f, STAGEPOS_Y, 1250.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(150.0f, STAGEPOS_Y, 1150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(250.0f, STAGEPOS_Y, 1150.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-f周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(1000.0f, STAGEPOS_Y, 800.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(5, 3, D3DXVECTOR3(1100.0f, STAGEPOS_Y, 1200.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(5, 3, D3DXVECTOR3(800.0f, STAGEPOS_Y, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-g周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(700.0f, STAGEPOS_Y, 1600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(2, 3, D3DXVECTOR3(700.0f, STAGEPOS_Y, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(1100.0f, STAGEPOS_Y, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-h周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(1000.0f, STAGEPOS_Y, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(5, 3, D3DXVECTOR3(1200.0f, STAGEPOS_Y, 1700.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(2, 3, D3DXVECTOR3(900.0f, STAGEPOS_Y, 1700.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-i周り---------------------------------
	SetWall1(5, 3, D3DXVECTOR3(600.0f, STAGEPOS_Y, 2400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(5, 3, D3DXVECTOR3(900.0f, STAGEPOS_Y, 1900.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(5, 3, D3DXVECTOR3(1000.0f, STAGEPOS_Y, 2300.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(500.0f, STAGEPOS_Y, 2400.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(500.0f, STAGEPOS_Y, 2000.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-j周り---------------------------------
	SetWall1(8, 3, D3DXVECTOR3(-200.0f, STAGEPOS_Y, 2300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-100.0f, STAGEPOS_Y, 2000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(2, 3, D3DXVECTOR3(400.0f, STAGEPOS_Y, 2000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-300.0f, STAGEPOS_Y, 2100.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-k周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(300.0f, STAGEPOS_Y, 1900.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(0.0f, STAGEPOS_Y, 1800.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-l周り---------------------------------
	SetWall1(2, 3, D3DXVECTOR3(-300.0f, STAGEPOS_Y, 1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-m周り---------------------------------
	SetWall1(4, 3, D3DXVECTOR3(-400.0f, STAGEPOS_Y, 900.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-500.0f, STAGEPOS_Y, 600.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//1-n周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(-400.0f, STAGEPOS_Y, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//1-o周り---------------------------------
	SetWall1(7, 3, D3DXVECTOR3(-700.0f, STAGEPOS_Y, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	//1-p周り---------------------------------
	SetWall1(3, 3, D3DXVECTOR3(-700.0f, STAGEPOS_Y, 1600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall1(1, 3, D3DXVECTOR3(-800.0f, STAGEPOS_Y, 1300.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-500.0f, STAGEPOS_Y, 1500.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-800.0f, STAGEPOS_Y, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));
	SetWall1(3, 3, D3DXVECTOR3(-800.0f, STAGEPOS_Y, 1400.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	MakeVertexWall1(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall1(void)
{
	//テクスチャの破棄
	if (g_pTextureWall1 != NULL)
	{
		g_pTextureWall1->Release();
		g_pTextureWall1 = NULL;
	}
	// 頂点バッファの開放
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_pVtxBuffWall1[nCnt] != NULL)
		{
			g_pVtxBuffWall1[nCnt]->Release();
			g_pVtxBuffWall1[nCnt] = NULL;
		}
		//インデックスバッファの破棄
		if (g_pIdxBuffWall1[nCnt] != NULL)
		{
			g_pIdxBuffWall1[nCnt]->Release();
			g_pIdxBuffWall1[nCnt] = NULL;
		}
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall1(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Wall1[nCnt].bUse == true)
		{
			if (nCnt < 20 || nCnt > 26)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Wall1[nCnt].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall1[nCnt].rot.y, g_Wall1[nCnt].rot.x, g_Wall1[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Wall1[nCnt].mtxWorld, &g_Wall1[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Wall1[nCnt].pos.x, g_Wall1[nCnt].pos.y, g_Wall1[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Wall1[nCnt].mtxWorld, &g_Wall1[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Wall1[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall1[nCnt], 0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffWall1[nCnt]);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall1);
			// ステージ1の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_Wall1[nCnt].VertexStage,	  //頂点数
				0,
				g_Wall1[nCnt].PolygonStage);//ポリゴン数

			if (nCnt > 20 && nCnt < 26)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリングしない
			}
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWall1(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	WORD*pIdx;		//インデックスデータへのポインタ
	int nCntIdx;	//インデックスのカウンター

					//テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTUER, &g_pTextureWall1);
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Wall1[nCnt].VertexStage * STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffWall1[nCnt],
			NULL);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_Wall1[nCnt].IndexStage*STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,		//16bit確保する
			D3DPOOL_MANAGED,
			&g_pIdxBuffWall1[nCnt],
			NULL);


		// 頂点情報の設定----------------------------------------
		//頂点をロックして頂点データへのポインタを取得
		g_pVtxBuffWall1[nCnt]->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntz = 0; nCntz <= g_Wall1[nCnt].Heidth; nCntz++)
		{//縦
			for (int nCntx = 0; nCntx <= g_Wall1[nCnt].Width; nCntx++)
			{//横
			 //頂点座標を設定
				pVtx[0].pos = D3DXVECTOR3(-STAGEVTX_X + (STAGEVTX_X * nCntx), STAGEVTX_Y - (STAGEVTX_Y * nCntz), 0.0f);

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
		g_pVtxBuffWall1[nCnt]->Unlock();

		// インデックス情報の設定----------------------------------------
		//インデックスバッファをロックしてインデックスバッファデータへのポインタを取得
		g_pIdxBuffWall1[nCnt]->Lock(0, 0, (void**)&pIdx, 0);

		nCntIdx = 0;//インデックスカウンターの初期化

		for (int nCntIdx_Z = 0; nCntIdx_Z < g_Wall1[nCnt].Heidth; nCntIdx_Z++)
		{//Z軸
			for (int nCntIdx_X = 0; nCntIdx_X <= g_Wall1[nCnt].Width; nCntIdx_X++, nCntIdx++)
			{//X軸（Idxも一緒に数える）
				pIdx[0] = (g_Wall1[nCnt].Width + 1) + nCntIdx;		//(横幅+1)+Idxカウント
				pIdx[1] = nCntIdx;									//Idxカウントの数
				pIdx += 2;											//ニコイチで進める

																	//横幅の端まで到達した　&&　縦幅のカウントが縦幅-1より小さい
				if (nCntIdx_X == g_Wall1[nCnt].Width && nCntIdx_Z < g_Wall1[nCnt].Heidth - 1)
				{
					pIdx[0] = nCntIdx;									//Idxカウントの数
					pIdx[1] = (g_Wall1[nCnt].Width + 1) + nCntIdx + 1;	//(横幅+1)+Idxカウント+1
					pIdx += 2;											//ニコイチで進める
				}
			}
		}
		//インデックスバッファのアンロック
		g_pIdxBuffWall1[nCnt]->Unlock();
	}

	//頂点座標を設定
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

	//インデックスの設定
	//pIdx[0] = 3;
	//pIdx[1] = 0;

	//pIdx[2] = 4;
	//pIdx[3] = 1;

	//pIdx[4] = 5;
	//pIdx[5] = 2;

	//pIdx[6] = 2;
	//pIdx[7] = 6;

	//pIdx[8] = 6;
	//pIdx[9] = 3;

	//pIdx[10] = 7;
	//pIdx[11] = 4;

	//pIdx[12] = 8;
	//pIdx[13] = 5;


}
//=============================================================================
// ステージ1の取得
//=============================================================================
Wall1*GetWall1(void)
{
	return &g_Wall1[0];
}

//=============================================================================
// ステージ1の配置
//=============================================================================
void SetWall1(int Width, int Heidth, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_Wall1[nCnt].bUse == false)
		{
			g_Wall1[nCnt].Heidth = Heidth;
			g_Wall1[nCnt].Width = Width;
			g_Wall1[nCnt].pos = pos;
			g_Wall1[nCnt].rot = rot;

			//Vtx,Index,Polygonの計算
			g_Wall1[nCnt].VertexStage = (Width + 1) * (Heidth + 1);
			g_Wall1[nCnt].IndexStage = (Width * Heidth) * 2 + (4 * (Heidth - 1)) + 2;
			g_Wall1[nCnt].PolygonStage = (Width * Heidth * 2) + (4 * (Heidth - 1));
			g_Wall1[nCnt].bUse = true;

			if (rot.y == 0)
			{// 奥
				g_Wall1[nCnt].aPos[0] = D3DXVECTOR3(-100.0f + pos.x, 0.0f, pos.z + 20.0f);
				g_Wall1[nCnt].aPos[1] = D3DXVECTOR3(-100.0f + pos.x + (100.0f * Width), 0.0f, pos.z + 20.0f);
				g_Wall1[nCnt].aPos[2] = D3DXVECTOR3(-100.0f + pos.x + (100.0f * Width), 0.0f, pos.z - 20.0f);
				g_Wall1[nCnt].aPos[3] = D3DXVECTOR3(-100.0f + pos.x, 0.0f, pos.z - 20.0f);
			}
			else if (rot.y == D3DX_PI * 0.5)
			{// 右
				g_Wall1[nCnt].aPos[0] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + 100.0f);
				g_Wall1[nCnt].aPos[1] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + 100.0f);
				g_Wall1[nCnt].aPos[2] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + (-100.0f * Width) + 100.0f);
				g_Wall1[nCnt].aPos[3] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + (-100.0f * Width) + 100.0f);
			}
			else if (rot.y == D3DX_PI * -0.5)
			{// 左
				g_Wall1[nCnt].aPos[0] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z - 100.0f);
				g_Wall1[nCnt].aPos[1] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z - 100.0f);
				g_Wall1[nCnt].aPos[2] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + (100.0f * Width) - 100.0f);
				g_Wall1[nCnt].aPos[3] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + (100.0f * Width) - 100.0f);
			}
			else if (rot.y == D3DX_PI)
			{// 手前
				g_Wall1[nCnt].aPos[0] = D3DXVECTOR3(100.0f + pos.x, 0.0f, pos.z + 20.0f);
				g_Wall1[nCnt].aPos[1] = D3DXVECTOR3(100.0f + pos.x + (-100.0f * Width), 0.0f, pos.z + 20.0f);
				g_Wall1[nCnt].aPos[2] = D3DXVECTOR3(100.0f + pos.x + (-100.0f * Width), 0.0f, pos.z - 20.0f);
				g_Wall1[nCnt].aPos[3] = D3DXVECTOR3(100.0f + pos.x, 0.0f, pos.z - 20.0f);
			}

			break;
		}
	}
}
//=============================================================================
// 奥面の壁あたり判定（プレイヤー）
//=============================================================================
void CollisionWall1_WIDTHBack_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pPlayer->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pPlayer->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pPlayer->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pPlayer->pos.z = g_Wall1[nCnt].aPos[2].z - ((pPlayer->vtxMaxModel.z + pPlayer->vtxMinModel.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 手前面の壁あたり判定（プレイヤー）
//=============================================================================
void CollisionWall1_WIDTHThisSide_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pPlayer->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pPlayer->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pPlayer->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pPlayer->pos.z = g_Wall1[nCnt].aPos[0].z - ((pPlayer->vtxMaxModel.z + pPlayer->vtxMinModel.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 右壁の当たり判定（プレイヤー）
//=============================================================================
void CollisionWall1_RIGHT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pPlayer->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pPlayer->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pPlayer->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pPlayer->pos.x = g_Wall1[nCnt].aPos[1].x - ((pPlayer->vtxMaxModel.x + pPlayer->vtxMinModel.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 左壁の当たり判定（プレイヤー）
//=============================================================================
void CollisionWall1_LEFT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pPlayer->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pPlayer->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pPlayer->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pPlayer->pos.x = g_Wall1[nCnt].aPos[1].x + ((pPlayer->vtxMaxModel.x + pPlayer->vtxMinModel.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// 奥面の壁あたり判定(敵)
//=============================================================================
void CollisionWall1_WIDTHBack_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 手前面の壁あたり判定(敵)
//=============================================================================
void CollisionWall1_WIDTHThisSide_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 右壁の当たり判定(敵)
//=============================================================================
void CollisionWall1_RIGHT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 左壁の当たり判定(敵)
//=============================================================================
void CollisionWall1_LEFT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}










//     //////////////////				///////////////					////////    
//=============================================================================
// 000000000000000000000000000000000000000000000
//=============================================================================
void CollisionWall1_WIDTHBack_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_0 *pEnemy = GetEnemy_0();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_0 *pEnemy = GetEnemy_0();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_0 *pEnemy = GetEnemy_0();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene0(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_0 *pEnemy = GetEnemy_0();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}





//     //////////////////				///////////////					////////    
//=============================================================================
// 11111111111111111111111111111111
//=============================================================================
void CollisionWall1_WIDTHBack_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_1 *pEnemy = GetEnemy_1();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_1 *pEnemy = GetEnemy_1();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_1 *pEnemy = GetEnemy_1();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene1(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_1 *pEnemy = GetEnemy_1();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}





//     //////////////////				///////////////					////////    
//=============================================================================
// 22222222222222222222222
//=============================================================================
void CollisionWall1_WIDTHBack_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_2 *pEnemy = GetEnemy_2();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_2 *pEnemy = GetEnemy_2();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_2 *pEnemy = GetEnemy_2();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene2(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_2 *pEnemy = GetEnemy_2();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}



//     //////////////////				///////////////					////////    
//=============================================================================
// 333333333333333333333333333333333333333333333
//=============================================================================
void CollisionWall1_WIDTHBack_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_3 *pEnemy = GetEnemy_3();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_3 *pEnemy = GetEnemy_3();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_3 *pEnemy = GetEnemy_3();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene3(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_3 *pEnemy = GetEnemy_3();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}





//     //////////////////				///////////////					////////    
//=============================================================================
// 4444444444444444444444444444444444444444444444444
//=============================================================================
void CollisionWall1_WIDTHBack_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_4 *pEnemy = GetEnemy_4();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_4 *pEnemy = GetEnemy_4();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_4 *pEnemy = GetEnemy_4();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene4(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_4 *pEnemy = GetEnemy_4();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}



//     //////////////////				///////////////					////////    
//=============================================================================
// 5555555555555555555555555555555555555555555555555555
//=============================================================================
void CollisionWall1_WIDTHBack_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_5 *pEnemy = GetEnemy_5();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_5 *pEnemy = GetEnemy_5();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_5 *pEnemy = GetEnemy_5();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene5(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_5 *pEnemy = GetEnemy_5();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}

//     //////////////////				///////////////					////////    
//=============================================================================
// 66666666666666666666666666666666666666666666666
//=============================================================================
void CollisionWall1_WIDTHBack_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_6 *pEnemy = GetEnemy_6();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_Wall1[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_WIDTHThisSide_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY_6 *pEnemy = GetEnemy_6();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_Wall1[nCnt].bUse == true)
		{//壁が出ている
			if (g_Wall1[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_Wall1[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_Wall1[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_Wall1[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_Wall1[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_RIGHT_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_6 *pEnemy = GetEnemy_6();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
void CollisionWall1_LEFT_Ene6(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY_6 *pEnemy = GetEnemy_6();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_Wall1[nCnt].bUse == true)
		{// 右壁
			if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_Wall1[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_Wall1[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_Wall1[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
						g_Wall1[nCnt].VecC = pEnemy->pos - g_Wall1[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_Wall1[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}







void CollisionWall1_WIDTHBack_Arrow(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ARROW *pArrow = GetArrow();

	for (int nCntArrow = 0; nCntArrow < STAGE_SPLIT; nCntArrow++, pArrow++)
	{
		for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
		{//壁の枚数
			if (g_Wall1[nCnt].bUse == true)
			{//壁が出ている
				if (g_Wall1[nCnt].rot.y == 0.0f)
				{//奥壁
					if (g_Wall1[nCnt].aPos[0].x < pArrow->pos.x &&
						g_Wall1[nCnt].aPos[1].x > pArrow->pos.x)
					{//壁の範囲
						if (g_Wall1[nCnt].pos.z > posOld->z)
						{//前から当たった
						 //ベクトル計算
							g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[2] - g_Wall1[nCnt].aPos[3];
							g_Wall1[nCnt].VecC = pArrow->pos - g_Wall1[nCnt].aPos[2];

							//あたり判定計算
							VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

							if (VecLine[nCnt] <= 0)
							{
								pArrow->bUse = false;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CollisionWall1_WIDTHThisSide_Arrow(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ARROW *pArrow = GetArrow();

	for (int nCntArrow = 0; nCntArrow < STAGE_SPLIT; nCntArrow++, pArrow++)
	{
		for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
		{//壁の枚数
			if (g_Wall1[nCnt].bUse == true)
			{//壁が出ている
				if (g_Wall1[nCnt].rot.y == D3DX_PI)
				{//手前壁
					if (g_Wall1[nCnt].aPos[1].x < pArrow->pos.x &&
						g_Wall1[nCnt].aPos[0].x > pArrow->pos.x)
					{//壁の範囲
						if (g_Wall1[nCnt].pos.z < posOld->z)
						{//後ろから当たった
						 //ベクトル計算
							g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[0] - g_Wall1[nCnt].aPos[1];
							g_Wall1[nCnt].VecC = pArrow->pos - g_Wall1[nCnt].aPos[0];

							//あたり判定計算
							VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

							if (VecLine[nCnt] >= 0)
							{
								pArrow->bUse = false;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CollisionWall1_RIGHT_Arrow(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ARROW *pArrow = GetArrow();

	for (int nCntArrow = 0; nCntArrow < STAGE_SPLIT; nCntArrow++, pArrow++)
	{
	// 壁の枚数
		for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
		{// 壁が出ている
			if (g_Wall1[nCnt].bUse == true)
			{// 右壁
				if (g_Wall1[nCnt].rot.y == D3DX_PI * 0.5)
				{// 壁の範囲だけ当たり判定
					if (g_Wall1[nCnt].aPos[1].z > pArrow->pos.z &&
						g_Wall1[nCnt].aPos[2].z < pArrow->pos.z)
					{// 前回の位置が壁よりも手前だったら
						if (g_Wall1[nCnt].pos.x > posOld->x)
						{// ベクトル計算
							g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
							g_Wall1[nCnt].VecC = pArrow->pos - g_Wall1[nCnt].aPos[1];

							// 当たり判定計算
							VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

							if (VecLine[nCnt] >= 0)
							{// 正か負か判別
								pArrow->bUse = false;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void CollisionWall1_LEFT_Arrow(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ARROW *pArrow = GetArrow();

	for (int nCntArrow = 0; nCntArrow < STAGE_SPLIT; nCntArrow++, pArrow++)
	{
		// 壁の枚数
		for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
		{// 壁が出ている
			if (g_Wall1[nCnt].bUse == true)
			{// 右壁
				if (g_Wall1[nCnt].rot.y == D3DX_PI * -0.5)
				{// 壁の範囲だけ当たり判定
					if (g_Wall1[nCnt].aPos[2].z > pArrow->pos.z &&
						g_Wall1[nCnt].aPos[1].z < pArrow->pos.z)
					{// 前回の位置が壁よりも手前だったら
						if (g_Wall1[nCnt].pos.x < posOld->x)
						{// ベクトル計算
							g_Wall1[nCnt].VecA = g_Wall1[nCnt].aPos[1] - g_Wall1[nCnt].aPos[2];
							g_Wall1[nCnt].VecC = pArrow->pos - g_Wall1[nCnt].aPos[1];

							// 当たり判定計算
							VecLine[nCnt] = (g_Wall1[nCnt].VecA.z*g_Wall1[nCnt].VecC.x) - (g_Wall1[nCnt].VecA.x*g_Wall1[nCnt].VecC.z);

							if (VecLine[nCnt] >= 0)
							{// 正か負か判別
								pArrow->bUse = false;
								break;
							}
						}
					}
				}
			}
		}
	}
}
