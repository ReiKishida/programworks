//=============================================================================
//
// 矢処理 [arrow.cpp]
// Author : KishidaRei
//
//=============================================================================
#include "arrow.h"
#include "player.h"
#include "particle.h"
#include "camera.h"
#include "SencerCol.h"
#include "Wall1.h"
#include "Wall2.h"
#include "WallBoss.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ARROW (50)
#define MODEL_ARROW			"data/MODEL/arrow.x"
#define ARROW_MOVR		(1.0f)

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXMESH			g_pMeshArrow = NULL;				//メッシュ情報（頂点情報）のポインタ
LPD3DXBUFFER		g_pBuffMatArrow = NULL;				//マテリアル情報へのポインタ
DWORD				g_nNumMatArrow = 0;					//マテリアル情報の数
LPDIRECT3DTEXTURE9	g_pTextureArrow = NULL;				//テクスチャへのポインタ
ARROW				g_Arrow[MAX_ARROW];							//モデルの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;						//頂点数
	DWORD sizeFVF;						//頂点フォーマットのサイズ
	BYTE *pVtxBuff;						//頂点バッファのポインタ

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		g_Arrow[nCntArrow].vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
		g_Arrow[nCntArrow].vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
		// 位置・向き・色の初期設定
		
		g_Arrow[nCntArrow].bUse = false;
	}
		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODEL_ARROW,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatArrow,
			NULL,
			&g_nNumMatArrow,
			&g_pMeshArrow);


		for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
		{
			//頂点数の取得
			nNumVtx = g_pMeshArrow->GetNumVertices();
			sizeFVF = D3DXGetFVFVertexSize(g_pMeshArrow->GetFVF());

			//頂点バッファのロック
			g_pMeshArrow->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
			{//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (g_Arrow[0].vtxMin.x > vtx.x)
				{//Xの最小値の書き換え
					g_Arrow[0].vtxMin.x = vtx.x;
				}
				if (g_Arrow[0].vtxMin.z > vtx.z)
				{//Xの最小値の書き換え
					g_Arrow[0].vtxMin.z = vtx.z;
				}
				if (g_Arrow[0].vtxMax.x < vtx.x)
				{//Xの最小値の書き換え
					g_Arrow[0].vtxMax.x = vtx.x;
				}
				if (g_Arrow[0].vtxMax.z < vtx.z)
				{//Xの最小値の書き換え
					g_Arrow[0].vtxMax.z = vtx.z;
				}

				pVtxBuff += sizeFVF;//サイズ分ポインタを進める
			}
			//頂点バッファのアンロック
			g_pMeshArrow->UnlockVertexBuffer();


		}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitArrow(void)
{
	// メッシュの開放
	if (g_pMeshArrow != NULL)
	{
		g_pMeshArrow->Release();
		g_pMeshArrow = NULL;
	}
	// マテリアルの開放
	if (g_pBuffMatArrow != NULL)
	{
		g_pBuffMatArrow->Release();
		g_pBuffMatArrow = NULL;
	}

	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateArrow(void)
{
	CAMERA pCamera;
	pCamera = *GetCamera();
	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();
	STAGEMODE g_Stage = GetStageMode();				//現在のステージ番号を取得


	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{//弾を移動させる
		if (g_Arrow[nCntArrow].bUse == true)
		{
			g_Arrow[nCntArrow].posold = g_Arrow[nCntArrow].pos;
			g_Arrow[nCntArrow].nLife--;

			/*static float fAngle;
			fAngle = atan2f(g_Arrow[nCntArrow].pos.x - pPlayer->pos.x, g_Arrow[nCntArrow].pos.z - pPlayer->pos.z);*/


			if (CollisionAttackEnemyArrow(&g_Arrow[nCntArrow].pos) == true)
			{
				g_Arrow[nCntArrow].bUse = false;

			}
			//g_Arrow[nCntArrow].rot.y = fAngle;

			SetParticle(g_Arrow[nCntArrow].pos, 20, D3DXCOLOR(1.0f, 0.0f, 0.9f, 0.6f), PARTICLETYPE_BULLET);
			
			if (g_Arrow[nCntArrow].nLife < 0)
			{
				g_Arrow[nCntArrow].bUse = false;
			}

			g_Arrow[nCntArrow].pos += g_Arrow[nCntArrow].move;




			if (g_Stage == STAGEMODE_STAGE1)
			{//現在のステージがステージ1
			 //奥壁のあたり判定
				CollisionWall1_WIDTHBack_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//手前壁のあたり判定
				CollisionWall1_WIDTHThisSide_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//右壁のあたり判定
				CollisionWall1_RIGHT_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//右壁のあたり判定
				CollisionWall1_LEFT_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE2)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				//CollisionWall2_WIDTHBack_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				////手前壁のあたり判定
				//CollisionWall2_WIDTHThisSide_Arrow(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				////右壁のあたり判定
				//CollisionWall2_RIGHT_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				////右壁のあたり判定
				//CollisionWall2_LEFT_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);
			}
			else if (g_Stage == STAGEMODE_STAGE3)
			{//現在のステージがステージ2
			 //奥壁のあたり判定
				CollisionWallBoss_WIDTHBack_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//手前壁のあたり判定
				CollisionWallBoss_WIDTHThisSide_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//右壁のあたり判定
				CollisionWallBoss_RIGHT_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);

				//右壁のあたり判定
				CollisionWallBoss_LEFT_Ene(&g_Arrow[nCntArrow].pos, &g_Arrow[nCntArrow].posold);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawArrow(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;						//マテリアルデータへのポインタ

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_Arrow[nCntArrow].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Arrow[nCntArrow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Arrow[nCntArrow].rot.y, g_Arrow[nCntArrow].rot.x, g_Arrow[nCntArrow].rot.z);
			D3DXMatrixMultiply(&g_Arrow[nCntArrow].mtxWorld, &g_Arrow[nCntArrow].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_Arrow[nCntArrow].pos.x, g_Arrow[nCntArrow].pos.y, g_Arrow[nCntArrow].pos.z);
			D3DXMatrixMultiply(&g_Arrow[nCntArrow].mtxWorld, &g_Arrow[nCntArrow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Arrow[nCntArrow].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatArrow->GetBufferPointer();

			for (int nCntMat = 0; nCntMat <= (int)g_nNumMatArrow; nCntMat++)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTextureArrow);

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// モデル(パーツ)の描画
				g_pMeshArrow->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
#endif
}
//=============================================================================
// モデルの取得
//=============================================================================
ARROW *GetArrow(void)
{
	return &g_Arrow[0];
}

void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nLife)
{
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	//頂点バッファのロック
	g_pMeshArrow->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_Arrow[nCntArrow].bUse == false)
		{
			g_Arrow[nCntArrow].pos = pos;
			g_Arrow[nCntArrow].rot = rot;
			g_Arrow[nCntArrow].move = move;
			g_Arrow[nCntArrow].nLife = nLife;
			g_Arrow[nCntArrow].bUse = true;

			break;
		}
	}
	//頂点バッファをアンロック
	g_pMeshArrow->UnlockVertexBuffer();
}
