//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "smoke.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE (4)

#define MOVE_SPEED (1.5f)
#define ROT_SPEED (0.1f)

#define STAGE_LIMIT_MAX_X (150.0f)
#define STAGE_LIMIT_MIN_X (-50.0f)
#define STAGE_LIMIT_MAX_Z (50.0f)
#define STAGE_LIMIT_MIN_Z (-150.0f)

#define MODEL_TEXTURENAME_0	"data/TEXTURE/rockandtekkotu.jpg"		//モデルのテクスチャ1
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureModel = NULL;		//テクスチャへのポインタ
LPD3DXMESH g_pMeshModel = NULL;	//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;	//マテリアル情報へのポインタ

DWORD g_nNumMatModel;	//マテリアルの情報の数


MODEL g_aModel[MAX_MODEL];
//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MODEL_TEXTURENAME_0, &g_pTextureModel);


	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		// 位置・向きの初期設定
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);
		g_aModel[nCntModel].VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);
		g_aModel[nCntModel].bUse = false;
	}


	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/rock.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel);

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshModel->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

		//頂点バッファをロック
		g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

			if (Vtx.x < g_aModel[nCntModel].VtxMinModel.x)
			{
				g_aModel[nCntModel].VtxMinModel.x = Vtx.x;
			}

			if (Vtx.y < g_aModel[nCntModel].VtxMinModel.y)
			{
				g_aModel[nCntModel].VtxMinModel.y = Vtx.y;
			}

			if (Vtx.z < g_aModel[nCntModel].VtxMinModel.z)
			{
				g_aModel[nCntModel].VtxMinModel.z = Vtx.x;
			}


			if (Vtx.x > g_aModel[nCntModel].VtxMaxModel.x)
			{
				g_aModel[nCntModel].VtxMaxModel.x = Vtx.x;
			}

			if (Vtx.y > g_aModel[nCntModel].VtxMaxModel.y)
			{
				g_aModel[nCntModel].VtxMaxModel.y = Vtx.y;
			}

			if (Vtx.z > g_aModel[nCntModel].VtxMaxModel.z)
			{
				g_aModel[nCntModel].VtxMaxModel.z = Vtx.z;
			}
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		g_pMeshModel->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{

		//テクスチャの破棄
	if (g_pTextureModel != NULL)
	{
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}

	// メッシュの開放
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ


	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);

			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);

			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxtrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, g_pTextureModel);

				// モデル(パーツ)の描画
				g_pMeshModel->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}


bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			if (  pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x
				&&pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y
				&&pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x  && pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x)
				{//左からきたとき
					pPos->x = g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x  && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x)
				{//右からきたとき
					pPos->x = g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z)
				{//手前から来た時
					pPos->z = g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z<= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
				{//奥から来た時
					pPos->z = g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z + VtxMax->z ;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y)
				{//下から来た時
					pPos->y = g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


bool CollisionModelEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *VtxMax, D3DXVECTOR3 *VtxMin)
{
	bool bLand = false; //着地したか

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			if (pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x
				&&pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y
				&&pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
			{

				if (pPosold->x + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x  && pPos->x + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x)
				{//左からきたとき
					pPos->x = g_aModel[nCntModel].VtxMinModel.x + g_aModel[nCntModel].pos.x - VtxMax->z;

					bLand = true;
				}

				else if (pPosold->x - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x  && pPos->x - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x)
				{//右からきたとき
					pPos->x = g_aModel[nCntModel].VtxMaxModel.x + g_aModel[nCntModel].pos.x + VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z + VtxMax->z <= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z && pPos->z + VtxMax->z >= g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z)
				{//手前から来た時
					pPos->z = g_aModel[nCntModel].VtxMinModel.z + g_aModel[nCntModel].pos.z - VtxMax->z;
					bLand = true;

				}
				else if (pPosold->z - VtxMax->z >= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z && pPos->z - VtxMax->z <= g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z)
				{//奥から来た時
					pPos->z = g_aModel[nCntModel].VtxMaxModel.z + g_aModel[nCntModel].pos.z + VtxMax->z;
					bLand = true;

				}

				else if (pPosold->y - VtxMax->y >= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y && pPos->y - VtxMax->y <= g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y)
				{//上から来た時
					pMove->y = 0.0f;

					pPos->y = g_aModel[nCntModel].VtxMaxModel.y + g_aModel[nCntModel].pos.y + VtxMax->y;
					bLand = true;

				}
				else if (pPosold->y + VtxMax->y <= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y && pPos->y + VtxMax->y >= g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y)
				{//下から来た時
					pPos->y = g_aModel[nCntModel].VtxMinModel.y + g_aModel[nCntModel].pos.y + VtxMin->y;


					bLand = true;

				}

			}
		}
	}
	return bLand;

}


void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	BYTE *pVtxBuff;	//頂点バッファのポインタ

					//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)	//ブロックが使用されていない
		{
			g_aModel[nCntModel].pos = pos;

			g_aModel[nCntModel].rot = rot;

			g_aModel[nCntModel].nType = nType;

			//影の設定
			g_aModel[nCntModel].g_nIndexShadow = SetShadow(g_aModel[nCntModel].pos, g_aModel[nCntModel].rot, 60.0f, 60.0f);

			g_aModel[nCntModel].bUse = true;	//使用している状態にする

			break;
		}

	}
	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();
}





MODEL *GetModel(void)
{
	return &g_aModel[0];
}