//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: プレイヤー、エネミーにくっつけている当たり判定
// Author : KishidaRei 
//
//=============================================================================
#include "SencerCol.h"
#include "input.h"
#include "camera.h"
#include "particle.h"
#include "enemy.h"
#include "enemy0.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy4.h"
#include "enemy5.h"
#include "enemy6.h"
#include "enemy7.h"
#include "enemy8.h"

#include "enemy_arrow.h"
#include "player.h"
#include "life.h"
#include "boss.h"
#include "sound.h"
#include "enemy_axe.h"
#include "enemy_axe2.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCERCOLLSION (50)
#define MAX_TEXTURE (2)

#define SENCERCOLLISION_VERTEX (  (g_SencerCollision_h + 1) * (g_SencerCollision_v + 1)   )	//頂点数
#define SENCERCOLLISION_INDEX (   (g_SencerCollision_h * g_SencerCollision_v )* 2 + (4 * (g_SencerCollision_v - 1)) + 2   ) //インデックス数
#define SENCERCOLLISION_POLYGON ( (g_SencerCollision_h * g_SencerCollision_v) * 2 + (4 * (g_SencerCollision_v - 1)) )	//ポリゴン数

#define SENCERCOL_RADIUS (35.0f)
#define SENCERCOL_HEIGHT (80.0f)

#define MESH_H (20) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencerCollision(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencerCollision(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencerCollision = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencerCollision = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencerCollision = NULL; //インデックスバッファへのポインタ

SENCERCOLLISION g_aSencerCollision[MAX_SENCERCOLLSION];

int g_SencerCollision_h, g_SencerCollision_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencerCollision(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SencerCollision_h = MESH_H;
	g_SencerCollision_v = MESH_V;


	MakeVertexSencerCollision(pDevice);
	MakeIndexSencerCollision(pDevice);

	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{
		g_aSencerCollision[nCntSencer].fRadius = SENCERCOL_RADIUS;
		g_aSencerCollision[nCntSencer].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencerCollision(void)
{

	//テクスチャの破棄
	if (g_pTextureSencerCollision != NULL)
	{
		g_pTextureSencerCollision->Release();
		g_pTextureSencerCollision = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffSencerCollision != NULL)
	{
		g_pVtxBuffSencerCollision->Release();
		g_pVtxBuffSencerCollision = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencerCollision != NULL)
	{
		g_pIdxBuffSencerCollision->Release();
		g_pIdxBuffSencerCollision = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencerCollision(void)
{
	
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencerCollision(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;


	
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング

	

	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{
		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencerCollision[nCntSencer].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencerCollision[nCntSencer].rot.y, g_aSencerCollision[nCntSencer].rot.x, g_aSencerCollision[nCntSencer].rot.z);

			D3DXMatrixMultiply(&g_aSencerCollision[nCntSencer].mtxWorld, &g_aSencerCollision[nCntSencer].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencerCollision[nCntSencer].pos.x, g_aSencerCollision[nCntSencer].pos.y, g_aSencerCollision[nCntSencer].pos.z);

			D3DXMatrixMultiply(&g_aSencerCollision[nCntSencer].mtxWorld, &g_aSencerCollision[nCntSencer].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencerCollision[nCntSencer].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencerCollision, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencerCollision);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCERCOLLISION_VERTEX, 0, SENCERCOLLISION_POLYGON);

		}
	}


#endif
}

void SetPositionSencerCollision(int nIdxSencer, D3DXVECTOR3 pos)
{	
		g_aSencerCollision[nIdxSencer].pos = pos;
}

void DeleteSencerCol(int nIdxSencerCol)
{
	g_aSencerCollision[nIdxSencerCol].bUse = false;
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencerCollision(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCERCOLLISION_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencerCollision,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencerCollision->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_SencerCollision_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_SencerCollision_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_SencerCollision_h + 1; nCntLife_H++)
		{

			if (fDestAngle >= D3DX_PI)
			{
				fDestAngle -= D3DX_PI * 2;
			}
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCERCOL_RADIUS;
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].pos.y = SENCERCOL_HEIGHT * nCntLife_V;
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCERCOL_RADIUS;
			//法線の設定
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].col = D3DXCOLOR(1.0f, 0.0, 0.0f, 0.3f);

			//テクスチャ座標
			pVtx[nCntLife_V * (g_SencerCollision_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_SencerCollision_h / 2)), nCntLife_V * -1.0f);

			fDestAngle += fAngle;
		}


	}

	//頂点バッファをアンロック
	g_pVtxBuffSencerCollision->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencerCollision(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCERCOLLISION_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencerCollision,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencerCollision->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_SencerCollision_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_SencerCollision_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_SencerCollision_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_SencerCollision_h && nCntIndex_V <= g_SencerCollision_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_SencerCollision_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencerCollision->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencerCollision(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SENCERCOLTYPE type)
{
	int nCntSencer;
	// 頂点情報の設定

	for ( nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{
		if (g_aSencerCollision[nCntSencer].bUse == false)
		{
			g_aSencerCollision[nCntSencer].pos = pos;
			g_aSencerCollision[nCntSencer].rot = rot;
			g_aSencerCollision[nCntSencer].type = type;
			g_aSencerCollision[nCntSencer].bUse = true;

			break;
		}
	}

	return nCntSencer;
}

//=============================================================================
// センサー判定
//=============================================================================
bool CollisionSencer_Col(D3DXVECTOR3 *pos, D3DXVECTOR3 *posold)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{
		

		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY || g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_ARROW
					|| g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
				{
					bCol = true;
					pos->x = posold->x;
					pos->z = posold->z;

				}
			}

		}
	}
#if  0
#endif
	return bCol;
}

//=============================================================================
// 魔法との当たり判定(通常時)
//=============================================================================
bool CollisionSencer_Magic(D3DXVECTOR3 * pos, int nCntEnemy)
{
	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{


		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY)
				{
					bCol = true;
					HitEnemy(-10, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}


				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY0)
				{
					bCol = true;
					HitEnemy_0(-10, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY1)
				{
					bCol = true;
					HitEnemy_1(-10, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY2)
				{
					bCol = true;
					HitEnemy_2(-10, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}


				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
				{
					bCol = true;
					HitEnemyAxe(-12, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE2)
				{
					bCol = true;
					HitEnemyAxe2(-12, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_ARROW)
				{
					bCol = true;
					HitEnemyArrow(-12, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_BOSS)
				{
					bCol = true;
					HitEnemy(-3, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
			}

		}
	}
	return bCol;
}

//=============================================================================
// 魔法との当たり判定(瘴気)
//=============================================================================
bool CollisionSencer_L_Magic(D3DXVECTOR3 * pos, int nCntEnemy)
{
	PLAYER *pPlayer = GetPlayer();


	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{


		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY)
				{
					bCol = true;
					HitEnemyAxe(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}

				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY0)
				{
					bCol = true;
					HitEnemy_0(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY1)
				{
					bCol = true;
					HitEnemy_1(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY2)
				{
					bCol = true;
					HitEnemy_2(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
				{
					bCol = true;
					HitEnemyAxe(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE2)
				{
					bCol = true;
					HitEnemyAxe2(-pPlayer->nCntCharge, nCntEnemy);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
			}

		}
	}
	return bCol;
}


//=============================================================================
// 魔法との当たり判定(bossとの当たり判定)(通常時)
//=============================================================================
bool CollisionSencer_Magic_Boss(D3DXVECTOR3 * pos)
{
	BOSS *pBoss;
	pBoss = GetBoss();

	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{


		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_BOSS)
				{
					bCol = true;
					HitBoss(-3);
					

				}
			}

		}
	}
	return bCol;
}

//=============================================================================
// 魔法との当たり判定(bossとの当たり判定)(瘴撃時)
//=============================================================================
bool CollisionSencer_L_Magic_Boss(D3DXVECTOR3 * pos)
{
	PLAYER *pPlayer = GetPlayer();

	BOSS *pBoss;
	pBoss = GetBoss();

	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{


		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{

				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_BOSS)
				{
					bCol = true;
					HitBoss(-pPlayer->nCntCharge);
					PlaySound(SOUND_LABEL_SE_EXPLOSION);

				}
			}

		}
	}
	return bCol;
}


//=============================================================================
// センサー判定(Player->Enemyへの攻撃判定)(通常時)
//=============================================================================
bool CollisionAttackPlayer(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemy)
{
	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0 ;

	
	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY)
			{
				bCol = true;
				HitEnemy(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY0)
			{
				bCol = true;
				HitEnemy_0(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY1)
			{
				bCol = true;
				HitEnemy_1(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY2)
			{
				bCol = true;
				HitEnemy_2(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY3)
			{
				bCol = true;
				HitEnemy_3(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY4)
			{
				bCol = true;
				HitEnemy_4(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY5)
			{
				bCol = true;
				HitEnemy_5(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY6)
			{
				bCol = true;
				HitEnemy_6(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
			{
				bCol = true;
				HitEnemyAxe(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE2)
			{
				bCol = true;
				HitEnemyAxe2(-1, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}
	

	return bCol;
}
//=============================================================================
// センサー判定(Player->Enemyへの攻撃判定)(弓)(通常時)
//=============================================================================
bool CollisionAttackPvA(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyArrow)
{
	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_ARROW)
			{
				bCol = true;
				HitEnemyArrow(-1, nCntEnemyArrow);
				PlaySound(SOUND_LABEL_SE_HIT);

			}


		}
	}


	return bCol;
}
//=============================================================================
// センサー判定(Player->Enemyへの攻撃判定)(斧)(通常時)
//=============================================================================
bool CollisionAttackAxe(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyAxe)
{
	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
			{
				bCol = true;
				HitEnemyArrow(-1, nCntEnemyAxe);
				PlaySound(SOUND_LABEL_SE_HIT);

			}

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE2)
			{
				bCol = true;
				HitEnemyAxe2(-1, nCntEnemyAxe);
				PlaySound(SOUND_LABEL_SE_HIT);

			}
		}
	}


	return bCol;
}


//=============================================================================
// センサー判定(Player->Enemyへの攻撃判定)(瘴撃)
//=============================================================================
bool Collision_L_AttackPlayer(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemy)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY)
			{
				bCol = true;
				HitEnemy(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);

			}

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY0)
			{
				bCol = true;
				HitEnemy_0(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY1)
			{
				bCol = true;
				HitEnemy_1(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY2)
			{
				bCol = true;
				HitEnemy_2(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY3)
			{
				bCol = true;
				HitEnemy_3(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY4)
			{
				bCol = true;
				HitEnemy_4(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY5)
			{
				bCol = true;
				HitEnemy_5(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY6)
			{
				bCol = true;
				HitEnemy_6(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
			{
				bCol = true;
				HitEnemy(-pPlayer->nCntCharge, nCntEnemy);
				PlaySound(SOUND_LABEL_SE_HIT2);

			}
		}
	}


	return bCol;
}


//=============================================================================
// センサー判定(Player->bossへの攻撃判定)(通常時)
//=============================================================================
bool CollisionAttackPvB(D3DXVECTOR3 pos, float fRadius, int nCntSencer)
{
	BOSS *pBoss;
	pBoss = GetBoss();


	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_BOSS)
			{
				bCol = true;
				HitBoss(-2);
				PlaySound(SOUND_LABEL_SE_HIT);

			}

		}
	}


	return bCol;
}


//=============================================================================
// センサー判定(Player->bossへの攻撃判定)(瘴撃時)
//=============================================================================
bool Collision_L_AttackPvB(D3DXVECTOR3 pos, float fRadius, int nCntSencer)
{
	BOSS *pBoss;
	pBoss = GetBoss();

	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_BOSS)
			{
				bCol = true;
				HitBoss(-pPlayer->nCntCharge);
				PlaySound(SOUND_LABEL_SE_HIT2);
			}

		}
	}


	return bCol;
}

bool Collision_L_AttackPvA(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyArrow)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_ARROW)
			{
				bCol = true;
				HitEnemyArrow(-pPlayer->nCntCharge, nCntEnemyArrow);
				PlaySound(SOUND_LABEL_SE_HIT2);

			}

		}
	}


	return bCol;
}

bool Collision_L_AttackAxe(D3DXVECTOR3 pos, float fRadius, int nCntSencer, int nCntEnemyAxe)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE)
			{
				bCol = true;
				HitEnemyArrow(-pPlayer->nCntCharge, nCntEnemyAxe);
				PlaySound(SOUND_LABEL_SE_HIT2);

			}
			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_ENEMY_AXE2)
			{
				bCol = true;
				HitEnemyAxe2(-pPlayer->nCntCharge, nCntEnemyAxe);
				PlaySound(SOUND_LABEL_SE_HIT2);

			}
		}
	}


	return bCol;
}


//=============================================================================
// センサー判定(Enemy->Playerへの攻撃判定)
//=============================================================================
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float fRadius, int nCntSencer, SENCERCOLTYPE Type)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y);
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_PLAYER && pPlayer->status == PLAYERSTATUS_NORMAL)
			{
				bCol = true;
				if (Type == SENCERCOLTYPE_ENEMY)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}
				if (Type == SENCERCOLTYPE_ENEMY0)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}
				if (Type == SENCERCOLTYPE_ENEMY1)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}if (Type == SENCERCOLTYPE_ENEMY2)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}if (Type == SENCERCOLTYPE_ENEMY3)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}if (Type == SENCERCOLTYPE_ENEMY4)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}if (Type == SENCERCOLTYPE_ENEMY5)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}
				if (Type == SENCERCOLTYPE_ENEMY6)
				{
					HitDamage(-5);
					PlaySound(SOUND_LABEL_SE_HIT);

				}
				if (Type == SENCERCOLTYPE_ENEMY_AXE)
				{
						HitDamage(-10);
						PlaySound(SOUND_LABEL_SE_HIT2);

				}
				if (Type == SENCERCOLTYPE_ENEMY_AXE2)
				{
					HitDamage(-10);
					PlaySound(SOUND_LABEL_SE_HIT2);

				}
				if (Type == SENCERCOLTYPE_ENEMY_BOSS)
				{
					HitDamage(-15);
					PlaySound(SOUND_LABEL_SE_HIT3);

				}
			}

		}
	}


	return bCol;
}

//=============================================================================
// センサー判定(Enemy->Playerへの攻撃判定)  (弓)
//=============================================================================
bool CollisionAttackEnemyArrow(D3DXVECTOR3 *pos)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCERCOLLSION; nCntSencer++)
	{


		fxpos = (g_aSencerCollision[nCntSencer].pos.x - pos->x) * (g_aSencerCollision[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencerCollision[nCntSencer].pos.z - pos->z) * (g_aSencerCollision[nCntSencer].pos.z - pos->z);
		fRadius = SENCERCOL_RADIUS * SENCERCOL_RADIUS;


		if (g_aSencerCollision[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_PLAYER&& pPlayer->status == PLAYERSTATUS_NORMAL)
				{
					bCol = true;
					HitDamage(-10);
					PlaySound(SOUND_LABEL_SE_HIT3);


				}
			}

		}
	}
	return bCol;
}

bool CollisionAttackBoss(D3DXVECTOR3 pos, float fRadius, int nCntSencer, SENCERCOLTYPE Type)
{
	PLAYER *pPlayer = GetPlayer();

	bool bCol = false;

	float fxpos = 0.0f;
	float fypos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fLength = 0.0f;
	float fRadius0;


	if (g_aSencerCollision[nCntSencer].bUse == true)
	{

		fxpos = (pos.x - g_aSencerCollision[nCntSencer].pos.x) * (pos.x - g_aSencerCollision[nCntSencer].pos.x);
		fypos = (pos.y - g_aSencerCollision[nCntSencer].pos.y) * (pos.y - g_aSencerCollision[nCntSencer].pos.y)/2;
		fzpos = (pos.z - g_aSencerCollision[nCntSencer].pos.z) * (pos.z - g_aSencerCollision[nCntSencer].pos.z);
		fLength = fxpos + fypos + fzpos;
		fRadius0 = (SENCERCOL_RADIUS + fRadius) * (SENCERCOL_RADIUS + fRadius);

		if (fLength < fRadius0)
		{

			if (g_aSencerCollision[nCntSencer].type == SENCERCOLTYPE_PLAYER && pPlayer->status == PLAYERSTATUS_NORMAL)
			{
				bCol = true;
				
				
					HitDamage(-30);
				
			}

		}
	}


	return bCol;
}

