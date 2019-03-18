//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
//
// チュートリアル処理 [tutorial.cpp]
// Author : 圷 和也
//
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
#include "main.h"
#include "tutorial.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// マクロ定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
#define MAX_TUTORIAL				(4)							// チュートリアルの最大数
#define MAX_TUTORIALPAGE			(3)							// チュートリアルのスライドの枚数
#define TUTORIALPAGE_PATTERN_ANIM	float(1.0f / float(MAX_TUTORIALPAGE))	// ページの加算数
#define TUTORIAL_DISP_NORMAL		(30)						// 通常時の点滅の間隔
#define TUTORIAL_DISP_FAST			(2)							// 決定時の点滅の間隔
#define SLIDE_TIME					(20)						// スクロールスピード

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// 構造体定義
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
typedef enum
{
	TUTORIAL_TEX_MAIN = 0,		// メイン画像
	TUTORIAL_TEX_PRESS_START,	// 押してください系
	TUTORIAL_TEX_ARROWW_RIGHT,	// 矢印右
	TUTORIAL_TEX_ARROWW_LEFT,	// 矢印左
	TUTORIAL_TEX_MAX,			// 総数
}TutorialTexName;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	TUTORIALTYPE type;	// 種類
	D3DXCOLOR	col;	// 色
	int nTexType;		// テクスチャの種類
	float fWidth;			// 横幅
	float fHeight;		// 縦幅
	int nMoveCounter;	// 様々な動きのカウンター 
	int nDispSpeed;		// 点滅のスピード
	bool bDisp;			// 表示
	bool bUse;			// 使用しているかどうか
}Tutorial;

typedef struct
{
	char *pFilename;	// ファイル名
} TutorialTexInfo;

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// プロトタイプ宣言
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice);

//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
// グローバル変数
//*-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**
LPDIRECT3DTEXTURE9 g_pTextureTutorial[TUTORIAL_TEX_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// 頂点バッファへのポインタ
Tutorial g_aTutorial[MAX_TUTORIAL];								// チュートリアルの情報

// 読み込むテクスチャ名の設定
TutorialTexInfo g_apTextureTutorialInfo[TUTORIAL_TEX_MAX] =
{
	{ "data//TEXTURE//tutorial004.png" },		// メイン画像
	{ "data//TEXTURE//press_enter.png" },		// 押してください系
	{ "data//TEXTURE//arrowRight.png" },		// 矢印右
	{ "data//TEXTURE//arrowLeft.png" },			// 矢印左
};

int g_nCntPage;				// 現在のページ数をカウント
int g_nSlideFrameCnt;		// スライド時間のカウンター
bool g_bSlideUse;			// スライド中かどうか
float g_fSlideTexAnim;		// スライドのテクスチャの現在の値
float g_fSlideTexAnimAdd;	// スライドのテクスチャの増加値
int g_nDispCounter;			// 点滅のカウンター

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// チュートリアル初期化処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// チュートリアルの情報の初期化
	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTutorial[nCntTutorial].type = TUTORIALTYPE_MAIN;
		g_aTutorial[nCntTutorial].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aTutorial[nCntTutorial].nTexType = TUTORIAL_TEX_MAIN;
		g_aTutorial[nCntTutorial].nDispSpeed = TUTORIAL_DISP_NORMAL;
		g_aTutorial[nCntTutorial].fWidth = 0;
		g_aTutorial[nCntTutorial].fHeight = 0;
		g_aTutorial[nCntTutorial].bDisp = false;
		g_aTutorial[nCntTutorial].bUse = false;
	}

	g_nCntPage = 0;		
	g_nSlideFrameCnt = 0;
	g_bSlideUse = false;	
	g_fSlideTexAnim = 0.0f;
	g_fSlideTexAnimAdd = 0.0f;
	g_nDispCounter = 0;

	// テクスチャの読み込み
	for (int nCntTutorialTex = 0; nCntTutorialTex < TUTORIAL_TEX_MAX; nCntTutorialTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_apTextureTutorialInfo[nCntTutorialTex].pFilename,
			&g_pTextureTutorial[nCntTutorialTex]);
	}

	// 頂点情報
	MakeVertexTutorial(pDevice);	

	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_MAIN,(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	// プリーズエンター
	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH -260), SCREEN_HEIGHT -50, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_PRESS_START, 480.0f, 80.0f);
	SetTutorial(D3DXVECTOR3(float(SCREEN_WIDTH - 40.0f), float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_ARROW_RIGHT, 70.0f, 200.0f);
	SetTutorial(D3DXVECTOR3(40.0f, float(SCREEN_HEIGHT / 2), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TUTORIALTYPE_ARROW_LEFT, 70.0f, 200.0f);
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// チュートリアル終了処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UninitTutorial(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TUTORIAL_TEX_MAX; nCntTex++)
	{
		if (g_pTextureTutorial[nCntTex] != NULL)
		{
			g_pTextureTutorial[nCntTex]->Release();
			g_pTextureTutorial[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// チュートリアル更新処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void UpdateTutorial(void)
{
	FADE pFade;
	// フェードモードの取得
	pFade = GetFade();
	g_nSlideFrameCnt++;
	g_nDispCounter++;

	if (g_bSlideUse == false)
	{ // スライド中で無い場合

		if ((GetKeyboardTrigger(DIK_RIGHT) == true || GetKeyboardTrigger(DIK_D) == true || GetGamePadTrigger(BUTTON_R1) == true
			|| GetPovTrigger(POV_RIGHT) == true || GetGamePadStickTrigger(ANALOG_STICK_RIGHT) == true) && g_nCntPage != MAX_TUTORIALPAGE-1)
		{ // ページが最大値でない場合
			// ページを進める
			g_nCntPage++;
			g_bSlideUse = true;
			g_nSlideFrameCnt = 0;
			g_fSlideTexAnimAdd = float(TUTORIALPAGE_PATTERN_ANIM / SLIDE_TIME);
			// SEの再生
			PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else if ((GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_A) == true || GetGamePadTrigger(BUTTON_L1) == true
			|| GetPovTrigger(POV_LEFT) == true || GetGamePadStickTrigger(ANALOG_STICK_LEFT) == true) && g_nCntPage != 0)
		{ // ページが最小値でない場合
			// ページ戻す
			g_nCntPage--;
			g_bSlideUse = true;
			g_nSlideFrameCnt = 0;
			g_fSlideTexAnimAdd = float(-TUTORIALPAGE_PATTERN_ANIM / SLIDE_TIME);
			// SEの再生
			PlaySound(SOUND_LABEL_SE_SELECT);
		}

	}

	// スライドのテクスチャの値に増加値を加算
	g_fSlideTexAnim += g_fSlideTexAnimAdd;

	if (g_nSlideFrameCnt == SLIDE_TIME && g_bSlideUse == true)
	{ // スライド中でフレームカウンターが規定値に達した場合
		g_nSlideFrameCnt = 0;
		g_bSlideUse = false;
		g_fSlideTexAnimAdd = 0.0f;
		g_fSlideTexAnim = g_nCntPage * TUTORIALPAGE_PATTERN_ANIM;
	}
	
	if (pFade == FADE_NONE)
	{ // フェード中かどうか
		if (g_nCntPage == MAX_TUTORIALPAGE-1 && (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(ELECOM_A) == true || GetGamePadTrigger(ELECOM_START) == true))
		{ // 決定キーが押されたかどうか
			// SEの再生
			PlaySound(SOUND_LABEL_SE_DECIDE);

			// モード設定
			SetFade(MODE_GAME);

			for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
			{
				if (g_aTutorial[nCntTutorial].bUse == true)
				{
					if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_PRESS_START)
					{
						g_aTutorial[nCntTutorial].nDispSpeed = TUTORIAL_DISP_FAST;
					}
				}
			}
		}
	}

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (g_aTutorial[nCntTutorial].bUse == true)
		{
			if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_MAIN)
			{ // スライド
				pVtx[0].tex = D3DXVECTOR2(g_fSlideTexAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_fSlideTexAnim + TUTORIALPAGE_PATTERN_ANIM, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_fSlideTexAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_fSlideTexAnim + TUTORIALPAGE_PATTERN_ANIM, 1.0f);
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_PRESS_START)
			{ // プッシュボタン
				if (g_nCntPage == MAX_TUTORIALPAGE - 1 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_ARROW_RIGHT)
			{ // プッシュボタン
				if (g_nCntPage != MAX_TUTORIALPAGE - 1 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
			else if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_ARROW_LEFT)
			{ // プッシュボタン
				if (g_nCntPage != 0 && g_bSlideUse == false)
				{
					if (g_nDispCounter % g_aTutorial[nCntTutorial].nDispSpeed == 0)
					{
						g_aTutorial[nCntTutorial].bDisp = g_aTutorial[nCntTutorial].bDisp ? false : true;
					}
				}
				else
				{
					g_aTutorial[nCntTutorial].bDisp = false;
				}
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// チュートリアル描画処理
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (g_aTutorial[nCntTutorial].bUse == true)
		{ // 敵が使用されている
			if (g_aTutorial[nCntTutorial].bDisp == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pTextureTutorial[g_aTutorial[nCntTutorial].nTexType]);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
			}
		}
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// Tutorialの設定
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void SetTutorial(D3DXVECTOR3 pos, D3DXCOLOR col, TUTORIALTYPE type, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++,pVtx+=4)
	{
		if (g_aTutorial[nCntTutorial].bUse == false)
		{ // タイトルが使用されていない場合
		  // 頂点位置の設定
			g_aTutorial[nCntTutorial].pos = pos;
			g_aTutorial[nCntTutorial].fWidth = fWidth;
			g_aTutorial[nCntTutorial].fHeight = fHeight;
			// 色の設定
			g_aTutorial[nCntTutorial].col = col;
			// チュートリアルの種類の設定
			g_aTutorial[nCntTutorial].type = type;
			g_aTutorial[nCntTutorial].nTexType = type;
			
			 // 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y - g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y - g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x - g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nCntTutorial].pos.x + g_aTutorial[nCntTutorial].fWidth / 2, g_aTutorial[nCntTutorial].pos.y + g_aTutorial[nCntTutorial].fHeight / 2, 0.0f);
	
			pVtx[0].col = g_aTutorial[nCntTutorial].col;
			pVtx[1].col = g_aTutorial[nCntTutorial].col;
			pVtx[2].col = g_aTutorial[nCntTutorial].col;
			pVtx[3].col = g_aTutorial[nCntTutorial].col;

			if (g_aTutorial[nCntTutorial].type == TUTORIALTYPE_MAIN)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(TUTORIALPAGE_PATTERN_ANIM, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(TUTORIALPAGE_PATTERN_ANIM, 1.0f);
			}

			g_aTutorial[nCntTutorial].bDisp = true;	// 表示状態にする
			g_aTutorial[nCntTutorial].bUse = true;	// 使用する状態にする			
			break;	// このbreakはかなり大切
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffTutorial->Unlock();
	}
}

//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
// 頂点情報の作成
//=+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==
void MakeVertexTutorial(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

}