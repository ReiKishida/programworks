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
#include "input.h"
#include "player.h"
#include "fade.h"
#include "time.h"
#include "gamepad.h"
#include "light.h"
#include "line.h"
#include "meshWall.h"
#include "meshField.h"
#include "model.h"
#include "orbit.h"
#include "shadow.h"
#include "effect.h"
#include "pause.h"
#include "time.h"
#include "sky.h"
#include "effect2D.h"
#include "sound.h"
#include "spGage.h"
#include "time.h"
#include "UI.h"
#include "effectUP.h"
#include "MessageWindow.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_CHAR	(256)
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
GAMESTATE g_gameState = GAMESTATE_START;	// ゲームの状態
int g_nCounterGameState;				// 状態管理カウンター
bool g_bPause;							// ポーズ中かどうか
bool g_bPauseView;						// ポーズ非表示
bool g_bGameState[GAMESTAGEMAX];		// ゲーム切り替わる
int g_nMessageWindow = 0;
bool g_bMessage[MESSAGETEX_MAX];
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitGame(void)
{
	Player *pPlayer = GetPlayer();
	// 各種オブジェクトの初期化処理
	g_bPause = false;			// ポーズしない
	g_bPauseView = false;		// ポーズを消さない
	for (int nCntState = 0; nCntState < GAMESTAGEMAX; nCntState++)
	{
		g_bGameState[nCntState] = false;
	}
	g_gameState = GAMESTATE_START;
	g_nCounterGameState = 0;
	g_nMessageWindow = 0;
	for (int nCnt = 0; nCnt < MESSAGETEX_MAX; nCnt++)
	{
		g_bMessage[nCnt] = false;
	}
	// カメラの初期化処理
	InitCamera();
	// 線の初期化処理
	InitLine();
	// ライトの初期化処理
	InitLight();
	// エフェクトの初期化処理
	InitEffect2D();

	// 影の初期化処理
	InitShadow();
	// プレイヤーの初期化処理
	InitPlayer();
	// メッシュ床の初期化処理
	InitMeshField();
	// メッシュ壁の初期化処理
	InitMeshWall();
	// モデルの初期化処理
	InitModel();
	// ファイル読み込み
	SetStagePos("data/stage1.txt");
	// 軌跡の初期化処理
	InitOrbit();
	// 空の生成
	InitSky();
	// SP
	InitSpGage();
	// メッセージウィンドウ
	InitMessageWindow();
	// タイム
	InitTime();
	SetPlayer(0, D3DXVECTOR3(0.0f, 600, 0));
	GetCamera()->posR = D3DXVECTOR3(0.0f, 500, 0);
	GetCamera()->posRDest = D3DXVECTOR3(0.0f, 500, 0);
	// ポーズの初期化処理
	InitPause();
	// エフェクト
	InitEffect();
	InitEffectUP();
	InitUI();
	SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 600, 500, MESSAGETEX_TITLE);
	g_bMessage[g_nMessageWindow] = true;
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
	// 影の終了処理
	UninitShadow();
	// 軌跡の終了処理
	UninitOrbit();
	// ポーズの終了処理
	UninitPause();
	// エフェクト
	UninitEffect();
	UninitEffect2D();
	UninitEffectUP();
	// 空
	UninitSky();
	// SP
	UninitSpGage();
	// タイム
	UninitTime();
	// メッセージウィンドウ
	UninitMessageWindow();
	UninitUI();
	g_gameState = GAMESTATE_START;
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
	// ポーズ処理
	if (GetKeyboardTrigger(DIK_P) == true)
	{// キーボード
		if (g_bPauseView == true)
		{
			g_bPauseView = false;
		}
		PlaySound(SOUND_LABEL_PAUSE000);
		SetPause(true, 0);
		g_bPause = g_bPause ? false : true;
	}

	// ポーズ中かどうか
	if (g_bPause == true)
	{
		if (GetKeyboardTrigger(DIK_V) == true)
		{// ポーズ画面消すかどうか
			g_bPauseView = g_bPauseView ? false : true;
		}

	}

	if (g_bPause == false)
	{
		// 各種オブジェクトの更新処理
		// カメラの更新処理
		UpdateCamera();
		// 線の更新処理
		UpdateLine();
		// 軌跡の描画
		UpdateOrbit();
		// ライトの更新処理
		UpdateLight();

		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// プレイヤーの更新処理
			UpdatePlayer(nCnt);
		}
		// 影の更新処理
		UpdateShadow();
		// メッシュ床の更新処理
		UpdateMeshField();
		// メッシュ壁の更新処理
		UpdateMeshWall();
		// モデルの更新処理
		UpdateModel();
		// 空
		UpdateSky();
		// エフェクト
		UpdateEffect();
		UpdateEffect2D();
		UpdateEffectUP();
		// SP
		UpdateSpGage();
		// メッセージウィンドウ
		UpdateMessageWindow();
		UpdateUI();
	}

	if (g_bPause == false)
	{
		switch (g_gameState)
		{
		case GAMESTATE_NONE:
			break;
		case GAMESTATE_START:
			if (g_bMessage[g_nMessageWindow])
			{
				if (g_nMessageWindow < 4)
				{
					if (GetKeyboardTrigger(DIK_S) || GetGamePadTrigger(0, BUTTON_B))
					{
						DeleteMessageWndow();
						g_bMessage[g_nMessageWindow] = false;
						g_nMessageWindow++;
						if (g_nMessageWindow >= 4)
						{
							g_nMessageWindow = 4 - 1;
							g_gameState = GAMESTATE_GAME;
							SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150, 600, 0.0f), D3DXCOLOR(1, 1, 1, 1), UITEX_L, 200 * 0.7f, 150 * 0.7f);
							SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2 + 150, 600, 0.0f), D3DXCOLOR(1, 1, 1, 1), UITEX_R, 200 * 0.7f, 150 * 0.7f);
							SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 400*1.2f, 300 * 1.2f, MESSAGETEX_KAERU);

						}
						else
						{
							if (g_nMessageWindow == MESSAGETEX_TEIJI)
							{
								PlaySound(SOUND_RESULT_TEIKI);
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 500, 300, MESSAGETEX_TEIJI);
							}
							if (g_nMessageWindow == MESSAGETEX_KAEROU)
							{
								PlaySound(SOUND_RESULT_SOKKOU);
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), 400, 300, MESSAGETEX_KAEROU);
							}
							if (g_nMessageWindow == MESSAGETEX_SOUSA)
							{
								SetMessageWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 700, 400, MESSAGETEX_SOUSA);
							}
						}
						g_bMessage[g_nMessageWindow] = true;
					}
				}
			}
			break;
		case GAMESTATE_GAME:
			if (g_bPause == false)
			{
				// タイム
				UpdateTime();
			}
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 250)
			{
				g_gameState = GAMESTATE_NONE;
				if (GetFade() == FADE_NONE)
				{
					// 画面モードの設定
					SetFade(MODE_RESULT);
				}
			}
			break;
		}

	}
	else
	{
		if (g_bPauseView == false)
		{
			UpdatePause(&g_bPause);		// ポーズ
		}
		else
		{
			// カメラの更新処理
			UpdateCamera();
		}
	}
	if (GetKeyboardTrigger(DIK_F9))
	{
		UninitLine();
		UninitModel();
		InitLine();
		SetObjectFileData();
		InitModel();
		InitMeshField();
		InitMeshWall();
		// ファイル読み込み
		SetStagePos("data/stage1.txt");
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		InitModel();
		InitMeshField();
		InitMeshWall();
		// ファイル読み込み
		SetStagePos("data/stage1.txt");

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
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
			// プレイヤーの描画処理
		DrawPlayer(nCnt, 0);
	}
	// 空
	DrawSky();
	// メッシュ床の描画処理
	DrawMeshField();
	// モデルの描画処理
	DrawModel();
	// メッシュ壁の描画処理
	DrawMeshWall();
	DrawEffectUP();
	// 影の描画処理
	DrawShadow();
	// 軌跡の描画
	DrawOrbit();
	// エフェクト
	DrawEffect();
	if (g_bPauseView == false)
	{// UI
		DrawEffect2D();
		// SP
		DrawSpGage();
		// タイム
		DrawTime();
		// メッセージウィンドウ
		DrawMessageWindow();
		DrawUI();
	}
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
void SetPlayerFileData(int nIdy, char *cFilename)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PlayerData *pPlayerData = GetPlayerData();
	Player *pPlayer = GetPlayer();
	FILE *pFile;
	char cLine[MAX_CHAR];	// 行全て読み込み
	char cData[MAX_CHAR];	// 文字判定
	pFile = fopen(cFilename, "r");
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
	D3DXVECTOR3 OrbitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 軌跡始め
	D3DXVECTOR3 OrbitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 軌跡終わり
	int nDamageIdx = 0;											// ダメージ番号
	D3DXVECTOR3 DamageOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ダメージオフセット
	float fDamageRadius = 0;									// ダメージ半径
	int nStopTime = 0;											// ストップ
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{// 判定の初期化
		pPlayerData[nIdy].ColAttack[nCollision].bUse = false;
	}

	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL || strcmp(cData, "END_SCRIPT") != 0)
		{
			sscanf(&cLine[0], "%s", &cData);
			if (strcmp(cData, "NUM_MODEL") == 0)		// モデルの数
			{// 使ってないからどこかで
				sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nNumParts);
			}
			// モデルのファイル読み込み
			if (strcmp(cData, "MODEL_FILENAME") == 0)
			{
				for (int nCntModel = 0; nCntModel < pPlayerData[nIdy].nNumParts; nCntModel++)
				{
					sscanf(&cLine[0], "%s %s %s", &cData[0], &cData[0], &cData[0]);
					pPlayerData[nIdy].aModel[nCntModel].pFileName = &cData[0];

					// Xファイルの読み込み
					D3DXLoadMeshFromX(pPlayerData[nIdy].aModel[nCntModel].pFileName,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&pPlayerData[nIdy].aModel[nCntModel].pBuffMat,
						NULL,
						&pPlayerData[nIdy].aModel[nCntModel].nNumMat,
						&pPlayerData[nIdy].aModel[nCntModel].pMesh);
					D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
														// マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL*)pPlayerData[nIdy].aModel[nCntModel].pBuffMat->GetBufferPointer();
					// テクスチャの配列設定
					pPlayerData[nIdy].aModel[nCntModel].pTextuer = new LPDIRECT3DTEXTURE9[(int)pPlayerData[nIdy].aModel[nCntModel].nNumMat];
					for (int nCntPlayerTex = 0; nCntPlayerTex < (int)pPlayerData[nIdy].aModel[nCntModel].nNumMat; nCntPlayerTex++)
					{
						pPlayerData[nIdy].aModel[nCntModel].pTextuer[nCntPlayerTex] = NULL;
						if (pMat[nCntPlayerTex].pTextureFilename != NULL)
						{
							// テクスチャの設定
							D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
								pMat[nCntPlayerTex].pTextureFilename,	// ファイルの名前
								&pPlayerData[nIdy].aModel[nCntModel].pTextuer[nCntPlayerTex]);		// テクスチャへのポインタ
																										// テクスチャの設定
						}
					}
					fgets(&cLine[0], MAX_CHAR, pFile);
				}
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
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].MoveSpeed);
					}
					else if (strcmp(cData, "JUMP") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].JumpSpeed);
					}
					else if (strcmp(cData, "RADIUS") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].fRadiusWidth);
					}
					else if (strcmp(cData, "HEIGHT") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].fRadiusHeight);
					}
					else if (strcmp(cData, "LIFE") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nInitLife);
					}
					else if (strcmp(cData, "NUM_PARTS") == 0)		// モデルの数
					{// モデル数
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].nNumParts);
					}
					else if (strcmp(cData, "BODY_COLLISION") == 0)		// モデルの数
					{// モデル数
						sscanf(&cLine[0], "%s %s %d %f %f %f %f", &cData[0], &cData[0], &nDamageIdx, &DamageOffset.x, &DamageOffset.y, &DamageOffset.z, &fDamageRadius);
						SetBodyCollisionPlayer(nIdy, nDamageIdx, DamageOffset, fDamageRadius);
					}
					if (strcmp(cData, "PARTSSET") == 0)
					{
						while (strcmp(cData, "END_PARTSSET") != 0)
						{
							fgets(&cLine[0], MAX_CHAR, pFile);
							sscanf(&cLine[0], "%s", &cData);
							if (strcmp(cData, "INDEX") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModel);
							}
							else if (strcmp(cData, "PARENT") == 0)
							{
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant);
							}
							else if (strcmp(cData, "POS") == 0)
							{
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].pos.z);
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].Initpos.z);
							}
							else if (strcmp(cData, "ROT") == 0)
							{
								sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.x, &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.y, &pPlayerData[nIdy].aModel[nCntPlayerModel].rot.z);
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
							pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].bLoop = false;
						}
						else if (nLoop == 1)
						{// ループする
							pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].bLoop = true;
						}
					}
					else if (strcmp(cData, "NUM_KEY") == 0)
					{
						sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].nNumKey);
					}
					else if (strcmp(cData, "SETORBIT") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d %f %f %f %f %f %f %d", &cData[0], &cData[0], &nIdyOrbit, &nOrbitLength, &OrbitStart.x, &OrbitStart.y, &OrbitStart.z, &OrbitEnd.x, &OrbitEnd.y, &OrbitEnd.z, &nOrbitType);
						if (pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].motion == PlayerMotionType(pPlayerData[nIdy].nNumMotion))
						{
							nOrbit++;
						}
						else
						{
							nOrbit = 0;
						}
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nIdyOrbit = nIdyOrbit;				// 軌跡の数
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nOrbitLength = nOrbitLength;		// 軌跡の長さ
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].OrbitStart = OrbitStart;			// 視点
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].OrbitEnd = OrbitEnd;				// 終点
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].nType = nOrbitType;					// 軌跡のタイプ
						pPlayerData[nIdy].orbit[pPlayerData[nIdy].nNumMotion][nOrbit].motion = PlayerMotionType(pPlayerData[nIdy].nNumMotion);	// モーション

					}
					else if (strcmp(cData, "COLLISION") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %d %d %d", &cData[0], &cData[0], &nCollisionIdy, &CollisionOffset.x, &CollisionOffset.y, &CollisionOffset.z, &fCollisionRadius, &nCollisionStart, &nCollisionEnd, &nStopTime);
						SetAttackCollisionPlayer(nIdy, nCollisionIdy, CollisionOffset, fCollisionRadius, nCollisionStart, nCollisionEnd, nStopTime, PlayerMotionType(pPlayerData[nIdy].nNumMotion));
					}
					else if (strcmp(cData, "ATTACK_POWER") == 0)
					{
						sscanf(&cLine[0], "%s %s %f", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fAttackPower);
					}
					else if (strcmp(cData, "COLLISION_DAMAGE") == 0)
					{
						sscanf(&cLine[0], "%s %s %f %f", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fDamageH, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].fDamageV);
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
								sscanf(&cLine[0], "%s %s %d", &cData[0], &cData[0], &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].nFrame);
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
											, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.x, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.y, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].pos.z);
									}
									else if (strcmp(cData, "ROT") == 0)
									{
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.x, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.y, &pPlayerData[nIdy].aMotion[pPlayerData[nIdy].nNumMotion].aKey[nCntKey].aPartsKey[nCntParts].rot.z);
										sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0]
											, &pPlayerData[nIdy].aModel[nCntParts].rot.x, &pPlayerData[nIdy].aModel[nCntParts].rot.y, &pPlayerData[nIdy].aModel[nCntParts].rot.z);
									}
								}
								nCntParts++;// 次のパーツへ
							}
						}
						nCntKey++;	// 次のキーへ
					}
				}
				pPlayerData[nIdy].nNumMotion++; // 次のモーションへ
			}
		}

	}

	fclose(pFile);

}
//==============================================================================================================================
// 各種オブジェクト処理
//==============================================================================================================================
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
	ModelData *pModelData2 = GetModelData();
	for (int nCntMat = 0; nCntMat < MAX_MODEL_TYPE; nCntMat++)
	{
		for (int nCntCollision = 0; nCntCollision < MAX_MODEL_COLLISION; nCntCollision++)
		{
			pModelData[nCntMat].bCollison[nCntCollision] = false;
		}
		pModelData[nCntMat].bCatch = false;// 持てない
		pModelData[nCntMat].bGetInfo = false;// 情報取得できない
	}
	if (pFile != NULL)
	{
		while (fgets(&cLine[0], MAX_CHAR, pFile) != NULL)
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
			if (strcmp(cData, "SETCATCH") == 0)
			{
				int nType = 0;
				int nCatch = 0;
				while (strcmp(cData, "ENDSETCATCH") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "CATCH") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nType, &nCatch);
						if (nCatch == 1)
						{
							pModelData2[nType].bCatch = true;
						}
					}
				}
			}
			if (strcmp(cData, "SETGETINFO") == 0)
			{// 情報取得
				int nType = 0;
				int nGetInfo = 0;
				while (strcmp(cData, "ENDSETGETINFO") != 0)
				{
					fgets(&cLine[0], 256, pFile);
					sscanf(&cLine[0], "%s", &cData);
					if (strcmp(cData, "GETINFO") == 0)
					{
						sscanf(&cLine[0], "%s %s %d %d", &cData[0], &cData[0], &nType, &nGetInfo);
						if (nGetInfo == 1)
						{
							pModelData2[nType].bGetInfo = true;
						}
					}
				}
			}
			if (strcmp(cData, "SETCOLLISION") == 0)
			{
				int nType = 0;
				D3DXVECTOR3 ofset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
						sscanf(&cLine[0], "%s %s %d %f %f %f %f %f %f %f", &cData[0], &cData[0], &nType, &ofset.x, &ofset.y, &ofset.z, &fRadius, &fWidth, &fDepth, &fHeight);
						SetCollisionModel(ofset, fRadius, fWidth, fDepth, fHeight, MODELTYPE(nType));
					}
				}
			}
		}
		fclose(pFile);
	}
}
//==============================================================================================================================
// ステージ設置
//==============================================================================================================================
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
						pMeshField->rot.x /= 57.3248408f;
						pMeshField->rot.y /= 57.3248408f;
						pMeshField->rot.z /= 57.3248408f;
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
					else if (strcmp(cData, "RANDOMPOS") == 0)
					{
						D3DXVECTOR3 randomPos;
						sscanf(&cLine[0], "%s %s %f %f %f", &cData[0], &cData[0], &randomPos.x, &randomPos.y, &randomPos.z);
						pModel->pos += D3DXVECTOR3((sinf(float(rand() % 624) / 100.0f)*randomPos.x), (cosf(float(rand() % 624) / 100.0f)*randomPos.y), (cosf(float(rand() % 624) / 100.0f)*randomPos.z));
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
//==============================================================================================================================
// 情報配置
//==============================================================================================================================
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
int GetGameStateCounter(void)
{
	return g_nCounterGameState;
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

