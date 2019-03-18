////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 地面エフェクト処理 [.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effectUP.h"
#include "game.h"
#include "effect.h"
#include "player.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_EFFECTUP	(2000)
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
	{ "data/TEXTURE/LeafEffect.jpg" },
	{ "data/TEXTURE/.jpg" },
	{ "data/TEXTURE/garasuu_002.png" },
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
		g_aEffectUP[nCntEffectUP].bUse = false;							// 使用されているかどうか
		g_aEffectUP[nCntEffectUP].nIdy = 0;								// 番号
		g_aEffectUP[nCntEffectUP].fAttackPower = 0;						// ダメージ
		g_aEffectUP[nCntEffectUP].fDamageH = 0;							// 水平方向ダメージ
		g_aEffectUP[nCntEffectUP].fDamageV = 0;							// 水平方向ダメージ

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
			case EFFECTUPTYPE_LEAF:
				//g_aEffectUP[nCntEffectUP].fRadius += 10;
				g_aEffectUP[nCntEffectUP].col.a += -0.02f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.3f;
				if (g_aEffectUP[nCntEffectUP].col.a <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				if ((rand() +1)%100==0&& g_aEffectUP[nCntEffectUP].col.a >= 0.3f)
				{
					SetEffect(g_aEffectUP[nCntEffectUP].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10,
						cosf(SetRandomEffect()) * 10,
						 cosf(SetRandomEffect()) * 10)
						, D3DXVECTOR3(sinf(SetRandomEffect()) * 1, 0, cosf(SetRandomEffect()) * 1), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),float( rand()%30+10), 0, EFFECTTYPE_CARTOON_WIND);
				}
				break;
			case EFFECTUPTYPE_PAPER:
				g_aEffectUP[nCntEffectUP].rot.x += 0.01f;
				g_aEffectUP[nCntEffectUP].rot.y += 0.01f;
				g_aEffectUP[nCntEffectUP].rot.z += 0.01f;
				g_aEffectUP[nCntEffectUP].move.x += sinf(SetRandomEffect())*0.5f;
				g_aEffectUP[nCntEffectUP].move.z += sinf(SetRandomEffect())*0.5f;
				g_aEffectUP[nCntEffectUP].move.y -= sinf((rand()%314)/100.0f)*0.1f;
				if (g_aEffectUP[nCntEffectUP].pos.y <= 0.0f)
				{
					g_aEffectUP[nCntEffectUP].bUse = false;
				}
				break;
			case EFFECTUPTYPE_GUARD:
				g_aEffectUP[nCntEffectUP].move.y -=0.9f;
					g_aEffectUP[nCntEffectUP].col.a += -0.001f;
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

	// ライト影響受けない
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffectUP = 0; nCntEffectUP < MAX_EFFECTUP; nCntEffectUP++)
	{
		if (g_aEffectUP[nCntEffectUP].bUse == true)
		{
			if (g_aEffectUP[nCntEffectUP].nType == EFFECTUPTYPE_LEAF)
			{
				// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
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

			if (g_aEffectUP[nCntEffectUP].nType == EFFECTUPTYPE_LEAF)
			{
				// αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			if (g_aEffectUP[nCntEffectUP].nType == EFFECTUPTYPE_LEAF)
			{// 風なら判定
				if (g_aEffectUP[nCntEffectUP].col.a >= 0.3f)
				{// 判定															// 位置を反映
					if (CollisionPlayer(g_aEffectUP[nCntEffectUP].pos,
						D3DXVECTOR3(g_aEffectUP[nCntEffectUP].mtxWorld._41, g_aEffectUP[nCntEffectUP].mtxWorld._42, g_aEffectUP[nCntEffectUP].mtxWorld._43),
						g_aEffectUP[nCntEffectUP].fRadius / 2, g_aEffectUP[nCntEffectUP].fAttackPower,
						g_aEffectUP[nCntEffectUP].fDamageH,
						g_aEffectUP[nCntEffectUP].fDamageV, g_aEffectUP[nCntEffectUP].nIdy))
					{
					}
				}
			}
		}
	}
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
void SetEffectUP(int nIdy,D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius)
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
			g_aEffectUP[nCntEffectUP].nIdy = nIdy;
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
			// 位置を反映
			D3DXMATRIX mtxTrans;				// 計算用マトリックス
			D3DXMatrixTranslation(&mtxTrans, g_aEffectUP[nCntEffectUP].pos.x, g_aEffectUP[nCntEffectUP].pos.y, g_aEffectUP[nCntEffectUP].pos.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxTrans);

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffectUP->Unlock();
}
void SetEffectUPDamage(int nIdy, float fAttackPower, float fDamageH, float fDamageV, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, EFFECTUPTYPE nType, float fRadius)
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
			g_aEffectUP[nCntEffectUP].nIdy = nIdy;
			g_aEffectUP[nCntEffectUP].nType = nType;
			g_aEffectUP[nCntEffectUP].fRadius = fRadius;
			g_aEffectUP[nCntEffectUP].nIdy = nIdy;								// 番号
			g_aEffectUP[nCntEffectUP].fAttackPower = fAttackPower;						// ダメージ
			g_aEffectUP[nCntEffectUP].fDamageH = fDamageH;							// 水平方向ダメージ
			g_aEffectUP[nCntEffectUP].fDamageV = fDamageV;							// 水平方向ダメージ

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
			// 位置を反映
			D3DXMATRIX mtxTrans;				// 計算用マトリックス
			D3DXMatrixTranslation(&mtxTrans, g_aEffectUP[nCntEffectUP].pos.x, g_aEffectUP[nCntEffectUP].pos.y, g_aEffectUP[nCntEffectUP].pos.z);
			D3DXMatrixMultiply(&g_aEffectUP[nCntEffectUP].mtxWorld, &g_aEffectUP[nCntEffectUP].mtxWorld, &mtxTrans);

			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffectUP->Unlock();
}