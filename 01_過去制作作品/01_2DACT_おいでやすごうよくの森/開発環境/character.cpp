//=============================================================================
//
// 発射位置処理 [shoot.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "character.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "main.h"
#include "item.h"
#include "block.h"

#include <stdlib.h> 
#include <time.h>   

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_SHOOT		(1)								// 発射位置の種類
#define BOSS_TEXTURENAME_0	"data/TEXTURE/human.png"		//発射位置ののテクスチャ1


#define CHARACTER_X (50)
#define CHARACTER_Y (80)



#define SHOOT_POWER (-20.0f)	//ジャンプ力

#define C_SHOOT_POWER (-5.0f)	//ジャンプ力
#define C_GRAVITY (1.2f)		//重力

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureCharacter = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacter = NULL;					// 頂点バッファへのポインタ

CHARACTER g_aCharacter;



//int g_nNumBoss;	//敵の総数
//=============================================================================
// 初期化処理
//=============================================================================
void InitCharacter(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//int nCntBoss;	//敵のカウント

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の情報の初期化

	g_aCharacter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aCharacter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_aCharacter.type = SHOOTTYPE_COIN;
	g_aCharacter.bUse = false;
	//g_aCharacter.direction = SHOOTDIRECTION_0;

	

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURENAME_0, &g_apTextureCharacter);
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacter,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y , 0.0f);

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
	g_pVtxBuffCharacter->Unlock();

	//g_nNumBoss = 0; //敵の総数のクリア

}

//=============================================================================
// 終了処理
//=============================================================================
void UnInitCharacter(void)
{
	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_SHOOT; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureCharacter != NULL)
		{
			g_apTextureCharacter->Release();
			g_apTextureCharacter = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffCharacter != NULL)
	{
		g_pVtxBuffCharacter->Release();
		g_pVtxBuffCharacter = NULL;
	}



	

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCharacter(void)
{

	// 頂点情報の作成
		VERTEX_2D *pVtx;

	D3DXVECTOR3 Shootpos;

	Shootpos.x = g_aCharacter.pos.x + CHARACTER_X + 10;
	Shootpos.y = g_aCharacter.pos.y - 5;


	D3DXVECTOR3 Shootmove;

	Shootmove.x = (rand() % 4) + 1.0f;
	Shootmove.y = SHOOT_POWER;




	g_aCharacter.move.x += -4.0f;

	if (g_aCharacter.bUse == true)
	{



		if (rand() % 80 == 0)
		{

			SetItem(Shootpos, ITEMTYPE_COIN, Shootmove);
		}

	}




	g_aCharacter.move.y += C_GRAVITY;
	

	g_aCharacter.move.x += (0.0f - g_aCharacter.move.x) * 0.35f;
	/*g_Player.move.y += (0.0f - g_Player.move.y) * 0.2f;*/



	g_aCharacter.pos.x += g_aCharacter.move.x;
	g_aCharacter.pos.y += g_aCharacter.move.y;




	/*if (g_aCharacter.pos.x >= 1310)
	{
		g_aCharacter.bUse = false;
	}

	if (g_aCharacter.pos.x <= -20)
	{
		g_aCharacter.bUse = false;
	}*/
	if (g_aCharacter.pos.x >= 1310)
	{
		g_aCharacter.pos = D3DXVECTOR3(10.0, g_aCharacter.pos.y, 0.0);
	}

	if (g_aCharacter.pos.x <= -10)
	{
		g_aCharacter.pos = D3DXVECTOR3(1280.0, g_aCharacter.pos.y, 0.0);
	}


	CollisionBlock(&g_aCharacter.pos, &g_aCharacter.posold, &g_aCharacter.move, g_aCharacter.fWidth, g_aCharacter.fHeight);
	


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCharacter->Unlock();
}
	//=============================================================================
	// 描画処理
	//=============================================================================
	void DrawCharacter(void)
	{
		LPDIRECT3DDEVICE9 pDevice;
		// ポリゴンの描画


		if (g_aCharacter.bUse == true)
		{

			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffCharacter, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);



			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureCharacter);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}



//=============================================================================
// 敵の設定
//=============================================================================
void SetCharacter(D3DXVECTOR3 pos, int nType)
{
	//	int nCntBoss;
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);


	if (g_aCharacter.bUse == false)	//敵が使用されていない
	{
		g_aCharacter.pos = pos;

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y - CHARACTER_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCharacter.pos.x - CHARACTER_X, g_aCharacter.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCharacter.pos.x + CHARACTER_X, g_aCharacter.pos.y, 0.0f);


		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		g_aCharacter.nType = nType;

		g_aCharacter.bUse = true;	//使用している状態にする


	}

	//頂点バッファをアンロック
	g_pVtxBuffCharacter->Unlock();
}

//=============================================================================
// 敵の取得
//=============================================================================
CHARACTER *GetCharacter(void)
{
	return &g_aCharacter;
}
