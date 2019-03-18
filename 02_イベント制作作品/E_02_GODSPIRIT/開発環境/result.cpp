////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// リザルトの処理 [result.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "gamepad.h"
#include "UI.h"

#include "camera.h"
#include "light.h"
#include "effectUP.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "enemy.h"
#include "effect.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshCylinder.h"
#include "sky.h"
#include "pause.h"
#include "fade.h"
#include "item.h"
#include "line.h"
#include "orbit.h"
#include "billboard.h"
#include "BillLife.h"
#include "effect2D.h"
#include "light.h"
#include "MessageWindow.h"
#include "effect3D.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	RESULTTEXTYPE_GAMECLEAR nType;// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	int nNumCnt;				// 文字数
} ResultInfoGameCrear;
typedef struct
{
	char *pFileName;
}ResultTextureGameCrear;

//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_RESULT_CLEAR	(50)									// リザルトクリア
#define SCORE_LEN			(8)
#define TIME_LEN			(3)
#define HIT_LEN				(3)
#define ARROW_LEN				(3)
#define ITEM_LEN			(1)
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexGameCrear(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_CLEAR] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
RESULTTYPE g_ResultType;								// リザルトのタイプ
// -------------------------- / ゲームクリア /-----------------------------------------------------------------------------------------/
Result g_aResultGameCrear[MAX_RESULT_CLEAR];
ResultInfoGameCrear g_aResultInfoGameCrear[] =
{
	{ D3DXVECTOR3(950, 630, 0.0f), RESULTTEXTYPE_SCORE_MAX ,40,55,SCORE_LEN },
	// 数字
	{ D3DXVECTOR3(950, 450, 0.0f), RESULTTEXTYPE_SCORE ,40,55,SCORE_LEN },
	{ D3DXVECTOR3(750, 510, 0.0f), RESULTTEXTYPE_TIME ,40,55,TIME_LEN },
	{ D3DXVECTOR3(750, 570, 0.0f), RESULTTEXTYPE_HIT ,40,55,HIT_LEN },
	// 文字
	{ D3DXVECTOR3(480, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_SCORE_MAX ,400,50,1 },
	{ D3DXVECTOR3(480, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_SCORE ,400,50,1 },
	{ D3DXVECTOR3(480, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_TIME ,400,50,1 },
	{ D3DXVECTOR3(480, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_HIT ,400,50,1 },
	// :
	{ D3DXVECTOR3(630, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON ,55,55,1 },
	{ D3DXVECTOR3(630, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON1 ,55,55,1 },
	{ D3DXVECTOR3(630, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON2 ,55,55,1 },
	{ D3DXVECTOR3(630, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_DOUBLEKORON3 ,55,55,1 },
	// 数え方
	{ D3DXVECTOR3(1000, 450 + 5, 0.0f), RESULTTEXTYPE_TEXT_PT ,100,50,1 },
	{ D3DXVECTOR3(800, 510 + 5, 0.0f), RESULTTEXTYPE_TEXT_BYOU ,100,50,1 },
	{ D3DXVECTOR3(830, 570 + 5, 0.0f), RESULTTEXTYPE_TEXT_HITMOJI ,100,50,1 },
	{ D3DXVECTOR3(1000, 630 + 5, 0.0f), RESULTTEXTYPE_TEXT_PT_MAX ,100,50,1 },

};
ResultTextureGameCrear g_aResultTexGameCrear[RESULTTEXTYPE_GAMECLEAR_MAX] =
{
	{ "data/TEXTURE/number000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/number000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/number000.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/number000.png" },		// 読み込むテクスチャ2

	{ "data/TEXTURE/TextSum.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextScore.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextTime.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextHit.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextDoubleKoron.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextPt.png" },				// 読み込むテクスチャ2
	{ "data/TEXTURE/TextByou.png" },				// 読み込むテクスチャ2
	{ "data/TEXTURE/TextHitMoji.png" },		// 読み込むテクスチャ2
	{ "data/TEXTURE/TextPt.png" },			// 読み込むテクスチャ2

};
int g_nResultScoreMax;	// リザルト用の最大スコア
int g_nResultScore;		// リザルト用のスコア
int g_nResultTime;		// リザルト用タイム
int g_nResultHit;		// リザルト用最大ヒット数

int g_nResultCnt;		// 桁数カウント
int g_nCounterFade;		// フェード
int g_nCounterNext;		// 次へ
int g_nCounterNextAuto;		// 次へ
int g_nMotionType[4];
//==============================================================================================================================
// 初期化処理（リザルト）
//==============================================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();
	g_nCounterFade = 0;	// フェード
	g_nResultCnt = 0;	// 桁数カウント
	g_nCounterNext = 0;// 次へ
	g_nCounterNextAuto = 0;// 自動
	srand((unsigned int)time(0));
	g_nMotionType[0] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[1] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[2] = rand() % PLAYER_ATTACK_TYPE_MAX;
	g_nMotionType[3] = rand() % PLAYER_ATTACK_TYPE_MAX;
	if (g_ResultType == RESULTTYPE_CLEAR)
	{
		//　スコア
		g_nResultScoreMax = g_nResultScore - g_nResultTime * 50 ;
	}
	if (g_ResultType == RESULTTYPE_GAMEOVER)
	{
		//　スコア
		g_nResultScoreMax = g_nResultScore - g_nResultTime * 50  ;
	}
	if (g_nResultScoreMax <= 0)
	{// ０以下
		g_nResultScoreMax = 0;
	}
	for (int nTex = 0; nTex < RESULTTEXTYPE_GAMECLEAR_MAX; nTex++)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			g_aResultTexGameCrear[nTex].pFileName,
			&g_pTextureResult[nTex]);
	}

		// 情報の初期化
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++)
	{
		g_aResultGameCrear[nCntResult].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標
		g_aResultGameCrear[nCntResult].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 背景の移動量
		g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// 色
		g_aResultGameCrear[nCntResult].nCounterAnim = 0;						// カウンター
		g_aResultGameCrear[nCntResult].nPatternAnim = 0;						// パターン
		g_aResultGameCrear[nCntResult].nCounterFade = 0;						// フェードカウンター
		g_aResultGameCrear[nCntResult].nType = 0;								// タイプ
		g_aResultGameCrear[nCntResult].fWidth = 0;								// 幅
		g_aResultGameCrear[nCntResult].fHeight = 0;								// 縦
		g_aResultGameCrear[nCntResult].bUse = false;							// 使用されているかどうか
	}
	MakeVertexGameCrear(pDevice);
	for (int nCnt = 0; nCnt < sizeof g_aResultInfoGameCrear / sizeof(ResultInfoGameCrear); nCnt++)
	{// セット
		for (int nCnt2 = 0; nCnt2 < g_aResultInfoGameCrear[nCnt].nNumCnt; nCnt2++)
		{
			SetResult(g_aResultInfoGameCrear[nCnt].pos - D3DXVECTOR3((g_aResultInfoGameCrear[nCnt].fWidth)*nCnt2, 0.0f, 0.0f)
				, g_aResultInfoGameCrear[nCnt].nType, g_aResultInfoGameCrear[nCnt].fWidth, g_aResultInfoGameCrear[nCnt].fHeight);
		}
	}
	// カメラの初期化処理
	InitCamera();
	// 線の初期化処理
	InitLine();
	// ライトの初期化処理
	InitLight();
	// 空の初期化処理
	InitSky();
	// 下向きエフェクトの初期化処理
	InitEffectUP();
	// メッシュ床の初期化処理
	InitMeshField();
	// メッシュ壁の初期化処理
	InitMeshWall();
	// メッシュ筒の初期化処理
	InitMeshCylinder();
	// 影の初期化処理
	InitShadow();
	// プレイヤーの初期化処理
	InitPlayer();
	// モデルの初期化処理
	InitModel();
	// ビルボードの初期化処理
	InitBillboard();
	// ビルボードライフの初期化処理
	InitBillLife();
	// 敵の初期化処理
	InitEnemy();
	// アイテムの初期化処理
	InitItem();
	// ポーズの初期化処理
	InitPause();
	// UIの初期化処理
	InitUI();
	// エフェクト２Dの初期化処理
	InitEffect2D();
	// メッセージウィンドウの初期化処理
	InitMessageWindow();
	// エフェクト３Dの初期化処理
	InitEffect3D();
	// エフェクトの初期化処理
	InitEffect();
	//------------------------------/ モデルのファイル読み込み /---------------------------------------------------------------------------//
	SetStagePos("data/stage1.txt");
	GetPlayer()->pos = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
	GetPlayer()->rot.y = 0;
	GetCamera()->nState = CAMERASTATE_BOSS_MOVIE_START;
	GetCamera()->fHeight = -30;
	GetCamera()->posR = GetPlayer()->pos;
	GetCamera()->Destrot.x = D3DX_PI / 4;
	GetCamera()->Destrot.y = D3DX_PI;
	GetCamera()->rot.x = D3DX_PI / 4;
	GetCamera()->rot.y = D3DX_PI;
	SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, 550, 0.0f), 900, 300, MESSAGETEX_NONE);
}

//==============================================================================================================================
// 終了処理（リザルト）
//==============================================================================================================================
void UninitResult(void)
{
		// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_RESULT_CLEAR; nCntTex++)
	{
		if (g_pTextureResult[nCntTex] != NULL)
		{
			g_pTextureResult[nCntTex]->Release();

			g_pTextureResult[nCntTex] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	SortRanking(g_nResultScoreMax);
	// カメラの終了処理
	UninitCamera();
	// ラインの終了処理
	UninitLine();
	// ライトの終了処理
	UninitLight();
	// 空の終了処理
	UninitSky();
	// 下向きエフェクトの終了処理
	UninitEffectUP();
	// メッシュ筒の終了処理
	UninitMeshCylinder();
	// ビルボードの終了処理
	UninitBillboard();
	// 影の終了処理
	UninitShadow();
	// アイテムの終了処理
	UninitItem();
	// ポーズの終了処理
	UninitPause();
	// UIの終了処理
	UninitUI();
	// ビルボードライフの終了処理
	UninitBillLife();
	// エフェクト２Dの終了処理
	UninitEffect2D();
	// メッセージウィンドウの終了処理
	UninitMessageWindow();
	// エフェクトの終了処理
	UninitEffect();
	// エフェクト3Dの終了処理
	UninitEffect3D();

}
//==============================================================================================================================
// 更新処理（リザルト）
//==============================================================================================================================
void UpdateResult(void)
{

	FADE pFADE;
	// フェードの取得
	pFADE = GetFade();
	Player *pPlayer = GetPlayer();
	if (pFADE == FADE_NONE)
	{
		g_nCounterNextAuto++;
		// 決定キー押したかどうか
		if (g_nCounterNextAuto % 60 == 0)
		{
			if (g_nCounterNext < 4)
			{// 自動は6未満まで
				g_nCounterNext++;
				if (g_nCounterNext >= 4)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);
				}
				else
				{
					PlaySound(SOUND_LABEL_SE_SELECT);
				}
			}
		}
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_B) == true)
		{
			g_nCounterNext++;
			g_nCounterNextAuto = 0;
			if (g_nCounterNext >= 4)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);

			}
			else
			{
				PlaySound(SOUND_LABEL_SE_SELECT);
			}
		}


		if (g_nCounterNext >= 5)
		{// 連打で終わる
			SetFade(MODE_RANKING);
		}
		if (g_nCounterNext >= 4)
		{
			g_nCounterNext = 4;
			g_nCounterFade++;
		}
		if (g_nCounterFade >= 300)
		{// 自動でランキングへ推移
			SetFade(MODE_RANKING);
		}
	}
	
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++, pVtx += 4)
	{
		if (g_aResultGameCrear[nCntResult].bUse == true)
		{
			switch (g_nCounterNext)
			{
			case 0:
				break;
			case 1:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_SCORE || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_SCORE
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_PT)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						pPlayer->bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				pPlayer->bAttackType[g_nMotionType[0]] = true;
				break;
			case 2:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TIME || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_TIME
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON1 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_BYOU)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						pPlayer->bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				pPlayer->bAttackType[g_nMotionType[1]] = true;
				break;
			case 3:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_HIT || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_HIT
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON2 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_HITMOJI)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						pPlayer->bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				pPlayer->bAttackType[g_nMotionType[2]] = true;
				break;
			case 4:
				if (g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_SCORE_MAX || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_SCORE_MAX
					|| g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_DOUBLEKORON3 || g_aResultGameCrear[nCntResult].nType == RESULTTEXTYPE_TEXT_PT_MAX)
				{
					g_aResultGameCrear[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (pPlayer->bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						pPlayer->bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				pPlayer->bAttackType[g_nMotionType[3]] = true;
	
				break;
			}

		
		}
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
		// 頂点カラーの設定
		pVtx[0].col = g_aResultGameCrear[nCntResult].col;
		pVtx[1].col = g_aResultGameCrear[nCntResult].col;
		pVtx[2].col = g_aResultGameCrear[nCntResult].col;
		pVtx[3].col = g_aResultGameCrear[nCntResult].col;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
	GetCamera()->fHeight = 20;

	UpdateCamera();
	// 線の更新処理
	UpdateLine();
	// ライトの更新処理
	UpdateLight();
	// 空の更新処理
	UpdateSky();
	// エフェクトの更新処理
	UpdateEffect();
	// 下向きエフェクトの更新処理
	UpdateEffectUP();
	// メッシュ床の更新処理
	UpdateMeshField();
	// メッシュ壁の更新処理
	UpdateMeshWall();
	// メッシュ筒の初期化処理
	UpdateMeshCylinder();
	// プレイヤーの更新処理
	UpdatePlayer(0);
	// モデルの更新処理
	UpdateModel();
	// ビルボードの更新処理
	UpdateBillboard();
	// 影の更新処理
	UpdateShadow();
	// 敵の更新処理
	UpdateEnemy();
	// アイテムの更新処理
	UpdateItem();
	// 軌跡の更新処理
	UpdateOrbit();
	// UIの更新処理
	UpdateUI();
	// ビルボードライフの更新処理
	UpdateBillLife();
	// エフェクト２Dの更新処理
	UpdateEffect2D();
	// エフェクト3Dの更新処理
	UpdateEffect3D();
	// メッセージウィンドウ
	UpdateMessageWindow();

}
//==============================================================================================================================
// 描画処理（リザルト）
//==============================================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	D3DVIEWPORT9 viewportDef;
	// ビューポート情報を取得
	pDevice->GetViewport(&viewportDef);

	// 各種オブジェクトの描画処理
	// カメラの設定
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		// 線の初期化処理
		DrawLine();
	}

	// 空の描画処理
	DrawSky();
	// メッシュ壁の描画処理
	DrawMeshWall();
	// メッシュ筒の初期化処理
	DrawMeshCylinder();
	// プレイヤーの描画処理
	DrawPlayer(0);
	// モデルの描画処理
	DrawModel();
	// メッシュ床の描画処理
	DrawMeshField();
	// 影の描画処理
	DrawShadow();
	// 下向きエフェクトの描画処理
	DrawEffectUP();
	// ビルボードの描画処理
	DrawBillboard();
	// 敵の描画処理
	DrawEnemy();
	// エフェクトの描画処理
	DrawEffect();
	// 軌跡の描画処理
	DrawOrbit();
	// アイテムの描画処理
	DrawItem();
	// エフェクト#Dの描画処理
	DrawEffect3D();
	// メッセージウィンドウの描画処理
	DrawMessageWindow();


	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++)
	{
		if (g_aResultGameCrear[nCntResult].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[g_aResultGameCrear[nCntResult].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntResult * 4,
				2);
		}
	}
	

}

//==============================================================================================================================
// 設定
//==============================================================================================================================
void SetResult(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	VERTEX_2D*pVtx;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResult = 0; nCntResult < MAX_RESULT_CLEAR; nCntResult++, pVtx += 4)
	{
		if (g_aResultGameCrear[nCntResult].bUse == false)
		{// 背景が使用されていない
			g_aResultGameCrear[nCntResult].pos = pos;
			g_aResultGameCrear[nCntResult].nType = nType;
			g_aResultGameCrear[nCntResult].fWidth = fWidth;
			g_aResultGameCrear[nCntResult].fHeight = fHeight;
			// 頂点情報の設定
			pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), -(g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;
			pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResult].fWidth / 2), (g_aResultGameCrear[nCntResult].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResult].pos;

			switch (g_aResultGameCrear[nCntResult].nType)
			{
				int nData;
			case RESULTTEXTYPE_SCORE_MAX:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultScoreMax % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= SCORE_LEN)
				{// 桁数分回ったら累乗の係数を0にする
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_SCORE:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultScore % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= SCORE_LEN)
				{// 桁数分回ったら累乗の係数を0にする
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_TIME:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultTime % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= TIME_LEN)
				{// 桁数分回ったら累乗の係数を0にする
					g_nResultCnt = 0;
				}
				break;
			case RESULTTEXTYPE_HIT:
				nData = int(powf(10, float(g_nResultCnt)));
				pVtx[0].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData) * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_nResultHit % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);
				g_nResultCnt++;
				if (g_nResultCnt >= HIT_LEN)
				{// 桁数分回ったら累乗の係数を0にする
					g_nResultCnt = 0;
				}
				break; 
			}
		
			// 頂点カラーの設定
			pVtx[0].col = g_aResultGameCrear[nCntResult].col;
			pVtx[1].col = g_aResultGameCrear[nCntResult].col;
			pVtx[2].col = g_aResultGameCrear[nCntResult].col;
			pVtx[3].col = g_aResultGameCrear[nCntResult].col;

			g_aResultGameCrear[nCntResult].bUse = true;	// 使用している状態にする
			break;
		}
	}
	g_pVtxBuffResult->Unlock();
}
//==============================================================================================================================
// リザルトタイプの取得
//==============================================================================================================================
RESULTTYPE GetResultType(void)
{
	return g_ResultType;
}
//==============================================================================================================================
// 頂点情報の設定  クリア
//==============================================================================================================================
void MakeVertexGameCrear(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D*pVtx;		// 頂点情報へのポインタ
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT_CLEAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultGameCrear = 0; nCntResultGameCrear < MAX_RESULT_CLEAR; nCntResultGameCrear++, pVtx += 4)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), -(g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[1].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), -(g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[2].pos = D3DXVECTOR3(-(g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), (g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		pVtx[3].pos = D3DXVECTOR3((g_aResultGameCrear[nCntResultGameCrear].fWidth / 2), (g_aResultGameCrear[nCntResultGameCrear].fHeight / 2), 0.0f) + g_aResultGameCrear[nCntResultGameCrear].pos;
		// rhwの設定
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].rhw = 1.0f;
		}

		// 頂点カラーの設定
		pVtx[0].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[1].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[2].col = g_aResultGameCrear[nCntResultGameCrear].col;
		pVtx[3].col = g_aResultGameCrear[nCntResultGameCrear].col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

//==============================================================================================================================
// スコアの表記
//==============================================================================================================================
void GetScore(int nScore)
{
	g_nResultScore = nScore;
}
void GetTime(int nTime)
{
	g_nResultTime = nTime;
}
void GetHit(int nHit)
{
	g_nResultHit = nHit;
}
//==============================================================================================================================
// リザルトの種類の設定
//==============================================================================================================================
void SetResultType(RESULTTYPE type)
{
	g_ResultType = type;
}