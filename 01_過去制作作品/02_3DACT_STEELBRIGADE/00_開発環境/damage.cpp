//=============================================================================
//
// 被弾処理 [Damage.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "damage.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DAMAGETYPE (4)

#define POLYGON_WIDTH (10.0f)
#define POLYGON_HEIGHT (20.0f)

#define ROT_SPEED (0.05f)

#define RADIUS (45.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexDamage(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

DAMAGE g_Damage;


//int nCnt = 0;

//int g_nNowSelect;

//int g_nCntCoolDown;


//=============================================================================
// 初期化処理
//=============================================================================
void InitDamage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/damage.png", &g_Damage.aPolygon[nCntDamage].pTexture);
	}

	MakeVertexDamage(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();


	g_Damage.bUse = true;
	g_Damage.pos.y = 10.0f + pPlayer.pos.y;

	g_Damage.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].nType = nCntDamage;

		g_Damage.aPolygon[nCntDamage].nIdxModelParent = -1;

	}

	g_Damage.aPolygon[0].fpos = 1.0f;
	g_Damage.aPolygon[1].fpos = -0.5f;
	g_Damage.aPolygon[2].fpos = 0.0f;
	g_Damage.aPolygon[3].fpos = 0.5f;

	//g_nCntCoolDown = 0;
	g_Damage.status = DAMAGESTATUS_NONE;
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].pos.x = sinf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.x;
		g_Damage.aPolygon[nCntDamage].pos.y = 0.0f + g_Damage.pos.y;
		g_Damage.aPolygon[nCntDamage].pos.z = cosf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.z;

		g_Damage.aPolygon[nCntDamage].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	g_Damage.nIdxModelParent = -1;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamage(void)
{
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

			//テクスチャの破棄
			if (g_Damage.aPolygon[nCntDamage].pTexture != NULL)
			{
				g_Damage.aPolygon[nCntDamage].pTexture->Release();
				g_Damage.aPolygon[nCntDamage].pTexture = NULL;
			}


		// 頂点バッファの開放
		if (g_Damage.aPolygon[nCntDamage].pVtxBuff != NULL)
		{
			g_Damage.aPolygon[nCntDamage].pVtxBuff->Release();
			g_Damage.aPolygon[nCntDamage].pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDamage(void)
{
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA *pCamera;
	pCamera = GetCamera();

	g_Damage.pos.x = pPlayer.pos.x;
	g_Damage.pos.y = 10.0f + pPlayer.pos.y;
	g_Damage.pos.z = pPlayer.pos.z;


	/*if (GetKeyboardTrigger(DIK_0) == true)
	{

	g_Damage.bUse = false;

	}
	if (GetKeyboardTrigger(DIK_O) == true)
	{

	g_Damage.bUse = true;

	}*/

	//g_Damage.fAngle = g_Damage.fDestAngle - g_Damage.rot.y;

	/*if (g_Damage.fAngle > D3DX_PI)
	{
	g_Damage.fAngle -= D3DX_PI * 2 ;
	}

	if (g_Damage.fAngle < -D3DX_PI)
	{
	g_Damage.fAngle += D3DX_PI * 2;
	}*/
	//g_Damage.rot.y += g_Damage.fAngle * ROT_SPEED;
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		if (g_Damage.bUse == true && g_Damage.bUse != false)
		{
			if (g_Damage.status == DAMAGESTATUS_NONE)
			{
				switch (g_Damage.state)
				{
				case DAMAGESTATE_NONE:
					g_Damage.aPolygon[nCntDamage].col.a -= 0.03f;
					if (g_Damage.aPolygon[nCntDamage].col.a <= 0.1f)
					{
						g_Damage.aPolygon[nCntDamage].col.a = 0.1f;
						g_Damage.state = DAMAGESTATE_USE;
					}
					break;

				case DAMAGESTATE_USE:
					g_Damage.aPolygon[nCntDamage].col.a += 0.03f;
					if (g_Damage.aPolygon[nCntDamage].col.a >= 1.0f)
					{
						g_Damage.aPolygon[nCntDamage].col.a = 1.0f;
						g_Damage.state = DAMAGESTATE_NONE;
					}
					break;
				}
			}

			g_Damage.rot.y -= 0.01f;
			g_Damage.nLife--;
			if (g_Damage.nLife <= 0)
			{
				g_Damage.bUse = false;
			}
		}
	}
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		g_Damage.aPolygon[nCntDamage].pos.x = sinf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.x;
		g_Damage.aPolygon[nCntDamage].pos.y = 0.0f + g_Damage.pos.y;
		g_Damage.aPolygon[nCntDamage].pos.z = cosf(g_Damage.rot.y + D3DX_PI - ((D3DX_PI * 2) / MAX_DAMAGE) * nCntDamage) * RADIUS + g_Damage.pos.z;
	}


	if (g_Damage.rot.y > D3DX_PI)
	{
		g_Damage.rot.y -= D3DX_PI * 2;
	}

	if (g_Damage.rot.y < -D3DX_PI)
	{
		g_Damage.rot.y += D3DX_PI * 2;
	}

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		//頂点バッファをロック
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);




			//頂点カラー
			pVtx[0].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[1].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[2].col = g_Damage.aPolygon[nCntDamage].col;
			pVtx[3].col = g_Damage.aPolygon[nCntDamage].col;


			pVtx += 4;


		//頂点バッファをアンロック
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Unlock();
	}
}





//=============================================================================
// 描画処理
//=============================================================================
void DrawDamage(void)
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


	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		if (g_Damage.nIdxModelParent == -1)
		{//体
			mtxParent = pPlayer.mtxWorld;
		}

		//========================//
		//プレイヤーのインデックス//
		//========================//
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Damage.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxrot,
			g_Damage.rot.y, g_Damage.rot.x, g_Damage.rot.z);
		D3DXMatrixMultiply(&g_Damage.mtxWorld, &g_Damage.mtxWorld, &mtxrot);


		// 位置を反映
		D3DXMatrixTranslation(&mtxtrans,
			g_Damage.pos.x, g_Damage.pos.y, g_Damage.pos.z);
		D3DXMatrixMultiply(&g_Damage.mtxWorld, &g_Damage.mtxWorld, &mtxtrans);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Damage.mtxWorld);



		for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
		{
			if (g_Damage.bUse == true && g_Damage.bUse != false)
			{

				if (g_Damage.aPolygon[nCntDamage].nIdxModelParent == -1)
				{//体
					mtxParent = g_Damage.mtxWorld;
				}

				//=======================================================================//
				//						    親モデルのインデックス						//
				//=====================================================================//
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Damage.aPolygon[nCntDamage].mtxWorld);

				// 回転を反映
				/*D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_Damage.aPolygon[nCntDamage].rot.y, g_Damage.aPolygon[nCntDamage].rot.x, g_Damage.aPolygon[nCntDamage].rot.z);
				D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxrot);*/


				pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

				g_Damage.aPolygon[nCntDamage].mtxWorld._11 = pCamera->mtxView._11;
				g_Damage.aPolygon[nCntDamage].mtxWorld._12 = pCamera->mtxView._21;
				g_Damage.aPolygon[nCntDamage].mtxWorld._13 = pCamera->mtxView._31;

				g_Damage.aPolygon[nCntDamage].mtxWorld._21 = pCamera->mtxView._12;
				g_Damage.aPolygon[nCntDamage].mtxWorld._22 = pCamera->mtxView._22;
				g_Damage.aPolygon[nCntDamage].mtxWorld._23 = pCamera->mtxView._32;

				g_Damage.aPolygon[nCntDamage].mtxWorld._31 = pCamera->mtxView._13;
				g_Damage.aPolygon[nCntDamage].mtxWorld._32 = pCamera->mtxView._23;
				g_Damage.aPolygon[nCntDamage].mtxWorld._33 = pCamera->mtxView._33;

				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Damage.aPolygon[nCntDamage].pos.x, g_Damage.aPolygon[nCntDamage].pos.y, g_Damage.aPolygon[nCntDamage].pos.z);
				D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxtrans);

				//親のマトリックス反映
				//D3DXMatrixMultiply(&g_Damage.aPolygon[nCntDamage].mtxWorld, &g_Damage.aPolygon[nCntDamage].mtxWorld, &mtxParent);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Damage.aPolygon[nCntDamage].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_Damage.aPolygon[nCntDamage].pVtxBuff, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Damage.aPolygon[nCntDamage].pTexture);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDamage * 4, 2);
			}
		}
	}


}
//=============================================================================
// コールオン
//=============================================================================
void HitDamage(void)
{
	g_Damage.bUse = true;
	g_Damage.nLife = 480;
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexDamage(LPDIRECT3DDEVICE9 pDevice)
{

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_DAMAGE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_Damage.aPolygon[nCntDamage].pVtxBuff,
			NULL);

		// 頂点情報の設定
		VERTEX_3D *pVtx;


		//頂点バッファをロック
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntDamage2 = 0; nCntDamage2 < MAX_DAMAGE; nCntDamage2++)
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
		g_Damage.aPolygon[nCntDamage].pVtxBuff->Unlock();
	}

}


