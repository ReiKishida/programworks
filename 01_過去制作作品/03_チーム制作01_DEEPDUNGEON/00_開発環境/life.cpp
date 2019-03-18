//=============================================================================
//
// ライフ処理 [Life.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "life.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "game.h"
#include "died.h"
#include "char_select.h"
#include "particle.h"
//****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIFE_VERTEX ((g_Life_h + 1) * (g_Life_v + 1))
#define LIFE_INDEX ( (g_Life_h * g_Life_v )* 2 + (4 * (g_Life_v - 1)) + 2 )
#define LIFE_POLYGON ( (g_Life_h * g_Life_v) * 2 + (4 * (g_Life_v - 1)) )

#define LIFE_RADIUS (38.0f)
#define LIFE_RADIUS2 (6.0f)

#define SOLDIER_LIFE (1000)
#define WARRIOR_LIFE (700)
#define SORCERER_LIFE (650)

#define LIFE_HEIGHT (5.0f)

#define LIFE_H (100) //メッシュの分割数(H)
#define LIFE_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffLife = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posLife;		//位置(中心座標)
D3DXVECTOR3 g_rotLife;		//向き
D3DXMATRIX g_mtxWorldLife;	//ワールドマトリックス
D3DXCOLOR g_colLife;
LIFE g_Life;
int g_nLife;		//見かけのライフ
int g_nMax_Life;		//見かけのライフ最大値
int g_nLifeNum;		//内部数値のライフ
int g_nMaxLifeNum;		//内部数値ライフの最大値
int g_Life_h, g_Life_v;

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	CHARACTER_TYPE pCharaType = GetCharaType();		// 選択キャラクター情報の取得

	switch (pCharaType)
	{
	case CHARACTER_TYPE_SOLDIER:
			g_Life_h = SOLDIER_LIFE;
			g_Life_v = 1;
			g_nLifeNum = SOLDIER_LIFE;
			g_nMaxLifeNum = 1000;
			break;

	case CHARACTER_TYPE_WARRIOR:
		g_Life_h = WARRIOR_LIFE;
		g_Life_v = 1;
		g_nLifeNum = WARRIOR_LIFE;
		g_nMaxLifeNum = 700;


		break;
	case CHARACTER_TYPE_SORCERER:
		g_Life_h = SORCERER_LIFE;
		g_Life_v = 1;
		g_nLifeNum = SORCERER_LIFE;
		g_nMaxLifeNum = 650;
		break;
	}
	
	int Polygon = (g_Life_h * g_Life_v) * 2 + (4 * (g_Life_v - 1));
	g_nMax_Life = 0;
	g_nLife = 0;
	MakeVertexLife(pDevice);
	MakeIndexLife(pDevice);

	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	g_posLife = pPlayer.pos;
	g_rotLife = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffLife != NULL)
	{
		g_pIdxBuffLife->Release();
		g_pIdxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	PLAYER pPlayer = *GetPlayer();

	CAMERA pCamera = *GetCamera();

	g_posLife = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y + 30.0f, pPlayer.pos.z);
	g_rotLife = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f + pPlayer.rot.y, 0.0f);

	float fCol = 0.0f;

	fCol += 0.01f;

	float fCol2 = 1.0f;

	fCol2 -= 0.01f;

	//if (GetKeyboardTrigger(DIK_I) == true )
	//{	
	//	HitDamage(-100);
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldLife);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotLife.y, g_rotLife.x, g_rotLife.z);

	D3DXMatrixMultiply(&g_mtxWorldLife, &g_mtxWorldLife, &mtxrot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posLife.x, g_posLife.y, g_posLife.z);

	D3DXMatrixMultiply(&g_mtxWorldLife, &g_mtxWorldLife, &mtxtrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLife);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffLife);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, 0);

	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, LIFE_VERTEX, 0, LIFE_POLYGON + g_nLife);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
//ライフ減少
//=============================================================================
void HitDamage(int nDamage)
{
	PLAYER *pPlayer = GetPlayer();
	DIED *pDied = GetDied();
	CHARACTER_TYPE pCharaType = GetCharaType();		// 選択キャラクター情報の取得


		

		if (g_nLifeNum >= g_nMaxLifeNum)
		{
			g_nLifeNum = g_nMaxLifeNum;
		}

		if (g_nLife >= g_nMax_Life)
		{
			g_nLife = g_nMax_Life;
		}


	if (pPlayer->status == PLAYERSTATUS_NORMAL)
	{
		g_nLife += nDamage * 2;
		g_nLifeNum += nDamage;
		pPlayer->nCntCoolDown = 2;
		pPlayer->status = PLAYERSTATUS_DAMAGE;

		if (nDamage <= 0)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 30.0f, pPlayer->pos.z), 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION);
			}
		}
		if (nDamage >= 0)
		{
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				SetParticle(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 30.0f, pPlayer->pos.z), 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION);
			}
		}
	}
	if (g_nLifeNum <= 0)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++, pDied++)
		{
			pDied->bUse = true;
		}
		pPlayer->status = PLAYERSTATUS_DEAD;
		SetPlayerState(PLAYERSTATE_DEATH);
		g_nLifeNum = 0;
		SetGameState(GAMESTATE_GAMEOVER);
	}

}
//=============================================================================
// 最大Hp減少
//=============================================================================
void DecreaseMaxLife(int nLife)
{
	g_nMaxLifeNum += nLife;
	g_nMax_Life   += (nLife * 2);

	if (g_nLifeNum >= g_nMaxLifeNum)
	{
		g_nLifeNum = g_nMaxLifeNum;
	}

	if (g_nLife >= g_nMax_Life)
	{
		g_nLife = g_nMax_Life;
	}
}



//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * LIFE_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Life_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_Life_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_Life_h + 1; nCntLife_H++)
		{
			if (nCntLife_V == 0)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * LIFE_RADIUS;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.y = 1.0f;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * LIFE_RADIUS;
				//法線の設定
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_Life_h / 2)), nCntLife_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntLife_V == 1)
			{
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * (LIFE_RADIUS - LIFE_RADIUS2);
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.y = 1.0f;
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * (LIFE_RADIUS - LIFE_RADIUS2);
				//法線の設定
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラー
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f);

				//テクスチャ座標
				pVtx[nCntLife_V * (g_Life_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_Life_h / 2)), nCntLife_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexLife(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * LIFE_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffLife,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

	//インデックスバッファをロック
	g_pIdxBuffLife->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIndex_V = 0; nCntIndex_V < g_Life_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Life_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Life_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Life_h && nCntIndex_V <= g_Life_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Life_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}

	//頂点バッファをアンロック
	g_pIdxBuffLife->Unlock();
}