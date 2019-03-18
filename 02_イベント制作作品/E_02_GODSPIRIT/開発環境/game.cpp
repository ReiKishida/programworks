////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ゲーム処理 [game.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>		// インクルード関数
#include <string.h>		// ストリング関数
#include <stdlib.h>		// ランド関数
#include "main.h"
#include "game.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "effectUP.h"
#include "input.h"
#include "gamepad.h"
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
#include "time.h"
#include "score.h"
#include "billboard.h"
#include "hit.h"
#include "UI.h"
#include "life.h"
#include "meshFar.h"
#include "BillLife.h"
#include "effect2D.h"
#include "result.h"
#include "light.h"
#include "MessageWindow.h"
#include "effect3D.h"
#include "spirit.h"
#include "meshDome.h"
#include "meshDome2.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR	(256)
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_GAME1;	// ゲームの状態
int g_nCounterGameState;				// 状態管理カウンター
bool g_bPause;							// ポーズ中かどうか
bool g_bPauseView;						// ポーズ非表示
bool g_bGameState[GAMESTAGEMAX];		// ゲーム切り替わる
bool g_bOpenBoss;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitGame(void)
{
	// 各種オブジェクトの初期化処理
	g_bPause = false;	// ポーズしない
	g_bPauseView = false;// ポーズを消さない
	for (int nCntState = 0; nCntState < GAMESTAGEMAX; nCntState++)
	{
		g_bGameState[nCntState] = false;
	}
	g_gameState = GAMESTATE_GAME1;
//	g_bGameState[GAMESTAGE2] = true;
	g_bOpenBoss = false;
	g_nCounterGameState = 0;
#if 1
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
	// 時間の初期化処理
	InitTime();
	// スコアの初期化処理
	InitScore();
	// ヒットの初期化処理
	InitHit();
	// UIの初期化処理
	InitUI();
	// ライフの初期化処理
	InitLife();
	// スピリットの初期化処理
	InitSpirit();
	// 円の初期化処理
	InitMeshFar();
	// エフェクト２Dの初期化処理
	InitEffect2D();
	// メッセージウィンドウの初期化処理
	InitMessageWindow();
	// エフェクト３Dの初期化処理
	InitEffect3D();
	// エフェクトの初期化処理
	InitEffect();
	// メッシュドームの初期化処理
	InitMeshDome();
	InitMeshDome2();
	//------------------------------/ モデルのファイル読み込み /---------------------------------------------------------------------------//
	if (g_gameState == GAMESTATE_GAME1)
	{
		SetStagePos("data/stage1.txt");
		// ビルボードの位置
		SetBillboardFileData("data/BillboardStage1.txt");
	}
	if (g_gameState == GAMESTATE_BOSS)
	{
		SetStagePos("data/stageBoss.txt");
	}
#endif

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitGame(void)
{
	// 各種オブジェクトの終了処理
#if 1
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
	// 時間の終了処理
	UninitTime();
	// スコアの終了処理
	UninitScore();
	// ヒットの終了処理
	UninitHit();
	// UIの終了処理
	UninitUI();
	// ライフの終了処理
	UninitLife();
	// スピリットの終了処理
	UninitSpirit();
	// 円の終了処理
	UninitMeshFar();
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
	// メッシュドームの終了処理
	UninitMeshDome();
	UninitMeshDome2();
	g_gameState = GAMESTATE_GAME1;
	g_bPause = false;
	g_bPauseView = false;
#endif
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel();
#if _DEBUG
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		DeleteLine(LINETYPE_MODEL);
		InitModel();
		InitMeshField();
		InitMeshWall();
		InitBillboard();
		SetStagePos("data/stage1.txt");
		SetBillboardFileData("data/BillboardStage1.txt");

	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		PrintStagePos();
	}
	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		g_bPauseView = true;
	}
#endif
	if (GetKeyboardTrigger(DIK_P) == true||GetGamePadTrigger(BUTTON_START)==true)
	{
		if (g_bPauseView == true)
		{
			g_bPauseView = false;
		}
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{
		if (GetKeyboardTrigger(DIK_V) == true || GetGamePadTrigger(BUTTON_A) == true)
		{
			g_bPauseView = g_bPauseView ? false : true;
		}
	}
	if (g_bPause == false)
	{
		// 各種オブジェクトの更新処理
		// カメラの更新処理
		UpdateCamera();
		if (pCamera->nState != CAMERASTATE_BOSS_MOVIE)
		{
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
			// 時間の更新処理
			UpdateTime();
			// スコアの更新処理
			UpdateScore();
			// 軌跡の更新処理
			UpdateOrbit();
			// ヒットの更新処理
			UpdateHit();
			// UIの更新処理
			UpdateUI();
			// ライフの更新処理
			UpdateLife();
			// スピリットの更新処理
			UpdateSpirit();
			// 円の更新処理
			UpdateMeshFar();
			// ビルボードライフの更新処理
			UpdateBillLife();
			// エフェクト２Dの更新処理
			UpdateEffect2D();
			// エフェクト3Dの更新処理
			UpdateEffect3D();
			// メッシュドームの更新処理
			UpdateMeshDome();
			UpdateMeshDome2();
		}
		// メッセージウィンドウの更新処理
		UpdateMessageWindow();

		switch (g_gameState)
		{
		case GAMESTATE_GAME1:
			if (g_bOpenBoss == false)
			{// とびらしまってる
			}
			if (GetFade() == FADE_NONE)
			{
				if (pPlayer->pos.x <= -3500 && pPlayer->pos.x >= -4000)
				{// ボスに行く
					if (pPlayer->pos.z <= -2070)
					{
						SetFadeNone();
						g_bGameState[GAMESTAGEBOSS] = true;
					}
				}
			}
			if (g_bGameState[GAMESTAGEBOSS] == true)
			{
				if (GetFade() == FADE_IN)
				{
					UninitBillLife();
					InitBillLife();
					InitEnemy();
					InitModel();
					InitMeshField();
					InitMeshWall();
					SetStagePos("data/stageBoss.txt");
					g_gameState = GAMESTATE_BOSS_START;
				}
			}
			break;

		case GAMESTATE_BOSS_START:

			g_gameState = GAMESTATE_BOSS;

			break;
		case GAMESTATE_BOSS:
			if (g_bGameState[GAMESTAGEBOSS] == true)
			{// ボスになったら
				StopSound(SOUND_LABEL_GAMEBGM1);
				PlaySound(SOUND_LABEL_BOSSBGM);
				g_bGameState[GAMESTAGEBOSS] = false;
				pCamera->nState = CAMERASTATE_BOSS_MOVIE;
				pCamera->nStateCounter = 20;
				pCamera->rot.x = 0.8f;
				pCamera->rot.y = 0.0f;
				pCamera->rot.z = 0.99f;
				pCamera->posV = D3DXVECTOR3(-3740.0f, 400.0f, -2700);
			}
			if (GetFade() == FADE_NONE)
			{
				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
				{
					if (pModel[nCntModel].nType == MODELTYPE_BOSSSAKU)
					{
						pModel[nCntModel].pos.y -= 6;
						if (pModel[nCntModel].pos.y <= pModel[nCntModel].Initpos.y)
						{
							pModel[nCntModel].pos.y = pModel[nCntModel].Initpos.y;
						}
					}
				}
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState == 120)
			{
				if (GetResultType() == RESULTTYPE_CLEAR)
				{
					pCamera->fLength = 250;
					pCamera->fHeight = 200;
					pCamera->rot.y = pPlayer->rot.y;
					pCamera->posR.x = pPlayer->pos.x + sinf(D3DX_PI / 2 + pCamera->rot.y) * pCamera->AttentionPos.x + sinf(pPlayer->rot.y - D3DX_PI) *pCamera->fLengthR;
					pCamera->posR.y = pPlayer->pos.y + cosf(pPlayer->rot.x) *pCamera->fHeightR;
					pCamera->posR.z = pPlayer->pos.z + cosf(D3DX_PI / 2 + pCamera->rot.y)*pCamera->AttentionPos.x + cosf(pPlayer->rot.y - D3DX_PI) * pCamera->fLengthR;
					pCamera->nState = CAMERASTATE_NONE;
				}
				g_gameState = GAMESTATE_END2;
			}			
			break;
		case GAMESTATE_END2:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 250)
			{
				g_gameState = GAMESTATE_NONE;
				// 画面モードの設定
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
	else
	{
		// カメラの更新処理
		UpdateCamera();
		if (g_bPauseView == false)
		{
			UpdatePause();		// ポーズ
		}

	}
}
//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();
	D3DVIEWPORT9 viewportDef;
	// ビューポート情報を取得
	pDevice->GetViewport(&viewportDef);

	// 各種オブジェクトの描画処理
#if 1
	// カメラの設定
	SetCamera(0);
	if (GetDrawDebug()[DRAW_DEBUG_LINE] == true)
	{
		// 線の初期化処理
		DrawLine();
	}

	// 空の描画処理
//	DrawSky();
// メッシュドームの更新処理
	DrawMeshDome();
	DrawMeshDome2();
	// メッシュ壁の描画処理
	DrawMeshWall();
	// 円の描画処理
	DrawMeshFar();
	// メッシュ筒の初期化処理
//	DrawMeshCylinder();
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

	if (pPlayer->state != PLAYERSTATE_DEATH&&pCamera->nState != CAMERASTATE_BOSS_MOVIE_START)
	{
		if (g_bPauseView == false)
		{// ポーズ表示か非表示か　死亡ムービーか
		 // ビルボードライフの描画処理
			DrawBillLife();
			// 時間の描画処理
			DrawTime();
			// スコアの描画処理
			DrawScore();
			// ヒットの描画処理
			DrawHit();
			// ライフの描画処理
			DrawLife();
			// スピリットの描画処理
			DrawSpirit();
			// エフェクト2Dの描画処理
			DrawEffect2D();

		}
	}

	if (g_bPauseView == false && pCamera->nState != CAMERASTATE_BOSS_MOVIE_START)
	{
		// UIの描画処理
		DrawUI();
		if (pPlayer->state != PLAYERSTATE_DEATH)
		{
			// プレイヤーの描画処理
			SetCamera(1);
			// Ｚバッファのビューポートのエリアをクリア
			pDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			DrawPlayer(0);
			// ライト影響受けない
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			// ビューポート情報をもとに戻す
			pDevice->SetViewport(&viewportDef);
		}
		DrawUI();
	}
	// メッセージウィンドウの描画処理
	DrawMessageWindow();
#endif	

	if (g_bPause == true)
	{// ポーズ
		if (g_bPauseView == false)
		{// ポーズ表示か非表示か
			 // カメラの設定
			SetCamera(0);
			DrawPause();
		}
	}
	// ビューポート情報をもとに戻す
	pDevice->SetViewport(&viewportDef);


}
//==============================================================================================================================
// 各種オブジェクト処理
//==============================================================================================================================
void SetPlayerFileData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen("data/player.txt", "r");
	pPlayer->nNumMotion = 0;				// モーション番号リセット
	int nLoop = 0;							// ループするかどうか[0:ループしない / 1:ループする]
	int nOrbit = 0;							// 軌跡の数
	int nOrbitLength = 0;					// 軌跡の長さ
	int nIdyOrbit = 0;						// 軌跡の親
	ORBITTYPE nOrbitType;					// 軌跡のタイプ
	int nCollisionIdy = 0;										// 攻撃判定の親
	D3DXVECTOR3 CollisionOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 攻撃判定のオフセット
	float fCollisionRadius = 0;									// 攻撃判定の半径
	int nCollisionStart = 0;									// 攻撃判定のスタート
	int nCollisionEnd = 0;										// 攻撃判定の終わり
	D3DXVECTOR3 OrbitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 OrbitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nDamageIdx = 0;
	D3DXVECTOR3 DamageOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDamageRadius = 0;
	int nCntWeapon = 0;
	for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
	{// 武器の状態
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// 判定の初期化
			pPlayer->CollisionAttack[nCollision][nCntWeapon].bUse = false;
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nNumParts);
			}
			if (strcmp(cData, "ALLMODEL_FILENAME") == 0)
			{
				int nCntModel = 0;	// モデル番号
				while (strcmp(cData, "END_ALLMODEL_FILENAME") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					// モデルのファイル読み込み
					if (strcmp(cData, "MODEL_FILENAME") == 0)
					{
						sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
						pPlayer->aModel[nCntModel][nCntWeapon].pFileName = &cData[0];

						// Xファイルの読み込み
						D3DXLoadMeshFromX(pPlayer->aModel[nCntModel][nCntWeapon].pFileName,
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&pPlayer->aModel[nCntModel][nCntWeapon].pBuffMat,
							NULL,
							&pPlayer->aModel[nCntModel][nCntWeapon].nNumMat,
							&pPlayer->aModel[nCntModel][nCntWeapon].pMesh);
						D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
						// マテリアルデータへのポインタを取得
						pMat = (D3DXMATERIAL*)pPlayer->aModel[nCntModel][nCntWeapon].pBuffMat->GetBufferPointer();
						// テクスチャの配列設定
						pPlayer->aModel[nCntModel][nCntWeapon].pTextuer = new LPDIRECT3DTEXTURE9[(int)pPlayer->aModel[nCntModel][nCntWeapon].nNumMat];
						for (int nCntPlayerTex = 0; nCntPlayerTex < (int)pPlayer->aModel[nCntModel][nCntWeapon].nNumMat; nCntPlayerTex++)
						{
							pPlayer->aModel[nCntModel][nCntWeapon].pTextuer[nCntPlayerTex] = NULL;
							if (pMat[nCntPlayerTex].pTextureFilename != NULL)
							{
								// テクスチャの設定
								D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
									pMat[nCntPlayerTex].pTextureFilename,	// ファイルの名前
									&pPlayer->aModel[nCntModel][nCntWeapon].pTextuer[nCntPlayerTex]);		// テクスチャへのポインタ
							}
						}
						nCntModel++;
					}
				}
				nCntWeapon++;

			}
			// プレイヤーのオフセット情報
			if (strcmp(cData, "CHARACTERSET") == 0)
			{
				int nCntPlayerModel = 0;	// モデル番号
				while (strcmp(cData, "END_CHARACTERSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "MOVE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->MoveSpeed);
					}
					else if (strcmp(cData, "JUMP") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->JumpSpeed);
					}
					else if (strcmp(cData, "RADIUS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->fRadiusWidth);
					}
					else if (strcmp(cData, "HEIGHT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayer->fRadiusHeight);
					}
					else if (strcmp(cData, "LIFE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nInitLife);
					}
					else if (strcmp(cData, "SPIRIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nInitSpirit);
					}
					else if (strcmp(cData, "NUM_PARTS") == 0)		// モデルの数
					{// モデル数
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->nNumParts);
					}
					else if (strcmp(cData, "BODY_COLLISION") == 0)		// モデルの数
					{// モデル数
						sscanf(&cLine[0], "%s %s %d %f %f %f %f", &cData[0], &cData[0], &nDamageIdx,&DamageOffset.x, &DamageOffset.y, &DamageOffset.z,&fDamageRadius);
						SetBodyCollisionPlayer(nDamageIdx, DamageOffset, fDamageRadius);
					}
					if (strcmp(cData, "PARTSSET") == 0)
					{
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "INDEX") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].nIdxModel);
								}
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].nIdxModelPareant);
								}
							}
							else if (strcmp(cData, "POS") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].pos.z);
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].Initpos.z);
								}
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
								{
									sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.x, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.y, &pPlayer->aModel[nCntPlayerModel][nCntWeapon].rot.z);
								}
							}
						}
						nCntPlayerModel++;
					}

				}
			}
			// プレイヤーのモーション設定
			if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;	// キー数
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "LOOP") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nLoop);
						if (nLoop == 0)
						{// ループしない
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].bLoop = false;
						}
						else if (nLoop == 1)
						{// ループする
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// 軌跡設定
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion),PLAYERWEAPON_HAND);
						nOrbit++;
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z,&fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset,fCollisionRadius, nCollisionStart, nCollisionEnd,PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAND);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].fDamageVertical);
					}
					if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].nFrame);
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(&cLine[0], MAX_CHAR, pFile);
									sscanf(&cLine[0], "%s", &cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAND].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// 次のパーツへ
							}
						}
						nCntKey++;	// 次のキーへ
					}
				}
				pPlayer->nNumMotion++; // 次のモーションへ
			}
		}

	}
	fclose(pFile);
//-------------------------------------/ 剣モーションの読み込み /--------------------------------------------------------//
	FILE *pFile2;
	pFile2 = fopen("data/TEXT/player_sord.txt", "r");
	pPlayer->nNumMotion = 0;				// モーション番号リセット

	if (pFile2 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;	// キー数
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "LOOP") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nLoop);
						if (nLoop == 0)
						{// ループしない
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].bLoop = false;
						}
						else if (nLoop == 1)
						{// ループする
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// 軌跡設定
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_SORD);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_SORD);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].fDamageVertical);
					}
					if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].nFrame);
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(&cLine[0], MAX_CHAR, pFile);
									sscanf(&cLine[0], "%s", &cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_SORD].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// 次のパーツへ
							}
						}
						nCntKey++;	// 次のキーへ
					}
				}
				pPlayer->nNumMotion++; // 次のモーションへ
			}
		}
	}
	fclose(pFile2);
//-------------------------------------/ 槍モーションの読み込み /--------------------------------------------------------//
	FILE *pFile3;
	pFile3 = fopen("data/TEXT/player_lance.txt", "r");
	pPlayer->nNumMotion = 0;				// モーション番号リセット

	if (pFile3 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;	// キー数
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "LOOP") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nLoop);
						if (nLoop == 0)
						{// ループしない
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].bLoop = false;
						}
						else if (nLoop == 1)
						{// ループする
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// 軌跡設定
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_LANCE);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_LANCE);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].fDamageVertical);
					}
					if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].nFrame);
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(&cLine[0], MAX_CHAR, pFile);
									sscanf(&cLine[0], "%s", &cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_LANCE].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// 次のパーツへ
							}
						}
						nCntKey++;	// 次のキーへ
					}
				}
				pPlayer->nNumMotion++; // 次のモーションへ
			}
		}
	}
	fclose(pFile3);
	//-------------------------------------/ 小槌モーションの読み込み /--------------------------------------------------------//
	FILE *pFile4;
	pFile4 = fopen("data/TEXT/player_hammer.txt", "r");
	pPlayer->nNumMotion = 0;				// モーション番号リセット

	if (pFile4 != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "MOTIONSET") == 0)
			{
				int nCntKey = 0;	// キー数
				while (strcmp(cData, "END_MOTIONSET") != 0)
				{
					fgets(&cLine[0], MAX_CHAR, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "LOOP") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nLoop);
						if (nLoop == 0)
						{// ループしない
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].bLoop = false;
						}
						else if (nLoop == 1)
						{// ループする
							pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						// 軌跡設定
						SetOrbit(nIdyOrbit, nOrbitLength, D3DXVECTOR3(OrbitStart.x, OrbitStart.y, OrbitStart.z), D3DXVECTOR3(OrbitEnd.x, OrbitEnd.y, OrbitEnd.z), nOrbitType, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAMMER);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd);
						SetAttackCollisionPlayer(nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, PlayerMotionType(pPlayer->nNumMotion), PLAYERWEAPON_HAMMER);
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %hhd", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].nAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].fDamageHorizontal, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].fDamageVertical);
					}
					if (strcmp(cData, "KEYSET") == 0)
					{
						int nCntParts = 0;
						while (strcmp(cData, "END_KEYSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "FRAME") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].nFrame);
							}
							else if (strcmp(cData, "KEY") == 0)
							{
								while (strcmp(cData, "END_KEY") != 0)
								{
									fgets(&cLine[0], MAX_CHAR, pFile);
									sscanf(&cLine[0], "%s", &cData);
									if (strcmp(cData, "POS") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayer->aMotion[pPlayer->nNumMotion][PLAYERWEAPON_HAMMER].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
									}
								}
								nCntParts++;// 次のパーツへ
							}
						}
						nCntKey++;	// 次のキーへ
					}
				}
				pPlayer->nNumMotion++; // 次のモーションへ
			}
		}
	}
	fclose(pFile4);

}
void SetObjectFileData(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	int nNumModel;
	pFile = fopen("data/model.txt", "r");
	MeshField *pMeshField = GetMeshField();
	MeshFieldData  *pMeshFieldData = GetMeshFieldData();
	MeshWall *pMeshWall = GetMeshWall();
	MeshWallData *pMeshWallData = GetMeshWallData();
	Model *pModel = GetModel();
	ModelData *pModelData = GetModelData();
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
		{
			pModelData[nCntMat].bCollison[nCntCollision] = false;
		}
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL )
		{
			sscanf(&cLine[0], "%s", &cData);

			if (strcmp(cData, "NUM_TEXTURE") == 0)
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshFieldData[0].nNumTex);
			}
			if (strcmp(cData, "TEXTURE_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pMeshFieldData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
					pMeshFieldData->pFileName,							// ファイルの名前
					&pMeshFieldData->pTexture);							// テクスチャへのポインタ
				pMeshFieldData++;
			}
			if (strcmp(cData, "NUM_WALL") == 0)
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshWallData[0].nNumTex);
			}
			if (strcmp(cData, "WALL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pMeshWallData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
						pMeshWallData->pFileName,						// ファイルの名前
						&pMeshWallData->pTexture);						// テクスチャへのポインタ
						pMeshWallData++;
			}

			if (strcmp(cData, "NUM_MODEL") == 0)
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nNumModel);
			}

			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pModelData->pFileName = &cData[0];

				// モデルデータの読み込み
				pModelData->pMesh = NULL;
				pModelData->pBuffMat = NULL;
				pModelData->nNumMat = 0;
				for (int nCnt = 0; nCnt < MAX_MODEL_COLLISION; nCnt++)
				{
					pModelData->CollisionPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					pModelData->fRadiusCollision[nCnt] = 0;
					pModelData->bCollison[nCnt] = false;
				}
				// Xファイルの読み込み
				D3DXLoadMeshFromX(pModelData->pFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&pModelData->pBuffMat,
					NULL,
					&pModelData->nNumMat,
					&pModelData->pMesh);
				D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)pModelData->pBuffMat->GetBufferPointer();
				// テクスチャの配列設定
				pModelData->pTextuer = new LPDIRECT3DTEXTURE9[(int)pModelData->nNumMat];

				// テクスチャの読み込み
				for (int nCntMatTex = 0; nCntMatTex < (int)pModelData->nNumMat; nCntMatTex++)
				{
					pModelData->pTextuer[nCntMatTex] = NULL;

					if (pMat[nCntMatTex].pTextureFilename != NULL)
					{
						// テクスチャの設定
						D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
							pMat[nCntMatTex].pTextureFilename,	// ファイルの名前
							&pModelData->pTextuer[nCntMatTex]);		// テクスチャへのポインタ
					}
				}
				pModelData++;
			}
			if (strcmp(cData, "SETCOLLISION") == 0)
			{
				int nType = 0;
				D3DXVECTOR3 ofset = D3DXVECTOR3(0.0f,0.0f,0.0f);
				float fRadius = 0;
				float fWidth = 0;
				float fDepth = 0;
				float fHeight = 0;
				while (strcmp(cData, "ENDSETCOLLISION") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %f %f", &cData[0], &cData[0], &nType,&ofset.x,&ofset.y,&ofset.z,&fRadius,&fWidth,&fDepth,&fHeight);
						SetCollisionModel(ofset, fRadius,fWidth,fDepth,fHeight,MODELTYPE(nType));				
					}
				}
			}
		}
		fclose(pFile);
	}
}
void SetStagePos(char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	pFile = fopen(cFilename, "r");
	MeshField *pMeshField = GetMeshField();
	MeshWall *pMeshWall = GetMeshWall();
	Model *pModel = GetModel();
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "WALLSET") == 0)
			{
				while (strcmp(cData, "END_WALLSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshWall->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshWall->pos.x, &pMeshWall->pos.y, &pMeshWall->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshWall->rot.x, &pMeshWall->rot.y, &pMeshWall->rot.z);

						if (pMeshWall->rot.y == 90.0f)
						{
							pMeshWall->rot.y = D3DX_PI / 2;
						}
						if (pMeshWall->rot.y == 180.0f)
						{
							pMeshWall->rot.y = D3DX_PI;
						}
						if (pMeshWall->rot.y == -90.0f)
						{
							pMeshWall->rot.y = -D3DX_PI / 2;
						}
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &pMeshWall->nBlock_X, &pMeshWall->nBlock_Y);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshWall->fWidth, &pMeshWall->fHeight);
					}
					else if (strcmp(cData, "CULL") == 0)
					{
						int nCull = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
						if (nCull == 0)
						{// カリングする
							pMeshWall->bCull = false;
						}
						else
						{// カリングしない
							pMeshWall->bCull = true;
						}
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollision = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollision);
						if (nCollision == 0)
						{// カリングする
							pMeshWall->bCollision = false;
						}
						else
						{// カリングしない
							pMeshWall->bCollision = true;
						}
					}
				}
				SetMeshWall(pMeshWall->pos, pMeshWall->rot, pMeshWall->nType, pMeshWall->nBlock_X, pMeshWall->nBlock_Y, pMeshWall->fWidth, pMeshWall->fHeight, pMeshWall->bCull);
				pMeshWall++;
			}
			if (strcmp(cData, "FIELDSET") == 0)
			{
				while (strcmp(cData, "END_FIELDSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pMeshField->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshField->pos.x, &pMeshField->pos.y, &pMeshField->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pMeshField->rot.x, &pMeshField->rot.y, &pMeshField->rot.z);
						//pMeshField->rot.x /= 57.3248408f;
						//pMeshField->rot.y /= 57.3248408f;
						//pMeshField->rot.z /= 57.3248408f;
						if (pMeshField->rot.x == 180.0f)
						{
							pMeshField->rot.x = D3DX_PI;
						}
					}
					else if (strcmp(cData, "BLOCK") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &pMeshField->nBlock_X, &pMeshField->nBlock_Z);
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fWidth, &pMeshField->fDepth);
					}
					else if (strcmp(cData, "SLOPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fSlopeSin, &pMeshField->fSlopeCos);
					}
					else if (strcmp(cData, "CULL") == 0)
					{
						int nCull = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCull);
						if (nCull == 0)
						{// カリングする
							pMeshField->bCull = false;
						}
						else
						{// カリングしない
							pMeshField->bCull = true;
						}
					}
					else if (strcmp(cData, "MOVE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pMeshField->fTexmove.x, &pMeshField->fTexmove.y);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollison = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollison);
						if (nCollison == 0)
						{// 判定なし
							pMeshField->bCollision = false;
						}
						else
						{// 判定あり
							pMeshField->bCollision = true;
						}
					}
				}
				SetMeshField(pMeshField->pos, pMeshField->rot, pMeshField->nType, pMeshField->nBlock_X, pMeshField->nBlock_Z, pMeshField->fWidth, pMeshField->fDepth, pMeshField->fSlopeSin, pMeshField->bCull);
				pMeshField++;
			}
	
			if (strcmp(cData, "SETMODEL") == 0)
			{
				while (strcmp(cData, "ENDSETMODEL") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pModel->pos.x, &pModel->pos.y, &pModel->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pModel->rot.x, &pModel->rot.y, &pModel->rot.z);
						pModel->rot.x /= 57.3248408f;
						pModel->rot.y /= 57.3248408f;
						pModel->rot.z /= 57.3248408f;
					}
					else if (strcmp(cData, "TYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pModel->nType);
					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						int nCollision = 0;
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &nCollision);
						if (nCollision == 0)
						{// 判定なし
							pModel->bCollision = false;
						}
						else
						{// 判定あり
							pModel->bCollision = true;
						}
					}
				}
				SetModel(pModel->pos, pModel->rot, pModel->nType);
				pModel++;
			}
		}
		fclose(pFile);
	}
}
void SetBillboardFileData(char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char cLine[MAX_CHAR];
	char cData[MAX_CHAR];
	pFile = fopen(cFilename, "r");
	Billboard *pBillboard = GetBillboard();
	BillboardData *pBillboardData = GetBillboardData();

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&cLine[0], "%s", &cData);

			if (strcmp(cData, "NUM_TEXTURE") == 0)
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pBillboardData[0].nNumTex);
			}
			if (strcmp(cData, "TEXTURE_FILENAME") == 0)
			{
				sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
				pBillboardData->pFileName = &cData[0];

				D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
					pBillboardData->pFileName,		// ファイルの名前
					&pBillboardData->pTexture);		// テクスチャへのポインタ
				pBillboardData++;
			}
			if (strcmp(cData, "BILLBOARDSET") == 0)
			{
				while (strcmp(cData, "END_BILLBOARDSET") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "TEXTYPE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pBillboard->nType);
					}
					else if (strcmp(cData, "POS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pBillboard->pos.x, &pBillboard->pos.y, &pBillboard->pos.z);
					}
					else if (strcmp(cData, "ROT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pBillboard->rot.x, &pBillboard->rot.y, &pBillboard->rot.z);
						pBillboard->rot.x /= 57.3248408f;
						pBillboard->rot.y /= 57.3248408f;
						pBillboard->rot.z /= 57.3248408f;
					}
					else if (strcmp(cData, "SIZE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pBillboard->fWidth, &pBillboard->fHeight);
					}
				}
				SetBillboard(pBillboard->pos, pBillboard->rot, pBillboard->nType, pBillboard->fWidth, pBillboard->fHeight);
				pBillboard++;
			}
		}
	}
}
void PrintStagePos(void)
{
	FILE *pFileQ;						// ファイル型のポインタ変数
	Model *pModel = GetModel();
	int nCollision = 0;
	pFileQ = fopen("data/ステージ情報.txt", "w");	// 問題の読み込み
	if (pFileQ != NULL)
	{// ファイルが開けたら
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true)
			{
				fprintf(pFileQ, "SETMODEL\n");
				fprintf(pFileQ, "	POS = %.1f %.1f %.1f\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);
				fprintf(pFileQ, "	ROT = %.1f %.1f %.1f\n", pModel->rot.x, pModel->rot.y, pModel->rot.z);
				fprintf(pFileQ, "	TYPE = %d\n", pModel->nType);
				if (pModel->bCollision == false)
				{
					nCollision = 0;
				}
				else
				{
					nCollision = 1;
				}
				fprintf(pFileQ, "	COLLISION = %d\n", nCollision);
				fprintf(pFileQ, "ENDSETMODEL\n");
			}
		}
		fclose(pFileQ);
	}
}
//==============================================================================================================================
// ゲームの状態の設定
//==============================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}
//==============================================================================================================================
// ゲームの状態の取得
//==============================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
//==============================================================================================================================
// ポーズ状態を変える
//==============================================================================================================================
void ReleaseGamePause(void)
{
	g_bPause = g_bPause ? false : true;
}
//==============================================================================================================================
// ポーズ取得
//==============================================================================================================================
bool GetPause(void)
{
	return g_bPause;
}
bool GetPauseView(void)
{
	return g_bPauseView;
}