////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect.h"
#include "player.h"
#include "game.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_EFFECT	    (2244)
#define ANIM			(0.2f)
#define MAX_PATTERN		(5)
#define MAX_ANIMSPEED	(5)

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}EffectTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_apTextureEffect[EFFECTTYPE_MAX] = {};	// テクスチャへのポインタ
Effect g_aEffect[MAX_EFFECT];

EffectTexture g_aEffectTexInfo[EFFECTTYPE_MAX] =
{
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/HitEffect000.jpg" },
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/StateUp.jpg" },
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/Water.jpg" },
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smokeEffect.png" },
	{ "data/TEXTURE/DirtyEffect.png" },
	{ "data/TEXTURE/Sunder001.jpg" },
	{ "data/TEXTURE/star.png" },
	{ "data/TEXTURE/EffectCartoon/slice000.png" },
	{ "data/TEXTURE/EffectCartoon/fire.png" },
	{ "data/TEXTURE/EffectCartoon/wind.png" },
	{ "data/TEXTURE/EffectCartoon/don.png" },
	{"data/TEXTURE/EffectCartoon/ta.png"},
};
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntEffectTex = 0; nCntEffectTex < EFFECTTYPE_MAX; nCntEffectTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aEffectTexInfo[nCntEffectTex].pFileName,		// ファイルの名前
			&g_apTextureEffect[nCntEffectTex]);				// テクスチャへのポインタ
	}
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		g_aEffect[nCntEffect].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
		g_aEffect[nCntEffect].Destpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の位置
		g_aEffect[nCntEffect].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 速さ
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
		g_aEffect[nCntEffect].fAngle = 0;							// 角度
		g_aEffect[nCntEffect].fLength = 0;							// 長さ
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_aEffect[nCntEffect].fWidth = 0;							// 幅
		g_aEffect[nCntEffect].fHeight = 0;							// 高さ
		g_aEffect[nCntEffect].fDepth = 0;							// 奥行き
		g_aEffect[nCntEffect].fRadius = 0;							// 長さ
		g_aEffect[nCntEffect].fSinCosMove = 0;						// 揺れ
		g_aEffect[nCntEffect].fChangeRadius = 0;					// 拡大縮小
		g_aEffect[nCntEffect].nLife = 0;							// 寿命
		g_aEffect[nCntEffect].nType = EFFECTTYPE_SMOKE;			// 種類
		g_aEffect[nCntEffect].nIdyShadow = 0;						// 影のID
		g_aEffect[nCntEffect].nCounterAnim = 0;						// アニメーションカウンター
		g_aEffect[nCntEffect].nPatternAnim = 0;						// アニメーションパターン
		g_aEffect[nCntEffect].nRandAnim = 0;					// カウンター
		g_aEffect[nCntEffect].bUse = false;							// 使用されているかどうか
		g_aEffect[nCntEffect].nIdy = 0;								// 番号
		g_aEffect[nCntEffect].fAttackPower = 0;						// ダメージ
		g_aEffect[nCntEffect].fDamageH = 0;							// 水平方向ダメージ
		g_aEffect[nCntEffect].fDamageV = 0;							// 水平方向ダメージ
		g_aEffect[nCntEffect].pModel = NULL;
	}
	MakeVertexEffect(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		if (g_apTextureEffect[nCntEffect] != NULL)
		{
			g_apTextureEffect[nCntEffect]->Release();
			g_apTextureEffect[nCntEffect] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateEffect(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	Player *pPlayer = GetPlayer();
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++,pVtx+=4)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			// 前回の位置の設定
			g_aEffect[nCnt].posOld = g_aEffect[nCnt].pos;
			// 移動量加算
			g_aEffect[nCnt].pos += g_aEffect[nCnt].move;

			switch (g_aEffect[nCnt].nType)
			{
			case EFFECTTYPE_SMOKE:			// 煙
				g_aEffect[nCnt].col.a -= 0.01f;
				g_aEffect[nCnt].fRadius -= 0.1f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_IMPACT:			// 衝撃は
				g_aEffect[nCnt].fRadius += 20.0f;
				g_aEffect[nCnt].col.a -= 0.05f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].pos.y <= -200)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_HITEXPLOSION:
				g_aEffect[nCnt].fRadius--;
				if (g_aEffect[nCnt].fRadius <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].move.x <= 1 && g_aEffect[nCnt].move.x >= -1)
				{
					g_aEffect[nCnt].col.a -= 0.01f;
				}

				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y *= 0.97f;
				break;
			case EFFECTTYPE_WATERFALL_0:
				g_aEffect[nCnt].fRadius++;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].move.x <= 2)
				{
					g_aEffect[nCnt].col.a -= 0.01f;
				}
				if (g_aEffect[nCnt].pos.y <= -100)
				{
					g_aEffect[nCnt].bUse = false;
				}
				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y -= 0.30f;
				break;
			case EFFECTTYPE_WATERFALL_1:	// けむり
				g_aEffect[nCnt].fRadius++;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].move.x <= 2)
				{
					g_aEffect[nCnt].col.a -= 0.01f;
				}
				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y += 0.001f;
				break;
			case EFFECTTYPE_STATE_UP:
				g_aEffect[nCnt].nLife--;
				g_aEffect[nCnt].fRadius--;
				g_aEffect[nCnt].col.a -= 0.01f;
				if (g_aEffect[nCnt].col.a <= 0||g_aEffect[nCnt].nLife<= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_FIRE:	// 炎
				g_aEffect[nCnt].fRadius += 0.3f;
				if (g_aEffect[nCnt].col.a <= 0.0f)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if ((g_aEffect[nCnt].move.x <= 1.0f&&g_aEffect[nCnt].move.x >= -1.0f) &&
					(g_aEffect[nCnt].move.z <= 1.0f&&g_aEffect[nCnt].move.z >= -1.0f))
				{
					g_aEffect[nCnt].col.a -= 0.1f;
				}
				if (rand() %50 == 0)
				{
					SetEffect(g_aEffect[nCnt].pos + D3DXVECTOR3(sinf(SetRandomEffect())*30, cosf(SetRandomEffect()) * 30, cosf(SetRandomEffect()) * 30)
						, g_aEffect[nCnt].move,
						D3DXVECTOR3(0, 0, 0),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 0, EFFECTTYPE_CARTOON_FIRE);
				}
				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y += 0.0001f;
				break;
			case EFFECTTYPE_HIBANA:
				g_aEffect[nCnt].col.a -= 0.1f;
				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y -= 0.1f;

				break;
			case EFFECTTYPE_WATER:	// 水しぶき
				g_aEffect[nCnt].move.y -= 0.49f;
				g_aEffect[nCnt].fRadius -= 0.1f;
				g_aEffect[nCnt].col.a -= 0.01f;
				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				if (g_aEffect[nCnt].fRadius <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (g_aEffect[nCnt].pos.y <= -300)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if (CollisionModelMinMax(&g_aEffect[nCnt].pos, &g_aEffect[nCnt].posOld, &D3DXVECTOR3(0, 0, 0),
					g_aEffect[nCnt].fRadius, g_aEffect[nCnt].fRadius, &g_aEffect[nCnt].pModel) == 2)
				{
					g_aEffect[nCnt].fRadius -= 0.5f;
					g_aEffect[nCnt].move.y *= -1;
				}
				break;
			case EFFECTTYPE_WALK:	// 土煙
				if ((g_aEffect[nCnt].move.x <= 1.0f&&g_aEffect[nCnt].move.x >= -1.0f) &&
					(g_aEffect[nCnt].move.z <= 1.0f&&g_aEffect[nCnt].move.z >= -1.0f))
				{
					g_aEffect[nCnt].col.a -= 0.1f;
				}

				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y += 0.0001f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_LASER:		// レーザー
				g_aEffect[nCnt].col.a -= 0.01f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_BOMB:		// 爆発
				g_aEffect[nCnt].col.a -= 0.1f;
				g_aEffect[nCnt].fRadius -= 0.1f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_DIRTY:	// 泥
				g_aEffect[nCnt].fRadius += 0.3f;
				if (g_aEffect[nCnt].col.a <= 0.0f)
				{
					g_aEffect[nCnt].bUse = false;
				}
				if ((g_aEffect[nCnt].move.x <= 1.0f&&g_aEffect[nCnt].move.x >= -1.0f) &&
					(g_aEffect[nCnt].move.z <= 1.0f&&g_aEffect[nCnt].move.z >= -1.0f))
				{
					g_aEffect[nCnt].col.a -= 0.1f;
				}

				g_aEffect[nCnt].move.x *= 0.97f;
				g_aEffect[nCnt].move.z *= 0.97f;
				g_aEffect[nCnt].move.y += 0.0001f;
				break;
			case EFFECTTYPE_SUNDER:	// 雷
				g_aEffect[nCnt].col.a -= 0.1f;
				if (g_aEffect[nCnt].col.a <= 0.0f)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			case EFFECTTYPE_CRASH: // 混乱
				g_aEffect[nCnt].rot.x += 0.1f;
				g_aEffect[nCnt].rot.z += 0.1f;
				if (g_aEffect[nCnt].rot.x > D3DX_PI)
				{// 差分が3.14を超えるなら
					g_aEffect[nCnt].rot.x -= D3DX_PI * 2.0f;
				}
				if (g_aEffect[nCnt].rot.y > D3DX_PI)
				{// 差分が3.14を超えるなら
					g_aEffect[nCnt].rot.y -= D3DX_PI * 2.0f;
				}
				g_aEffect[nCnt].move.x = sinf(-D3DX_PI / 2 + g_aEffect[nCnt].rot.x)*3;
				g_aEffect[nCnt].move.z = cosf(-D3DX_PI / 2 + g_aEffect[nCnt].rot.z)*3;
				g_aEffect[nCnt].nLife--;
				if (g_aEffect[nCnt].nLife <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			default:
				g_aEffect[nCnt].col.a -= 0.1f;
				if (g_aEffect[nCnt].col.a <= 0)
				{
					g_aEffect[nCnt].bUse = false;
				}
				break;
			}
			if (g_aEffect[nCnt].pos.y <= -300)
			{
				g_aEffect[nCnt].bUse = false;
			}
			//// 頂点情報の設定
			//pVtx[0].pos = D3DXVECTOR3(sinf(-g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.x)*g_aEffect[nCnt].fLength,
			//	cosf(-g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.y)*g_aEffect[nCnt].fLength, 0.0f) + g_aEffect[nCnt].pos;
			//pVtx[1].pos = D3DXVECTOR3(sinf(g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.x)*g_aEffect[nCnt].fLength,
			//	cosf( g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.y)*g_aEffect[nCnt].fLength, 0.0f) + g_aEffect[nCnt].pos;
			//pVtx[2].pos = D3DXVECTOR3(D3DX_PI + sinf(-g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.x)*g_aEffect[nCnt].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.y)*g_aEffect[nCnt].fLength, 0.0f) + g_aEffect[nCnt].pos;
			//pVtx[3].pos = D3DXVECTOR3(D3DX_PI + sinf(g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.x)*g_aEffect[nCnt].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCnt].fAngle + g_aEffect[nCnt].rot.y)*g_aEffect[nCnt].fLength, 0.0f) + g_aEffect[nCnt].pos;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCnt].col;
			pVtx[1].col = g_aEffect[nCnt].col;
			pVtx[2].col = g_aEffect[nCnt].col;
			pVtx[3].col = g_aEffect[nCnt].col;

			g_aEffect[nCnt].nCounterAnim++;

			if (g_aEffect[nCnt].nType == EFFECTTYPE_SMOKE|| g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_0
				|| g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_1|| g_aEffect[nCnt].nType==EFFECTTYPE_FIRE||
				g_aEffect[nCnt].nType == EFFECTTYPE_WALK||g_aEffect[nCnt].nType== EFFECTTYPE_BOMB||
				g_aEffect[nCnt].nType == EFFECTTYPE_DIRTY)
			{
				if (g_aEffect[nCnt].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCnt].nPatternAnim = (g_aEffect[nCnt].nPatternAnim + 1) % 8;
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8) *0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
				}
			}
			else if (g_aEffect[nCnt].nType == EFFECTTYPE_SUNDER)
			{// 雷
				if (g_aEffect[nCnt].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCnt].nPatternAnim = (g_aEffect[nCnt].nPatternAnim + 1) % 4;

					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4) *0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
				}
			}
			else
			{
				// テクスチャの設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();




}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawEffect(void)
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

	// Zソース
	//pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse )
		{
			if (g_aEffect[nCnt].nType == EFFECTTYPE_SMOKE
				|| g_aEffect[nCnt].nType == EFFECTTYPE_DIRTY||g_aEffect[nCnt].nType >=EFFECTTYPE_CARTOON_SLICE_0)
			{
				// αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			else
			{
				// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtxWorld);

			// ビルボードの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// 逆行列の設定
			g_aEffect[nCnt].mtxWorld._11 = mtxView._11;
			g_aEffect[nCnt].mtxWorld._12 = mtxView._21;
			g_aEffect[nCnt].mtxWorld._13 = mtxView._31;
			g_aEffect[nCnt].mtxWorld._21 = mtxView._12;
			g_aEffect[nCnt].mtxWorld._22 = mtxView._22;
			g_aEffect[nCnt].mtxWorld._23 = mtxView._32;
			g_aEffect[nCnt].mtxWorld._31 = mtxView._13;
			g_aEffect[nCnt].mtxWorld._32 = mtxView._23;
			g_aEffect[nCnt].mtxWorld._33 = mtxView._33;
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCnt].mtxWorld, &g_aEffect[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEffect[g_aEffect[nCnt].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);


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
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

}
//==============================================================================================================================
// エフェクトの設定
//==============================================================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++, pVtx += 4)
	{
		if (g_aEffect[nCnt].bUse == false)
		{
			g_aEffect[nCnt].pos = pos;
			g_aEffect[nCnt].Initpos = pos;
			g_aEffect[nCnt].Destpos = pos;
			g_aEffect[nCnt].move = move;
			g_aEffect[nCnt].rot = rot;
			g_aEffect[nCnt].col = col;
			g_aEffect[nCnt].fRadius = fRadius;
			g_aEffect[nCnt].nType = nType;
			g_aEffect[nCnt].nLife = nLife;
			g_aEffect[nCnt].fChangeRadius = 0;
			g_aEffect[nCnt].nPatternAnim = 0;
			g_aEffect[nCnt].nCounterAnim = 0;
			g_aEffect[nCnt].nRandAnim = rand()%8;
			g_aEffect[nCnt].fAttackPower = 0;						// ダメージ
			g_aEffect[nCnt].fDamageH = 0;							// 水平方向ダメージ
			g_aEffect[nCnt].fDamageV = 0;							// 水平方向ダメージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCnt].col;
			pVtx[1].col = g_aEffect[nCnt].col;
			pVtx[2].col = g_aEffect[nCnt].col;
			pVtx[3].col = g_aEffect[nCnt].col;
			if (g_aEffect[nCnt].nType == EFFECTTYPE_SMOKE || g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_0
				|| g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_1 || g_aEffect[nCnt].nType == EFFECTTYPE_FIRE ||
				g_aEffect[nCnt].nType == EFFECTTYPE_WALK || g_aEffect[nCnt].nType == EFFECTTYPE_BOMB||
				g_aEffect[nCnt].nType == EFFECTTYPE_DIRTY)
			{
				g_aEffect[nCnt].nPatternAnim = rand() % 8;
				// テクスチャの設定
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8) *0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
			}
			if (g_aEffect[nCnt].nType == EFFECTTYPE_SUNDER)
			{
				g_aEffect[nCnt].nPatternAnim = rand() % 4;
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4) *0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
			}
			g_aEffect[nCnt].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
void SetEffectDamage(int nIdy, float fAttackPower, float fDamageH, float fDamageV, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE nType)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++, pVtx += 4)
	{
		if (g_aEffect[nCnt].bUse == false)
		{
			g_aEffect[nCnt].pos = pos;
			g_aEffect[nCnt].Initpos = pos;
			g_aEffect[nCnt].Destpos = pos;
			g_aEffect[nCnt].move = move;
			g_aEffect[nCnt].rot = rot;
			g_aEffect[nCnt].col = col;
			g_aEffect[nCnt].fRadius = fRadius;
			g_aEffect[nCnt].nType = nType;
			g_aEffect[nCnt].nLife = nLife;
			g_aEffect[nCnt].fChangeRadius = 0;
			g_aEffect[nCnt].nPatternAnim = 0;
			g_aEffect[nCnt].nCounterAnim = 0;
			g_aEffect[nCnt].nRandAnim = rand() % 8;
			g_aEffect[nCnt].nIdy = nIdy;								// 番号
			g_aEffect[nCnt].fAttackPower = fAttackPower;						// ダメージ
			g_aEffect[nCnt].fDamageH = fDamageH;							// 水平方向ダメージ
			g_aEffect[nCnt].fDamageV = fDamageV;							// 水平方向ダメージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), (g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCnt].fRadius / 2), -(g_aEffect[nCnt].fRadius / 2), 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCnt].col;
			pVtx[1].col = g_aEffect[nCnt].col;
			pVtx[2].col = g_aEffect[nCnt].col;
			pVtx[3].col = g_aEffect[nCnt].col;
			if (g_aEffect[nCnt].nType == EFFECTTYPE_SMOKE || g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_0
				|| g_aEffect[nCnt].nType == EFFECTTYPE_WATERFALL_1 || g_aEffect[nCnt].nType == EFFECTTYPE_FIRE||
				g_aEffect[nCnt].nType == EFFECTTYPE_WALK|| g_aEffect[nCnt].nType == EFFECTTYPE_BOMB||
				g_aEffect[nCnt].nType == EFFECTTYPE_DIRTY)
			{
				g_aEffect[nCnt].nPatternAnim = rand() % 8;
				// テクスチャの設定
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8) *0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 8)*0.125f + 0.125f, (g_aEffect[nCnt].nPatternAnim / 8)*0.125f*g_aEffect[nCnt].nRandAnim + 0.125f);
			}
			if (g_aEffect[nCnt].nType == EFFECTTYPE_SUNDER )
			{
				g_aEffect[nCnt].nPatternAnim = rand() % 4;
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4) *0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCnt].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCnt].nPatternAnim / 4)*1.0f + 1.0f);
			}
			g_aEffect[nCnt].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

}
//==============================================================================================================================
// ランダム爆発の取得
//==============================================================================================================================
float SetRandomEffect(void)
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
Effect *GetEffect(void)
{
	return &g_aEffect[0];
}
//==============================================================================================================================
// ヒット
//==============================================================================================================================
void HitEffect(D3DXVECTOR3 pos)
{
	for (int nCntEffect = 0; nCntEffect < 5; nCntEffect++)
	{// 火花
		float nDirection = SetRandomEffect();
		float nDirectio2 = SetRandomEffect();
		float nDirectio3 = SetRandomEffect();
		SetEffect(pos,
			D3DXVECTOR3(sinf(nDirection) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirectio2) * (float(rand() % 50 / 10.0f) + 3), cosf(nDirectio3) * (float(rand() % 30 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 20) + 10, 30, EFFECTTYPE_HITEXPLOSION);
		SetEffect(pos,
			D3DXVECTOR3(sinf(nDirectio2) *(float(rand() % 50 / 10.0f) + 3), cosf(nDirectio3) * (float(rand() % 30 / 10.0f) + 3), cosf(nDirection) * (float(rand() % 50 / 10.0f) + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.8f, float(rand() % 100 / 100.0f), 0.0f, 0.8f), float(rand() % 30) + 10, 30, EFFECTTYPE_HITEXPLOSION);
	}
	for (int nCntEffect = 0; nCntEffect < 1; nCntEffect++)
	{// 衝撃は
		SetEffect(pos,
			D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.8f, 0.5f, 0.0f, 0.8f), 10, 30, EFFECTTYPE_IMPACT);
	}
}
