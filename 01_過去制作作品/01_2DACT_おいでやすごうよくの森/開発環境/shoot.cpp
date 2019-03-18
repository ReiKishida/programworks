//=============================================================================
//
// 発射位置処理 [shoot.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "shoot.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "main.h"
#include "item.h"
#include "timer.h"

#include <stdlib.h> 
#include <time.h>   

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_SHOOT		(1)								// 発射位置の種類
#define BOSS_TEXTURENAME_0	"data/TEXTURE/zoku.png"		//発射位置ののテクスチャ1


#define SHOOT_X (100)
#define SHOOT_Y (100)



#define SHOOT_POWER (-20.5f)	//ジャンプ力


//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureShoot = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShoot = NULL;					// 頂点バッファへのポインタ

SHOOT g_aShoot;

int nDirection;


//int g_nNumBoss;	//シューターの総数
//=============================================================================
// 初期化処理
//=============================================================================
void InitShoot(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//int nCntBoss;	//シューターのカウント

	// デバイスの取得
	pDevice = GetDevice();

	// シューターの情報の初期化

	g_aShoot.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aShoot.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aShoot.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_aShoot.type = SHOOTTYPE_COIN;
	g_aShoot.bUse = false;
	g_aShoot.direction = SHOOTDIRECTION_0;
	g_aShoot.fHeight = 110.0f;
	g_aShoot.fWidth = 110.0f;
	//g_aShoot.fLength = 0.0f;

	nDirection = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURENAME_0, &g_apTextureShoot);
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShoot,
		NULL);


	g_aShoot.fLength = sqrtf((g_aShoot.fWidth / 2+ g_aShoot.fWidth / 2) * (g_aShoot.fHeight / 2 + g_aShoot.fHeight / 2));
	g_aShoot.fAngle =atan2f(g_aShoot.fWidth /2, g_aShoot.fHeight / 2);
	//g_aShoot.fAngle = D3DX_PI / 4;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							  g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);



	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	// 頂点バッファをアンロックする
	g_pVtxBuffShoot->Unlock();

	//g_nNumBoss = 0; //シューターの総数のクリア

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitShoot(void)
{
	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_SHOOT; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureShoot != NULL)
		{
			g_apTextureShoot->Release();
			g_apTextureShoot = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffShoot != NULL)
	{
		g_pVtxBuffShoot->Release();
		g_pVtxBuffShoot = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShoot(void)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	D3DXVECTOR3 Shootpos;
	D3DXVECTOR3 Shootmove;

	PLAYER pPlayer;
	pPlayer = GetPlayer();

	TIMER pTimer;
	pTimer = GetTimer();

	float Scroll_0;	//上へ(-)
	float Scroll_1;	//下へ(+)

	Scroll_0 = 35.0f;
	Scroll_1 = 35.0f;


	PAUSECHECK2 pScroll;
	pScroll = GetPauseCheck2();

	//g_aShoot.rot.x += 0.01f;
	//g_aShoot.rot.y += 0.01f;

	if (rand() % 50 == 0)
	{
		g_aShoot.direction = SHOOTDIRECTION_0;
	}

	if (rand() % 50 == 0)
	{
		g_aShoot.direction = SHOOTDIRECTION_1;

	}

	switch (g_aShoot.direction)
	{
	case SHOOTDIRECTION_0:
		Shootpos.x = g_aShoot.pos.x + SHOOT_X ;
		Shootpos.y = g_aShoot.pos.y - 10;
		Shootmove.x = (rand() % 11 - + 0.01f) ;
		Shootmove.y = SHOOT_POWER;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		// 頂点バッファをアンロックする
		g_pVtxBuffShoot->Unlock();


		if (pTimer.nTime <= 10)
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 10000 == 0)
				{

					//SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 2 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 15 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 20 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		else
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 60 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 3 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 25 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 150 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		break;

	case SHOOTDIRECTION_1:
		Shootpos.x = g_aShoot.pos.x - SHOOT_X + 5;
		Shootpos.y = g_aShoot.pos.y - 10;
		Shootmove.x = (rand() % 11 + 0.01f)  * -1;
		Shootmove.y = SHOOT_POWER;
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点バッファをアンロックする
		g_pVtxBuffShoot->Unlock();

		

		if (pTimer.nTime <= 10)
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 10000 == 0)
				{

					//SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 2 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 15 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 20 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		else
		{
			if (g_aShoot.bUse == true)
			{
				if (rand() % 60 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_AREY, D3DXVECTOR3(Shootmove.x + 5.0f, Shootmove.y, 0.0f));


				}

				if (rand() % 3 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_COIN, D3DXVECTOR3(Shootmove.x + 3.0f, Shootmove.y, 0.0f));

				}

				if (rand() % 25 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_VODKA, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
				if (rand() % 150 == 0)
				{

					SetItem(Shootpos, ITEMTYPE_PAPER, D3DXVECTOR3(Shootmove.x + 1, Shootmove.y, 0.0f));

				}
			}
		}
		break;
	}



	

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


	pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
		g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffShoot->Unlock();



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShoot(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画


	if (g_aShoot.bUse == true)
	{

		// デバイスの取得
		pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffShoot, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);



		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureShoot);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


//=============================================================================
// シューターの設定
//=============================================================================
void SetShoot(D3DXVECTOR3 pos)
{
	//	int nCntBoss;
	VERTEX_2D *pVtx;

	g_aShoot.fLength = sqrtf((g_aShoot.fWidth / 2 + g_aShoot.fWidth / 2) * (g_aShoot.fHeight / 2 + g_aShoot.fHeight / 2));
	g_aShoot.fAngle = atan2f(g_aShoot.fWidth / 2, g_aShoot.fHeight / 2);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShoot->Lock(0, 0, (void**)&pVtx, 0);


	if (g_aShoot.bUse == false)	//シューターが使用されていない
	{
		g_aShoot.pos = pos;

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(-D3DX_PI + g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[1].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(D3DX_PI - g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[2].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(-g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
								  g_aShoot.pos.y + cosf(-g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		pVtx[3].pos = D3DXVECTOR3(g_aShoot.pos.x + sinf(g_aShoot.fAngle + g_aShoot.rot.x) * g_aShoot.fLength,
							      g_aShoot.pos.y + cosf(g_aShoot.fAngle + g_aShoot.rot.y) * g_aShoot.fLength, 0.0f);


		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);




		g_aShoot.bUse = true;	//使用している状態にする


	}

	//頂点バッファをアンロック
	g_pVtxBuffShoot->Unlock();
}

//=============================================================================
// シューターの取得
//=============================================================================
SHOOT *GetShoot(void)
{
	return &g_aShoot;
}
