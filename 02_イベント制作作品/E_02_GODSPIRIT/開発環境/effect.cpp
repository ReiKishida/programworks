////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// エフェクトの処理 [effect.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "effect.h"
#include "player.h"
#include "enemy.h"
#include "effectUP.h"
#include "game.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_EFFECT	    (6244)
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
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/Water.jpg" },
	{ "data/TEXTURE/snow.jpg" },
	{ "data/TEXTURE/rockon.jpg" },
	{ "data/TEXTURE/EffectExplosion001.png" },
	{ "data/TEXTURE/watereffect.png" },
	{ "data/TEXTURE/HitEffect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/HitEffect001.png" },
	{ "data/TEXTURE/Sunder000.jpg" },
	{ "data/TEXTURE/Sunder001.jpg" },
	{ "data/TEXTURE/Pill.png" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/effect000.jpg" },
	{ "data/TEXTURE/smoke.jpg" },
	{ "data/TEXTURE/smoke.png" },
	{ "data/TEXTURE/FlozeEffect.png"},
	{ "data/TEXTURE/Sunder001.jpg" },
	{ "data/TEXTURE/Water.jpg" },

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
		g_aEffect[nCntEffect].nType = EFFECTTYPE_EXPLOSION;			// 種類
		g_aEffect[nCntEffect].nIdyShadow = 0;						// 影のID
		g_aEffect[nCntEffect].nCounterAnim = 0;						// アニメーションカウンター
		g_aEffect[nCntEffect].nPatternAnim = 0;						// アニメーションパターン
		g_aEffect[nCntEffect].nSpeedCounter = 0;					// カウンター
		g_aEffect[nCntEffect].bUse = false;							// 使用されているかどうか
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
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pVtx+=4)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// 前回の位置の設定
			g_aEffect[nCntEffect].posOld = g_aEffect[nCntEffect].pos;
			// 移動量加算
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			switch (g_aEffect[nCntEffect].nType)
			{
			case EFFECTTYPE_EXPLOSION:	// 爆発エフェクト
				g_aEffect[nCntEffect].fRadius--;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_PLAYER:	// プレイヤー
				g_aEffect[nCntEffect].pos.y -= 0.1f;
				g_aEffect[nCntEffect].fRadius -= 0.1f;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}

				break;
			case EFFECTTYPE_WATER:	// 水しぶき
				g_aEffect[nCntEffect].move.y -= 0.49f;
				//g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -130)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_DRAIN:	// 吸収
				if (g_aEffect[nCntEffect].nLife <= 0)
				{
					g_aEffect[nCntEffect].nSpeedCounter++;
					g_aEffect[nCntEffect].pos += (g_aEffect[nCntEffect].Destpos - g_aEffect[nCntEffect].pos)*0.01f*float(g_aEffect[nCntEffect].nSpeedCounter);
					g_aEffect[nCntEffect].fRadius -= 0.1f;
					g_aEffect[nCntEffect].col.a -= 0.02f;
				}
				g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].Destpos = D3DXVECTOR3(pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._41, pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._42, pPlayer->aModel[0][pPlayer->nWeaponState].mtxWorld._43);
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_AIM:
				g_aEffect[nCntEffect].rot.x += 0.1f;
				g_aEffect[nCntEffect].rot.y += 0.1f;
				g_aEffect[nCntEffect].fChangeRadius += 0.1f;
				g_aEffect[nCntEffect].pos = GetEnemy()[GetPlayer()->nIdyLockEnemy].pos + D3DXVECTOR3(0.0f, GetEnemy()[GetPlayer()->nIdyLockEnemy].fRadius, 0.0f);
				g_aEffect[nCntEffect].fRadius += sinf(g_aEffect[nCntEffect].fChangeRadius) * 5;
				if (GetPlayer()->bAttention == false)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				// 頂点の長さ回転
				g_aEffect[nCntEffect].fAngle = atan2f((g_aEffect[nCntEffect].fRadius), (g_aEffect[nCntEffect].fRadius));
				g_aEffect[nCntEffect].fLength = sqrtf((g_aEffect[nCntEffect].fRadius)*(g_aEffect[nCntEffect].fRadius) + (g_aEffect[nCntEffect].fRadius)*(g_aEffect[nCntEffect].fRadius));
				break;
			case EFFECTTYPE_DEATH:	// 死亡エフェクト
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				//	g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1)
				{
					g_aEffect[nCntEffect].fRadius--;

				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.01f;

				break;
			case EFFECTTYPE_WATERFALL:
				g_aEffect[nCntEffect].move.y -= 0.49f;
				//g_aEffect[nCntEffect].col.a -= 0.01f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -130)
				{
					g_aEffect[nCntEffect].bUse = false;
				}

				break;
			case EFFECTTYPE_IMPACT:
				g_aEffect[nCntEffect].fRadius += 20.0f;
				g_aEffect[nCntEffect].col.a -= 0.05f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].pos.y <= -200)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SMOKE:	// けむり
				g_aEffect[nCntEffect].fRadius++;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 2)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.003f;
				break;
			case EFFECTTYPE_SMOKEDOWN:	// 煙↓
				g_aEffect[nCntEffect].fRadius++;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 2)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}
				if (g_aEffect[nCntEffect].pos.y <= -100)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y -= 0.98f;
				break;
			case EFFECTTYPE_HIT:
				g_aEffect[nCntEffect].fRadius += 20.0f;
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SUNDER:
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_SUNDER1:
				g_aEffect[nCntEffect].col.a -= 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_POISON:
				g_aEffect[nCntEffect].col.a -= 0.03f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].fRadius -= 0.01f;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_POISONSMOKE:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1.5f&&g_aEffect[nCntEffect].move.x >= -1.5f ||
					g_aEffect[nCntEffect].move.z <= 1.5f&&g_aEffect[nCntEffect].move.z >= -1.5f)
				{
					g_aEffect[nCntEffect].col.a -= 0.015f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.0001f;
				break;
			case EFFECTTYPE_HITEXPLOSION:
				g_aEffect[nCntEffect].fRadius--;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1 && g_aEffect[nCntEffect].move.x >= -1)
				{
					g_aEffect[nCntEffect].col.a -= 0.01f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y *= 0.97f;
				break;
			case EFFECTTYPE_FIRE:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0.1f)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				if (g_aEffect[nCntEffect].move.x <= 1.5f&&g_aEffect[nCntEffect].move.x >= -1.5f ||
					g_aEffect[nCntEffect].move.z <= 1.5f&&g_aEffect[nCntEffect].move.z >= -1.5f)
				{
					g_aEffect[nCntEffect].col.a -= 0.1f;
				}

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.0001f;
				break;
			case EFFECTTYPE_WIND:
				g_aEffect[nCntEffect].fRadius += 0.3f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.03f;

				g_aEffect[nCntEffect].move.x *= 0.97f;
				g_aEffect[nCntEffect].move.z *= 0.97f;
				g_aEffect[nCntEffect].move.y += 0.003f;
				break;
			case EFFECTTYPE_FLOZE:
				g_aEffect[nCntEffect].fRadius += 0.1f;
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				g_aEffect[nCntEffect].col.a -= 0.03f;
				break;
			case EFFECTTYPE_SUNDER_DOWN:
				g_aEffect[nCntEffect].move.y -= 3.49f;
				if (pPlayer->pos.y >= g_aEffect[nCntEffect].pos.y)
				{
					g_aEffect[nCntEffect].move.y = 0;
					if (g_aEffect[nCntEffect].nSpeedCounter <= 0)
					{
						float nDirection2 = SetRandomEffect();
						g_aEffect[nCntEffect].move.x = sinf(nDirection2)*(rand() % 15);
						g_aEffect[nCntEffect].move.z = cosf(nDirection2)*(rand() % 15);
					}
					g_aEffect[nCntEffect].col.a -= 0.03f;
					g_aEffect[nCntEffect].nSpeedCounter++;

				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			case EFFECTTYPE_WATER_DOWN:
				g_aEffect[nCntEffect].move.y -= 3.49f;
				if (pPlayer->pos.y >= g_aEffect[nCntEffect].pos.y)
				{
					g_aEffect[nCntEffect].move.y = 0;
					if (g_aEffect[nCntEffect].nSpeedCounter <= 0)
					{
						float nDirection2 = SetRandomEffect();
						g_aEffect[nCntEffect].move.x = sinf(nDirection2)*(rand() % 15);
						g_aEffect[nCntEffect].move.z = cosf(nDirection2)*(rand() % 15);
					}
					g_aEffect[nCntEffect].col.a -= 0.03f;
					g_aEffect[nCntEffect].nSpeedCounter++;

				}
				if (g_aEffect[nCntEffect].col.a <= 0)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				break;
			}
			if (g_aEffect[nCntEffect].pos.y <= -300)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			//// 頂点情報の設定
			//pVtx[0].pos = D3DXVECTOR3(sinf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[1].pos = D3DXVECTOR3(sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf( g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[2].pos = D3DXVECTOR3(D3DX_PI + sinf(-g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			//pVtx[3].pos = D3DXVECTOR3(D3DX_PI + sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x)*g_aEffect[nCntEffect].fLength,
			//	cosf(D3DX_PI - g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.y)*g_aEffect[nCntEffect].fLength, 0.0f) + g_aEffect[nCntEffect].pos;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			g_aEffect[nCntEffect].nCounterAnim++;
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_WATERFALL)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 3 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % MAX_PATTERN;
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5) *ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM + ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f + 0.33f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 5)*ANIM + ANIM, (g_aEffect[nCntEffect].nPatternAnim / 5)*0.33f + 0.33f);
				}
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_DEATH)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % 7;
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7) *0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f + 0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 7)*0.142f +0.142f, (g_aEffect[nCntEffect].nPatternAnim / 7)*1.0f + 1.0f);
				}
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER1|| g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER_DOWN)
			{
				if (g_aEffect[nCntEffect].nCounterAnim % 5 == 0)
				{
					g_aEffect[nCntEffect].nPatternAnim = (g_aEffect[nCntEffect].nPatternAnim + 1) % 4;
					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4) *0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
					pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
					pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
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
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_POISON||g_aEffect[nCntEffect].nType == EFFECTTYPE_WIND||g_aEffect[nCntEffect].nType == EFFECTTYPE_FLOZE)
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
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			// ビルボードの設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			// 逆行列の設定
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEffect[g_aEffect[nCntEffect].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

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

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].Initpos = pos;
			g_aEffect[nCntEffect].Destpos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].rot = rot;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].fChangeRadius = 0;
			g_aEffect[nCntEffect].nPatternAnim = 0;
			g_aEffect[nCntEffect].nCounterAnim = 0;
			g_aEffect[nCntEffect].nSpeedCounter = 0;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER1|| g_aEffect[nCntEffect].nType == EFFECTTYPE_SUNDER_DOWN)
			{
				g_aEffect[nCntEffect].nPatternAnim = rand() % 4;
				pVtx[0].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4) *0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aEffect[nCntEffect].nPatternAnim % 4)*0.25f + 0.25f, (g_aEffect[nCntEffect].nPatternAnim / 4)*1.0f + 1.0f);
			}
			g_aEffect[nCntEffect].bUse = true;
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