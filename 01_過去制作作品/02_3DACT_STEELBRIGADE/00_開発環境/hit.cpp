//=============================================================================
//
// ヒットコール処理 [hit.cpp]
// Author :
//
//=============================================================================
#include "hit.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEMTYPE (4)

#define POLYGON_WIDTH (10.0f)
#define POLYGON_HEIGHT (20.0f)

#define ROT_SPEED (0.05f)

#define RADIUS (35.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

ITEM g_Item;


int nCnt = 0;

int g_nNowSelect;

int g_nCntCoolDown;


//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hit.png", &g_Item.aPolygon[nCntItem].pTexture);
	}

	MakeVertexItem(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();

	g_nNowSelect = ITEMTYPE_0;
	g_Item.bUse = true;
	g_Item.pos.x = /*sinf(pPlayer.rot.y + D3DX_PI *  0.75f) + */pPlayer.pos.x + 80.0f;
	g_Item.pos.y = 40.0f + pPlayer.pos.y;
	g_Item.pos.z = /*cosf(pPlayer.rot.y + D3DX_PI *  0.75f) + */pPlayer.pos.z + 90.0f;

	g_Item.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item.aPolygon[nCntItem].nType = nCntItem;

		g_Item.aPolygon[nCntItem].nIdxModelParent = -1;

	}

	g_Item.aPolygon[0].fpos = 1.0f;
	g_Item.aPolygon[1].fpos = -0.5f;
	g_Item.aPolygon[2].fpos = 0.0f;
	g_Item.aPolygon[3].fpos = 0.5f;

	g_nCntCoolDown = 0;
	g_Item.status = HITSTATUS_NONE;
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item.aPolygon[nCntItem].pos.x = sinf(g_Item.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_ITEM) * nCntItem) * RADIUS + g_Item.pos.x;
		g_Item.aPolygon[nCntItem].pos.y = 0.0f + g_Item.pos.y;
		g_Item.aPolygon[nCntItem].pos.z = cosf(g_Item.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_ITEM) * nCntItem) * RADIUS + g_Item.pos.z;

		g_Item.aPolygon[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	g_Item.nIdxModelParent = -1;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

			//テクスチャの破棄
			if (g_Item.aPolygon[nCntItem].pTexture != NULL)
			{
				g_Item.aPolygon[nCntItem].pTexture->Release();
				g_Item.aPolygon[nCntItem].pTexture= NULL;
			}

		// 頂点バッファの開放
		if (g_Item.aPolygon[nCntItem].pVtxBuff != NULL)
		{
			g_Item.aPolygon[nCntItem].pVtxBuff->Release();
			g_Item.aPolygon[nCntItem].pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Item.pos.x = pPlayer.pos.x;
	g_Item.pos.y = 10.0f + pPlayer.pos.y;
	g_Item.pos.z = pPlayer.pos.z;


	/*if (GetKeyboardTrigger(DIK_0) == true)
	{

		g_Item.bUse = false;

	}
	if (GetKeyboardTrigger(DIK_O) == true)
	{

		g_Item.bUse = true;

	}*/

	//g_Item.fAngle = g_Item.fDestAngle - g_Item.rot.y;

	/*if (g_Item.fAngle > D3DX_PI)
	{
	g_Item.fAngle -= D3DX_PI * 2 ;
	}

	if (g_Item.fAngle < -D3DX_PI)
	{
	g_Item.fAngle += D3DX_PI * 2;
	}*/
	//g_Item.rot.y += g_Item.fAngle * ROT_SPEED;
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		if (g_Item.bUse == true && g_Item.bUse != false)
		{
			if (g_Item.status == HITSTATUS_NONE)
			{
				switch (g_Item.state)
				{
				case HITSTATE_NONE:
					g_Item.aPolygon[nCntItem].col.a -= 0.03f;
					if (g_Item.aPolygon[nCntItem].col.a <= 0.1f)
					{
						g_Item.aPolygon[nCntItem].col.a = 0.1f;
						g_Item.state = HITSTATE_USE;
					}
					break;

				case HITSTATE_USE:
					g_Item.aPolygon[nCntItem].col.a += 0.03f;
					if (g_Item.aPolygon[nCntItem].col.a >= 1.0f)
					{
						g_Item.aPolygon[nCntItem].col.a = 1.0f;
						g_Item.state = HITSTATE_NONE;
					}
					break;
				}
			}

			g_Item.rot.y += 0.01f;
			g_Item.nLife--;
			if (g_Item.nLife <= 0)
			{
				g_Item.bUse = false;
			}
		}
	}



	if (g_Item.rot.y > D3DX_PI)
	{
		g_Item.rot.y -= D3DX_PI * 2;
	}

	if (g_Item.rot.y < -D3DX_PI)
	{
		g_Item.rot.y += D3DX_PI * 2;
	}

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item.aPolygon[nCntItem].pos.x = sinf(g_Item.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_ITEM) * nCntItem) * RADIUS + g_Item.pos.x;
		g_Item.aPolygon[nCntItem].pos.y = 17.0f + g_Item.pos.y;
		g_Item.aPolygon[nCntItem].pos.z = cosf(g_Item.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_ITEM) * nCntItem) * RADIUS + g_Item.pos.z;

		//頂点バッファをロック
		g_Item.aPolygon[nCntItem].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntItem2 = 0; nCntItem2 < MAX_ITEM; nCntItem2++)
		{



			//頂点カラー
			pVtx[0].col = g_Item.aPolygon[nCntItem].col;
			pVtx[1].col = g_Item.aPolygon[nCntItem].col;
			pVtx[2].col = g_Item.aPolygon[nCntItem].col;
			pVtx[3].col = g_Item.aPolygon[nCntItem].col;


			pVtx += 4;
		}

		//頂点バッファをアンロック
		g_Item.aPolygon[nCntItem].pVtxBuff->Unlock();
	}
}





//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
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


	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item.nIdxModelParent == -1)
		{//体
			mtxParent = pPlayer.mtxWorld;
		}

		//========================//
		//プレイヤーのインデックス//
		//========================//
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Item.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Item.rot.y, g_Item.rot.x, g_Item.rot.z);
		D3DXMatrixMultiply(&g_Item.mtxWorld, &g_Item.mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Item.pos.x, g_Item.pos.y, g_Item.pos.z);
		D3DXMatrixMultiply(&g_Item.mtxWorld, &g_Item.mtxWorld, &mtxtrans);

		//親のマトリックス反映
		//D3DXMatrixMultiply(&g_Item.mtxWorld, &g_Item.mtxWorld, &mtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Item.mtxWorld);



		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (g_Item.bUse == true && g_Item.bUse != false)
			{

				if (g_Item.aPolygon[nCntItem].nIdxModelParent == -1)
				{//体
					mtxParent = g_Item.mtxWorld;
				}

				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Item.aPolygon[nCntItem].mtxWorld);

				// 回転を反映
				/*D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_Item.aPolygon[nCntItem].rot.y, g_Item.aPolygon[nCntItem].rot.x, g_Item.aPolygon[nCntItem].rot.z);
				D3DXMatrixMultiply(&g_Item.aPolygon[nCntItem].mtxWorld, &g_Item.aPolygon[nCntItem].mtxWorld, &mtxrot);*/


				pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

				g_Item.aPolygon[nCntItem].mtxWorld._11 = pCamera->mtxView._11;
				g_Item.aPolygon[nCntItem].mtxWorld._12 = pCamera->mtxView._21;
				g_Item.aPolygon[nCntItem].mtxWorld._13 = pCamera->mtxView._31;

				g_Item.aPolygon[nCntItem].mtxWorld._21 = pCamera->mtxView._12;
				g_Item.aPolygon[nCntItem].mtxWorld._22 = pCamera->mtxView._22;
				g_Item.aPolygon[nCntItem].mtxWorld._23 = pCamera->mtxView._32;

				g_Item.aPolygon[nCntItem].mtxWorld._31 = pCamera->mtxView._13;
				g_Item.aPolygon[nCntItem].mtxWorld._32 = pCamera->mtxView._23;
				g_Item.aPolygon[nCntItem].mtxWorld._33 = pCamera->mtxView._33;

				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Item.aPolygon[nCntItem].pos.x, g_Item.aPolygon[nCntItem].pos.y, g_Item.aPolygon[nCntItem].pos.z);
				D3DXMatrixMultiply(&g_Item.aPolygon[nCntItem].mtxWorld, &g_Item.aPolygon[nCntItem].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				//D3DXMatrixMultiply(&g_Item.aPolygon[nCntItem].mtxWorld, &g_Item.aPolygon[nCntItem].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Item.aPolygon[nCntItem].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_Item.aPolygon[nCntItem].pVtxBuff, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Item.aPolygon[nCntItem].pTexture);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
			}
		}
	}


}
//=============================================================================
// コールオン
//=============================================================================
void HitCall(void)
{
	g_Item.bUse = true;
	g_Item.nLife = 360;
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_Item.aPolygon[nCntItem].pVtxBuff,
			NULL);

		// 頂点情報の設定
		VERTEX_3D *pVtx;


		//頂点バッファをロック
		g_Item.aPolygon[nCntItem].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntItem2 = 0; nCntItem2 < MAX_ITEM; nCntItem2++)
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
		g_Item.aPolygon[nCntItem].pVtxBuff->Unlock();
	}

}


