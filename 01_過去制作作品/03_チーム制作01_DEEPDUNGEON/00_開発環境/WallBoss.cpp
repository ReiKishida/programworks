//=============================================================================
//
// ステージ1処理 [WallBoss.cpp]
// Author : 後閑茜
//
//=============================================================================
#include "WallBoss.h"
#include "player.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_TEXTUER "data\\TEXTURE\\Wall000.jpg"
#define STAGE_SPLIT (4)		//ステージ1を構成するフィールドの分割数
#define STAGEVTX_X (100.0f)		//頂点を置く横間隔
#define STAGEVTX_Y (100.0f)		//頂点を置く高さ間隔
#define STAGEPOS_Y (200.0f)		//壁の位置（高さ）

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexWallBoss(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWallBoss[STAGE_SPLIT] = {};	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWallBoss = NULL;			//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWallBoss[STAGE_SPLIT] = {};		//インデックスバッファへのポインタ
WallBoss g_WallBoss[STAGE_SPLIT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitWallBoss(void)
{
	/*フロア移動判定はいってるモデルの位置を
	D3DXVECTOR3(200.0f, 0.0f, 1300.0f);　
	に変更お願いします*/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		g_WallBoss[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_WallBoss[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_WallBoss[nCnt].bUse = false;
		g_WallBoss[nCnt].Heidth = 0;
		g_WallBoss[nCnt].Width = 0;
		g_WallBoss[nCnt].VertexStage = 0;
		g_WallBoss[nCnt].IndexStage = 0;
		g_WallBoss[nCnt].PolygonStage = 0;
	}

	SetWallBoss(7, 3, D3DXVECTOR3(0.0f, STAGEPOS_Y, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWallBoss(7, 3, D3DXVECTOR3(500.0f, STAGEPOS_Y, -1100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWallBoss(12, 3, D3DXVECTOR3(600.0f, STAGEPOS_Y, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	SetWallBoss(12, 3, D3DXVECTOR3(-100.0f, STAGEPOS_Y, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI*-0.5f, 0.0f));

	MakeVertexWallBoss(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWallBoss(void)
{
	//テクスチャの破棄
	if (g_pTextureWallBoss != NULL)
	{
		g_pTextureWallBoss->Release();
		g_pTextureWallBoss = NULL;
	}
	// 頂点バッファの開放
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_pVtxBuffWallBoss[nCnt] != NULL)
		{
			g_pVtxBuffWallBoss[nCnt]->Release();
			g_pVtxBuffWallBoss[nCnt] = NULL;
		}
		//インデックスバッファの破棄
		if (g_pIdxBuffWallBoss[nCnt] != NULL)
		{
			g_pIdxBuffWallBoss[nCnt]->Release();
			g_pIdxBuffWallBoss[nCnt] = NULL;
		}
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateWallBoss(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWallBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_WallBoss[nCnt].bUse == true)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング

																				// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_WallBoss[nCnt].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_WallBoss[nCnt].rot.y, g_WallBoss[nCnt].rot.x, g_WallBoss[nCnt].rot.z);
			D3DXMatrixMultiply(&g_WallBoss[nCnt].mtxWorld, &g_WallBoss[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_WallBoss[nCnt].pos.x, g_WallBoss[nCnt].pos.y, g_WallBoss[nCnt].pos.z);
			D3DXMatrixMultiply(&g_WallBoss[nCnt].mtxWorld, &g_WallBoss[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_WallBoss[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWallBoss[nCnt], 0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffWallBoss[nCnt]);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWallBoss);
			// ステージ1の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_WallBoss[nCnt].VertexStage,	  //頂点数
				0,
				g_WallBoss[nCnt].PolygonStage);//ポリゴン数

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリングしない

		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWallBoss(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	WORD*pIdx;		//インデックスデータへのポインタ
	int nCntIdx;	//インデックスのカウンター

					//テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTUER, &g_pTextureWallBoss);
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{

		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_WallBoss[nCnt].VertexStage * STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffWallBoss[nCnt],
			NULL);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_WallBoss[nCnt].IndexStage*STAGE_SPLIT,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,		//16bit確保する
			D3DPOOL_MANAGED,
			&g_pIdxBuffWallBoss[nCnt],
			NULL);


		// 頂点情報の設定----------------------------------------
		//頂点をロックして頂点データへのポインタを取得
		g_pVtxBuffWallBoss[nCnt]->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntz = 0; nCntz <= g_WallBoss[nCnt].Heidth; nCntz++)
		{//縦
			for (int nCntx = 0; nCntx <= g_WallBoss[nCnt].Width; nCntx++)
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
		g_pVtxBuffWallBoss[nCnt]->Unlock();

		// インデックス情報の設定----------------------------------------
		//インデックスバッファをロックしてインデックスバッファデータへのポインタを取得
		g_pIdxBuffWallBoss[nCnt]->Lock(0, 0, (void**)&pIdx, 0);

		nCntIdx = 0;//インデックスカウンターの初期化

		for (int nCntIdx_Z = 0; nCntIdx_Z < g_WallBoss[nCnt].Heidth; nCntIdx_Z++)
		{//Z軸
			for (int nCntIdx_X = 0; nCntIdx_X <= g_WallBoss[nCnt].Width; nCntIdx_X++, nCntIdx++)
			{//X軸（Idxも一緒に数える）
				pIdx[0] = (g_WallBoss[nCnt].Width + 1) + nCntIdx;		//(横幅+1)+Idxカウント
				pIdx[1] = nCntIdx;									//Idxカウントの数
				pIdx += 2;											//ニコイチで進める

																	//横幅の端まで到達した　&&　縦幅のカウントが縦幅-1より小さい
				if (nCntIdx_X == g_WallBoss[nCnt].Width && nCntIdx_Z < g_WallBoss[nCnt].Heidth - 1)
				{
					pIdx[0] = nCntIdx;									//Idxカウントの数
					pIdx[1] = (g_WallBoss[nCnt].Width + 1) + nCntIdx + 1;	//(横幅+1)+Idxカウント+1
					pIdx += 2;											//ニコイチで進める
				}
			}
		}
		//インデックスバッファのアンロック
		g_pIdxBuffWallBoss[nCnt]->Unlock();
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
WallBoss*GetWallBoss(void)
{
	return &g_WallBoss[0];
}

//=============================================================================
// ステージ1の配置
//=============================================================================
void SetWallBoss(int Width, int Heidth, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{
		if (g_WallBoss[nCnt].bUse == false)
		{
			g_WallBoss[nCnt].Heidth = Heidth;
			g_WallBoss[nCnt].Width = Width;
			g_WallBoss[nCnt].pos = pos;
			g_WallBoss[nCnt].rot = rot;

			//Vtx,Index,Polygonの計算
			g_WallBoss[nCnt].VertexStage = (Width + 1) * (Heidth + 1);
			g_WallBoss[nCnt].IndexStage = (Width * Heidth) * 2 + (4 * (Heidth - 1)) + 2;
			g_WallBoss[nCnt].PolygonStage = (Width * Heidth * 2) + (4 * (Heidth - 1));
			g_WallBoss[nCnt].bUse = true;

			if (rot.y == 0)
			{// 奥
				g_WallBoss[nCnt].aPos[0] = D3DXVECTOR3(-100.0f + pos.x, 0.0f, pos.z + 20.0f);
				g_WallBoss[nCnt].aPos[1] = D3DXVECTOR3(-100.0f + pos.x + (100.0f * Width), 0.0f, pos.z + 20.0f);
				g_WallBoss[nCnt].aPos[2] = D3DXVECTOR3(-100.0f + pos.x + (100.0f * Width), 0.0f, pos.z - 20.0f);
				g_WallBoss[nCnt].aPos[3] = D3DXVECTOR3(-100.0f + pos.x, 0.0f, pos.z - 20.0f);
			}
			else if (rot.y == D3DX_PI * 0.5)
			{// 右
				g_WallBoss[nCnt].aPos[0] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + 100.0f);
				g_WallBoss[nCnt].aPos[1] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + 100.0f);
				g_WallBoss[nCnt].aPos[2] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + (-100.0f * Width) + 100.0f);
				g_WallBoss[nCnt].aPos[3] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + (-100.0f * Width) + 100.0f);
			}
			else if (rot.y == D3DX_PI * -0.5)
			{// 左
				g_WallBoss[nCnt].aPos[0] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z - 100.0f);
				g_WallBoss[nCnt].aPos[1] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z - 100.0f);
				g_WallBoss[nCnt].aPos[2] = D3DXVECTOR3(pos.x + 20.0f, 0.0f, pos.z + (100.0f * Width) - 100.0f);
				g_WallBoss[nCnt].aPos[3] = D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z + (100.0f * Width) - 100.0f);
			}
			else if (rot.y == D3DX_PI)
			{// 手前
				g_WallBoss[nCnt].aPos[0] = D3DXVECTOR3(100.0f + pos.x, 0.0f, pos.z + 20.0f);
				g_WallBoss[nCnt].aPos[1] = D3DXVECTOR3(100.0f + pos.x + (-100.0f * Width), 0.0f, pos.z + 20.0f);
				g_WallBoss[nCnt].aPos[2] = D3DXVECTOR3(100.0f + pos.x + (-100.0f * Width), 0.0f, pos.z - 20.0f);
				g_WallBoss[nCnt].aPos[3] = D3DXVECTOR3(100.0f + pos.x, 0.0f, pos.z - 20.0f);
			}


			break;
		}
	}

}
//=============================================================================
// 奥面の壁あたり判定(敵)
//=============================================================================
void CollisionWallBoss_WIDTHBack_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_WallBoss[nCnt].bUse == true)
		{//壁が出ている
			if (g_WallBoss[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_WallBoss[nCnt].aPos[0].x < pPlayer->pos.x &&
					g_WallBoss[nCnt].aPos[1].x > pPlayer->pos.x)
				{//壁の範囲
					if (g_WallBoss[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[2] - g_WallBoss[nCnt].aPos[3];
						g_WallBoss[nCnt].VecC = pPlayer->pos - g_WallBoss[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pPlayer->pos.z = g_WallBoss[nCnt].aPos[2].z - ((pPlayer->vtxMaxModel.z + pPlayer->vtxMinModel.z) / 2);
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
void CollisionWallBoss_WIDTHThisSide_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_WallBoss[nCnt].bUse == true)
		{//壁が出ている
			if (g_WallBoss[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_WallBoss[nCnt].aPos[1].x < pPlayer->pos.x &&
					g_WallBoss[nCnt].aPos[0].x > pPlayer->pos.x)
				{//壁の範囲
					if (g_WallBoss[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[0] - g_WallBoss[nCnt].aPos[1];
						g_WallBoss[nCnt].VecC = pPlayer->pos - g_WallBoss[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pPlayer->pos.z = g_WallBoss[nCnt].aPos[0].z - ((pPlayer->vtxMaxModel.z + pPlayer->vtxMinModel.z) / 2);
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
void CollisionWallBoss_RIGHT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_WallBoss[nCnt].bUse == true)
		{// 右壁
			if (g_WallBoss[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_WallBoss[nCnt].aPos[1].z > pPlayer->pos.z &&
					g_WallBoss[nCnt].aPos[2].z < pPlayer->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_WallBoss[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[1] - g_WallBoss[nCnt].aPos[2];
						g_WallBoss[nCnt].VecC = pPlayer->pos - g_WallBoss[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pPlayer->pos.x = g_WallBoss[nCnt].aPos[1].x - ((pPlayer->vtxMaxModel.x + pPlayer->vtxMinModel.x) / 2);
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
void CollisionWallBoss_LEFT_PL(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	PLAYER*pPlayer = GetPlayer();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_WallBoss[nCnt].bUse == true)
		{// 右壁
			if (g_WallBoss[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_WallBoss[nCnt].aPos[2].z > pPlayer->pos.z &&
					g_WallBoss[nCnt].aPos[1].z < pPlayer->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_WallBoss[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[1] - g_WallBoss[nCnt].aPos[2];
						g_WallBoss[nCnt].VecC = pPlayer->pos - g_WallBoss[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pPlayer->pos.x = g_WallBoss[nCnt].aPos[1].x + ((pPlayer->vtxMaxModel.x + pPlayer->vtxMinModel.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 奥面の壁あたり判定（敵）
//=============================================================================
void CollisionWallBoss_WIDTHBack_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_WallBoss[nCnt].bUse == true)
		{//壁が出ている
			if (g_WallBoss[nCnt].rot.y == 0.0f)
			{//奥壁
				if (g_WallBoss[nCnt].aPos[0].x < pEnemy->pos.x &&
					g_WallBoss[nCnt].aPos[1].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_WallBoss[nCnt].pos.z > posOld->z)
					{//前から当たった
					 //ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[2] - g_WallBoss[nCnt].aPos[3];
						g_WallBoss[nCnt].VecC = pEnemy->pos - g_WallBoss[nCnt].aPos[2];

						//あたり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] <= 0)
						{
							pEnemy->pos.z = g_WallBoss[nCnt].aPos[2].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
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
void CollisionWallBoss_WIDTHThisSide_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		//あたり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{//壁の枚数
		if (g_WallBoss[nCnt].bUse == true)
		{//壁が出ている
			if (g_WallBoss[nCnt].rot.y == D3DX_PI)
			{//手前壁
				if (g_WallBoss[nCnt].aPos[1].x < pEnemy->pos.x &&
					g_WallBoss[nCnt].aPos[0].x > pEnemy->pos.x)
				{//壁の範囲
					if (g_WallBoss[nCnt].pos.z < posOld->z)
					{//後ろから当たった
					 //ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[0] - g_WallBoss[nCnt].aPos[1];
						g_WallBoss[nCnt].VecC = pEnemy->pos - g_WallBoss[nCnt].aPos[0];

						//あたり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{
							pEnemy->pos.z = g_WallBoss[nCnt].aPos[0].z - ((pEnemy->VtxMaxEnemy.z + pEnemy->VtxMinEnemy.z) / 2);
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
void CollisionWallBoss_RIGHT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_WallBoss[nCnt].bUse == true)
		{// 右壁
			if (g_WallBoss[nCnt].rot.y == D3DX_PI * 0.5)
			{// 壁の範囲だけ当たり判定
				if (g_WallBoss[nCnt].aPos[1].z > pEnemy->pos.z &&
					g_WallBoss[nCnt].aPos[2].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_WallBoss[nCnt].pos.x > posOld->x)
					{// ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[1] - g_WallBoss[nCnt].aPos[2];
						g_WallBoss[nCnt].VecC = pEnemy->pos - g_WallBoss[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_WallBoss[nCnt].aPos[1].x - ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
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
void CollisionWallBoss_LEFT_Ene(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld)
{
	float VecLine[STAGE_SPLIT];		// 当たり判定の計算結果
	ENEMY*pEnemy = GetEnemy();

	// 壁の枚数
	for (int nCnt = 0; nCnt < STAGE_SPLIT; nCnt++)
	{// 壁が出ている
		if (g_WallBoss[nCnt].bUse == true)
		{// 右壁
			if (g_WallBoss[nCnt].rot.y == D3DX_PI * -0.5)
			{// 壁の範囲だけ当たり判定
				if (g_WallBoss[nCnt].aPos[2].z > pEnemy->pos.z &&
					g_WallBoss[nCnt].aPos[1].z < pEnemy->pos.z)
				{// 前回の位置が壁よりも手前だったら
					if (g_WallBoss[nCnt].pos.x < posOld->x)
					{// ベクトル計算
						g_WallBoss[nCnt].VecA = g_WallBoss[nCnt].aPos[1] - g_WallBoss[nCnt].aPos[2];
						g_WallBoss[nCnt].VecC = pEnemy->pos - g_WallBoss[nCnt].aPos[1];

						// 当たり判定計算
						VecLine[nCnt] = (g_WallBoss[nCnt].VecA.z*g_WallBoss[nCnt].VecC.x) - (g_WallBoss[nCnt].VecA.x*g_WallBoss[nCnt].VecC.z);

						if (VecLine[nCnt] >= 0)
						{// 正か負か判別
							pEnemy->pos.x = g_WallBoss[nCnt].aPos[1].x + ((pEnemy->VtxMaxEnemy.x + pEnemy->VtxMinEnemy.x) / 2);
							break;
						}
					}
				}
			}
		}
	}
}