////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// モデル処理 [model.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "effect.h"	
#include "line.h"
#include "player.h"
#include "fade.h"
#include "game.h"
#include "light.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_SPEED	(1.0f)	// 移動量
#define DOWN_SPEED	(0.15f)	// 減速量
#define MAX_HEIGHT	(100)	// 上の上限
#define MIN_HEIGHT	(-50)	// 下の上限

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Model g_aModel[MAX_MODEL];
ModelData g_aModelData[MAX_MODEL_TYPE];
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitModel(void)
{
	// 位置・向きの初期設定
	for (int nCntMat = 0; nCntMat < MAX_MODEL; nCntMat++)
	{
		g_aModel[nCntMat].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aModel[nCntMat].nType = MODELTYPE_TREE0;
		g_aModel[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntMat].bCollision = true;
		g_aModel[nCntMat].nIdy = 0;
		for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
		{
			g_aModel[nCntMat].nIdyLine[nCntCollision] = 0;
		}
		g_aModel[nCntMat].bUse = false;
	}	

	DeleteLine(LINETYPE_MODEL);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitModel(void)
{
	for (int nCntMat = 0; nCntMat < g_aModelData[0].nNumModel; nCntMat++)
	{
		// メッシュの開放
		if (g_aModelData[nCntMat].pMesh != NULL)
		{
			g_aModelData[nCntMat].pMesh->Release();
			g_aModelData[nCntMat].pMesh = NULL;
		}
		// マテリアルの開放
		if (g_aModelData[nCntMat].pBuffMat != NULL)
		{
			g_aModelData[nCntMat].pBuffMat->Release();
			g_aModelData[nCntMat].pBuffMat = NULL;
		}
		// テクスチャの開放
		for (int nCntMatTex = 0; nCntMatTex < int(g_aModelData[nCntMat].nNumMat); nCntMatTex++)
		{
			if (g_aModelData[nCntMat].pTextuer[nCntMatTex] != NULL)
			{
				g_aModelData[nCntMat].pTextuer[nCntMatTex]->Release();
				g_aModelData[nCntMat].pTextuer[nCntMatTex] = NULL;
			}
		}
	}
	for (int nCntMat = 0; nCntMat < MAX_MODEL; nCntMat++)
	{
		// マテリアルの開放
		if (g_aModel[nCntMat].pBuffMat != NULL)
		{
			g_aModel[nCntMat].pBuffMat->Release();
			g_aModel[nCntMat].pBuffMat = NULL;
		}
	}
}
//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			if (g_aModel[nCntModel].nType == MODELTYPE_HUNSUI)
			{
				// エフェクト
				for (int nCntEffect = 0; nCntEffect < 5; nCntEffect++)
				{
					float fRandRadius = float(rand() % 60 + 30);
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aModel[nCntModel].pos+ D3DXVECTOR3(0.0f,400 + float(rand()%80),0.0f), D3DXVECTOR3(sinf(nDirection) * float(rand()%8), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) * float(rand() % 8)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_WATER);
					SetEffect(g_aModel[nCntModel].pos + D3DXVECTOR3(0.0f, 400 + float(rand() % 80), 0.0f), D3DXVECTOR3(sinf(nDirection2) * float(rand() % 8), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) * float(rand() % 8)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 20, 30, EFFECTTYPE_WATER);
					SetEffect(g_aModel[nCntModel].pos + D3DXVECTOR3(0.0f, 400 + float(rand() % 80), 0.0f), D3DXVECTOR3(sinf(nDirection2) * float(rand() % 5), cosf(nDirection2) * float(rand() % 5), cosf(nDirection) * float(rand() % 5)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_WATER);
					SetEffect(g_aModel[nCntModel].pos + D3DXVECTOR3(0.0f, 400 + float(rand() % 80), 0.0f), D3DXVECTOR3(sinf(nDirection2) * float(rand() % 4), sinf(nDirection) * float(rand() % 4), cosf(nDirection) * float(rand() % 4)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius, 30, EFFECTTYPE_WATER);
					SetEffect(g_aModel[nCntModel].pos + D3DXVECTOR3(0.0f, 400 + float(rand() % 80), 0.0f), D3DXVECTOR3(sinf(nDirection2) * float(rand() % 2), cosf(nDirection2) * 1, cosf(nDirection) * float(rand() % 2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_WATER);
					SetEffect(g_aModel[nCntModel].pos + D3DXVECTOR3(0.0f, 400 + float(rand() % 80), 0.0f), D3DXVECTOR3(sinf(nDirection2) * float(rand() % 2), sinf(nDirection) * 1, cosf(nDirection) * float(rand() % 2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(0.1f, float(rand() % 100 / 100.0f), 1.0f, 1.0f), fRandRadius + 30, 30, EFFECTTYPE_WATER);
				}
			}
		}
	}
}
//==============================================================================================================================
// モデルのあたり判定
//==============================================================================================================================
int CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 rot, float fRadius)
{
	int nCollision = 0;
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
			{
				if (g_aModelData[g_aModel[nCntModel].nType].bCollison[nCntCollision] == true)
				{

					// 敵と当たるものの距離
					float fPosX = (g_aModel[nCntModel].mtxWorld._41 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - pPos->x);
					float fPosY = (g_aModel[nCntModel].mtxWorld._42 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y- pPos->y);
					float fPosZ = (g_aModel[nCntModel].mtxWorld._43 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z- pPos->z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;
					fRadiusB = (g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + fRadius)* (g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + fRadius);

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{
						float fAngle2 = atan2f(pPos->x - (g_aModel[nCntModel].mtxWorld._41 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x) , pPos->z - (g_aModel[nCntModel].mtxWorld._43 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z ));
						pPos->x = g_aModel[nCntModel].mtxWorld._41 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + sinf(fAngle2)* (g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + fRadius/2);
						pPos->z = g_aModel[nCntModel].mtxWorld._43 + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + cosf(fAngle2)* (g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + fRadius/2);
						pMove->x = 0;
						pMove->z = 0;
						nCollision = 1;
						break;

					}
				}
			}
		}
	}
	return nCollision;
}

//==============================================================================================================================
// モデルのあたり判定
//==============================================================================================================================
int CollisionModelMinMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadiusWidth, float fRadiusHeight, Model **pModel)
{
	int nCollision = 0;
	*pModel = NULL;
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true && g_aModel[nCntModel].bCollision == true)
		{
			for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
			{
				if (g_aModelData[g_aModel[nCntModel].nType].bCollison[nCntCollision] == true)
				{
					if (g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y <= pPos->y + fRadiusHeight&&
						g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y + g_aModelData[g_aModel[nCntModel].nType].fHeightCollision[nCntCollision] >= pPos->y)
					{// Yが範囲内
						if (g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] -g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 +3< pPos->x + fRadiusWidth&&	// 左
							g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] +g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 -3> pPos->x - fRadiusWidth)		// 右
						{// Xが範囲内
							if (g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]-g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 < pPos->z + fRadiusWidth&&
								g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 >= pPosOld->z + fRadiusWidth)
							{// 前
								pPos->z = g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 - fRadiusWidth;
								nCollision = 1; // 当たった
								pMove->z = 0;
								*pModel = &g_aModel[nCntModel];
							}
							if (g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]+ g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 > pPos->z - fRadiusWidth&&
								g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]+ g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 <= pPosOld->z - fRadiusWidth)
							{// 奥
								pPos->z = g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 + fRadiusWidth;
								nCollision = 1; // 当たった
								pMove->z = 0;
								*pModel = &g_aModel[nCntModel];
							}
						}
						if (g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]- g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 <= pPos->z + fRadiusWidth&&	// 手前
							g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]+ g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 >= pPos->z - fRadiusWidth)		// 奥
						{// Zが範囲内
							if (g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 < pPos->x + fRadiusWidth&&
								g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 >= pPosOld->x + fRadiusWidth)
							{// 左
								pPos->x = g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 - fRadiusWidth;
								nCollision = 1; // 当たった
								*pModel = &g_aModel[nCntModel];

							}
							if (g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2> pPos->x - fRadiusWidth&&
								g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 <= pPosOld->x - fRadiusWidth)
							{// 右
								pPos->x = g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 + fRadiusWidth;
								nCollision = 1; // 当たった
								*pModel = &g_aModel[nCntModel];

							}
						}
					}
					if (g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 <= pPos->x + fRadiusWidth&&	// 左
						g_aModel[nCntModel].pos.x + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].x + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fWidthCollision[nCntCollision] / 2 >= pPos->x - fRadiusWidth&&		// 右
						g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z - g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision]- g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 <= pPos->z + fRadiusWidth&&	// 手前
						g_aModel[nCntModel].pos.z + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].z + g_aModelData[g_aModel[nCntModel].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntModel].nType].fDepthCollision[nCntCollision]/2 >= pPos->z - fRadiusWidth)		// 奥
					{// X Z が範囲内
						if (g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y < pPos->y + fRadiusHeight&&
							g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y  >= pPosOld->y + fRadiusHeight)
						{// 下
							pPos->y = g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y- fRadiusHeight;
							pMove->y = 0;
							//bCollision = true; // 当たった
							*pModel = &g_aModel[nCntModel];
						}
						if (g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y + g_aModelData[g_aModel[nCntModel].nType].fHeightCollision[nCntCollision] > pPos->y &&
							g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y + g_aModelData[g_aModel[nCntModel].nType].fHeightCollision[nCntCollision] <= pPosOld->y)
						{// 上
							pPos->y = g_aModel[nCntModel].pos.y + g_aModelData[g_aModel[nCntModel].nType].CollisionPos[nCntCollision].y + g_aModelData[g_aModel[nCntModel].nType].fHeightCollision[nCntCollision];
							pMove->y = 0;
							nCollision = 2; // 当たった
							*pModel = &g_aModel[nCntModel];
						}
					}
				}
			}
		}
	}
	return nCollision;
}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	// αテストを設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// α値が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// より大きい

	for (int nCntMat = 0; nCntMat < MAX_MODEL; nCntMat++)
	{
		if (g_aModel[nCntMat].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntMat].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntMat].rot.y, g_aModel[nCntMat].rot.x, g_aModel[nCntMat].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntMat].mtxWorld, &g_aModel[nCntMat].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntMat].pos.x, g_aModel[nCntMat].pos.y, g_aModel[nCntMat].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntMat].mtxWorld, &g_aModel[nCntMat].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntMat].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得

			pMat = (D3DXMATERIAL*)g_aModel[nCntMat].pBuffMat->GetBufferPointer();

			for (int nCntMat2 = 0; nCntMat2 < (int)g_aModelData[g_aModel[nCntMat].nType].nNumMat; nCntMat2++)
			{
				//
				pMat[nCntMat2].MatD3D.Diffuse.a = 1.0f;
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat2].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_aModelData[g_aModel[nCntMat].nType].pTextuer[nCntMat2]);			
				// モデル(パーツ)の描画
				g_aModelData[g_aModel[nCntMat].nType].pMesh->DrawSubset(nCntMat2);
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}		
		}	
	}
	// αテストを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストを無効化

}

//==============================================================================================================================
// モデルの設定
//==============================================================================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType)
{
	for (int nCntMat = 0; nCntMat < MAX_MODEL; nCntMat++)
	{
		if (g_aModel[nCntMat].bUse == false)
		{
			g_aModel[nCntMat].pos = pos;
			g_aModel[nCntMat].Initpos = pos;
			g_aModel[nCntMat].rot = rot;
			g_aModel[nCntMat].nType = nType;
			g_aModel[nCntMat].pBuffMat = g_aModelData[g_aModel[nCntMat].nType].pBuffMat;
			// 影の設定
			//g_aModel[nCntModel].nIdyShadow = SetShadow(D3DXVECTOR3(g_aModel[nCntModel].pos.x, 0.0f, g_aModel[nCntModel].pos.z), g_aModel[nCntModel].rot,
			//	g_aModelData[g_aModel[nCntModel].nType].vtxMax.x, g_aModelData[g_aModel[nCntModel].nType].vtxMax.z);
			g_aModel[nCntMat].nIdy = nCntMat;
			if (g_aModel[nCntMat].bCollision == true)
			{
				for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
				{
					if (g_aModelData[g_aModel[nCntMat].nType].bCollison[nCntCollision] == true)
					{
						g_aModel[nCntMat].nIdyLine[nCntCollision] = CollisionLine(g_aModel[nCntMat].pos + g_aModelData[g_aModel[nCntMat].nType].CollisionPos[nCntCollision],
							D3DXVECTOR3(-g_aModelData[g_aModel[nCntMat].nType].fRadiusCollision[nCntCollision] - g_aModelData[g_aModel[nCntMat].nType].fWidthCollision[nCntCollision]/2, 0.0f, -g_aModelData[g_aModel[nCntMat].nType].fRadiusCollision[nCntCollision]- g_aModelData[g_aModel[nCntMat].nType].fDepthCollision[nCntCollision]/2),
							D3DXVECTOR3(g_aModelData[g_aModel[nCntMat].nType].fRadiusCollision[nCntCollision] + g_aModelData[g_aModel[nCntMat].nType].fWidthCollision[nCntCollision]/2, g_aModelData[g_aModel[nCntMat].nType].fHeightCollision[nCntCollision], g_aModelData[g_aModel[nCntMat].nType].fRadiusCollision[nCntCollision]+ g_aModelData[g_aModel[nCntMat].nType].fDepthCollision[nCntCollision]/2),LINETYPE_MODEL);

					}
				}
			}
		
			g_aModel[nCntMat].bUse = true;
			break;
		}
	}
}
//==============================================================================================================================
// あたり判定の生成
//==============================================================================================================================
void SetCollisionModel(D3DXVECTOR3 pos, float fRadiusCollision, float fWidthCollision, float fDepthCollision,float fHeightCollision, MODELTYPE nType)
{
	for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
	{
		if (g_aModelData[nType].bCollison[nCntCollision] == false)
		{
			g_aModelData[nType].CollisionPos[nCntCollision] = pos;
			g_aModelData[nType].fRadiusCollision[nCntCollision] = fRadiusCollision;
			g_aModelData[nType].fWidthCollision[nCntCollision] = fWidthCollision;
			g_aModelData[nType].fDepthCollision[nCntCollision] = fDepthCollision;
			g_aModelData[nType].fHeightCollision[nCntCollision] = fHeightCollision;
			g_aModelData[nType].bCollison[nCntCollision] = true;
			break;
		}
	}

}
//==============================================================================================================================
// モデルの取得
//==============================================================================================================================
Model *GetModel(void)
{
	return &g_aModel[0];
}
ModelData *GetModelData(void)
{
	return &g_aModelData[0];
}
