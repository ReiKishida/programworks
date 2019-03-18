//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "particle.h"
#include "input.h"
#include "camera.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_SHOWER_X_Z (8) //X_Z軸の動き(シャワー時)
#define MOVE_SHOWER_Y (10)	//高さ(シャワー時)

#define MOVE_EXPLOSION_X_Z (2) //X_Z軸の動き(爆発時)
#define MOVE_EXPLOSION_Y (3)	//高さ(爆発時)

#define MOVE_SMOKE_X_Z (1) //X_Z軸の動き(煙時)
#define MOVE_SMOKE_Y (6)	//高さ(煙時)

#define MOVE_BULLET_X_Z (3) //X_Z軸の動き(弾時)
#define MOVE_BILLET_Y (2)	//高さ(弾時)

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ

PARTICLE g_aParticle[MAX_PARTICLE];

D3DXCOLOR g_Color;

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	MakeVertexParticle(pDevice);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
	}

	g_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;	// 頂点情報の作成
	CAMERA pCamera = *GetCamera();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].posold = g_aParticle[nCntParticle].pos;

		g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

		if (g_aParticle[nCntParticle].bUse == true)
		{
			g_aParticle[nCntParticle].nLife--;
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
			
			g_aParticle[nCntParticle].col.a -= 0.01f;
			
			g_aParticle[nCntParticle].fRadius -= 0.1f;
			if (g_aParticle[nCntParticle].fRadius <= 0)
			{
				g_aParticle[nCntParticle].fRadius = 0;
				g_aParticle[nCntParticle].bUse = false;
			}
			
			if (g_aParticle[nCntParticle].type == PARTICLETYPE_SHOWER)
			{
				g_aParticle[nCntParticle].move.y -= 0.5f;

				if (g_aParticle[nCntParticle].posold.y >= 0 && g_aParticle[nCntParticle].pos.y <= 0)
				{//バウンド
					g_aParticle[nCntParticle].move.y = (g_aParticle[nCntParticle].posold.y - g_aParticle[nCntParticle].pos.y);
				}
			}
		}

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, 0.0f, 0.0f);

		pVtx[0].col = g_aParticle[nCntParticle].col ;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	CAMERA pCamera = *GetCamera();
	D3DXMATRIX mtxrot, mtxtrans;

	


	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &pCamera.mtxView);

			g_aParticle[nCntParticle].mtxWorld._11 = pCamera.mtxView._11;
			g_aParticle[nCntParticle].mtxWorld._12 = pCamera.mtxView._21;
			g_aParticle[nCntParticle].mtxWorld._13 = pCamera.mtxView._31;
			g_aParticle[nCntParticle].mtxWorld._21 = pCamera.mtxView._12;
			g_aParticle[nCntParticle].mtxWorld._22 = pCamera.mtxView._22;
			g_aParticle[nCntParticle].mtxWorld._23 = pCamera.mtxView._32;
			g_aParticle[nCntParticle].mtxWorld._31 = pCamera.mtxView._13;
			g_aParticle[nCntParticle].mtxWorld._32 = pCamera.mtxView._23;
			g_aParticle[nCntParticle].mtxWorld._33 = pCamera.mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxtrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}

	}

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, 0.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// 壁の生成
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, PARTICLETYPE type)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	CAMERA pCamera = *GetCamera();

	//頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			
			g_aParticle[nCntParticle].pos = pos;

			g_aParticle[nCntParticle].nLife = nLife;

			g_aParticle[nCntParticle].type = type;

			g_aParticle[nCntParticle].fRadius = 12.0f;

			if (g_aParticle[nCntParticle].type == PARTICLETYPE_SHOWER)
			{
				g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * (FLOAT)rand() + rand() % MOVE_SHOWER_X_Z);
				g_aParticle[nCntParticle].move.y = (FLOAT)rand() / (FLOAT)RAND_MAX + rand() % MOVE_SHOWER_Y; 
				g_aParticle[nCntParticle].move.z = cosf(D3DX_PI * (FLOAT)rand() + rand() % MOVE_SHOWER_X_Z);
			}
			if (g_aParticle[nCntParticle].type == PARTICLETYPE_EXPLOSION)
			{
				g_aParticle[nCntParticle].move.x = sinf(D3DX_PI  * (FLOAT)rand() + rand() % MOVE_EXPLOSION_X_Z);
				g_aParticle[nCntParticle].move.y = (FLOAT)rand() / (FLOAT)RAND_MAX + rand() % MOVE_EXPLOSION_Y;
				g_aParticle[nCntParticle].move.z = cosf(D3DX_PI * (FLOAT)rand() + rand() % MOVE_EXPLOSION_X_Z);
			}
			if (g_aParticle[nCntParticle].type == PARTICLETYPE_SMOKE)
			{
				g_aParticle[nCntParticle].move.x = sinf(D3DX_PI  * (FLOAT)rand() + rand() % MOVE_SMOKE_X_Z);
				g_aParticle[nCntParticle].move.y = (FLOAT)rand() / (FLOAT)RAND_MAX + rand() % MOVE_SMOKE_Y;
				g_aParticle[nCntParticle].move.z = cosf(D3DX_PI * (FLOAT)rand() + rand() % MOVE_SMOKE_X_Z);
			}

			if (g_aParticle[nCntParticle].type == PARTICLETYPE_BULLET)
			{
				g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * (FLOAT)rand() - rand() % MOVE_BULLET_X_Z);
				g_aParticle[nCntParticle].move.y = (FLOAT)rand() / (FLOAT)RAND_MAX / (FLOAT)10 + rand() % MOVE_BILLET_Y;
				g_aParticle[nCntParticle].move.z = cosf(D3DX_PI * (FLOAT)rand()  - rand() % MOVE_BULLET_X_Z);
			}
			g_aParticle[nCntParticle].col = col;

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aParticle[nCntParticle].bUse = true;
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}