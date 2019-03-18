//=============================================================================
//
// 煙処理 [smoke.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "smoke.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_SHOWER_X (1) //X軸の動き(シャワー時)
#define MOVE_SHOWER_Z (1) //Z軸の動き(シャワー時)
#define MOVE_SHOWER_Y (1)	//高さ(シャワー時)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSmoke(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSmoke = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSmoke = NULL;	//頂点バッファへのポインタ

SMOKE g_aSmoke[MAX_SMOKE];

//int g_nIndexSmokeShadow[MAX_SMOKE];	//影の番号


//=============================================================================
// 初期化処理
//=============================================================================
void InitSmoke(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得


	MakeVertexSmoke(pDevice);

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{


		/*g_aSmoke[nCntSmoke].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSmoke[nCntSmoke].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSmoke[nCntSmoke].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);*/
		g_aSmoke[nCntSmoke].bUse = false;
		//g_aSmoke[nCntSmoke].fRadius = 0.0f;
		//g_aSmoke[nCntSmoke].nLife = 0;
	}



}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSmoke(void)
{
	//テクスチャの破棄
	if (g_pTextureSmoke != NULL)
	{
		g_pTextureSmoke->Release();
		g_pTextureSmoke = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSmoke != NULL)
	{
		g_pVtxBuffSmoke->Release();
		g_pVtxBuffSmoke = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSmoke(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	// 頂点情報の作成
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		/*if (GetKeyboardTrigger(DIK_0) == true)
		{
			SetSmoke(D3DXVECTOR3(35.0f, 30.0f, -70.0f), 80, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.09f));
		}*/


		g_aSmoke[nCntSmoke].posold = g_aSmoke[nCntSmoke].pos;


		g_aSmoke[nCntSmoke].pos += g_aSmoke[nCntSmoke].move;

		if (g_aSmoke[nCntSmoke].bUse == true)
		{
			g_aSmoke[nCntSmoke].nLife--;
			if (g_aSmoke[nCntSmoke].nLife <= 0)
			{
				g_aSmoke[nCntSmoke].bUse = false;
			}

			g_aSmoke[nCntSmoke].fRadius += 0.1f;
			if (g_aSmoke[nCntSmoke].fRadius >= 60.0f)
			{
				g_aSmoke[nCntSmoke].fRadius =600.0f;
				g_aSmoke[nCntSmoke].bUse = false;
			}




		}

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aSmoke[nCntSmoke].fRadius, g_aSmoke[nCntSmoke].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].fRadius, g_aSmoke[nCntSmoke].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSmoke[nCntSmoke].fRadius, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].fRadius, 0.0f, 0.0f);

		pVtx += 4;
	}



	// 頂点バッファをアンロックする
	g_pVtxBuffSmoke->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSmoke(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;

	CAMERA *pCamera;
	pCamera = GetCamera();


	/*pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DCMP_GREATER);*/


	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		if (g_aSmoke[nCntSmoke].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSmoke[nCntSmoke].mtxWorld);
			pDevice->GetTransform(D3DTS_VIEW, &pCamera->mtxView);

			g_aSmoke[nCntSmoke].mtxWorld._11 = pCamera->mtxView._11;
			g_aSmoke[nCntSmoke].mtxWorld._12 = pCamera->mtxView._21;
			g_aSmoke[nCntSmoke].mtxWorld._13 = pCamera->mtxView._31;
			g_aSmoke[nCntSmoke].mtxWorld._21 = pCamera->mtxView._12;
			g_aSmoke[nCntSmoke].mtxWorld._22 = pCamera->mtxView._22;
			g_aSmoke[nCntSmoke].mtxWorld._23 = pCamera->mtxView._32;
			g_aSmoke[nCntSmoke].mtxWorld._31 = pCamera->mtxView._13;
			g_aSmoke[nCntSmoke].mtxWorld._32 = pCamera->mtxView._23;
			g_aSmoke[nCntSmoke].mtxWorld._33 = pCamera->mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSmoke[nCntSmoke].pos.x, g_aSmoke[nCntSmoke].pos.y, g_aSmoke[nCntSmoke].pos.z);

			D3DXMatrixMultiply(&g_aSmoke[nCntSmoke].mtxWorld, &g_aSmoke[nCntSmoke].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSmoke[nCntSmoke].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSmoke, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, g_pTextureSmoke);


			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSmoke * 4, 2);
		}

	}
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexSmoke(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/smoke000.jpg", &g_pTextureSmoke);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SMOKE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSmoke,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aSmoke[nCntSmoke].fRadius, g_aSmoke[nCntSmoke].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].fRadius, g_aSmoke[nCntSmoke].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSmoke[nCntSmoke].fRadius, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].fRadius, 0.0f, 0.0f);

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
	g_pVtxBuffSmoke->Unlock();
}

////=============================================================================
//// 壁の生成
////=============================================================================
void SetSmoke(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	CAMERA *pCamera;
	pCamera = GetCamera();

	//頂点バッファをロック
	g_pVtxBuffSmoke->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		if (g_aSmoke[nCntSmoke].bUse == false)
		{


			g_aSmoke[nCntSmoke].pos = pos;

			g_aSmoke[nCntSmoke].nLife = nLife;

			g_aSmoke[nCntSmoke].fRadius = (FLOAT)rand() / (FLOAT)RAND_MAX / 20 + rand() % 7 + 1;


				g_aSmoke[nCntSmoke].move.x = sinf(D3DX_PI * (FLOAT)rand() / (FLOAT)RAND_MAX  + rand() % MOVE_SHOWER_X);
				g_aSmoke[nCntSmoke].move.y = (FLOAT)rand() / (FLOAT)RAND_MAX + rand() % MOVE_SHOWER_Y ;
				g_aSmoke[nCntSmoke].move.z = cosf(D3DX_PI * (FLOAT)rand() / (FLOAT)RAND_MAX / (FLOAT)10 - rand() % MOVE_SHOWER_Z);



			g_aSmoke[nCntSmoke].col = col;

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aSmoke[nCntSmoke].bUse = true;
			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffSmoke->Unlock();
}