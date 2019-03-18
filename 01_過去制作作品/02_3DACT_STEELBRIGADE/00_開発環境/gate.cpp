//=============================================================================
//
// 門処理 [gate.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "gate.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGate = NULL;		//テクスチャへのポインタ
LPD3DXMESH g_pMeshGate = NULL;	//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatGate = NULL;	//マテリアル情報へのポインタ

DWORD g_nNumMatGate;	//マテリアルの情報の数


GATE g_aGate[MAX_GATE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		// 位置・向きの初期設定
		g_aGate[nCntGate].VtxMinGate = D3DXVECTOR3(10000, 10000, 10000);
		g_aGate[nCntGate].VtxMaxGate = D3DXVECTOR3(-10000, -10000, -10000);
		g_aGate[nCntGate].bUse = false;
		//g_aGate[nCntGate].nCnt = 0;

	}
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/seidou.jpeg", &g_pTextureGate);


	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/gate2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatGate,
		NULL,
		&g_nNumMatGate,
		&g_pMeshGate);

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshGate->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshGate->GetFVF());

		//頂点バッファをロック
		g_pMeshGate->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_aGate[nCntGate].VtxMinGate.x)
			{
				g_aGate[nCntGate].VtxMinGate.x = Vtx.x;
			}

			if (Vtx.y < g_aGate[nCntGate].VtxMinGate.y)
			{
				g_aGate[nCntGate].VtxMinGate.y = Vtx.y;
			}

			if (Vtx.z < g_aGate[nCntGate].VtxMinGate.z)
			{
				g_aGate[nCntGate].VtxMinGate.z = Vtx.z;
			}


			if (Vtx.x > g_aGate[nCntGate].VtxMaxGate.x)
			{
				g_aGate[nCntGate].VtxMaxGate.x = Vtx.x;
			}

			if (Vtx.y > g_aGate[nCntGate].VtxMaxGate.y)
			{
				g_aGate[nCntGate].VtxMaxGate.y = Vtx.y;
			}

			if (Vtx.z > g_aGate[nCntGate].VtxMaxGate.z)
			{
				g_aGate[nCntGate].VtxMaxGate.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		g_pMeshGate->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGate(void)
{

	//テクスチャの破棄
	if (g_pTextureGate != NULL)
	{
		g_pTextureGate->Release();
		g_pTextureGate = NULL;
	}

	// メッシュの開放
	if (g_pMeshGate != NULL)
	{
		g_pMeshGate->Release();
		g_pMeshGate = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatGate != NULL)
	{
		g_pBuffMatGate->Release();
		g_pBuffMatGate = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGate(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGate(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ


	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGate[nCntGate].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aGate[nCntGate].rot.y, g_aGate[nCntGate].rot.x, g_aGate[nCntGate].rot.z);

			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aGate[nCntGate].pos.x, g_aGate[nCntGate].pos.y, g_aGate[nCntGate].pos.z);

			D3DXMatrixMultiply(&g_aGate[nCntGate].mtxWorld, &g_aGate[nCntGate].mtxWorld, &mtxtrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGate[nCntGate].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatGate->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatGate; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTextureGate);

				// モデル(パーツ)の描画
				g_pMeshGate->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionGate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x && pPos->x - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x
				&&pPos->y + VtxMax->y >= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y && pPos->y - VtxMax->y <= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y
				&&pPos->z + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z && pPos->z - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x  && pPos->x + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x)
				{//左からきたとき
					pPos->x = g_aGate[nCntGate].VtxMinGate.x + g_aGate[nCntGate].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x  && pPos->x - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x)
				{//右からきたとき
					pPos->x = g_aGate[nCntGate].VtxMaxGate.x + g_aGate[nCntGate].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z && pPos->z + VtxMax->z >= g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z)
				{//手前から来た時
					pPos->z = g_aGate[nCntGate].VtxMinGate.z + g_aGate[nCntGate].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z && pPos->z - VtxMax->z <= g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z)
				{//奥から来た時
					pPos->z = g_aGate[nCntGate].VtxMaxGate.z + g_aGate[nCntGate].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y && pPos->y - VtxMax->y <= g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aGate[nCntGate].VtxMaxGate.y + g_aGate[nCntGate].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y && pPos->y + VtxMax->y >= g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y)
				{//下から来た時
					pPos->y = g_aGate[nCntGate].VtxMinGate.y + g_aGate[nCntGate].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}




void SetGate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	BYTE *pVtxBuff;	//頂点バッファのポインタ

					//頂点バッファをロック
	g_pMeshGate->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntGate = 0; nCntGate < MAX_GATE; nCntGate++)
	{
		if (g_aGate[nCntGate].bUse == false)	//ブロックが使用されていない
		{
			g_aGate[nCntGate].pos = pos;

			g_aGate[nCntGate].rot = rot;

			g_aGate[nCntGate].nLife = nLife;

			g_aGate[nCntGate].bUse = true;	//使用している状態にする
			g_aGate[nCntGate].nCnt++;
			break;
		}

	}
	//頂点バッファをアンロック
	g_pMeshGate->UnlockVertexBuffer();
}


//=============================================================================
// 被ダメージ処理
//=============================================================================
void HitGate(int nDamage , int nCntGate)
{

		if (g_aGate[nCntGate].bUse == true)
		{
			g_aGate[nCntGate].nLife += nDamage;
			for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
			{
				SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
			}

			if (g_aGate[nCntGate].nLife <= 0)
			{
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				g_aGate[nCntGate].bUse = false;
				for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
				{
					SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
					SetParticle(g_aGate[nCntGate].pos, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);

				}
				AddScore(5000);
				g_aGate[nCntGate].nCnt--;
			}
		}

}



GATE *GetGate(void)
{
	return &g_aGate[0];
}