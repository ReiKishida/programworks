//=============================================================================
//
// メッシュシリンダーライフ処理 [Life.cpp]
// Author :  Kishida Rei
//
//=============================================================================
#include "life.h"
#include "input.h"
#include "camera.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIFE_VERTEX ((g_Life_h + 1) * (g_Life_v + 1))
#define LIFE_INDEX ( (g_Life_h * g_Life_v )* 2 + (4 * (g_Life_v - 1)) + 2 )
#define LIFE_POLYGON ( (g_Life_h * g_Life_v) * 2 + (4 * (g_Life_v - 1)) )

#define LIFE_RADIUS (35.0f)
#define LIFE_RADIUS2 (7.0f)


#define LIFE_HEIGHT (5.0f)

#define LIFE_H (120) //メッシュの分割数(H)
#define LIFE_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffLife = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posLife;		//位置(中心座標)
D3DXVECTOR3 g_rotLife;		//向き
D3DXMATRIX g_mtxWorldLife;	//ワールドマトリックス
D3DXCOLOR g_colLife;
LIFE_STATE g_LifeState;
LIFE_TYPE g_LifeType;

//float g_fAngle;

int g_nLife;

int g_Life_h, g_Life_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Life_h = LIFE_H;
	g_Life_v = LIFE_V;
	//g_fAngle = 1.0f / (g_Life_h / 2);

	int Polygon = (g_Life_h * g_Life_v) * 2 + (4 * (g_Life_v - 1));
	g_nLife = 0;

	MakeVertexLife(pDevice);
	MakeIndexLife(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	g_posLife = D3DXVECTOR3(0.0f, 10.0f, -50.0f) + pPlayer.pos;
	g_rotLife = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f);
	g_colLife = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_LifeState = LIFESTATE_NONE;
	g_LifeType = LIFETYPE_NONE;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffLife != NULL)
	{
		g_pIdxBuffLife->Release();
		g_pIdxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	CAMERA pCamera;
	pCamera = *GetCamera();

	g_posLife = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y, pPlayer.pos.z);
	g_rotLife = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f + pPlayer.rot.y, 0.0f);

	/*if (GetKeyboardTrigger(DIK_O) == true)
	{
		g_nLife += 10;


	}
*/

	if (g_nLife <= -120 )
	{
	g_colLife = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	}
	if (g_nLife <= -160)
	{
		if (g_LifeState = LIFESTATE_NONE)
		{
			g_colLife.r = 1.0f;
			g_colLife.g = 0.0f;
			g_colLife.b = 0.0f;
			g_colLife.a -= 0.2f;
			if (g_colLife.a <= 0.1f)
			{
				g_colLife.a = 0.1f;
				g_LifeState = LIFESTATE_USE;
			}
		}
		if (g_LifeState = LIFESTATE_USE)
		{
			g_colLife.r = 1.0f;
			g_colLife.g = 0.0f;
			g_colLife.b = 0.0f;

			g_colLife.a += 0.2f;
			if (g_colLife.a >= 1.0f)
			{
				g_colLife.a = 1.0f;
				g_LifeState = LIFESTATE_NONE;
			}

		}
	}
	//頂点バッファをロック
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLife_V = 0; nCntLife_V < g_Life_v + 1; nCntLife_V++)
	{
		for (int nCntLife_H = 0; nCntLife_H < g_Life_h + 1; nCntLife_H++)
		{
			if (nCntLife_V == 0)
			{
				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = g_colLife;
			}
			if (nCntLife_V == 1)
			{
				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = g_colLife;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffLife->Unlock();

}

//if (g_nLife.TitleType == TITLETYPE_NONE)
//{
//	switch (g_nLife.state)
//	{
//	case LIFESTATE_NONE:
//		g_colLife.a -= 0.02f;
//		if (g_colLife.a <= 0.1f)
//		{
//			g_colLife.a = 0.1f;
//			g_nLife.state = LIFESTATE_USE;
//		}
//		break;
//
//	case LIFESTATE_USE:
//		g_colLife.a += 0.02f;
//		if (g_colLife.a >= 1.0f)
//		{
//			g_colLife.a = 1.0f;
//			g_nLife.state = LIFESTATE_NONE;
//		}
//		break;
//	}
//}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;


	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldLife);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotLife.y, g_rotLife.x, g_rotLife.z);

	D3DXMatrixMultiply(&g_mtxWorldLife, &g_mtxWorldLife, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posLife.x, g_posLife.y, g_posLife.z);

	D3DXMatrixMultiply(&g_mtxWorldLife, &g_mtxWorldLife, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLife);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffLife);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, 0);


	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, LIFE_VERTEX, 0, LIFE_POLYGON + g_nLife);



	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}



////=============================================================================
////ライフ減少
////=============================================================================
void HitDamage(int nDamage)
{


	g_nLife += nDamage;

}

//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * LIFE_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Life_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_Life_v +1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_Life_h + 1; nCntLife_H++)
		{
			if (nCntLife_V == 0)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * LIFE_RADIUS;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.y = 1.0f;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * LIFE_RADIUS;
				//法線の設定
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_Life_h / 2)), nCntLife_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntLife_V == 1)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * (LIFE_RADIUS - LIFE_RADIUS2);
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.y = 1.0f;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * (LIFE_RADIUS - LIFE_RADIUS2);
				//法線の設定
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f);

				//テクスチャ座標
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_Life_h / 2)), nCntLife_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffLife->Unlock();



}

//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * LIFE_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffLife,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffLife->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Life_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Life_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Life_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Life_h && nCntIndex_V <= g_Life_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Life_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffLife->Unlock();

}




