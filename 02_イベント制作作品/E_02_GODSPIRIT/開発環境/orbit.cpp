////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 軌跡の処理 [orbit.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "orbit.h"
#include "player.h"
#include "gamepad.h"
#include "effect.h"
#include "input.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}OrbitTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureOrbit[ORBITTYPE_MAX] = {};	// テクスチャへのポインタ
Orbit g_aOrbit[MAX_ORBIT];
OrbitTexture g_aOrbitTexInfo[ORBITTYPE_MAX] =
{
	{ "data/TEXTURE/orbit000.jpg" },
	{ "data/TEXTURE/orbit001.jpg" },
	{ "data/TEXTURE/orbit002.jpg" },
	{ "data/TEXTURE/orbit003.jpg" },
};
int g_nCntOrbit;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	
	for (int nCntOrbitTex = 0; nCntOrbitTex < ORBITTYPE_MAX; nCntOrbitTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aOrbitTexInfo[nCntOrbitTex].pFileName,	// ファイルの名前
			&g_apTextureOrbit[nCntOrbitTex]);			// テクスチャへのポインタ
	}

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aOrbit[nCntOrbit].nType = ORBITTYPE_0;					// 種類
		g_aOrbit[nCntOrbit].nMotionType = PLAYER_MOTION_TYPE_MAX;
		g_aOrbit[nCntOrbit].Start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 始点
		g_aOrbit[nCntOrbit].End = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 長さ
		g_aOrbit[nCntOrbit].InitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 始点
		g_aOrbit[nCntOrbit].InitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 長さ
		g_aOrbit[nCntOrbit].nIdxParent = 0;							// 親モデル
		g_aOrbit[nCntOrbit].nLength = MAX_ORBIT_LENGTH;				// 長さ
		g_aOrbit[nCntOrbit].nPlus = 0;								// 減らす量
		g_aOrbit[nCntOrbit].nCharacterType = ORBIT_PLAYER;				// どっちで使うか
		g_aOrbit[nCntOrbit].nWeaponState = PLAYERWEAPON_HAND;			// 武器タイプ
		g_aOrbit[nCntOrbit].nCntTrueTime = 0;						// 消えるまでの時間
		g_aOrbit[nCntOrbit].bUse = false;							// 使用されているかどうか
	}

	MakeVertexOrbit(pDevice);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitOrbit(void)
{
	// テクスチャの破棄
	for (int nCntOrbit = 0; nCntOrbit < ORBITTYPE_MAX; nCntOrbit++)
	{
		if (g_apTextureOrbit[nCntOrbit] != NULL)
		{
			g_apTextureOrbit[nCntOrbit]->Release();
			g_apTextureOrbit[nCntOrbit] = NULL;
		}
	}
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// 頂点バッファの破棄
		if (g_aOrbit[nCntOrbit].pVtxBuff != NULL)
		{
			g_aOrbit[nCntOrbit].pVtxBuff->Release();
			g_aOrbit[nCntOrbit].pVtxBuff = NULL;
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateOrbit(void)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// 頂点バッファをロックし頂点データへのポインタを取得
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (g_aOrbit[nCntOrbit].bUse == true)
		{
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++)
			{
				pVtx[0 + nCntLength * 2].pos = pVtx[2 + nCntLength * 2].pos;
				pVtx[1 + nCntLength * 2].pos = pVtx[3 + nCntLength * 2].pos;
				pVtx[0 + nCntLength * 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f*nCntLength);
				pVtx[1 + nCntLength * 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.01f*nCntLength);
			}
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 1].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[g_aOrbit[nCntOrbit].nLength * 2 - 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
			g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++)
			{
				pVtx[0 + nCntLength * 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
				pVtx[1 + nCntLength * 2].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			}

		}
		// 頂点バッファをアンロックする
		g_aOrbit[nCntOrbit].pVtxBuff->Unlock();
	}
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	Player *pPlayer = GetPlayer();
	D3DXMATRIX mtxPlayerWorld;
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング																		
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);					// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ライト影響受けない
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].nCharacterType == ORBIT_PLAYER)
		{
			 mtxPlayerWorld = pPlayer->aModel[g_aOrbit[nCntOrbit].nIdxParent][pPlayer->nWeaponState].mtxWorld;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxStart);
		D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxStart, g_aOrbit[nCntOrbit].Start.x, g_aOrbit[nCntOrbit].Start.y, g_aOrbit[nCntOrbit].Start.z);	// スタート
		D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxStart, &g_aOrbit[nCntOrbit].mtxStart, &mtxPlayerWorld);	// [始点*プレイヤー]のマトリックス計算
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxStart);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxEnd);
		D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxEnd, g_aOrbit[nCntOrbit].End.x, g_aOrbit[nCntOrbit].End.y, g_aOrbit[nCntOrbit].End.z);	// エンド
		D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxEnd, &g_aOrbit[nCntOrbit].mtxEnd, &mtxPlayerWorld);		// [終点*プレイヤー]のマトリックス計算
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxEnd);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxWorld);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxWorld);

		if (g_aOrbit[nCntOrbit].bUse == true)
		{	
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength + g_aOrbit[nCntOrbit].nPlus ; nCntLength++)
			{
				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_aOrbit[nCntOrbit].pVtxBuff, 0, sizeof(VERTEX_3D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_apTextureOrbit[g_aOrbit[nCntOrbit].nType]);
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLength, 2);				
			}
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);					// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライト影響受けない
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * MAX_ORBIT_LENGTH,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_aOrbit[nCntOrbit].pVtxBuff,
			NULL);
		// 頂点バッファをロックし頂点データへのポインタを取得
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_LENGTH; nCntOrbit++, pVtx += 2)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャの設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点バッファをアンロックする
		g_aOrbit[nCntOrbit].pVtxBuff->Unlock();

	}
}
//==============================================================================================================================
// 軌跡の設定
//==============================================================================================================================
void SetOrbit(int nIdxParent,int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		// 頂点バッファをロックし頂点データへのポインタを取得
		g_aOrbit[nCntOrbit].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		if (g_aOrbit[nCntOrbit].bUse == false)
		{
			g_aOrbit[nCntOrbit].nIdxParent = nIdxParent;
			g_aOrbit[nCntOrbit].Start = Start;
			g_aOrbit[nCntOrbit].End = End;
			g_aOrbit[nCntOrbit].InitStart = Start;
			g_aOrbit[nCntOrbit].InitEnd = End;
			g_aOrbit[nCntOrbit].nCharacterType = ORBIT_PLAYER;
			g_aOrbit[nCntOrbit].nType = nType;
			g_aOrbit[nCntOrbit].nMotionType = nMotionType;
			g_aOrbit[nCntOrbit].nWeaponState = nWeaponState;
			g_aOrbit[nCntOrbit].nLength = nLength;
			if (g_aOrbit[nCntOrbit].nLength >= 5)
			{	
				g_aOrbit[nCntOrbit].nPlus = g_aOrbit[nCntOrbit].nLength - 3;
			}
			for (int nCntLength = 0; nCntLength < g_aOrbit[nCntOrbit].nLength; nCntLength++, pVtx += 2)
			{
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxStart._41, g_aOrbit[nCntOrbit].mtxStart._42, g_aOrbit[nCntOrbit].mtxStart._43);
				pVtx[1].pos = D3DXVECTOR3(g_aOrbit[nCntOrbit].mtxEnd._41, g_aOrbit[nCntOrbit].mtxEnd._42, g_aOrbit[nCntOrbit].mtxEnd._43);
			}
			g_aOrbit[nCntOrbit].bUse = true;
			// 頂点バッファをアンロックする
			g_aOrbit[nCntOrbit].pVtxBuff->Unlock();

			break;

		}
	}
}
void SetPosOrbit(PlayerMotionType nMotionType, PLAYERWEAPON nWeaponState)
{
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].nCharacterType == ORBIT_PLAYER)
		{
			if (g_aOrbit[nCntOrbit].nWeaponState == nWeaponState)
			{
				if (g_aOrbit[nCntOrbit].nMotionType == nMotionType)
				{
					g_aOrbit[nCntOrbit].bUse = true;

					g_aOrbit[nCntOrbit].nCntTrueTime = g_aOrbit[nCntOrbit].nLength;
					if (g_aOrbit[nCntOrbit].nMotionType == PLAYER_MOTION_TYPE_MOVE)
					{// 歩きだけは早めに切る
						g_aOrbit[nCntOrbit].nCntTrueTime = 10;
					}
					// オービットが伸びる

					if (g_aOrbit[nCntOrbit].InitStart.x > 0 || g_aOrbit[nCntOrbit].InitStart.x < 0)
					{
						g_aOrbit[nCntOrbit].Start.x *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.x > 0 || g_aOrbit[nCntOrbit].InitEnd.x < 0)
					{
						g_aOrbit[nCntOrbit].End.x *= 1.02f;
					}
					if (g_aOrbit[nCntOrbit].InitStart.y > 0 || g_aOrbit[nCntOrbit].InitStart.y < 0)
					{
						g_aOrbit[nCntOrbit].Start.y *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.y > 0 || g_aOrbit[nCntOrbit].InitEnd.y < 0)
					{
						g_aOrbit[nCntOrbit].End.y *= 1.02f;
					}
					if (g_aOrbit[nCntOrbit].InitStart.z > 0 || g_aOrbit[nCntOrbit].InitStart.z < 0)
					{
						g_aOrbit[nCntOrbit].Start.z *= 1.01f;
					}
					if (g_aOrbit[nCntOrbit].InitEnd.z > 0 || g_aOrbit[nCntOrbit].InitEnd.z < 0)
					{
						g_aOrbit[nCntOrbit].End.z *= 1.02f;
					}

				}
				else
				{
					g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
					g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

					g_aOrbit[nCntOrbit].nCntTrueTime--;
					if (g_aOrbit[nCntOrbit].nCntTrueTime <= 0)
					{
						g_aOrbit[nCntOrbit].nCntTrueTime = 0;
						g_aOrbit[nCntOrbit].bUse = false;
					}
				}
			}
			else
			{
				g_aOrbit[nCntOrbit].Start = g_aOrbit[nCntOrbit].InitStart;
				g_aOrbit[nCntOrbit].End = g_aOrbit[nCntOrbit].InitEnd;

				g_aOrbit[nCntOrbit].nCntTrueTime--;
				if (g_aOrbit[nCntOrbit].nCntTrueTime <= 0)
				{
					g_aOrbit[nCntOrbit].nCntTrueTime = 0;
					g_aOrbit[nCntOrbit].bUse = false;
				}
			}
		}
	}
}
