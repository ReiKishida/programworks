//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);




//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ



SHADOW g_Shadow[MAX_SHADOW];

//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得


	MakeVertexShadow(pDevice);



	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//影の位置
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//影の回転
		float fWidth = 0.0f;			//影の幅
		float fDepth = 0.0f;			//影の奥行き
		g_Shadow[nCntShadow].bUse = false;
	}
	//g_Shadow[0].bUse = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;

	
	
		// 減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


		for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
		{

			if (g_Shadow[nCntShadow].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxrot,
					g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);

				D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxrot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxtrans,
					g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);

				D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxtrans);


				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);


				// テクスチャの設定
				//pDevice->SetFVF(D3DXGetFVFVertexSize);
				pDevice->SetTexture(0, g_pTextureShadow);


				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);

			}
		}
		// 減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 1.0f, g_Shadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 1.0f, g_Shadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 1.0f, -g_Shadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 1.0f, -g_Shadow[nCntShadow].fDepth);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の設定
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float Depth)
{
	int nCntShadow;
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].rot = rot;

			g_Shadow[nCntShadow].fWidth = fWidth;
			g_Shadow[nCntShadow].fDepth = Depth;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 0.1f, g_Shadow[nCntShadow].fDepth);
			pVtx[1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 0.1f, g_Shadow[nCntShadow].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fWidth, 0.1f, -g_Shadow[nCntShadow].fDepth);
			pVtx[3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fWidth, 0.1f, -g_Shadow[nCntShadow].fDepth);

			g_Shadow[nCntShadow].bUse = true;
			break;

			
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
	return nCntShadow;
}

//=============================================================================
// 影の位置の設定
//=============================================================================
void SetPositionShadow(int IndexShadow, D3DXVECTOR3 pos)
{
	g_Shadow[IndexShadow].pos.x = pos.x;
	g_Shadow[IndexShadow].pos.z = pos.z;

}
void SetPositionBulletShadow(int IndexShadow, D3DXVECTOR3 pos)
{
	g_Shadow[IndexShadow].pos.x = pos.x;
	g_Shadow[IndexShadow].pos.x = pos.y;
	g_Shadow[IndexShadow].pos.z = pos.z;

}


//=============================================================================
// 影の位置の設定
//=============================================================================
void DeleteShadow(int IndexShadow)
{
	g_Shadow[IndexShadow].bUse = false;
}

//=============================================================================
// 影の大きさの設定
//=============================================================================
void SetScaleShadow(int IndexShadow)
{
	/*g_Shadow[IndexShadow].fWidth ;
	g_Shadow[IndexShadow].fDepth;*/


}
