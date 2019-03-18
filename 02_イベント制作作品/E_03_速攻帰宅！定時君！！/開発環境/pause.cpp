//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ポーズ処理 [pause.cpp]
// Author : suzuki
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "gamepad.h"
#include "sound.h"
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// マクロ定義
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define MAX_TOPPAUSE			(4)								// 頂点数
#define PAUSE_TEXTURENAME		"data/TEXTURE/PauseBG000.png"	// 読み込むテクスチャの名前
#define PAUSE_TEXTURENAME1		"data/TEXTURE/Continue000.png"	// 読み込むテクスチャの名前
#define PAUSE_TEXTURENAME2		"data/TEXTURE/ReStart000.png"	// 読み込むテクスチャの名前
#define PAUSE_TEXTURENAME3		"data/TEXTURE/Select000.png"	// 読み込むテクスチャの名前
#define PAUSE_TEXTURENAME4		"data/TEXTURE/Select001.png"	// 読み込むテクスチャの名前
#define PAUSE_TEXTURENAME5		"data/TEXTURE/TitleQuit000.png"	// 読み込むテクスチャの名前
#define PAUSE_MENUBG_WIDTH		(350)							// 背景の幅
#define PAUSE_MENUBG_HEIGHT		(300)							// 背景の高さ
#define PAUSE_CONTINUE_WIDTH	(200)							// メニュー[続ける]の幅
#define PAUSE_CONTINUE_HEIGHT	(60)							// メニュー[続ける]の高さ
#define PAUSE_RETRY_WIDTH		(200)							// メニュー[やり直し]の幅
#define PAUSE_RETRY_HEIGHT		(60)							// メニュー[やり直し]の高さ
#define PAUSE_CHARA_WIDTH		(200)							// メニュー[キャラ選択]の幅
#define PAUSE_CHARA_HEIGHT		(60)							// メニュー[キャラ選択]の高さ
#define PAUSE_MODE_WIDTH		(200)							// メニュー[モード選択]の幅
#define PAUSE_MODE_HEIGHT		(60)							// メニュー[モード選択]の高さ
#define PAUSE_QUIT_WIDTH		(200)							// メニュー[タイトルに戻る]の幅
#define PAUSE_QUIT_HEIGHT		(60)							// メニュー[タイトルに戻る]の高さ
#define PAUSE_SELECTSPEED		(15)							// メニューの選択スピード

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// プロトタイプ
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetSelectCol(void);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// グローバル変数
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
LPDIRECT3DTEXTURE9	g_pTexturePause[PAUSETEX_MAX + PAUSEMENU_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;
D3DXVECTOR3 g_aPos[PAUSETEX_MAX + PAUSEMENU_MAX];		// 位置
int g_nCntPress;										// キーの押してる長さ
bool g_bDrawPause;
int g_nSelectMenu;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ポリゴンの初期化
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitPause(void)
{
	int nCntVertex;
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	LPDIRECT3DDEVICE9 pDevice;
	g_aPos[PAUSETEX_BG] = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aPos[PAUSETEX_MENU] = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX] = D3DXVECTOR3(SCREEN_WIDTH / 2, 150.0f, 0.0f);
	g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX] = D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f);
	g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX] = D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f);
	g_nCntPress = 0;
	g_bDrawPause = true;
	g_nSelectMenu = 0;

	// デバイスの所得
	pDevice = GetDevice();

	// テクスチャの読み
	D3DXCreateTextureFromFile(pDevice,
		"",
		&g_pTexturePause[PAUSETEX_BG]);
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_TEXTURENAME,
		&g_pTexturePause[PAUSETEX_MENU]);
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_TEXTURENAME1,
		&g_pTexturePause[PAUSEMENU_CONTINUE + PAUSETEX_MAX]);
	D3DXCreateTextureFromFile(pDevice,
		PAUSE_TEXTURENAME2,
		&g_pTexturePause[PAUSEMENU_RETRY + PAUSETEX_MAX]);

	D3DXCreateTextureFromFile(pDevice,
		PAUSE_TEXTURENAME5,
		&g_pTexturePause[PAUSEMENU_QUIT + PAUSETEX_MAX]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (PAUSETEX_MAX + PAUSEMENU_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 頂点lバッファをロックし頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < PAUSETEX_MAX + PAUSEMENU_MAX; nCntTex++, pVtx += 4)
	{
		if (nCntTex == PAUSETEX_BG)
		{
			// 座標
			pVtx[0].pos = D3DXVECTOR3(g_aPos[PAUSETEX_BG].x, g_aPos[PAUSETEX_BG].y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPos[PAUSETEX_BG].x, g_aPos[PAUSETEX_BG].y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPos[PAUSETEX_BG].x, g_aPos[PAUSETEX_BG].y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPos[PAUSETEX_BG].x, g_aPos[PAUSETEX_BG].y, 0.0f);
		}
		else if (nCntTex == PAUSETEX_MENU)
		{
			// 座標
			pVtx[0].pos = D3DXVECTOR3(g_aPos[PAUSETEX_MENU].x - PAUSE_MENUBG_WIDTH, g_aPos[PAUSETEX_MENU].y - PAUSE_MENUBG_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPos[PAUSETEX_MENU].x + PAUSE_MENUBG_WIDTH, g_aPos[PAUSETEX_MENU].y - PAUSE_MENUBG_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPos[PAUSETEX_MENU].x - PAUSE_MENUBG_WIDTH, g_aPos[PAUSETEX_MENU].y + PAUSE_MENUBG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPos[PAUSETEX_MENU].x + PAUSE_MENUBG_WIDTH, g_aPos[PAUSETEX_MENU].y + PAUSE_MENUBG_HEIGHT, 0.0f);
		}
		else if (nCntTex == PAUSEMENU_CONTINUE + PAUSETEX_MAX)
		{// コンティニュー
			// 座標
			pVtx[0].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].x - PAUSE_CONTINUE_WIDTH, g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].y - PAUSE_CONTINUE_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].x + PAUSE_CONTINUE_WIDTH, g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].y - PAUSE_CONTINUE_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].x - PAUSE_CONTINUE_WIDTH, g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].y + PAUSE_CONTINUE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].x + PAUSE_CONTINUE_WIDTH, g_aPos[PAUSEMENU_CONTINUE + PAUSETEX_MAX].y + PAUSE_CONTINUE_HEIGHT, 0.0f);
		}
		else if (nCntTex == PAUSEMENU_RETRY + PAUSETEX_MAX)
		{// リトライ
			// 座標
			pVtx[0].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].x - PAUSE_RETRY_WIDTH, g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].y - PAUSE_RETRY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].x + PAUSE_RETRY_WIDTH, g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].y - PAUSE_RETRY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].x - PAUSE_RETRY_WIDTH, g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].y + PAUSE_RETRY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].x + PAUSE_RETRY_WIDTH, g_aPos[PAUSEMENU_RETRY + PAUSETEX_MAX].y + PAUSE_RETRY_HEIGHT, 0.0f);
		}

		else if (nCntTex == PAUSEMENU_QUIT + PAUSETEX_MAX)
		{// 戻る
			// 座標
			pVtx[0].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].x - PAUSE_QUIT_WIDTH, g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].y - PAUSE_QUIT_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].x + PAUSE_QUIT_WIDTH, g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].y - PAUSE_QUIT_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].x - PAUSE_QUIT_WIDTH, g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].y + PAUSE_QUIT_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].x + PAUSE_QUIT_WIDTH, g_aPos[PAUSEMENU_QUIT + PAUSETEX_MAX].y + PAUSE_QUIT_HEIGHT, 0.0f);
		}

		// rhw
		for (nCntVertex = 0; nCntVertex < MAX_TOPPAUSE; nCntVertex++)
		{
			pVtx[nCntVertex].rhw = 1.0f;
		}

		if (nCntTex == PAUSETEX_MENU)
		{
			// 色
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// 色
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < PAUSETEX_MAX + PAUSEMENU_MAX; nCntTex++)
	{
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdatePause(bool *pPause)
{
	if (GetFade() == FADE_NONE)
	{

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{// スペースキーが押された場合
			g_bDrawPause = g_bDrawPause ? false : true;	// ポーズ描画ON/OFF
		}
		else if (g_bDrawPause == false)
		{
			return;
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(0, BUTTON_B))
		{
			FADE pFade;
			pFade = GetFade();
			if (g_nSelectMenu == PAUSEMENU_CONTINUE)
			{// 続ける
				*pPause = false;
			}
			else if (g_nSelectMenu == PAUSEMENU_RETRY)
			{// リトライ
				if (pFade == FADE_NONE)
				{
					PlaySound(SOUND_DECISION_1);

					SetFade(MODE_GAME);
				}
			}
			else if (g_nSelectMenu == PAUSEMENU_QUIT)
			{// タイトルに戻る
				if (pFade == FADE_NONE)
				{
					PlaySound(SOUND_DECISION_1);
					SetFade(MODE_GAME);
				}
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{// 上に移動
			if (GetKeyboardTrigger(DIK_W) == true)
			{// トリガー
				g_nSelectMenu = (g_nSelectMenu + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;
				SetSelectCol();
				PlaySound(SOUND_SELECTMOVE);
				g_nCntPress = 0;
			}
			g_nCntPress--;			// カウントを加算する
			if (g_nCntPress <= -PAUSE_SELECTSPEED)
			{// 長く押したとき
				g_nSelectMenu = (g_nSelectMenu + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;
				PlaySound(SOUND_SELECTMOVE);
				SetSelectCol();
				g_nCntPress = 0;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{// 下に移動
			if (GetKeyboardTrigger(DIK_S) == true)
			{// トリガー
				g_nSelectMenu = (g_nSelectMenu + 1) % PAUSEMENU_MAX;
				PlaySound(SOUND_SELECTMOVE);
				SetSelectCol();
				g_nCntPress = 0;
			}
			g_nCntPress++;			// カウントを加算する
			if (g_nCntPress >= PAUSE_SELECTSPEED)
			{// 長く押したとき
				g_nSelectMenu = (g_nSelectMenu + 1) % PAUSEMENU_MAX;
				PlaySound(SOUND_SELECTMOVE);
				SetSelectCol();
				g_nCntPress = 0;
			}
		}
		for (int nCnt = 0; nCnt < MAX_GAMEPAD; nCnt++)
		{
			if (GetGamePadTrigger(nCnt, BUTTON_B))
			{
				FADE pFade;
				pFade = GetFade();
				if (g_nSelectMenu == PAUSEMENU_CONTINUE)
				{// 続ける
					*pPause = false;
				}
				else if (g_nSelectMenu == PAUSEMENU_RETRY)
				{// リトライ
					if (pFade == FADE_NONE)
					{
						SetFade(MODE_GAME);
					}
				}

				else if (g_nSelectMenu == PAUSEMENU_QUIT)
				{// タイトルに戻る
					if (pFade == FADE_NONE)
					{
						SetFade(MODE_GAME);
					}
				}
			}
			else if (GetGamePadStickPress(nCnt, ANALOG_STICK_UP) == true)
			{// 上に移動
				if (GetGamePadStickTrigger(nCnt, ANALOG_STICK_UP) == true)
				{// トリガー
					g_nSelectMenu = (g_nSelectMenu + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;
					SetSelectCol();
					PlaySound(SOUND_SELECTMOVE);
					g_nCntPress = 0;
				}
				g_nCntPress--;			// カウントを加算する
				if (g_nCntPress <= -PAUSE_SELECTSPEED)
				{// 長く押したとき
					g_nSelectMenu = (g_nSelectMenu + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;
					PlaySound(SOUND_SELECTMOVE);
					SetSelectCol();
					g_nCntPress = 0;
				}
			}
			else if (GetGamePadStickPress(nCnt, ANALOG_STICK_DOWN) == true)
			{// 下に移動
				if (GetGamePadStickTrigger(nCnt, ANALOG_STICK_DOWN) == true)
				{// トリガー
					g_nSelectMenu = (g_nSelectMenu + 1) % PAUSEMENU_MAX;
					PlaySound(SOUND_SELECTMOVE);
					SetSelectCol();
					g_nCntPress = 0;
				}
				g_nCntPress++;			// カウントを加算する
				if (g_nCntPress >= PAUSE_SELECTSPEED)
				{// 長く押したとき
					g_nSelectMenu = (g_nSelectMenu + 1) % PAUSEMENU_MAX;
					PlaySound(SOUND_SELECTMOVE);
					SetSelectCol();
					g_nCntPress = 0;
				}
			}

		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを所得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bDrawPause == true)
	{
		for (int nCntTex = 0; nCntTex < PAUSETEX_MAX + PAUSEMENU_MAX; nCntTex++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePause[nCntTex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntTex,
				2);
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点カラーの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetSelectCol(void)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	// 頂点lバッファをロックし頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * PAUSETEX_MAX;
	for (int nCntTex = 0; nCntTex < PAUSEMENU_MAX; nCntTex++, pVtx += 4)
	{
		// 頂点カラーの設定
		if (nCntTex == g_nSelectMenu)
		{// 選択してる
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ポーズ画面の設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetPause(bool bDraw, int nSelect)
{
	g_bDrawPause = bDraw;
	g_nSelectMenu = nSelect;
	SetSelectCol();
}
