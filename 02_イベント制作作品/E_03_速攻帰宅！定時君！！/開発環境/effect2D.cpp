////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect2D.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect2D.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}Effect2DTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define ANIM				(0.25f)	// アニメーション
#define LENGTH				(0.5f)	// 長さ
#define PATTERN				(4)		// パターン
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexEffect2D(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数宣言
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect2D[EFFECTTYPE_2D_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2D = NULL;	// 頂点バッファへのポインタ
EFFECT2D				g_aEffect2D[MAX_EFFECT_2D];		// エフェクトの情報
int						g_nRandType;				// ランダムエフェクト
Effect2DTexture g_aEffect2DTex[EFFECTTYPE_2D_MAX] =
{
	{ "data/TEXTURE/SpeedEffect.png" },	// 毒


};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		g_aEffect2D[nCntEffect2D].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// エフェクトの中心座標
		g_aEffect2D[nCntEffect2D].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// エフェクトの移動量
		g_aEffect2D[nCntEffect2D].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// エフェクトの色
		g_aEffect2D[nCntEffect2D].nCounterAnim = 0;
		g_aEffect2D[nCntEffect2D].nPatternAnim = 0;
		g_aEffect2D[nCntEffect2D].fWidth = 0;	// 半径
		g_aEffect2D[nCntEffect2D].fHeight = 0;	// 半径
		g_aEffect2D[nCntEffect2D].nLife = 0;								// エフェクトの寿命
		g_aEffect2D[nCntEffect2D].nType = EFFECTTYPE_2D_MAX;					// エフェクトの種類
		g_aEffect2D[nCntEffect2D].bUse = false;								// エフェクトが使用されているかどうか
	}
	g_nRandType = 0;													// ランダムなエフェクト
	// テクスチャの読み込み
	for (int nTex = 0; nTex < EFFECTTYPE_2D_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEffect2DTex[nTex].pFileName,
			&g_pTextureEffect2D[nTex]);
	}
	// 頂点情報の作成
	MakeVertexEffect2D(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitEffect2D(void)
{
	// テクスチャの開放
	for(int nCnt = 0;nCnt < EFFECTTYPE_2D_MAX;nCnt++)
	if (g_pTextureEffect2D[nCnt] != NULL)
	{
		g_pTextureEffect2D[nCnt]->Release();
		g_pTextureEffect2D[nCnt] = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect2D != NULL)
	{
		g_pVtxBuffEffect2D->Release();
		g_pVtxBuffEffect2D = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateEffect2D(void)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{// エフェクトが使用されている
			g_aEffect2D[nCntEffect2D].pos += g_aEffect2D[nCntEffect2D].move;			// 移動量加算
			if (g_aEffect2D[nCntEffect2D].fWidth <= 0)
			{// 反転防止
				g_aEffect2D[nCntEffect2D].fWidth = 0;
				g_aEffect2D[nCntEffect2D].bUse = false;
			}
			if (g_aEffect2D[nCntEffect2D].nLife <= 0)
			{// 寿命がなくなったらfalseにする
				g_aEffect2D[nCntEffect2D].bUse = false;
			}

			switch (g_aEffect2D[nCntEffect2D].nType)
			{
			case EFFECTTYPE_2D_SPEED:	// ちょうどのエフェクト
				break;
			default:
				if (g_aEffect2D[nCntEffect2D].col.a <= 0.0f)
				{// 色がなくなったらfalseにする
					g_aEffect2D[nCntEffect2D].bUse = false;
				}
				break;
			}
			g_aEffect2D[nCntEffect2D].nCounterAnim++;
			if (g_aEffect2D[nCntEffect2D].nType == EFFECTTYPE_2D_SPEED)
			{
				if (g_aEffect2D[nCntEffect2D].nCounterAnim % 3 == 0)
				{
					int nPattern = rand() % PATTERN;
					if (nPattern == g_aEffect2D[nCntEffect2D].nPatternAnim)
					{
						g_aEffect2D[nCntEffect2D].nPatternAnim = (g_aEffect2D[nCntEffect2D].nPatternAnim + 1) % PATTERN;
					}
					else
					{
						g_aEffect2D[nCntEffect2D].nPatternAnim = nPattern;
					}
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN) *ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
				}
			}
			else
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[3].col = g_aEffect2D[nCntEffect2D].col;
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;

		}

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect2D->Unlock();
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawEffect2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffEffect2D, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		//if (g_aEffect2D[nCntEffect2D].nType != EFFECTTYPE_2D_ARROW)
		//{
		//	// αブレンディングを加算合成に設定
		//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//}
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect2D[g_aEffect2D[nCntEffect2D].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntEffect2D,
				2);
		}
			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==============================================================================================================================
// 頂点の作成
//==============================================================================================================================
void MakeVertexEffect2D(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT_2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2D,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
		pVtx[3].col = g_aEffect2D[nCntEffect2D].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect2D->Unlock();
}


//==============================================================================================================================
// エフェクト設定
//==============================================================================================================================
void SetEffect2D(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife,EFFECTTYPE_2D nType)
{
	VERTEX_2D*pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++, pVtx += 4)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == false)
		{// 弾がが使用されていない
			g_aEffect2D[nCntEffect2D].pos = pos;			// 中心座標
			g_aEffect2D[nCntEffect2D].move = move;			// 移動量
			g_aEffect2D[nCntEffect2D].col = col;			// 色
			g_aEffect2D[nCntEffect2D].fWidth = fWidth;	// 半径
			g_aEffect2D[nCntEffect2D].fHeight = fHeight;	// 半径
			g_aEffect2D[nCntEffect2D].nLife = nLife;		// 寿命
			g_aEffect2D[nCntEffect2D].nType = nType;		// 寿命
			g_aEffect2D[nCntEffect2D].nCounterAnim = 0;
			g_aEffect2D[nCntEffect2D].nPatternAnim = 0;
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), -(g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aEffect2D[nCntEffect2D].fWidth), (g_aEffect2D[nCntEffect2D].fHeight), 0.0f) + g_aEffect2D[nCntEffect2D].pos;
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[1].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[2].col = g_aEffect2D[nCntEffect2D].col;
			pVtx[3].col = g_aEffect2D[nCntEffect2D].col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			if (g_aEffect2D[nCntEffect2D].nType == EFFECTTYPE_2D_SPEED)
			{
					g_aEffect2D[nCntEffect2D].nPatternAnim = (g_aEffect2D[nCntEffect2D].nPatternAnim + 1) % PATTERN;
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN) *ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect2D[nCntEffect2D].nPatternAnim % PATTERN)*ANIM + ANIM, (g_aEffect2D[nCntEffect2D].nPatternAnim / PATTERN)*1.0f + 1.0f);

			}
			g_aEffect2D[nCntEffect2D].bUse = true;	// 使用している状態にする
			break;
		}
	}
	g_pVtxBuffEffect2D->Unlock();
}
//==============================================================================================================================
// ランダム爆発の取得
//==============================================================================================================================
float SetRandomEffect2D(void)
{
	float nDirection = 0;
	int nData;

	// エフェクトの方向
	nData = rand() % 2;
	if (nData == 0)
	{// 右側
		nDirection = float(rand() % 314);
	}
	else if (nData == 1)
	{// 左側
		nDirection = float(rand() % 314) * -1;
	}
	nDirection /= 100;	// 小数点を戻す

	return nDirection;
}
//==============================================================================================================================
// エフェクトの取得
//==============================================================================================================================
EFFECT2D *GetEffect2D(void)
{
	return &g_aEffect2D[0];
}

void SetEffectSpeed(float fMove, float fMaxMove)
{
	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT_2D; nCntEffect2D++)
	{
		if (g_aEffect2D[nCntEffect2D].bUse == true)
		{
			switch (g_aEffect2D[nCntEffect2D].nType)
			{
			case EFFECTTYPE_2D_SPEED:	// ちょうどのエフェクト
				g_aEffect2D[nCntEffect2D].col.a = fMove / fMaxMove;
				break;
			}
		}
	}
}