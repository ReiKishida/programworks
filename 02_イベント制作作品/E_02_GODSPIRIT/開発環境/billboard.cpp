////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ビルボードの処理 [billboard.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "billboard.h"
#include "shadow.h"

//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_BILLBOARD	(100)
#define MAX_BILLBOARD_TYPE	(24)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 rot;			// 向き
	BILLBOARDTYPE nType;		// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
}BillboardInfo;
typedef struct
{
	char *pFileName;
}BillboardTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;						// 頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];
BillboardData g_aBillboardData[MAX_BILLBOARD_TYPE];

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aBillboard[nCntBillboard].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
		g_aBillboard[nCntBillboard].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 初期位置
		g_aBillboard[nCntBillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 速さ
		g_aBillboard[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aBillboard[nCntBillboard].fWidth = 0;								// 幅
		g_aBillboard[nCntBillboard].fHeight = 0;							// 高さ
		g_aBillboard[nCntBillboard].fDepth = 0;								// 奥行き
		g_aBillboard[nCntBillboard].nType = BILLBOARDTYPE_TREE;				// 種類
		g_aBillboard[nCntBillboard].nIdyShadow = 0;							// 影のID
		g_aBillboard[nCntBillboard].bUse = false;							// 使用されているかどうか
	}
	MakeVertexBillboard(pDevice);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitBillboard(void)
{
	// テクスチャの破棄
	for (int nCntBillboard = 0; nCntBillboard < g_aBillboardData[0].nNumTex; nCntBillboard++)
	{
		if (g_aBillboardData[nCntBillboard].pTexture != NULL)
		{
			g_aBillboardData[nCntBillboard].pTexture->Release();
			g_aBillboardData[nCntBillboard].pTexture = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateBillboard(void)
{

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// 前回の位置の設定
			g_aBillboard[nCntBillboard].posOld = g_aBillboard[nCntBillboard].pos;
			// 移動量加算
			g_aBillboard[nCntBillboard].pos += g_aBillboard[nCntBillboard].move;
//--------------------------/ 影を移動する /---------------------------------------------------------------------------------------//
		//	SetPositionShadow(g_aBillboard[nCntBillboard].nIdyShadow, g_aBillboard[nCntBillboard].pos, g_aBillboard[nCntBillboard].Initpos);
		}
	}

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビルボード用マトリックス
	
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtxWorld);

			// ビルボードの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// 逆行列の設定
			g_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			g_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			g_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			g_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			g_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			g_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			g_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			g_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtxWorld, &g_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aBillboardData[g_aBillboard[nCntBillboard].nType].pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aBillboard[nCntBillboard].fWidth / 2), (g_aBillboard[nCntBillboard].fHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aBillboard[nCntBillboard].fWidth / 2), (g_aBillboard[nCntBillboard].fHeight),0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(g_aBillboard[nCntBillboard].fWidth / 2), 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aBillboard[nCntBillboard].fWidth / 2), 0.0f, 0.0f);
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();

}
//==============================================================================================================================
// ビルボードの設定
//==============================================================================================================================
void SetBillboard(D3DXVECTOR3 pos,D3DXVECTOR3 rot,BILLBOARDTYPE nType, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pVtx += 4)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)
		{
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].Initpos = pos;
			g_aBillboard[nCntBillboard].rot = rot;
			g_aBillboard[nCntBillboard].nType = nType;
			g_aBillboard[nCntBillboard].fWidth = fWidth;
			g_aBillboard[nCntBillboard].fHeight = fHeight;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aBillboard[nCntBillboard].fWidth / 2), (g_aBillboard[nCntBillboard].fHeight), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aBillboard[nCntBillboard].fWidth / 2), (g_aBillboard[nCntBillboard].fHeight), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aBillboard[nCntBillboard].fWidth / 2), 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aBillboard[nCntBillboard].fWidth / 2), 0.0f, 0.0f);

			// 影の設定
			//g_aBillboard[nCntBillboard].nIdyShadow = SetShadow(D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x, 0.1f, g_aBillboard[nCntBillboard].pos.z), g_aBillboard[nCntBillboard].rot,
			//	g_aBillboard[nCntBillboard].fWidth /2, g_aBillboard[nCntBillboard].fWidth /2);

			g_aBillboard[nCntBillboard].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}
//==============================================================================================================================
// ビルボードの取得
//==============================================================================================================================
Billboard *GetBillboard(void)
{
	return &g_aBillboard[0];
}
BillboardData *GetBillboardData(void)
{
	return &g_aBillboardData[0];
}
