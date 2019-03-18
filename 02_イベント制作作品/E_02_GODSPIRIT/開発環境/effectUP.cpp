////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 地面エフェクト処理 [.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effectUP.h"
#include "game.h"
#include "effect.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_EFFECTUP	(500)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 rot;			// 向き
	EFFECTUPTYPE nType;			// 種類
	float fWidth;				// 幅
	float fDepth;				// 奥行き
} EffectUPInfo;
typedef struct
{
	char *pFileName;
}EffectUPTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexEffectUP(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffectUP = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_apTextureEffectUP[EFFECTUPTYPE_MAX] = {};	// テクスチャへのポインタ
EffectUP g_aEffectUP[MAX_EFFECTUP];
EffectUPTexture g_aEffectUPTexInfo[EFFECTUPTYPE_MAX] =
{
	{ "data/TEXTURE/mohoujin.png" },
	{ "data/TEXTURE/LeafEffect.jpg" },
	{ "data/TEXTURE/LeafEffect.jpg" },
};
float fTexmove;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitEffectUP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntEffectUPTex = 0; nCntEffectUPTex < EFFECTUPTYPE_MAX; nCntEffectUPTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aEffectUPTexInfo[nCntEffectUPTex].pFileName,	// ファイルの名前
			&g_apTextureEffectUP[nCntEffectUPTex]);			// テクスチャへのポインタ
	}

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++)
	{
		g_aEffectUP[nCntEffectUP].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aEffectUP[nCntEffectUP].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffectUP[nCntEffectUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffectUP[nCntEffectUP].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aEffectUP[nCntEffectUP].fRadius = 0;								// 半径
		g_aEffectUP[nCntEffectUP].nType = EFFECTUPTYPE_MAHOUJIN;				// 種類
		g_aEffectUP[nCntEffectUP].bUse = false;							// 使用されているかどうか
	}
	fTexmove = 0;
	MakeVertexEffectUP(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitEffectUP(void)
{
	// テクスチャの破棄
	for (int nCntEffectUP = 0; nCntEffectUP < EFFECTUPTYPE_MAX; nCntEffectUP++)
	{
		if (g_apTextureEffectUP[nCntEffectUP] != NULL)
		{
			g_apTextureEffectUP[nCntEffectUP]->Release();
			g_apTextureEffectUP[nCntEffectUP] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffEffectUP != NULL)
	{
		g_pVtxBuffEffectUP->Release();
		g_pVtxBuffEffectUP = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateEffectUP(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == true)
		{
			switch (g_aEffectUP[nCntEffectUP].nType)
			{
			case EFFECTUPTYPE_MAHOUJIN:
				g_aEffectUP[nCntEffectUP].col.a += -0.01f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				if (rand() % 3 == 0)
				{// しびれエフェクト
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aEffectUP[nCntEffectUP].pos + D3DXVECTOR3(sinf(nDirection) * (float(rand() % 400)), cosf(rand() % 314 / 100.0f) * (float(rand() % 400)), cosf(nDirection2) * (float(rand() % 400))),
						D3DXVECTOR3(sinf(nDirection) * float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f)), cosf(nDirection2) *  float((rand() % 10 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), float(rand() % 200) + 1, 10, EFFECTTYPE_SUNDER1);
					SetEffect(g_aEffectUP[nCntEffectUP].pos + D3DXVECTOR3(sinf(nDirection) * (float(rand() % 400)), cosf(rand() % 314 / 100.0f) * (float(rand() % 400)), cosf(nDirection2) * (float(rand() % 400))),
						D3DXVECTOR3(sinf(nDirection) *  float((rand() % 10 / 10.0f)), sinf(nDirection2) *  float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, 0.1f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 200) + 1, 10, EFFECTTYPE_SUNDER1);
				}
				break;
			case EFFECTUPTYPE_LEAF:
				g_aEffectUP[nCntEffectUP].fRadius += 10;
				g_aEffectUP[nCntEffectUP].col.a += -0.02f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.1f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				break;
			case EFFECTUPTYPE_HURRICANE:
				g_aEffectUP[nCntEffectUP].fRadius += 5;
				g_aEffectUP[nCntEffectUP].col.a += -0.02f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.3f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				break;
			}
			g_aEffectUP[nCntEffectUP].pos += g_aEffectUP[nCntEffectUP].move;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			// 頂点カラーの設定
			pVtx[0].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[1].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[2].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[3].col = g_aEffectUP[nCntEffectUP].col;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffectUP->Unlock();

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawEffectUP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

												// αテストを設定
												//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
												//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
												//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい
												// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// ライト影響受けない
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffectUP[nCntEffectUP].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffectUP[nCntEffectUP].rot.y, g_aEffectUP[nCntEffectUP].rot.x, g_aEffectUP[nCntEffectUP].rot.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffectUP[nCntEffectUP].pos.x, g_aEffectUP[nCntEffectUP].pos.y, g_aEffectUP[nCntEffectUP].pos.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffectUP[nCntEffectUP].mtxWorld);
			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffectUP, 0, sizeof(VERTEX_3D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEffectUP[g_aEffectUP[nCntEffectUP].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffectUP * 4, 2);

		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zソース
	//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αテストを元に戻す
	//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexEffectUP(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECTUP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffectUP,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
		pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
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
	g_pVtxBuffEffectUP->Unlock();

}
//==============================================================================================================================
// ポリゴンの設定
//==============================================================================================================================
void SetEffectUP(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffectUP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++, pVtx += 4)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == false)
		{
			g_aEffectUP[nCntEffectUP].pos = pos;
			g_aEffectUP[nCntEffectUP].move = move;
			g_aEffectUP[nCntEffectUP].rot = rot;
			g_aEffectUP[nCntEffectUP].col = col;
			g_aEffectUP[nCntEffectUP].nType = nType;
			g_aEffectUP[nCntEffectUP].fRadius = fRadius;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[1].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, (g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			pVtx[3].pos = D3DXVECTOR3((g_aEffectUP[nCntEffectUP].fRadius / 2), 0.0f, -(g_aEffectUP[nCntEffectUP].fRadius / 2));
			// 頂点カラーの設定
			pVtx[0].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[1].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[2].col = g_aEffectUP[nCntEffectUP].col;
			pVtx[3].col = g_aEffectUP[nCntEffectUP].col;
			g_aEffectUP[nCntEffectUP].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffectUP->Unlock();
}