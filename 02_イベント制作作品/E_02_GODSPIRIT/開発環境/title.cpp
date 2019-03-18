////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// タイトルの処理 [title.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "game.h"
#include "gamepad.h"
#include "camera.h"
#include "meshField.h"
#include "model.h"
#include "shadow.h"
#include "line.h"
#include "orbit.h"
#include "light.h"
#include "sky.h"
#include "meshWall.h"
#include "meshCylinder.h"
#include "effectUP.h"
#include "billboard.h"
#include "effect2D.h"
#include "meshDome.h"
#include "meshDome2.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// ブロックの中心座標
	TITLETYPE nType;			// ブロックの種類
	float fWidth;				// ブロックの幅
	float fHeight;				// ブロックの高さ
} TitleInfo;
typedef struct
{
	char *pFileName;
}TitleTexture;
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define ANIM				(0.33334f)								// アニメーション座標
#define ANIMPATTERN			(3)										// アニメーションパターン
#define ANIMSPEED			(5)									// アニメーションスピード
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		// 頂点バッファへのポインタ
Title g_aTitle[TITLETYPE_MAX];
int g_nPressEnterCounter;
int g_nTitleEndCounter;
int g_nTitleAutoCounter;
bool g_bDecide;
TITLESTATE g_nTitleState;
TitleInfo g_aTitleInfo[] =
{
	//Set    	 pos,								,						nType,	fWidth,	fHeight,	
	{D3DXVECTOR3(900, 250, 0.0f), TITLETYPE_LOGOBG, 600, 600},										// ロゴ背景
	{D3DXVECTOR3(SCREEN_WIDTH / 2 , 300, 0.0f), TITLETYPE_LOGO, 1150, 500},							// ロゴ
	{D3DXVECTOR3(SCREEN_WIDTH / 2 , 600, 0.0f), TITLETYPE_PRESSENTER, 700, 130 },					// PressEnter
};
TitleTexture g_aTitleTex[TITLETYPE_MAX] =
{
	{"data/TEXTURE/TitleLogoBG000.png"},	// 読み込むテクスチャ
	{"data/TEXTURE/title001.png"},		// 読み込むテクスチャ
	{"data/TEXTURE/pressElse000.png"}	,		// 読み込むテクスチャ
};
//==============================================================================================================================
// 初期化処理（ポリゴン）
//==============================================================================================================================
void InitTitle(void)
{
	InitLight();
	InitCamera();
	InitMeshField();
	InitModel();
	InitShadow();
	InitLine();
	InitSky();
	InitMeshWall();

	InitEffect();
	InitEffect2D();

	InitEffectUP();
	InitMeshCylinder();
	InitBillboard();
	SetBillboardFileData("data/BillboardStage1.txt");
	SetStagePos("data/stageTitle.txt");
	// メッシュドームの初期化処理
	InitMeshDome();
	InitMeshDome2();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ
	for (int nTex = 0; nTex < TITLETYPE_MAX; nTex++)		
	{// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				g_aTitleTex[nTex].pFileName,
				&g_pTextureTitle[nTex]);
	}

	// 背景の情報の初期化
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		g_aTitle[nCntTitle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標
		g_aTitle[nCntTitle].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 初期座標
		g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);// 色
		g_aTitle[nCntTitle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_aTitle[nCntTitle].nType = TITLETYPE_LOGOBG;					// タイプ
		g_aTitle[nCntTitle].nCounterAnim = 0;						// アニメーションカウンター
		g_aTitle[nCntTitle].nPatternAnim = 0;						// アニメーションパターン
		g_aTitle[nCntTitle].fWidth = 0;								// 幅
		g_aTitle[nCntTitle].fHeight = 0;							// 高さ
		g_aTitle[nCntTitle].nCounterFade = 0;						// フェードカウンター
		g_aTitle[nCntTitle].nFade = TITLEFADE_FADEOUT;				// ロゴのフェード
		g_aTitle[nCntTitle].bUse = false;							// 使用されているかどうか
	}
	g_aTitle[TITLETYPE_LOGOBG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, -0.5f);// 色
	g_aTitle[TITLETYPE_LOGO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// 色
	g_aTitle[TITLETYPE_PRESSENTER].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// 色
	g_nTitleState = TITLESTATE_NONE;								// タイトルの状態
	g_nPressEnterCounter = 0;										// エンターカウンター
	g_nTitleEndCounter = 0;											// タイトル終わるためのカウンター
	g_nTitleAutoCounter = 0;										// 自動カウンター
	g_bDecide = false;												// 始めているかどうか
	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	// タイトルの設定
	for (int nCntTitle = 0; nCntTitle < sizeof g_aTitleInfo / sizeof(TitleInfo); nCntTitle++)
	{
		SetTitle(g_aTitleInfo[nCntTitle].pos, g_aTitleInfo[nCntTitle].nType, g_aTitleInfo[nCntTitle].fWidth, g_aTitleInfo[nCntTitle].fHeight);
	}

}
//==============================================================================================================================
// 終了処理（ポリゴン）
//==============================================================================================================================
void UninitTitle(void)
{
	UninitLight();
	UninitCamera();
	UninitShadow();
	UninitLine();
	UninitSky();
	UninitEffectUP();
	UninitMeshCylinder();
	UninitBillboard();
	UninitEffect();
	UninitEffect2D();
	// メッシュドームの終了処理
	UninitMeshDome();
	UninitMeshDome2();

	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TITLETYPE_MAX; nCntTex++)
	{
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//==============================================================================================================================
// 更新処理（ポリゴン）
//==============================================================================================================================
void UpdateTitle(void)
{
	UpdateLight();
	UpdateCamera();
	UpdateMeshField();
	UpdateModel();	
	UpdateShadow();
	UpdateLine();
	UpdateOrbit();
	UpdateSky();
	UpdateMeshWall();
	UpdateEffect();
	UpdateEffect2D();
	UpdateEffectUP();
	UpdateMeshCylinder();
	UpdateBillboard();
	// メッシュドームの更新処理
	UpdateMeshDome();
	UpdateMeshDome2();

	// フェードの取得
	FADE pFADE;
	pFADE = GetFade();

	if (g_bDecide == false)
	{
		if (g_aTitle[TITLETYPE_LOGO].col.a >= 1.0f)
		{// ロゴが定位置になったら
			// 決定キー押したかどうか Enterで始める
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(nCntKey) == true)
				{
					g_bDecide = true;
				}

			}
		}
	}
	switch (g_nTitleState)
	{
	case TITLESTATE_NONE:
		g_nTitleAutoCounter++;
		if (pFADE == FADE_NONE)
		{
			if (g_nTitleAutoCounter >= 1500)
			{
				SetFade(MODE_RANKING);
			}
		}
		break;
	case TITLESTATE_END:
		g_nPressEnterCounter++;
		g_nTitleEndCounter++;
		if (g_nTitleEndCounter == 1)
		{
			PlaySound(SOUND_LABEL_SE_JUMP_SPIN);
		}
		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{// しびれエフェクト
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect2D( D3DXVECTOR3(SCREEN_WIDTH/2+ sinf(nDirection)* float(rand() % 500), 550 + (float(rand() % 100)),0.0f),
				D3DXVECTOR3(sinf(nDirection) * float((rand() % 10 / 10.0f)), cosf(nDirection) *  float((rand() % 10 / 10.0f)), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 100) + 1, 10, EFFECTTYPE_2D_SUNDER);
			SetEffect2D(D3DXVECTOR3(SCREEN_WIDTH / 2 + sinf(nDirection)*float(rand() % 500), 550 + (float(rand() % 100)), 0.0f),
				D3DXVECTOR3(sinf(nDirection) *  float((rand() % 10 / 10.0f)), sinf(nDirection2) *  float((rand() % 10 / 10.0f)),0.0f), 
				D3DXCOLOR(1.0f, 1.0f, 0.0f, float(rand() % 100 / 100.0f)), float(rand() % 100) + 1, 10, EFFECTTYPE_2D_SUNDER);
		}
		break;
	}
	if (pFADE == FADE_NONE)
	{
		if (g_bDecide == true)
		{// 決定キー押した
			g_nTitleState = TITLESTATE_END;
			if (g_nTitleEndCounter >= 30)
			{
				SetFade(MODE_TUTORIAL);
			}

		}
	}
//-------------------------/ タイトルの更新情報 /--------------------------------------------------------------------------//
	VERTEX_2D*pVtx;
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		switch (g_aTitle[nCntTitle].nType)
		{
		case TITLETYPE_LOGOBG:
			if (g_aTitle[nCntTitle].nFade != TITLEFADE_NONE)
			{// ロゴの後ろの点滅
				if (g_aTitle[nCntTitle].nFade == TITLEFADE_FADEIN)
				{// 非表示していく
					g_aTitle[nCntTitle].col.a -= 0.01f;
					if (g_aTitle[nCntTitle].col.a <= 0.0f)
					{
						g_aTitle[nCntTitle].nFade = TITLEFADE_FADEOUT;
					}
				}
				if (g_aTitle[nCntTitle].nFade == TITLEFADE_FADEOUT)
				{// 表示していく
					g_aTitle[nCntTitle].col.a += 0.01f;
					if (g_aTitle[nCntTitle].col.a >= 1.5f)
					{
						g_aTitle[nCntTitle].nFade = TITLEFADE_FADEIN;
					}
				}
			}
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				for (int nCntKeyBoard = 0; nCntKeyBoard < NUM_KEY_MAX; nCntKeyBoard++)
				{
					if (GetKeyboardTrigger(nCntKeyBoard) == true || GetGamePadTrigger(nCntKey) == true)
					{
						g_aTitle[nCntTitle].col.a = 1.0f;
					}
				}
			}
			break;
		case TITLETYPE_LOGO:
			g_aTitle[nCntTitle].col.a += 0.005f;
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				for (int nCntKeyBoard = 0; nCntKeyBoard < NUM_KEY_MAX; nCntKeyBoard++)
				{
					if (GetKeyboardTrigger(nCntKeyBoard) == true || GetGamePadTrigger(nCntKey) == true)
					{
						g_aTitle[nCntTitle].col.a = 1.0f;
					}
				}
			}
			break;
		case TITLETYPE_PRESSENTER:
			if (g_aTitle[TITLETYPE_LOGO].col.a >= 1.0f)
			{
				g_nPressEnterCounter++;
				g_nPressEnterCounter = g_nPressEnterCounter % 120;
				if (g_nPressEnterCounter <= 60)
				{
					g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				else
				{
					g_aTitle[nCntTitle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			break;
		}
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		// 頂点カラーの設定
		pVtx[0].col = g_aTitle[nCntTitle].col;
		pVtx[1].col = g_aTitle[nCntTitle].col;
		pVtx[2].col = g_aTitle[nCntTitle].col;
		pVtx[3].col = g_aTitle[nCntTitle].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


}
//==============================================================================================================================
// 描画処理（ポリゴン）
//==============================================================================================================================
void DrawTitle(void)
{
	SetCamera(0);
	// メッシュドームの更新処理
	DrawMeshDome();
	DrawMeshDome2();

	DrawMeshField();
	DrawModel();	
	DrawShadow();
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		DrawLine();
	}
	DrawOrbit();
//	DrawSky();
	DrawMeshWall();
	DrawEffect2D();
	DrawEffect();
	DrawEffectUP();
//	DrawMeshCylinder();
	DrawBillboard();
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[g_aTitle[nCntTitle].nType]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
								nCntTitle * 4,
								2);
	}

}
//==============================================================================================================================
// タイトルの設定
//==============================================================================================================================
void SetTitle(D3DXVECTOR3 pos, TITLETYPE nType,float fWidth,float fHeight)
{
	VERTEX_2D*pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		if (g_aTitle[nCntTitle].bUse == false)
		{// 背景が使用されていない
			g_aTitle[nCntTitle].pos = pos;
			g_aTitle[nCntTitle].Initpos = pos;
			g_aTitle[nCntTitle].nType = nType;
			g_aTitle[nCntTitle].fWidth = fWidth;
			g_aTitle[nCntTitle].fHeight = fHeight;

			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight/ 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;

			g_aTitle[nCntTitle].bUse = true;	// 使用している状態にする
			break;
		}
	}
	g_pVtxBuffTitle->Unlock();
}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), -(g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aTitle[nCntTitle].fWidth / 2), (g_aTitle[nCntTitle].fHeight / 2), 0.0f) + g_aTitle[nCntTitle].pos;
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
		pVtx[0].col = g_aTitle[nCntTitle].col;
		pVtx[1].col = g_aTitle[nCntTitle].col;
		pVtx[2].col = g_aTitle[nCntTitle].col;
		pVtx[3].col = g_aTitle[nCntTitle].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

}