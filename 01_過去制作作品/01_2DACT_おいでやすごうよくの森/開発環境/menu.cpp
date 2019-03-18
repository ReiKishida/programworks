//************************************************************************************************************************
//
// ポーズメニュー処理 [PauseMenu.cpp]
// Author : Kishida Rei
//
//************************************************************************************************************************
#include "menu.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "block.h"
#include "fg.h"
#include "fg2.h"

//*****************************************************************************************************************************************************************************
// マクロ定義
//*****************************************************************************************************************************************************************************
#define	MAX_TYPE_MENU		(3)								// キーの種類
#define MENU_TEXTURENAME_0	"data/TEXTURE/gamemode.png"			//キーのテクスチャ1
#define MENU_TEXTURENAME_1	"data/TEXTURE/menuranking.png"		//キーのテクスチャ2
#define MENU_TEXTURENAME_2	"data/TEXTURE/quit.png"				//キーのテクスチャ3




#define MENU_POS_X		(300.0f)							//背景左上X座標
#define MENU_POS_Y		(100.0f)							//背景左上Y座標


#define MENU_GAMEMODE (0)		//コンティニュー
#define MENU_MENU_RANKING (1)		//リトライ
#define MENU_QUIT (2)		//modori



//*****************************************************************************************************************************************************************************
// グローバル変数宣言
//*****************************************************************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureMenu[MAX_TYPE_MENU] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;					// 頂点バッファへのポインタ

PAUSEMENU g_aMenu[MAX_TYPE_MENU];
int g_nNowMenuSelect;    //現在の選択状態

int nCnt;

//************************************************************************************************************************
// 初期化処理
//************************************************************************************************************************
void InitMenu(void)
{

	//背景初期化
	InitBG();

	//ブロック初期化
	InitBlock();

	nCnt = 0;

	SetBlock(D3DXVECTOR3(0.0f, 580, 0), 1280.0f, 150.0f, 1, BLOCKSTATE_NORMAL, 0.0f, 0.0f);

	//前景
	InitFg();
	InitFg2();
	


	SetFg(D3DXVECTOR3(270, 660, 0.0f), 0, 85, 150);
	SetFg(D3DXVECTOR3(810, 660, 0.0f), 1, 75, 150);
	SetFg(D3DXVECTOR3(1100, 650, 0.0f), 0, 65, 150);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH / 2, 600, 0.0f), 0, 50, 500);

	SetFg2(D3DXVECTOR3(-10, 645, 0.0f), 4, 250, 120);
	SetFg2(D3DXVECTOR3(SCREEN_WIDTH + 10, 645, 0.0f), 5, 250, 120);


	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;	//キーのカウント

	// デバイスの取得
	pDevice = GetDevice();


	g_nNowMenuSelect = GAMEMODE;
	
		g_aMenu[GAMEMODE].pos = D3DXVECTOR3(640, 170, 0);
		g_aMenu[MENU_RANKING].pos = D3DXVECTOR3(640, 400, 0);
		g_aMenu[QUIT].pos = D3DXVECTOR3(640, 610, 0);

		g_aMenu[GAMEMODE].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aMenu[MENU_RANKING].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aMenu[QUIT].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		g_aMenu[GAMEMODE].nType = 0;
		g_aMenu[MENU_RANKING].nType = 1;
		g_aMenu[QUIT].nType = 2;


		g_aMenu[GAMEMODE].state = MENUSTATE_NORMAL;
		g_aMenu[MENU_RANKING].state = MENUSTATE_NORMAL;
		g_aMenu[QUIT].state = MENUSTATE_NORMAL;

		g_aMenu[GAMEMODE].uState = MENUSTATE_NONE;
		g_aMenu[MENU_RANKING].uState = MENUSTATE_NONE;
		g_aMenu[QUIT].uState = MENUSTATE_NONE;

	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_0, &g_apTextureMenu[MENU_GAMEMODE]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_1, &g_apTextureMenu[MENU_MENU_RANKING]);
	D3DXCreateTextureFromFile(pDevice, MENU_TEXTURENAME_2, &g_apTextureMenu[MENU_QUIT]);




	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenu,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < MAX_TYPE_MENU; nCntMenu++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x - MENU_POS_X, g_aMenu[nCntMenu].pos.y - MENU_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x + MENU_POS_X, g_aMenu[nCntMenu].pos.y - MENU_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x - MENU_POS_X, g_aMenu[nCntMenu].pos.y + MENU_POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aMenu[nCntMenu].pos.x + MENU_POS_X, g_aMenu[nCntMenu].pos.y + MENU_POS_Y, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
		
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffMenu->Unlock();
}

//************************************************************************************************************************
// 終了処理
//************************************************************************************************************************
void UnInitMenu(void)
{
	//背景終了
	UnInitBG();

	//ブロック終了
	UnInitBlock();

	UnInitFg();
	UnInitFg2();
	
	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_MENU; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureMenu[nCntTex] != NULL)
		{
			g_apTextureMenu[nCntTex]->Release();
			g_apTextureMenu[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffMenu != NULL)
	{
		g_pVtxBuffMenu->Release();
		g_pVtxBuffMenu = NULL;
	}
}

//************************************************************************************************************************
// 更新処理
//************************************************************************************************************************
void UpdateMenu(void)
{
	//背景更新
	UpdateBG();

	//ブロック更新
	UpdateBlock();

	UpdateFg();
	UpdateFg2();
	


	nCnt++;


	if (nCnt % 1800 == 0)
	{
		InitBG();
	}

	//g_nNowMenuSelect = GAMEMODE;
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();



	PushMenu();

	VERTEX_2D *pVtx;
			


	if (g_aMenu[g_nNowMenuSelect].state == MENUSTATE_SELECT)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
		switch (g_aMenu[g_nNowMenuSelect].uState)
		{
		case MENUSTATE_NONE:
			g_aMenu[g_nNowMenuSelect].col.a -= 0.02f;
			if (g_aMenu[g_nNowMenuSelect].col.a <= 0.0f)
			{
				g_aMenu[g_nNowMenuSelect].col.a = 0.0f;
				g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_USE;
			}
			break;

		case MENUSTATE_USE:
			g_aMenu[g_nNowMenuSelect].col.a += 0.02f;
			if (g_aMenu[g_nNowMenuSelect].col.a >= 1.0f)
			{
				g_aMenu[g_nNowMenuSelect].col.a = 1.0f;
				g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_NONE;
			}
			break;
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffMenu->Unlock();
	}







	
			if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true)
			{//Wキー
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowMenuSelect = (g_nNowMenuSelect + (MENU_MAX - 1)) % MENU_MAX;//選択状態を一つ上げる
				//PushPauseMenu();
				
			}


			else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true) 
			{//Sキー
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowMenuSelect = (g_nNowMenuSelect + 1 ) % MENU_MAX;//選択状態を一つ下げる
				//PushPauseMenu();
				
			}

			else if (GetKeyboardTrigger(DIK_RETURN) == true)
			{//RETURNキー
				//PushPauseMenu();
				switch (g_nNowMenuSelect)
				{//現在選択中の状態によって処理を分岐
				case GAMEMODE://ゲーム選択中なら
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_TUTO);
						}
					}
					break;


				case MENU_RANKING://設定選択中なら
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_MENURANKING);
						}
					}
					break;

				case QUIT://設定選択中なら
					if (pFade == FADE_NONE)
					{
						if (GetKeyboardTrigger(DIK_RETURN) == true)
						{
							PlaySound(SOUND_LABEL_SE_DECIDE);
							SetFade(MODE_TITLE);
						}
					}
					break;
				}
			}







			if (g_nNowMenuSelect == GAMEMODE)
			{//選択時
				g_aMenu[GAMEMODE].state = MENUSTATE_SELECT;
				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (GAMEMODE * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != GAMEMODE)
			{//通常時
				g_aMenu[GAMEMODE].state = MENUSTATE_NORMAL;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (GAMEMODE * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}


			if (g_nNowMenuSelect == MENU_RANKING)
			{//選択時
				g_aMenu[MENU_RANKING].state = MENUSTATE_SELECT;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (MENU_RANKING * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[4].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != MENU_RANKING)
			{//通常時
				g_aMenu[MENU_RANKING].state = MENUSTATE_NORMAL;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (MENU_RANKING * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}

			if (g_nNowMenuSelect == QUIT)
			{//選択時
			 // 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}
			if (g_nNowMenuSelect != QUIT)
			{//通常時
			 // 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}


			if (g_aMenu[g_nNowMenuSelect].state == MENUSTATE_SELECT)
			{
				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
				switch (g_aMenu[g_nNowMenuSelect].uState)
				{
				case MENUSTATE_NONE:
					g_aMenu[g_nNowMenuSelect].col.a -= 0.02f;
					if (g_aMenu[g_nNowMenuSelect].col.a <= 0.0f)
					{
						g_aMenu[g_nNowMenuSelect].col.a = 0.0f;
						g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_USE;
					}
					break;

				case MENUSTATE_USE:
					g_aMenu[g_nNowMenuSelect].col.a += 0.02f;
					if (g_aMenu[g_nNowMenuSelect].col.a >= 1.0f)
					{
						g_aMenu[g_nNowMenuSelect].col.a = 1.0f;
						g_aMenu[g_nNowMenuSelect].uState = MENUSTATE_NONE;
					}
					break;
				}
				// 頂点バッファをアンロックする
				g_pVtxBuffMenu->Unlock();
			}



}
	

//************************************************************************************************************************
// 描画処理
//************************************************************************************************************************
void DrawMenu(void)
{
	//背景描画
	DrawBG();

	//ブロック描画
	DrawBlock();

	DrawFg2();
	DrawFg();


	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntMenu = 0; nCntMenu < MAX_TYPE_MENU; nCntMenu++)
	{
		

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMenu[g_aMenu[nCntMenu].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMenu * 4, 2);
		}
	

}


//************************************************************************************************************************
// キー入力
//************************************************************************************************************************
//void PushPauseMenu(int nCntMenu)
void PushMenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();

	//VERTEX_2D *pVtx;




	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//エンターキーが押されたら

		switch (g_nNowMenuSelect)
		{//現在選択中の状態によって処理を分岐
		case GAMEMODE://ゲーム選択中なら
			pPause->bPause = false;
			break;


		case MENU_RANKING://設定選択中なら
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					SetFade(MODE_GAME);
				}
			}
			break;

		case QUIT://設定選択中なら
			if (pFade == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
					SetFade(MODE_TITLE);
				}
			}
			break;
		}
	}
}
	
	
		
	
	
	
//************************************************************************************************************************
// キー画像の設定
//************************************************************************************************************************
void SetMenu(D3DXVECTOR3 pos, int nType)
{
	
}


//************************************************************************************************************************
// キーの取得
//************************************************************************************************************************
PAUSEMENU *GetMenu(void)
{
	return &g_aMenu[0];
}





