////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UI処理 [UI.cpp]
// Author : RyotaKinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "UI.h"
#include "player.h"
#include "effect2D.h"
#include "input.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	D3DXCOLOR  col;				// 色
	UITEX nType;				// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	D3DXVECTOR3 Stoppos;
} UIInfo;
typedef struct
{
	char *pFileName;
}UITexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************

#define MAX_UI	(17)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUI[UITEX_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;		// 頂点バッファへのポインタ
UI						g_aUI[MAX_UI];
UIInfo g_aUIInfo[] =
{
	// 黒帯
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,-25,0.0f),D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),UITEX_BG,SCREEN_WIDTH,50 ,  D3DXVECTOR3(SCREEN_WIDTH / 2,25,0.0f) },
	{ D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT+25,0.0f),D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),UITEX_BG1,SCREEN_WIDTH,50 ,  D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT-25,0.0f) },

	// ゲームCLEAROVER
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_RESULT,SCREEN_WIDTH,SCREEN_HEIGHT,D3DXVECTOR3(0.0f,0.0f,0.0f) },// パーセント
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_GAMECLEAR,600,600,D3DXVECTOR3(0.0f,0.0f,0.0f) },// パーセント
	{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),UITEX_GAMEOVER,500,500,D3DXVECTOR3(0.0f,0.0f,0.0f) },// パーセント

	// 画面UI
	{ D3DXVECTOR3(180,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_BOTTON,225,225,D3DXVECTOR3(0.0f,0.0f,0.0f) },// パンチ
	
	{ D3DXVECTOR3(280,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_LANCE,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// パンチ
	{ D3DXVECTOR3(180,450,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_SORD,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// キック
	{ D3DXVECTOR3(80,550,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_HAMMER,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// スピン
	{ D3DXVECTOR3( 180,650,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),UITEX_PUNCH,100,100,D3DXVECTOR3(0.0f,0.0f,0.0f) },// ジャンプ

};
// テクスチャ名
UITexture  g_aUITex[UITEX_MAX] =
{
	{ "data/TEXTURE/ui.png" },			// 黒い幕
	{ "data/TEXTURE/ui2.png" },			// 黒い幕
	{ "data/TEXTURE/UI.jpg" },			// 黒
	{ "data/TEXTURE/GameClearUI.png" },		// ゲームクリア
	{ "data/TEXTURE/GameOverUI.png" },		// ゲームオーバー
	{ "data/TEXTURE/botann.png" },	// ボタン
	{ "data/TEXTURE/yari.png" },	// パンチ
	{ "data/TEXTURE/ken.png" },	// キック
	{ "data/TEXTURE/hanma.png" },	// スピン
	{ "data/TEXTURE/hand.png" },	// ジャンプ

};
bool g_bResultUI[2];				// 終わったかどうか
int g_bSoundUICnt;					// サウンド用カウント
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			g_aUITex[nTex].pFileName,		// ファイルの名前
			&g_pTextureUI[nTex]);	// テクスチャへのポインタ
	}
	// 初期化
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		g_aUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心
		g_aUI[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aUI[nCnt].nType = UITEX_MAX;						// 種類
		g_aUI[nCnt].fWidth = 0;									// 幅
		g_aUI[nCnt].fHeight = 0;									// 高さ
		g_aUI[nCnt].bUse = false;								// 使用されているかどうか
	}
	// 頂点情報の作成
	MakeVertexUI(pDevice);
	g_bResultUI[0] = false;	// リザルトなったかどうか
	g_bResultUI[1] = false;	// リザルトなったかどうか
	g_bSoundUICnt = 0;
	for (int nCnt = 0; nCnt < sizeof g_aUIInfo / sizeof(UIInfo); nCnt++)
	{
		SetUI(g_aUIInfo[nCnt].pos , g_aUIInfo[nCnt].col, g_aUIInfo[nCnt].nType, g_aUIInfo[nCnt].fWidth, g_aUIInfo[nCnt].fHeight );
	}
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitUI(void)
{
	// テクスチャの開放
	for (int nTex = 0; nTex < UITEX_MAX; nTex++)
	{
		if (g_pTextureUI[nTex] != NULL)
		{
			g_pTextureUI[nTex]->Release();
			g_pTextureUI[nTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateUI(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	Player *pPlayer = GetPlayer();
	int nCntPow = 0;
	int nCntPow2 = 0;
//-------------------------------------/ ヒット数に応じたもの /----------------------------------------------------------------//
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == true)
		{
			if (g_aUI[nCntUI].nType == UITEX_BG|| g_aUI[nCntUI].nType == UITEX_BG1)
			{// 黒幕
				if (pPlayer->bAttention == true)
				{
					g_aUI[nCntUI].pos.y += (g_aUIInfo[nCntUI].Stoppos.y - g_aUI[nCntUI].pos.y) / 5;
				}
				else
				{
					g_aUI[nCntUI].pos.y += (g_aUIInfo[nCntUI].pos.y - g_aUI[nCntUI].pos.y) / 10;

				}
			}
	
			if (g_aUI[nCntUI].nType == UITEX_RESULT)
			{
				if (GetGameState() == GAMESTATE_END||GetGameState() == GAMESTATE_END2)
				{
					g_aUI[nCntUI].col.a += 0.005f;
					if (g_aUI[nCntUI].col.a >= 0.5f)
					{
						g_aUI[nCntUI].col.a = 0.5f;
						if (GetResultType() == RESULTTYPE_CLEAR)
						{
							g_bResultUI[0] = true;
						}
						else if (GetResultType() == RESULTTYPE_GAMEOVER)
						{
							g_bResultUI[1] = true;
						}
					}
				}
			
			}	
			if (g_aUI[nCntUI].nType == UITEX_GAMECLEAR)
			{// ゲームクリア
				if (g_bResultUI[0] == true)
				{
					g_bSoundUICnt++;
					g_aUI[nCntUI].col.a = 1.0f;
					g_aUI[nCntUI].fWidth += (g_aUIInfo[nCntUI].fWidth - g_aUI[nCntUI].fWidth)*0.3f;
					g_aUI[nCntUI].fHeight += (g_aUIInfo[nCntUI].fHeight - g_aUI[nCntUI].fHeight)*0.3f;
					if (g_bSoundUICnt == 1)
					{
						PlaySound(SOUND_LABEL_SE_GAMECLEARUI);
					}
				}
			}
			if (g_aUI[nCntUI].nType == UITEX_GAMEOVER)
			{// ゲームオーバー
				if (g_bResultUI[1] == true)
				{
					g_bSoundUICnt++;
					g_aUI[nCntUI].col.a = 1.0f;
					g_aUI[nCntUI].fWidth += (g_aUIInfo[nCntUI].fWidth - g_aUI[nCntUI].fWidth)*0.3f;
					g_aUI[nCntUI].fHeight += (g_aUIInfo[nCntUI].fHeight - g_aUI[nCntUI].fHeight)*0.3f;
					if (g_bSoundUICnt == 1)
					{
						PlaySound(SOUND_LABEL_SE_GAMEOVERUI);
					}
				}
			}
		}
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
		// 頂点カラーの設定
		pVtx[0].col = g_aUI[nCntUI].col;
		pVtx[1].col = g_aUI[nCntUI].col;
		pVtx[2].col = g_aUI[nCntUI].col;
		pVtx[3].col = g_aUI[nCntUI].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();

}


//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nTex = 0; nTex < MAX_UI; nTex++)
	{
		if (g_aUI[nTex].bUse == true)
		{
			if (GetPlayer()->state != PLAYERSTATE_DEATH||g_aUI[nTex].nType == UITEX_RESULT || g_aUI[nTex].nType == UITEX_GAMEOVER || g_aUI[nTex].nType == UITEX_GAMECLEAR)
			{		
					// テクスチャの設定
					pDevice->SetTexture(0, g_pTextureUI[g_aUI[nTex].nType]);

					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
						4 * nTex,
						2);
				
			}
		}
	}
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexUI(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, -g_aUI[nCnt].fHeight/2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aUI[nCnt].fWidth / 2, g_aUI[nCnt].fHeight / 2, 0.0f) + g_aUI[nCnt].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}
		// 頂点カラーの設定
		pVtx[0].col = g_aUI[nCnt].col;
		pVtx[1].col = g_aUI[nCnt].col;
		pVtx[2].col = g_aUI[nCnt].col;
		pVtx[3].col = g_aUI[nCnt].col;
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}
//==============================================================================================================================
// UIの設定
//==============================================================================================================================
void SetUI(D3DXVECTOR3 pos, D3DXCOLOR col, UITEX nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++, pVtx += 4)
	{
		if (g_aUI[nCntUI].bUse == false)
		{
			g_aUI[nCntUI].pos = pos;			// 中心
			g_aUI[nCntUI].col = col;			// 色
			g_aUI[nCntUI].nType = nType;		// 種類
			g_aUI[nCntUI].fWidth = fWidth;		// 幅
			g_aUI[nCntUI].fHeight = fHeight;	// 高さ
			g_aUI[nCntUI].bUse = true;
			// 頂点情報を設定
			pVtx[0].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, -g_aUI[nCntUI].fHeight/2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].fWidth / 2, g_aUI[nCntUI].fHeight / 2, 0.0f) + g_aUI[nCntUI].pos;

			// 頂点カラーの設定
			pVtx[0].col = g_aUI[nCntUI].col;
			pVtx[1].col = g_aUI[nCntUI].col;
			pVtx[2].col = g_aUI[nCntUI].col;
			pVtx[3].col = g_aUI[nCntUI].col;
			
			if (g_aUI[nCntUI].nType == UITEX_GAMECLEAR || g_aUI[nCntUI].nType == UITEX_GAMEOVER)
			{
				g_aUI[nCntUI].fWidth = 3000;		// 幅
				g_aUI[nCntUI].fHeight = 3000;	// 高さ
			}
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}
//==============================================================================================================================
// UIの取得
//==============================================================================================================================
UI *GetUI(void)
{
	return &g_aUI[0];
}
