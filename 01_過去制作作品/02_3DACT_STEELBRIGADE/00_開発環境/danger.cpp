//=============================================================================
//
// 警告処理 [Danger.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "danger.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DANGERTYPE (4)

#define POLYGON_WIDTH (10.0f)
#define POLYGON_HEIGHT (20.0f)

#define ROT_SPEED (0.05f)

#define RADIUS (30.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexDanger(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

DANGER g_Danger;

//=============================================================================
// 初期化処理
//=============================================================================
void InitDanger(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/caution.png", &g_Danger.aPolygon[nCntDanger].pTexture);
	}

	MakeVertexDanger(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();


	g_Danger.bUse = false;
	g_Danger.pos.y = 10.0f + pPlayer.pos.y;

	g_Danger.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		g_Danger.aPolygon[nCntDanger].nType = nCntDanger;

		g_Danger.aPolygon[nCntDanger].nIdxModelParent = -1;

	}

	g_Danger.aPolygon[0].fpos = 1.0f;
	g_Danger.aPolygon[1].fpos = -0.5f;
	g_Danger.aPolygon[2].fpos = 0.0f;
	g_Danger.aPolygon[3].fpos = 0.5f;

	//g_nCntCoolDown = 0;
	g_Danger.status = DANGERSTATUS_NONE;
	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		g_Danger.aPolygon[nCntDanger].pos.x = sinf(g_Danger.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DANGER) * nCntDanger) * RADIUS + g_Danger.pos.x;
		g_Danger.aPolygon[nCntDanger].pos.y = 0.0f + g_Danger.pos.y;
		g_Danger.aPolygon[nCntDanger].pos.z = cosf(g_Danger.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DANGER) * nCntDanger) * RADIUS + g_Danger.pos.z;

		g_Danger.aPolygon[nCntDanger].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	g_Danger.nIdxModelParent = -1;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDanger(void)
{
	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{

		//テクスチャの破棄
		if (g_Danger.aPolygon[nCntDanger].pTexture != NULL)
		{
			g_Danger.aPolygon[nCntDanger].pTexture->Release();
			g_Danger.aPolygon[nCntDanger].pTexture = NULL;
		}


		// 頂点バッファの開放
		if (g_Danger.aPolygon[nCntDanger].pVtxBuff != NULL)
		{
			g_Danger.aPolygon[nCntDanger].pVtxBuff->Release();
			g_Danger.aPolygon[nCntDanger].pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDanger(void)
{
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Danger.pos.x = pPlayer.pos.x;
	g_Danger.pos.y = 10.0f + pPlayer.pos.y;
	g_Danger.pos.z = pPlayer.pos.z;



	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		if (pPlayer.nLife <= 50)
		{
			g_Danger.bUse = true;
			if (g_Danger.bUse == true && g_Danger.bUse != false)
			{
				if (g_Danger.status == DANGERSTATUS_NONE)
				{
					switch (g_Danger.state)
					{
					case DANGERSTATE_NONE:
						g_Danger.aPolygon[nCntDanger].col.a -= 0.01f;
						if (g_Danger.aPolygon[nCntDanger].col.a <= 0.1f)
						{
							g_Danger.aPolygon[nCntDanger].col.a = 0.1f;
							g_Danger.state = DANGERSTATE_USE;
						}
						break;

					case DANGERSTATE_USE:
						g_Danger.aPolygon[nCntDanger].col.a += 0.01f;
						if (g_Danger.aPolygon[nCntDanger].col.a >= 1.0f)
						{
							g_Danger.aPolygon[nCntDanger].col.a = 1.0f;
							g_Danger.state = DANGERSTATE_NONE;
						}
						break;
					}
				}

				g_Danger.rot.y -= 0.01f;

			}
		}
	}
	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		g_Danger.aPolygon[nCntDanger].pos.x = sinf(g_Danger.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DANGER) * nCntDanger) * RADIUS + g_Danger.pos.x;
		g_Danger.aPolygon[0].pos.y = 10.0f + g_Danger.pos.y;
		g_Danger.aPolygon[1].pos.y = 0.0f + g_Danger.pos.y;
		g_Danger.aPolygon[2].pos.y = 20.0f + g_Danger.pos.y;
		g_Danger.aPolygon[3].pos.y = 5.0f + g_Danger.pos.y;

		g_Danger.aPolygon[nCntDanger].pos.z = cosf(g_Danger.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DANGER) * nCntDanger) * RADIUS + g_Danger.pos.z;
	}


	if (g_Danger.rot.y > D3DX_PI)
	{
		g_Danger.rot.y -= D3DX_PI * 2;
	}

	if (g_Danger.rot.y < -D3DX_PI)
	{
		g_Danger.rot.y += D3DX_PI * 2;
	}

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{

		//頂点バッファをロック
		g_Danger.aPolygon[nCntDanger].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);




		//頂点カラー
		pVtx[0].col = g_Danger.aPolygon[nCntDanger].col;
		pVtx[1].col = g_Danger.aPolygon[nCntDanger].col;
		pVtx[2].col = g_Danger.aPolygon[nCntDanger].col;
		pVtx[3].col = g_Danger.aPolygon[nCntDanger].col;


		pVtx += 4;


		//頂点バッファをアンロック
		g_Danger.aPolygon[nCntDanger].pVtxBuff->Unlock();
	}
}





//=============================================================================
// 描画処理
//=============================================================================
void DrawDanger(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;


	D3DXMATRIX mtxParent;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();


	CAMERA *pCamera;
	pCamera = GetCamera();


	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{
		if (g_Danger.nIdxModelParent == -1)
		{//体
			mtxParent = pPlayer.mtxWorld;
		}

		//========================//
		//プレイヤーのインデックス//
		//========================//
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Danger.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Danger.rot.y, g_Danger.rot.x, g_Danger.rot.z);
		D3DXMatrixMultiply(&g_Danger.mtxWorld, &g_Danger.mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Danger.pos.x, g_Danger.pos.y, g_Danger.pos.z);
		D3DXMatrixMultiply(&g_Danger.mtxWorld, &g_Danger.mtxWorld, &mtxtrans);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Danger.mtxWorld);



		for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
		{
			if (g_Danger.bUse == true && g_Danger.bUse != false)
			{

				if (g_Danger.aPolygon[nCntDanger].nIdxModelParent == -1)
				{//体
					mtxParent = g_Danger.mtxWorld;
				}

				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Danger.aPolygon[nCntDanger].mtxWorld);



				pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

				g_Danger.aPolygon[nCntDanger].mtxWorld._11 = pCamera->mtxView._11;
				g_Danger.aPolygon[nCntDanger].mtxWorld._12 = pCamera->mtxView._21;
				g_Danger.aPolygon[nCntDanger].mtxWorld._13 = pCamera->mtxView._31;

				g_Danger.aPolygon[nCntDanger].mtxWorld._21 = pCamera->mtxView._12;
				g_Danger.aPolygon[nCntDanger].mtxWorld._22 = pCamera->mtxView._22;
				g_Danger.aPolygon[nCntDanger].mtxWorld._23 = pCamera->mtxView._32;

				g_Danger.aPolygon[nCntDanger].mtxWorld._31 = pCamera->mtxView._13;
				g_Danger.aPolygon[nCntDanger].mtxWorld._32 = pCamera->mtxView._23;
				g_Danger.aPolygon[nCntDanger].mtxWorld._33 = pCamera->mtxView._33;

				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Danger.aPolygon[nCntDanger].pos.x, g_Danger.aPolygon[nCntDanger].pos.y, g_Danger.aPolygon[nCntDanger].pos.z);
				D3DXMatrixMultiply(&g_Danger.aPolygon[nCntDanger].mtxWorld, &g_Danger.aPolygon[nCntDanger].mtxWorld, &mtxtrans);


				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Danger.aPolygon[nCntDanger].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_Danger.aPolygon[nCntDanger].pVtxBuff, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Danger.aPolygon[nCntDanger].pTexture);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDanger * 4, 2);
			}
		}
	}


}
//=============================================================================
// コールオン
//=============================================================================
void HitDanger(void)
{
	//g_Danger.bUse = true;
	////g_Danger.nLife = 480;
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexDanger(LPDIRECT3DDEVICE9 pDevice)
{

	for (int nCntDanger = 0; nCntDanger < MAX_DANGER; nCntDanger++)
	{

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_DANGER,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_Danger.aPolygon[nCntDanger].pVtxBuff,
			NULL);

		// 頂点情報の設定
		VERTEX_3D *pVtx;


		//頂点バッファをロック
		g_Danger.aPolygon[nCntDanger].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntDanger2 = 0; nCntDanger2 < MAX_DANGER; nCntDanger2++)
		{

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(POLYGON_WIDTH, POLYGON_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POLYGON_WIDTH, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POLYGON_WIDTH, 0.0f, 0.0f);

			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}

		//頂点バッファをアンロック
		g_Danger.aPolygon[nCntDanger].pVtxBuff->Unlock();
	}

}


