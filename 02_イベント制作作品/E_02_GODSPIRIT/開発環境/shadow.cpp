////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 影の処理 [shadow.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "shadow.h"
#include "meshField.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_SHADOW	(1000)

//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		"data/TEXTURE/shadow000.jpg",	// ファイルの名前
		&g_pTextureShadow);				// テクスチャへのポインタ

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);	// 色
		g_aShadow[nCntShadow].fWidth = 0;							// 幅
		g_aShadow[nCntShadow].fDepth = 0;							// 奥行き
		g_aShadow[nCntShadow].bUse = false;							// 使用されているかどうか
	}

	MakeVertexShadow(pDevice);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateShadow(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
			// 頂点カラーの設定
			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス


	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zバッファへの書き込み終了
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
		pVtx[1].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
		pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
		pVtx[3].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[0].col = g_aShadow[nCntShadow].col;
		pVtx[1].col = g_aShadow[nCntShadow].col;
		pVtx[2].col = g_aShadow[nCntShadow].col;
		pVtx[3].col = g_aShadow[nCntShadow].col;
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

}
//==============================================================================================================================
// 影の設定
//==============================================================================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	int nCntShadow;
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pVtx+= 4)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos +D3DXVECTOR3(0.0f,0.1f,0.0f);
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].fWidth = fWidth;
 			g_aShadow[nCntShadow].fDepth = fDepth;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, (g_aShadow[nCntShadow].fDepth / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aShadow[nCntShadow].fWidth / 2), 0.0f, -(g_aShadow[nCntShadow].fDepth / 2));
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}
//==============================================================================================================================
// 影の位置を設定
//==============================================================================================================================
void SetPositionShadow(int nIdyShadow, D3DXVECTOR3 pos, D3DXVECTOR3 Initpos)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * nIdyShadow;
	if (g_aShadow[nIdyShadow].bUse == true)
	{
		g_aShadow[nIdyShadow].pos.x = pos.x;
		g_aShadow[nIdyShadow].pos.z = pos.z;
		// 高さによって影の透明度変更
		g_aShadow[nIdyShadow].col.a -= (Initpos.y - pos.y)*0.005f;
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nIdyShadow].fWidth / 2), 0.0f, (g_aShadow[nIdyShadow].fDepth / 2));
		pVtx[1].pos = D3DXVECTOR3((g_aShadow[nIdyShadow].fWidth / 2), 0.0f, (g_aShadow[nIdyShadow].fDepth / 2));
		pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nIdyShadow].fWidth / 2), 0.0f, -(g_aShadow[nIdyShadow].fDepth / 2));
		pVtx[3].pos = D3DXVECTOR3((g_aShadow[nIdyShadow].fWidth / 2), 0.0f, -(g_aShadow[nIdyShadow].fDepth / 2));
		// 頂点カラーの設定
		pVtx[0].col = g_aShadow[nIdyShadow].col;
		pVtx[1].col = g_aShadow[nIdyShadow].col;
		pVtx[2].col = g_aShadow[nIdyShadow].col;
		pVtx[3].col = g_aShadow[nIdyShadow].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

}
//==============================================================================================================================
// 影の色を設定
//==============================================================================================================================
void SetColorShadow(int nIdyShadow, float fAlpha)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * nIdyShadow;
	if (g_aShadow[nIdyShadow].bUse == true)
	{
		g_aShadow[nIdyShadow].col.a = fAlpha;
		// 頂点カラーの設定
		pVtx[0].col = g_aShadow[nIdyShadow].col;
		pVtx[1].col = g_aShadow[nIdyShadow].col;
		pVtx[2].col = g_aShadow[nIdyShadow].col;
		pVtx[3].col = g_aShadow[nIdyShadow].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}
//==============================================================================================================================
// 影を削除
//==============================================================================================================================
void DeleteShadow(int nIdyShadow)
{
 	g_aShadow[nIdyShadow].bUse = false;
}
//==============================================================================================================================
// 影を取得
//==============================================================================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}