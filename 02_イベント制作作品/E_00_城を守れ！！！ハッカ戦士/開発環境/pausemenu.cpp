//************************************************************************************************************************
//
// ポーズメニュー処理 [PauseMenu.cpp]
// Author : Kishida Rei
//
//************************************************************************************************************************
#include "pausemenu.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************************************************************************************************************
// マクロ定義
//*****************************************************************************************************************************************************************************
#define	MAX_TYPE_PAUSEMENU		(3)								// キーの種類
#define PAUSEMENU_TEXTURENAME_0	"data/TEXTURE/pause000.png"		//キーのテクスチャ1
#define PAUSEMENU_TEXTURENAME_1	"data/TEXTURE/pause001.png"		//キーのテクスチャ2
#define PAUSEMENU_TEXTURENAME_2	"data/TEXTURE/pause002.png"		//キーのテクスチャ3




#define PAUSEMENU_POS_X		(200.0f)							//背景左上X座標
#define PAUSEMENU_POS_Y		(45.0f)							//背景左上Y座標


#define MENU_CONTINUE (0)		//コンティニュー
#define MENU_RETRY (1)		//リトライ
#define MENU_QUIT (2)		//modori



//*****************************************************************************************************************************************************************************
// グローバル変数宣言
//*****************************************************************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePauseMenu[MAX_TYPE_PAUSEMENU] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseMenu = NULL;					// 頂点バッファへのポインタ

PAUSEMENU g_aPauseMenu[MAX_TYPE_PAUSEMENU];
int g_nNowSelect;    //現在の選択状態
//************************************************************************************************************************
// 初期化処理
//************************************************************************************************************************
void InitPauseMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPauseMenu;	//キーのカウント

						// デバイスの取得
	pDevice = GetDevice();


	g_nNowSelect = CONTINUE;
	
		g_aPauseMenu[CONTINUE].pos = D3DXVECTOR3(640, 250, 0);
		g_aPauseMenu[RETRY].pos = D3DXVECTOR3(640, 350, 0);
		g_aPauseMenu[QUIT].pos = D3DXVECTOR3(640, 450, 0);

		g_aPauseMenu[CONTINUE].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aPauseMenu[RETRY].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		g_aPauseMenu[QUIT].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

		g_aPauseMenu[CONTINUE].nType = 0;
		g_aPauseMenu[RETRY].nType = 1;
		g_aPauseMenu[QUIT].nType = 2;


		g_aPauseMenu[CONTINUE].state = PAUSEMENUSTATE_NORMAL;
		g_aPauseMenu[RETRY].state = PAUSEMENUSTATE_NORMAL;
		g_aPauseMenu[QUIT].state = PAUSEMENUSTATE_NORMAL;

		g_aPauseMenu[CONTINUE].uState = PAUSEMENUSTATE_NONE;
		g_aPauseMenu[RETRY].uState = PAUSEMENUSTATE_NONE;
		g_aPauseMenu[QUIT].uState = PAUSEMENUSTATE_NONE;

	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, PAUSEMENU_TEXTURENAME_0, &g_apTexturePauseMenu[MENU_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, PAUSEMENU_TEXTURENAME_1, &g_apTexturePauseMenu[MENU_RETRY]);
	D3DXCreateTextureFromFile(pDevice, PAUSEMENU_TEXTURENAME_2, &g_apTexturePauseMenu[MENU_QUIT]);




	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_PAUSEMENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseMenu,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPauseMenu = 0; nCntPauseMenu < MAX_TYPE_PAUSEMENU; nCntPauseMenu++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPauseMenu[nCntPauseMenu].pos.x - PAUSEMENU_POS_X, g_aPauseMenu[nCntPauseMenu].pos.y - PAUSEMENU_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPauseMenu[nCntPauseMenu].pos.x + PAUSEMENU_POS_X, g_aPauseMenu[nCntPauseMenu].pos.y - PAUSEMENU_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPauseMenu[nCntPauseMenu].pos.x - PAUSEMENU_POS_X, g_aPauseMenu[nCntPauseMenu].pos.y + PAUSEMENU_POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPauseMenu[nCntPauseMenu].pos.x + PAUSEMENU_POS_X, g_aPauseMenu[nCntPauseMenu].pos.y + PAUSEMENU_POS_Y, 0.0f);

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
	g_pVtxBuffPauseMenu->Unlock();
}

//************************************************************************************************************************
// 終了処理
//************************************************************************************************************************
void UnInitPauseMenu(void)
{
	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_PAUSEMENU; nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTexturePauseMenu[nCntTex] != NULL)
		{
			g_apTexturePauseMenu[nCntTex]->Release();
			g_apTexturePauseMenu[nCntTex] = NULL;
		}
	}


	// 頂点バッファの開放
	if (g_pVtxBuffPauseMenu != NULL)
	{
		g_pVtxBuffPauseMenu->Release();
		g_pVtxBuffPauseMenu = NULL;
	}
}

//************************************************************************************************************************
// 更新処理
//************************************************************************************************************************
void UpdatePauseMenu(void)
{
	//g_nNowSelect = CONTINUE;
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();



	PushPauseMenu();

	VERTEX_2D *pVtx;
			


	if (g_aPauseMenu[g_nNowSelect].state == PAUSEMENUSTATE_SELECT)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
		switch (g_aPauseMenu[g_nNowSelect].uState)
		{
		case PAUSEMENUSTATE_NONE:
			g_aPauseMenu[g_nNowSelect].col.a -= 0.02f;
			if (g_aPauseMenu[g_nNowSelect].col.a <= 0.0f)
			{
				g_aPauseMenu[g_nNowSelect].col.a = 0.0f;
				g_aPauseMenu[g_nNowSelect].uState = PAUSEMENUSTATE_USE;
			}
			break;

		case PAUSEMENUSTATE_USE:
			g_aPauseMenu[g_nNowSelect].col.a += 0.02f;
			if (g_aPauseMenu[g_nNowSelect].col.a >= 1.0f)
			{
				g_aPauseMenu[g_nNowSelect].col.a = 1.0f;
				g_aPauseMenu[g_nNowSelect].uState = PAUSEMENUSTATE_NONE;
			}
			break;
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffPauseMenu->Unlock();
	}







	
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキー
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowSelect = (g_nNowSelect + (MAX_MENU - 1)) % MAX_MENU;//選択状態を一つ上げる
				//PushPauseMenu();
				
			}


			else if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキー
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_nNowSelect = (g_nNowSelect + 1 ) % MAX_MENU;//選択状態を一つ下げる
				//PushPauseMenu();
				
			}

			else if (GetKeyboardTrigger(DIK_RETURN) == true)
			{//RETURNキー
				//PushPauseMenu();
				switch (g_nNowSelect)
				{//現在選択中の状態によって処理を分岐
				case CONTINUE://ゲーム選択中なら
					pPause->bPause = false;
					break;


				case RETRY://設定選択中なら
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







			if (g_nNowSelect == CONTINUE)
			{//選択時
				g_aPauseMenu[CONTINUE].state = PAUSEMENUSTATE_SELECT;
				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (CONTINUE * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}
			if (g_nNowSelect != CONTINUE)
			{//通常時
				g_aPauseMenu[CONTINUE].state = PAUSEMENUSTATE_NORMAL;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (CONTINUE * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}


			if (g_nNowSelect == RETRY)
			{//選択時
				g_aPauseMenu[RETRY].state = PAUSEMENUSTATE_SELECT;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (RETRY * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}
			if (g_nNowSelect != RETRY)
			{//通常時
				g_aPauseMenu[RETRY].state = PAUSEMENUSTATE_NORMAL;

				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (RETRY * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}

			if (g_nNowSelect == QUIT)
			{//選択時
			 // 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}
			if (g_nNowSelect != QUIT)
			{//通常時
			 // 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (QUIT * 4);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}


			if (g_aPauseMenu[g_nNowSelect].state == PAUSEMENUSTATE_SELECT)
			{
				// 頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);
				switch (g_aPauseMenu[g_nNowSelect].uState)
				{
				case PAUSEMENUSTATE_NONE:
					g_aPauseMenu[g_nNowSelect].col.a -= 0.02f;
					if (g_aPauseMenu[g_nNowSelect].col.a <= 0.0f)
					{
						g_aPauseMenu[g_nNowSelect].col.a = 0.0f;
						g_aPauseMenu[g_nNowSelect].uState = PAUSEMENUSTATE_USE;
					}
					break;

				case PAUSEMENUSTATE_USE:
					g_aPauseMenu[g_nNowSelect].col.a += 0.02f;
					if (g_aPauseMenu[g_nNowSelect].col.a >= 1.0f)
					{
						g_aPauseMenu[g_nNowSelect].col.a = 1.0f;
						g_aPauseMenu[g_nNowSelect].uState = PAUSEMENUSTATE_NONE;
					}
					break;
				}
				// 頂点バッファをアンロックする
				g_pVtxBuffPauseMenu->Unlock();
			}



}
	

//************************************************************************************************************************
// 描画処理
//************************************************************************************************************************
void DrawPauseMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	// ポリゴンの描画

			// デバイスの取得
			pDevice = GetDevice();

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffPauseMenu, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TYPE_PAUSEMENU; nCntPauseMenu++)
	{
		

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePauseMenu[g_aPauseMenu[nCntPauseMenu].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPauseMenu * 4, 2);
		}
	

}


//************************************************************************************************************************
// キー入力
//************************************************************************************************************************
//void PushPauseMenu(int nCntPauseMenu)
void PushPauseMenu(void)
{
	PAUSECHECK *pPause;
	pPause = GetPauseCheck();

	FADE pFade;
	pFade = *GetFade();

	//VERTEX_2D *pVtx;




	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//エンターキーが押されたら

		switch (g_nNowSelect)
		{//現在選択中の状態によって処理を分岐
		case CONTINUE://ゲーム選択中なら
			pPause->bPause = false;
			break;


		case RETRY://設定選択中なら
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
void SetPauseMenu(D3DXVECTOR3 pos, int nType)
{
	
}


//************************************************************************************************************************
// キーの取得
//************************************************************************************************************************
PAUSEMENU *GetPauseMenu(void)
{
	return &g_aPauseMenu[0];
}





