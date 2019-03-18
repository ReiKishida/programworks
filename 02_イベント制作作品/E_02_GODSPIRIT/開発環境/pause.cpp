////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ポーズの処理 [pause.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "gamepad.h"
#include "tutorial.h"
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXCOLOR col;				// 色
	PAUSETYPE nType;			// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
} PauseInfo;
typedef struct
{
	char *pFileName;
}PauseTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define PAUSESELECT_MAX	(3)
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSETYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		// 頂点バッファへのポインタ
PAUSE g_aPause[PAUSETYPE_MAX];
int g_nSelect;
bool g_bSelect;
int g_nPressCounter;
PauseInfo g_aPauseInfo[] =
{
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), PAUSETYPE_BG,SCREEN_WIDTH,SCREEN_HEIGHT },			// 背景
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSETYPE_PAUSE ,SCREEN_WIDTH ,SCREEN_HEIGHT },	// ポーズ土台
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 320.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_CONTINUE ,290,70 },		// 再開
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 260, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_RETRY ,380,90 },				// もう一度踊る
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), PAUSETYPE_QUIT,380,90 },				// タイトルへ戻る
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 353, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_PUNCH,90,130 },// パンチ->パンチ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 177, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_PUNCH_PUNCH,90,130 },// パンチ->パンチ->パンチ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2		, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_SPIN,90,130 },// パンチ->スピン
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 173, SCREEN_HEIGHT / 2 - 275, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_PUNCH_SPIN_JUMP,90,130 },// パンチ->スピン->ジャンプ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_KICK,90,130 },// キック->キック
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 176, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_KICK_KICK,90,130 },// キック->キック->キック
	{ D3DXVECTOR3(SCREEN_WIDTH / 2		, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_SPIN,90,130 },// キック->スピン
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 173, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_JUMP,90,130 },// キック->ジャンプ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 335, SCREEN_HEIGHT / 2 - 98 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_KICK_JUMP_PUNCH,90,130 },// キック->ジャンプ->パンチ	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 355, SCREEN_HEIGHT / 2 + 82 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_PUNCH,90,130 },// スピン->パンチ
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 + 80 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_KICK,90,130 },// スピン->キック
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 5  , SCREEN_HEIGHT / 2 + 80 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_SPIN_PUNCH_KICK,90,130 },// スピン->パンチ->キック	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_KICK,90,130 },// ジャンプ->キック	
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 - 173, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_SPIN,90,130 },// ジャンプ->スピン
	{ D3DXVECTOR3(SCREEN_WIDTH / 2 + 2, SCREEN_HEIGHT / 2 + 265 , 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSE_LOCK_JUMP_PUNCH,90,130 },// ジャンプ->パンチ
};
PauseTexture g_aPauseTex[PAUSETYPE_MAX] =
{
	{ "data / TEXTURE / " },	// 読み込むテクスチャ
	{ "data/TEXTURE/pauseBG.jpg" },	// 読み込むテクスチャ1
	{ "data/TEXTURE/menu000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/menu001.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/menu002.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/Lock000.png" },		// 読み込むテクスチャ2
};
//==============================================================================================================================
// 初期化処理（ポリゴン）
//==============================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D*pVtx;				// 頂点情報へのポインタ
								// デバイスの取得
	pDevice = GetDevice();

	for (int nTex = 0; nTex < PAUSETYPE_MAX; nTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			g_aPauseTex[nTex].pFileName,
			&g_apTexturePause[nTex]);
	}
	// 背景の情報の初期化
	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期座標
		g_aPause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色
		g_aPause[nCntPause].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 背景の移動量
		g_aPause[nCntPause].nType = 0;								// タイプ
		g_aPause[nCntPause].nCounterAnim = 0;						// カウンター
		g_aPause[nCntPause].nPatternAnim = 0;						// パターンNo
		g_aPause[nCntPause].fWidth = 0;								// 幅
		g_aPause[nCntPause].fHeight = 0;							// 高さ
		g_aPause[nCntPause].bUse = false;							// 使用されているかどうか
	}
	g_nSelect = 0;
	g_bSelect = false;
	g_nPressCounter = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}

		// 頂点カラーの設定
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	// 情報のセット
	for (int nCntPause = 0; nCntPause < sizeof g_aPauseInfo / sizeof(PauseInfo); nCntPause++)
	{
		SetPause(g_aPauseInfo[nCntPause].pos, g_aPauseInfo[nCntPause].col, g_aPauseInfo[nCntPause].nType, g_aPauseInfo[nCntPause].fWidth, g_aPauseInfo[nCntPause].fHeight);
	}
}
//==============================================================================================================================
// 終了処理（ポリゴン）
//==============================================================================================================================
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < PAUSETYPE_MAX; nCntTex++)
	{
		if (g_apTexturePause[nCntTex] != NULL)
		{
			g_apTexturePause[nCntTex]->Release();
			g_apTexturePause[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//==============================================================================================================================
// 更新処理（ポリゴン）
//==============================================================================================================================
void UpdatePause(void)
{
	VERTEX_2D*pVtx;
	FADE pFADE;
	pFADE = GetFade();

	if (pFADE == FADE_NONE)
	{
		if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_CONTINUE)
		{// continu
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// ゲームなら
					ReleaseGamePause();
				}
			
			}
		}
		else if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_RETRY)
		{// retry
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// ゲームなら
					SetFade(MODE_GAME);
				}
				if (GetMode() == MODE_TUTORIAL)
				{// チュートリアルなら
					SetFade(MODE_TUTORIAL);
				}
			}
		}
		else if (g_nSelect + PAUSETYPE_CONTINUE == PAUSETYPE_QUIT)
		{// quit
			g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
			{

				PlaySound(SOUND_LABEL_SE_DECIDE);
				if (GetMode() == MODE_GAME)
				{// ゲームなら
					SetFade(MODE_TITLE);
					SetGameState(GAMESTATE_NONE);
				}
				
			}
		}
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetGamePadStickTrigger(ANALOG_STICK_DOWN) == true)
	{// 下に移動(単体）
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = (g_nSelect + 1) % PAUSESELECT_MAX;
	}
	else if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true || GetGamePadStickTrigger(ANALOG_STICK_UP) == true)
	{// 上に移動(単体)
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = (g_nSelect + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;
	}
	if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true || GetGamePadStickPress(ANALOG_STICK_DOWN) == true)
	{// 下に移動(連続)
		g_nPressCounter++;
		if (g_nPressCounter >= 30)
		{
			if (g_nPressCounter % 10 == 0)
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				g_nSelect = (g_nSelect + 1) % PAUSESELECT_MAX;
			}
		}
	}
	else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true || GetGamePadStickPress(ANALOG_STICK_UP) == true)
	{// 上に移動(連続)
		g_nPressCounter++;
		if (g_nPressCounter >= 30)
		{
			if (g_nPressCounter % 10 == 0)
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
				g_aPause[g_nSelect + PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				g_nSelect = (g_nSelect + (PAUSESELECT_MAX - 1)) % PAUSESELECT_MAX;
			}
		}
	}
	if (GetKeyboardTrigger(DIK_W) == true || GetGamePadStickTrigger(ANALOG_STICK_UP) == true)
	{// 離したらカウンターリセット
		g_nPressCounter = 0;
	}
	else if (GetKeyboardTrigger(DIK_S) == true || GetGamePadStickTrigger(ANALOG_STICK_DOWN) == true)
	{// 離したらカウンターリセット
		g_nPressCounter = 0;
	}
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(BUTTON_START) == true)
	{
		g_aPause[PAUSETYPE_QUIT].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_aPause[PAUSETYPE_RETRY].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_aPause[PAUSETYPE_CONTINUE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		g_nSelect = 0;
	}

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		// 頂点カラーの設定
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================================
// 描画処理（ポリゴン）
//==============================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
								// デバイスを取得する
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		if (g_aPause[nCntPause].bUse == true)
		{
			// 頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePause[g_aPause[nCntPause].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntPause * 4,
				2);
		}
	}
}
//==============================================================================================================================
// ポーズの設定
//==============================================================================================================================
void SetPause(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++, pVtx += 4)
	{
		if (g_aPause[nCntPause].bUse == false)
		{// 背景が使用されていない
			g_aPause[nCntPause].pos = pos;
			g_aPause[nCntPause].Initpos = pos;
			g_aPause[nCntPause].col = col;
			g_aPause[nCntPause].nType = nType;
			g_aPause[nCntPause].fWidth = fWidth;
			g_aPause[nCntPause].fHeight = fHeight;

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), -(g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aPause[nCntPause].fWidth / 2), (g_aPause[nCntPause].fHeight / 2), 0.0f) + g_aPause[nCntPause].pos;

			g_aPause[nCntPause].bUse = true;	// 使用している状態にする
			break;
		}
	}
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================================
// ポーズの獲得
//==============================================================================================================================
PAUSE *GetPauseData(void)
{
	return &g_aPause[0];
}
