//=============================================================================
//
// 情報画面処理 [Tuto.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "main.h"
#include "tuto.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "bg.h"
#include "block.h"
#include "fg.h"
#include "fg2.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TYPE_TUTO (3)
#define MAX_TUTO (3)

#define TUTO_TEXTURENAME	"DATA/TEXTURE/tuto001-2.png"	//タイトルのテクスチャ名
#define TUTO_TEXTURENAME_2	"DATA/TEXTURE/tuto002.png"	//タイトルのテクスチャ名
#define TUTO_TEXTURENAME_3	"DATA/TEXTURE/press_enter3.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTuto[MAX_TYPE_TUTO] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTuto = NULL;	//頂点バッファへのポインタ

TUTO g_aTuto[MAX_TUTO];

//int 	g_nSelectPase;
int nAnimation_X;
int nReturnCount;

int g_nCntTutoPase;
//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTuto(void)
{
	InitBG();
	InitBlock();
	InitFg();
	InitFg2();


	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 80, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);

	//草
	SetFg(D3DXVECTOR3(100, 640, 0.0f), 2, 50, 25);
	SetFg(D3DXVECTOR3(640, 630, 0.0f), 2, 60, 30);
	SetFg(D3DXVECTOR3(450, 630, 0.0f), 2, 50, 30);
	SetFg(D3DXVECTOR3(600, 650, 0.0f), 2, 40, 20);



	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 590, 0.0f), 0, 50, 480);
	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);

	SetBlock(D3DXVECTOR3(0, 580, 0), SCREEN_WIDTH, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);



	g_nCntTutoPase = 0;

	LPDIRECT3DDEVICE9 pDevice;


	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntTuto = 0; nCntTuto < MAX_TUTO; nCntTuto++)
	{
		//g_aTuto[nCntTuto].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT /2, 0.0f);
		g_aTuto[nCntTuto].state = TUTOSTATE_NONE;
		g_aTuto[nCntTuto].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTuto[nCntTuto].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	nAnimation_X = 0;
	nReturnCount = 0;

	g_aTuto[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aTuto[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 -350 , SCREEN_HEIGHT -70, 0.0f); 
	g_aTuto[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 330, SCREEN_HEIGHT - 70, 0.0f);



	g_aTuto[0].bUse = true;
	g_aTuto[1].bUse = true;
	g_aTuto[2].bUse = false;

	/*g_nSelectPase = PASE_ONE;*/

	

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TUTO_TEXTURENAME, &g_pTextureTuto[0]);
	D3DXCreateTextureFromFile(pDevice, TUTO_TEXTURENAME_2, &g_pTextureTuto[1]);
	D3DXCreateTextureFromFile(pDevice, TUTO_TEXTURENAME_3, &g_pTextureTuto[2]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTuto,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);

	
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTuto[0].pos.x - SCREEN_WIDTH / 2 , g_aTuto[0].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTuto[0].pos.x + SCREEN_WIDTH / 2, g_aTuto[0].pos.y - SCREEN_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTuto[0].pos.x - SCREEN_WIDTH / 2, g_aTuto[0].pos.y + SCREEN_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTuto[0].pos.x + SCREEN_WIDTH / 2, g_aTuto[0].pos.y + SCREEN_HEIGHT / 2, 0.0f);


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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	
		pVtx += 4;

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTuto[1].pos.x - 300, g_aTuto[1].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTuto[1].pos.x + 300, g_aTuto[1].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTuto[1].pos.x - 300, g_aTuto[1].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTuto[1].pos.x + 300, g_aTuto[1].pos.y + 50, 0.0f);


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
	
		pVtx += 4;


		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTuto[2].pos.x - 300, g_aTuto[2].pos.y - 70, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTuto[2].pos.x + 300, g_aTuto[2].pos.y - 70, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTuto[2].pos.x - 300, g_aTuto[2].pos.y + 70, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTuto[2].pos.x + 300, g_aTuto[2].pos.y + 70, 0.0f);

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
	//頂点バッファをアンロック
	g_pVtxBuffTuto->Unlock();

	
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UnInitTuto(void)
{
	UnInitBG();
	UnInitBlock();
	UnInitFg();
	UnInitFg2();

	for (int nCntTuto = 0; nCntTuto < MAX_TYPE_TUTO; nCntTuto++)
	{
		//テクスチャの破棄
		if (g_pTextureTuto[nCntTuto] != NULL)
		{
			g_pTextureTuto[nCntTuto]->Release();
			g_pTextureTuto[nCntTuto] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTuto != NULL)
	{
		g_pVtxBuffTuto->Release();
		g_pVtxBuffTuto = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTuto(void)
{
	UpdateBG();
	UpdateBlock();
	UpdateFg();
	UpdateFg2();


	FADE pFade;
	pFade = *GetFade();

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	

	g_nCntTutoPase++;

	if (g_nCntTutoPase % 900 == 0)
	{
		nAnimation_X = (nAnimation_X + 1);
	}
	
			if (GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_RETURN)== true || GetKeyboardTrigger(DIK_RIGHTARROW) == true)
			{

				//g_nSelectPase = (g_nSelectPase + (PASE_MAX - 1)) % PASE_MAX;//選択状態を一つ上げる
				PlaySound(SOUND_LABEL_SE_MEKURI);
				nAnimation_X = (nAnimation_X +  1 );
				if (nAnimation_X > 3)
				{
					nAnimation_X = 3;
				}
			}


			if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_LEFTARROW) == true)
			{

				//g_nSelectPase = (g_nSelectPase + 1) % PASE_MAX;//選択状態を一つ下げる

				PlaySound(SOUND_LABEL_SE_MEKURI);

				nAnimation_X = (nAnimation_X - 1) ;
				if (nAnimation_X < 0)
				{
					nAnimation_X = 0;
				}
			}

		
		


		/*	if (g_nSelectPase == PASE_FIVE)
			{
				g_aTuto[0].bUse = true;
				g_aTuto[1].bUse = false;
				g_aTuto[2].bUse = true;
			}
			if (g_nSelectPase != PASE_FIVE)
			{
				g_aTuto[0].bUse = true;
				g_aTuto[1].bUse = true;
				g_aTuto[2].bUse = false;
			}*/

			if (nAnimation_X == 3)
			{
				g_aTuto[0].bUse = true;
				g_aTuto[1].bUse = false;
				g_aTuto[2].bUse = true;
			}
			if (nAnimation_X != 3)
			{
				g_aTuto[0].bUse = true;
				g_aTuto[1].bUse = true;
				g_aTuto[2].bUse = false;
			}





			if (nAnimation_X == 3)
			{
				if (pFade == FADE_NONE)
				{

					if (GetKeyboardTrigger(DIK_SPACE) == true)
					{
						
						PlaySound(SOUND_LABEL_SE_DECIDE);

						//モード選択
						//SetMode(MODE_TUTO);
						SetFade(MODE_GAME);
					}
				}
			}



	//if (g_nSelectPase == PASE_FOUR)
	//{
	//	if (pFade == FADE_NONE)
	//	{

	//		if (GetKeyboardTrigger(DIK_RETURN) == true)
	//		{
	//			PlaySound(SOUND_LABEL_SE_DECIDE);

	//			//モード選択
	//			//SetMode(MODE_TUTO);
	//			SetFade(MODE_GAME);
	//		}
	//	}
	//}
	


	


	if (g_aTuto[1].bUse == true)
	{
		switch (g_aTuto[1].state)
		{
		case TUTOSTATE_NONE:
			g_aTuto[1].col.a -= 0.02f;
			if (g_aTuto[1].col.a <= 0.0f)
			{
				g_aTuto[1].col.a = 0.0f;
				g_aTuto[1].state = TUTOSTATE_USE;
			}
			break;

		case TUTOSTATE_USE:
			g_aTuto[1].col.a += 0.02f;
			if (g_aTuto[1].col.a >= 1.0f)
			{
				g_aTuto[1].col.a = 1.0f;
				g_aTuto[1].state = TUTOSTATE_NONE;
			}
			break;
		}
	}
	

	if (g_aTuto[2].bUse == true)
	{
		switch (g_aTuto[2].state)
		{
		case TUTOSTATE_NONE:
			g_aTuto[2].col.a -= 0.01f;
			if (g_aTuto[2].col.a <= 0.0f)
			{
				g_aTuto[2].col.a = 0.0f;
				g_aTuto[2].state = TUTOSTATE_USE;
			}
			break;

		case TUTOSTATE_USE:
			g_aTuto[2].col.a += 0.01f;
			if (g_aTuto[2].col.a >= 1.0f)
			{
				g_aTuto[2].col.a = 1.0f;
				g_aTuto[2].state = TUTOSTATE_NONE;
			}
			break;
		}
	}
	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTuto->Lock(0, 0, (void**)&pVtx, 0);
	
		////テクスチャ設定
		//pVtx[0].tex = D3DXVECTOR2(0.25f * g_nSelectPase, 0.0f);
		//pVtx[1].tex = D3DXVECTOR2(0.25f* (g_nSelectPase + 1), 0.0f);
		//pVtx[2].tex = D3DXVECTOR2(0.25f* g_nSelectPase, 1.0f);
		//pVtx[3].tex = D3DXVECTOR2(0.25f* (g_nSelectPase + 1), 1.0f);


	if (nAnimation_X < 4)
	{
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * nAnimation_X, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f* (nAnimation_X + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f* nAnimation_X, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f* (nAnimation_X + 1), 1.0f);
	}

		pVtx += 4;

		//頂点カラー
		pVtx[0].col = g_aTuto[1].col;
		pVtx[1].col = g_aTuto[1].col;
		pVtx[2].col = g_aTuto[1].col;
		pVtx[3].col = g_aTuto[1].col;


		pVtx += 4;

		//頂点カラー
		pVtx[0].col = g_aTuto[2].col;
		pVtx[1].col = g_aTuto[2].col;
		pVtx[2].col = g_aTuto[2].col;
		pVtx[3].col = g_aTuto[2].col;
	
	//頂点バッファをアンロック
	g_pVtxBuffTuto->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTuto(void)
{
	DrawBG();
	DrawBlock();
	DrawFg();
	DrawFg2();


	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ



	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTuto, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTuto = 0; nCntTuto < MAX_TYPE_TUTO; nCntTuto++)
	{
		if (g_aTuto[nCntTuto].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTuto[nCntTuto]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTuto * 4, 2);

		}
	}
}
