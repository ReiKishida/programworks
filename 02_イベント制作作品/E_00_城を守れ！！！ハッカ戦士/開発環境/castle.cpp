//=============================================================================
//
// プレイヤー処理 [Castle.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "Castle.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "main.h"
#include "castle.h"
#include "enemy.h"
#include "life.h"
#include "explosion.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PATTERN (10)
#define POS_X (0.2f * (g_nPatternAnim % 5))
#define POS_Y (0.5f * (g_nPatternAnim / 5))
#define MOVE (1.2f)

#define HALF_X (180.0f)	//X軸半分
#define HALF_Y (180.0f)	//Y軸半分

#define LIMIT_MIN_X (45)	//画面端-X
#define LIMIT_MAX_X (1250)	//画面端X
#define LIMIT_MIN_Y (25)	//画面端-Y
#define LIMIT_MAX_Y (700)	//画面端Y




//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pcastlePolygon = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxCastlePolygon = NULL;	//頂点バッファへのポインタ
CASTLE g_Castle;	//プレイヤー
//=============================================================================
// プレイヤー生成処理
//=============================================================================
void InitCastle(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

								//デバイスの取得
	pDevice = GetDevice();

	g_Castle.pos = D3DXVECTOR3(150, 410, 0.0f);//プレイヤー中心座標
	g_Castle.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
	g_Castle.nStock = 4;	//ストック
	g_Castle.nLife = 5;		//ライフ


	g_Castle.state = CASTLESTATE_NORMAL;
	g_Castle.bDisp = true;



	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/castle.png", &g_pcastlePolygon);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxCastlePolygon,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxCastlePolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標

	pVtx[0].pos = D3DXVECTOR3(g_Castle.pos.x - HALF_X, g_Castle.pos.y - HALF_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Castle.pos.x + HALF_X, g_Castle.pos.y - HALF_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Castle.pos.x - HALF_X, g_Castle.pos.y + HALF_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Castle.pos.x + HALF_X, g_Castle.pos.y + HALF_Y, 0.0f);


	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxCastlePolygon->Unlock();
}
//=============================================================================
// プレイヤー消去処理
//=============================================================================
void UnInitCastle(void)
{
	//テクスチャの破棄
	if (g_pcastlePolygon != NULL)
	{
		g_pcastlePolygon->Release();
		g_pcastlePolygon = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxCastlePolygon != NULL)
	{
		g_pVtxCastlePolygon->Release();
		g_pVtxCastlePolygon = NULL;
	}
}
//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdateCastle(void)
{
	int nReload = 0;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	MODE *pMode;
	pMode = GetMode();

	switch (g_Castle.state)
	{
	case CASTLESTATE_NORMAL:

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxCastlePolygon->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点バッファをアンロックする
		g_pVtxCastlePolygon->Unlock();
		break;

	case CASTLESTATE_DAMAGE:
		g_Castle.nCounterState--;
		if (g_Castle.nCounterState <= 0)
		{
			g_Castle.state = CASTLESTATE_NORMAL;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxCastlePolygon->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロックする
			g_pVtxCastlePolygon->Unlock();

			g_Castle.nCounterState = 0;
		}
		break;

	}

		ENEMY *pEnemy;
		pEnemy = GetEnemy();



		if (g_Castle.state == CASTLESTATE_NORMAL)
		{
			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)	//敵が使用している
				{
					if (g_Castle.pos.x > pEnemy->pos.x - 200
						&& g_Castle.pos.x < pEnemy->pos.x + 200
						&& g_Castle.pos.y >  pEnemy->pos.y - 200
						&& g_Castle.pos.y < pEnemy->pos.y + 200)
					{
						
							HitCastle(1);
							pEnemy->bUse = false;
					}
				}
			}
		}
	
}
	

	
	



//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawCastle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

							  //デバイスを取得する
	pDevice = GetDevice();

	if (g_Castle.bDisp == true)
	{

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxCastlePolygon, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャ
		pDevice->SetTexture(0, g_pcastlePolygon);


		//プレイヤー描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}


void HitCastle(int nDamage)
{
	LIFE *pLife;
	pLife = GetLife();


	PlaySound(SOUND_LABEL_SE_HIT000);
	g_Castle.nLife -= nDamage;
	AddLife(g_Castle.nLife);
	if (g_Castle.nLife <= 0)
	{
		g_Castle.bDisp = false;
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		SetExplosion(g_Castle.pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		g_Castle.nCounterState = 100;
		g_Castle.state = CASTLESTATE_DEATH;
		SetGameState(GAMESTATE_FAILED);
		
	}

	

	else
	{

		g_Castle.state = CASTLESTATE_DAMAGE;

		VERTEX_2D *pVtx;
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxCastlePolygon->Lock(0, 0, (void**)&pVtx, 0);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		// 頂点バッファをアンロックする
		g_pVtxCastlePolygon->Unlock();

		g_Castle.nCounterState = 15;
	}

}


//=============================================================================
// プレイヤーの取得
//=============================================================================
//Castle *GetCastle(void)
//{
//	return &g_Castle;
//}


