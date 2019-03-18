////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プレイヤー処理 [player.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "model.h"
#include "fade.h"
#include "meshField.h"
#include "meshWall.h"
#include "line.h"
#include "orbit.h"
#include "sound.h"
#include "game.h"
#include "orbit.h"
#include "gamepad.h"
#include "effect.h"
#include "effect2D.h"
#include "spGage.h"
#include "MessageWindow.h"
#include "time.h"
#include "effectUP.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define DOWN_SPEED						(0.01f)	// 減速量
#define PLAYER_ROT_SPEED_NORMAL			(0.3f)	// 通常時の角度を変える量
#define PLAYER_ROT_SPEED_ATTACK			(0.03f)	// 攻撃時の角度を変える量
#define GRAVITY_SPEED					(0.6f)	// 重力加算スピード
#define INIT_JUMP_SPEED					(7)		// ジャンプ力
#define INIT_MOVE_SPEED					(0.3f)	// 移動量
#define BLEND_FRAME						(3)		// ブレンド
#define WARKZONE						(50)	// 歩きのゾーン
#define PLAYER_MOTION_MOVE_ZONE			(1.0f)	// 歩きのモーションゾーン
#define MAX_SKY							(1.0f)	// ガードの最大ライフ
#define MAX_CAMERASTATE					(30)
#define DOWN_JUMP_SPEED					(1.5f)	// ジャンプ
#define MAX_SPEED						(30.0f)	//
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void SetPlayerVoice( PlayerMotionType motion);
void PlayerLifeZero(int nIdy,float fAngle, float fDamageH, float fDamageV);
//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Player g_aPlayer[MAX_PLAYER];
PlayerData g_aPlayerData[MAX_PLAYER];
int g_nNumAll = 0;
int g_nCutInTime;
int g_nSpecialPlayer;
bool g_bCutIn;
bool g_bStartMove;
bool g_bStopStart;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_bStopStart = true;
	// 位置・向きの初期設定
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_aPlayer[nCnt].pos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);		// 座標
		g_aPlayer[nCnt].posOld = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);		// 前回の座標
		g_aPlayer[nCnt].Diffpos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);	// 座標の差分
		g_aPlayer[nCnt].Initpos = D3DXVECTOR3(0.0f + nCnt * 100, 0.0f, 0.0f);	// 初期座標
		g_aPlayer[nCnt].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nCnt].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nCnt].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nCnt].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aPlayer[nCnt].moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aPlayer[nCnt].moveL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aPlayer[nCnt].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の差分
		g_aPlayer[nCnt].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の回転
		g_aPlayer[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{// 音
			g_aPlayer[nCnt].bSound[nCntSound] = false;
		}
		;		g_aPlayer[nCnt].state = PLAYERSTATE_NONE;
		g_aPlayer[nCnt].nCntState = 0;
		g_aPlayer[nCnt].pMeshFieldLand = NULL;
		g_aPlayer[nCnt].pModel = NULL;
		g_aPlayer[nCnt].pModelAtkData = NULL;
		g_aPlayer[nCnt].pModelAtk = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nCnt].bMotionState[nbMotion] = false;
			g_aPlayer[nCnt].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nCnt].bBlend = false;
		g_aPlayer[nCnt].nDamage = 0;
		g_aPlayer[nCnt].bJump = false;							// ジャンプ
		g_aPlayer[nCnt].nFlyCounter = 0;						// 飛んでいる時間
		g_aPlayer[nCnt].fRotSpeed = 0.0f;						// 回転の減速スピード
		g_aPlayer[nCnt].nAirCounter = 0;						// 浮遊時の攻撃制御カウンター
		g_aPlayer[nCnt].nLoopCounter = 0;						// ループカウンター
		g_aPlayer[nCnt].nIdy = nCnt;							// 番号
		g_aPlayer[nCnt].nKeyCounter = 0;		// キー数
		g_aPlayer[nCnt].nFrameCounter = 0;		// フレーム数
		g_aPlayer[nCnt].nFrameAllCounter = 0;	// 最大フレーム
		g_aPlayer[nCnt].nFrame = 0;				// フレーム
		g_aPlayer[nCnt].bUse = false;
		g_aPlayer[nCnt].bDraw = true;			// 映ってるかどうか
	}
	g_aPlayer[0].col = D3DXCOLOR(0.92f, 0.43f, 0.62f, 1.0f);	// ピンク
	g_nCutInTime = 0;
	g_nSpecialPlayer = 0;
	g_bCutIn = false;
	g_bStartMove = false;


	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		for (int nCntParts = 0; nCntParts < g_aPlayerData[nCnt].nNumParts; nCntParts++)
		{// Key - 現在 間の差分計算
			g_aPlayerData[nCnt].aModel[nCntParts].rot = g_aPlayerData[nCnt].aMotion[PLAYER_MOTION_TYPE_NEUTRAL].aKey[0].aPartsKey[nCntParts].rot;
		}
	}
	g_nNumAll = 0;
	SetEffect2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_HEIGHT, SCREEN_HEIGHT, 1, EFFECTTYPE_2D_SPEED);
}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		for (int nCntModel = 0; nCntModel < g_aPlayerData[nCnt].nNumParts; nCntModel++)
		{
			// メッシュの開放
			if (g_aPlayerData[nCnt].aModel[nCntModel].pMesh != NULL)
			{
				g_aPlayerData[nCnt].aModel[nCntModel].pMesh->Release();
				g_aPlayerData[nCnt].aModel[nCntModel].pMesh = NULL;
			}
			// マテリアルの開放
			if (g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat != NULL)
			{
				g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat->Release();
				g_aPlayerData[nCnt].aModel[nCntModel].pBuffMat = NULL;
			}
			// テクスチャの開放
			for (int nCntMat = 0; nCntMat < int(g_aPlayerData[nCnt].aModel[nCntModel].nNumMat); nCntMat++)
			{
				for (int nCntCol = 0; nCntCol < MAX_PLAYER; nCntCol++)
				{
					if (g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat] != NULL)
					{
						g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat]->Release();
						g_aPlayerData[nCnt].aModel[nCntModel].pTextuer[nCntMat] = NULL;
					}
				}
			}
		}
	}

}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdatePlayer(int nIdy)
{
	MeshField *pMeshField = GetMeshField();
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel();


	if (g_aPlayer[nIdy].bUse)
	{
		// 前回の位置 タイプ
		g_aPlayer[nIdy].posOld = g_aPlayer[nIdy].pos;
		g_aPlayer[nIdy].nMotionTypeOld = g_aPlayer[nIdy].nMotionType;

		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{// 前回のモーション
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = g_aPlayer[nIdy].bMotionState[nbMotion];
		}
	}
	switch (g_aPlayer[nIdy].state)
	{
	case PLAYERSTATE_NONE:
		break;
	case PLAYERSTATE_DAMAGESTART:
		g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_FALL:	// 落下
		g_aPlayer[nIdy].move.y = 0;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_INVINCIBLE:
		g_aPlayer[nIdy].nCntState--;
		if (g_aPlayer[nIdy].nCntState % 2 == 0)
		{// 点滅
			g_aPlayer[nIdy].bDraw = g_aPlayer[nIdy].bDraw ? false : true;
		}
		if (g_aPlayer[nIdy].nCntState <= 0)
		{
			g_aPlayer[nIdy].bDraw = true;
			g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_DEATH:
		DeleteShadow(g_aPlayer[nIdy].nIdyShadow);
		g_aPlayer[nIdy].move = D3DXVECTOR3(0, 0, 0);
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;
		if (g_nNumAll <= 1)
		{
			if (GetGameState() != GAMESTATE_END)
			{
				SetGameState(GAMESTATE_END);
			}
		}
		break;
	}
	switch (pCamera[0].nState)
	{
	case CAMERASTATE_NONE:
		break;
	}
	if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
	{// 生きているなら
		if (GetMode() != MODE_RESULT&&GetGameState() != GAMESTATE_END)
		{
			if (GetGameState() != GAMESTATE_START)
			{
				PlayerController(nIdy);
			}
			// ダメージの時の計算
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] || g_aPlayer[nIdy].state == PLAYERSTATE_DAMAGE)
			{// 吹っ飛び力がまだあったら
				if (g_aPlayer[nIdy].fFlyPower.x >= 0.1f ||
					g_aPlayer[nIdy].fFlyPower.x <= -0.1f)
				{// 水平方向の吹っ飛び力があるなら
					g_aPlayer[nIdy].move.x = g_aPlayer[nIdy].fFlyPower.x;	// 吹っ飛び力を加算
				}
				if (g_aPlayer[nIdy].fFlyPower.z >= 0.1f ||
					g_aPlayer[nIdy].fFlyPower.z <= -0.1f)
				{// 水平方向の吹っ飛び力があるなら
					g_aPlayer[nIdy].move.z = g_aPlayer[nIdy].fFlyPower.z;	// 吹っ飛び力を加算
				}

				if (g_aPlayer[nIdy].nFlyCounter > 0)
				{// 吹っ飛ぶカウンターが0より大きかったら
					g_aPlayer[nIdy].fFlyPower.y -= 0.5f;
					if (g_aPlayer[nIdy].fFlyPower.y > 0.0f)
					{// ジャンプ力がまだあったら
						g_aPlayer[nIdy].move.y = g_aPlayer[nIdy].fFlyPower.y;			// 上昇していく
					}
					if (rand() % 2 == 0)
					{// 煙エフェクト
						float fDirection[3] = { SetRandomEffect(),SetRandomEffect(),SetRandomEffect() };
						SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(fDirection[0]) * 10, cosf(fDirection[1]) * 10, cosf(fDirection[2]) * 10),
							D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), g_aPlayer[nIdy].col, float(rand() % 80 + 10), 10, EFFECTTYPE_SMOKE);
					}
				}
			}

		}
	}
	if (g_aPlayer[nIdy].nMotionType != g_aPlayer[nIdy].nMotionTypeOld)
	{
		g_aPlayer[nIdy].fRotSpeed = PLAYER_ROT_SPEED_NORMAL;			// 通常の回転速度を入れる

	}

	// 重力
	g_aPlayer[nIdy].move.y -= GRAVITY_SPEED;
	// 移動量加算
	g_aPlayer[nIdy].pos += g_aPlayer[nIdy].move;
	if (g_bStopStart)
	{
		g_aPlayer[nIdy].pos = g_aPlayer[nIdy].Initpos;
		if (GetGameStartTime())
		{
			DeleteMessageWndow();

			g_bStopStart = false;
			g_aPlayer[nIdy].move.x = 0.0f;
		}
	}

	// 減速更新
	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
	{
		g_aPlayer[nIdy].moveR.x += (0 - g_aPlayer[nIdy].move.x)*DOWN_SPEED;
		g_aPlayer[nIdy].moveR.z += (0 - g_aPlayer[nIdy].move.z)*DOWN_SPEED;
		g_aPlayer[nIdy].moveL.x += (0 - g_aPlayer[nIdy].move.x)*DOWN_SPEED;
		g_aPlayer[nIdy].moveL.z += (0 - g_aPlayer[nIdy].move.z)*DOWN_SPEED;
		if (g_aPlayer[nIdy].moveL.z >= MAX_SPEED)
		{
			g_aPlayer[nIdy].moveL.z = MAX_SPEED;
		}
		if (g_aPlayer[nIdy].moveR.z >= MAX_SPEED)
		{
			g_aPlayer[nIdy].moveR.z = MAX_SPEED;
		}
		g_aPlayer[nIdy].move = D3DXVECTOR3((g_aPlayer[nIdy].moveR.x + g_aPlayer[nIdy].moveL.x) / 2.0f,
			(g_aPlayer[nIdy].move.y + g_aPlayer[nIdy].move.y) / 2.0f,
			(g_aPlayer[nIdy].moveR.z + g_aPlayer[nIdy].moveL.z) / 2.0f);

	}

	//--------------------------/ モデルの動きをなめらかにする /---------------------------------------------------------------------------------------//
	if (g_aPlayer[nIdy].Destrot.y > D3DX_PI)
	{// 差分が3.14を超えるなら
		g_aPlayer[nIdy].Destrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].Destrot.y < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		g_aPlayer[nIdy].Destrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nIdy].Diffrot.y = g_aPlayer[nIdy].Destrot.y - g_aPlayer[nIdy].rot.y;// 現在の角度と目的の角度の差分
	if (g_aPlayer[nIdy].Diffrot.y > D3DX_PI)
	{// 差分が3.14を超えるなら
		g_aPlayer[nIdy].Diffrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].Diffrot.y < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		g_aPlayer[nIdy].Diffrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nIdy].rot.y += g_aPlayer[nIdy].Diffrot.y * g_aPlayer[nIdy].fRotSpeed;	// 角度の加算
	if (g_aPlayer[nIdy].rot.y > D3DX_PI)
	{// 現在の角度が3.14を超えるなら
		g_aPlayer[nIdy].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nIdy].rot.y < -D3DX_PI)
	{// 現在の角度が-3.14を超えるなら
		g_aPlayer[nIdy].rot.y += D3DX_PI * 2.0f;
	}
	//----------------------------------/ 当たり判定 /---------------------------------------------------------------------------------------//
		// メッシュフィールド
	if (CollisionMeshField(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move,
		g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pMeshFieldLand) == true)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nIdy].nIdyShadow].pos.y = g_aPlayer[nIdy].pos.y + 0.1f;
		pShadow[g_aPlayer[nIdy].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nIdy].bJump = false;
		g_aPlayer[nIdy].nAirCounter = 0;// 空中攻撃制御カウンター

		if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
			}
		}
		if (!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] && g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
		{// 着地した
			for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
			{// 着地エフェクト
				float nDirection = SetRandomEffect();
				float nDirection2 = SetRandomEffect();
				SetEffect(g_aPlayer[nIdy].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_SMOKE);
				SetEffect(g_aPlayer[nIdy].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_SMOKE);
			}
			PlaySound(SOUND_LABEL_LAND);
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
		}
	}
	else
	{
	}
	if (g_aPlayer[nIdy].pMeshFieldLand != NULL)
	{// 床に乗っている
		if ((g_aPlayer[nIdy].move.x > 1.0f || g_aPlayer[nIdy].move.z > 1.0f ||
			g_aPlayer[nIdy].move.x < -1.0f || g_aPlayer[nIdy].move.z < -1.0f))
		{// 歩いている
			if (!g_aPlayer[nIdy].bSound[SOUND_CHAR])
			{
				if (GetGameState() != GAMESTATE_START)
				{
					PlaySound(SOUND_CHAR);
					g_aPlayer[nIdy].bSound[SOUND_CHAR] = true;
				}
			}
		}
		else
		{
		}
	}
	else
	{
		g_aPlayer[nIdy].bSound[SOUND_CHAR] = false;

	}
	// プレイヤー同士
	PlayerOverLap(g_aPlayer[nIdy].nIdy);
	if (CollisionModelMinMax(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pModel) == 1)
	{// ぶつかる
	}
	if (g_aPlayer[nIdy].pModel != NULL)
	{
		if (g_aPlayer[nIdy].pModel->nType == MODELTYPE_HAUSE)
		{
			if (GetFade() == FADE_NONE)
			{
				SetFade(MODE_RESULT);
			}
		}
		else
		{
			HitEffect(D3DXVECTOR3(g_aPlayer[nIdy].aModel[2].mtxWorld._41, g_aPlayer[nIdy].aModel[2].mtxWorld._42, g_aPlayer[nIdy].aModel[2].mtxWorld._43));
			PlaySound(SOUND_HIT_HAC);
			g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
			g_aPlayer[nIdy].nCntState = 10;

			// 水平方向の吹っ飛び力を代入
			g_aPlayer[nIdy].fFlyPower.z = -20;
			// 垂直方向の吹っ飛び力を代入
			g_aPlayer[nIdy].fFlyPower.y = 10;
			g_aPlayer[nIdy].nFlyCounter = 10;
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;	// 吹っ飛び中に設定

			if (g_aPlayer[nIdy].pModel->nType == MODELTYPE_GUARD)
			{
				g_aPlayer[nIdy].state = PLAYERSTATE_DAMAGE;
				g_aPlayer[nIdy].nCntState = 10;

				// 水平方向の吹っ飛び力を代入
				g_aPlayer[nIdy].fFlyPower.x = -g_aPlayer[nIdy].move.x;
				g_aPlayer[nIdy].fFlyPower.z = -10;
				// 垂直方向の吹っ飛び力を代入
				g_aPlayer[nIdy].fFlyPower.y = 10;
				g_aPlayer[nIdy].nFlyCounter = 10;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = true;	// 吹っ飛び中に設定
			}
		}
		g_aPlayer[nIdy].moveR = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aPlayer[nIdy].moveL = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// モデル
	if (CollisionModelMinMax(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, &g_aPlayer[nIdy].pModel) == 2)
	{// 上にのる
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nIdy].nIdyShadow].pos.y = g_aPlayer[nIdy].pos.y + 0.1f;
		pShadow[g_aPlayer[nIdy].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nIdy].bJump = false;
		g_aPlayer[nIdy].nAirCounter = 0;// 空中攻撃制御カウンター
		if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
			}
		}
		if (!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] && g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
		{// 着地した
			for (int nCntEffect = 0; nCntEffect < 20; nCntEffect++)
			{// 着地エフェクト
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, 0.0f, cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * float(rand() % 5), cosf(SetRandomEffect())*0.5f, cosf(SetRandomEffect()) * float(rand() % 5)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_WALK);
			}
			PlaySound(SOUND_LABEL_LAND);
		}
		if ((g_aPlayer[nIdy].move.x > g_aPlayer[nIdy].MoveSpeed || g_aPlayer[nIdy].move.z > g_aPlayer[nIdy].MoveSpeed ||
			g_aPlayer[nIdy].move.x < -g_aPlayer[nIdy].MoveSpeed || g_aPlayer[nIdy].move.z < -g_aPlayer[nIdy].MoveSpeed))
		{// 歩いている　歩く　走る
		 // 敵と当たるものの距離
			float fPosX = (g_aPlayer[nIdy].pos.x - 200);
			float fPosZ = (g_aPlayer[nIdy].pos.z - 200);
			float fRadiusA = fPosX*fPosX + fPosZ*fPosZ;
			float fRadiusB = (150 + 150)* (150 + 150);

			if (fRadiusB > fRadiusA&&
				-fRadiusB < fRadiusA)
			{// 石の上
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_WALK);
				if (rand() % 5 == 0) {
					SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10) + D3DXVECTOR3(sinf(g_aPlayer[nIdy].rot.y) * 30, cosf(SetRandomEffect()), cosf(g_aPlayer[nIdy].rot.y) * 30),
						D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), 0.5f, cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 2.0f), float(rand() % 4) * 10, 0, EFFECTTYPE_CARTOON_TA);
				}
			}
			else
			{// 土
				SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect()) * 10, cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 10),
					D3DXVECTOR3(sinf(SetRandomEffect()) * 2 + sinf(g_aPlayer[nIdy].rot.y), cosf(SetRandomEffect()), cosf(SetRandomEffect()) * 2 + cosf(g_aPlayer[nIdy].rot.y)),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.9f, 0.9f, 1.0f), float(rand() % 30), 0, EFFECTTYPE_DIRTY);
			}

		}
		else
		{
		}
	}

	if (g_aPlayer[nIdy].pos.z >= 800&&g_aPlayer[nIdy].pos.z <= 850)
	{
		PlaySound(SOUND_RESULT_GARASU);
		for (int nCntE = 0; nCntE < 400; nCntE++)
		{// クラッシュ
			D3DXVECTOR3 rot = D3DXVECTOR3(SetRandomEffect(), SetRandomEffect(), SetRandomEffect());
			D3DXVECTOR3 move = D3DXVECTOR3(sinf(rot.x) * 40, cosf(rot.y) * 40, cosf(rot.z) * 40 + 30);
			SetEffectUP(4, D3DXVECTOR3(0, 550, 800+float(rand()%100)),
				move, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EFFECTUPTYPE_GUARD, float(rand() % 200 + 50));
		}
	}
	// メッシュウォール
	if (CollisionMeshWall(&g_aPlayer[nIdy].pos, &g_aPlayer[nIdy].posOld, &g_aPlayer[nIdy].move, g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight))
	{

	}


	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] != g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_JUMP])
	{// 前回と今回のジャンプモーションが違ったら着地モーション
		g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = true;
	}
	//------------------------------------/ 基本モーション /-----------------------------------------------------------------/


	if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY])
	{// ダメージ(吹っ飛び)中なら
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_FLY;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP])
	{// プレイヤーがジャンプ中なら
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_JUMP;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING])
	{// 着地中なら
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_LANDING;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L])
	{// キックL
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_KICK_L;
	}
	else if (g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R])
	{// キックR
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_KICK_R;
	}
	else
	{// プレイヤーが動いてないとき
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
	}
	// モーション
	MotionInfoPlayer(nIdy);

	//********************************************************************************************************************************
	// キャラクター制御
	//*************************************************************************************************************************
	switch (g_aPlayer[nIdy].nMotionType)
	{// プレイヤーのモーションが
	case PLAYER_MOTION_TYPE_JUMP:						// ジャンプモーションだったら
		break;
	case PLAYER_MOTION_TYPE_LANDING:					// 着地モーションだったら
		break;
	case PLAYER_MOTION_TYPE_FLY:						// ダメージ(吹っ飛び)モーションだったら
		break;
	}
	if (g_aPlayer[nIdy].bMotionStateOld[PLAYER_BOOL_FLY])
	{// 起き上がり終わったら
		g_aPlayer[nIdy].nCntState = 60;
		g_aPlayer[nIdy].state = PLAYERSTATE_INVINCIBLE;	// 無敵
	}

	//-------------------------------------/ 影 線 を移動する /---------------------------------------------------------------------------------------//
	SetPositionShadow(g_aPlayer[nIdy].nIdyShadow, D3DXVECTOR3(g_aPlayer[nIdy].aModel[0].mtxWorld._41, g_aPlayer[nIdy].aModel[0].mtxWorld._42, g_aPlayer[nIdy].aModel[0].mtxWorld._43), g_aPlayer[nIdy].Initpos);
	SetPositionLine(g_aPlayer[nIdy].nIdyLine, g_aPlayer[nIdy].pos, g_aPlayer[nIdy].rot);

	SetEffectSpeed(g_aPlayer[nIdy].move.z, MAX_SPEED);

	for (int nCnt = 0; nCnt < int(g_aPlayer[nIdy].move.z); nCnt++)
	{
		SetEffect(g_aPlayer[nIdy].pos + D3DXVECTOR3(sinf(SetRandomEffect())*(rand() % 10), cosf(SetRandomEffect())*(rand() % 3), cosf(SetRandomEffect())*(rand() % 10)),
			D3DXVECTOR3(sinf(SetRandomEffect())*(rand() % 7), cosf(SetRandomEffect())*(rand() % 1), cosf(SetRandomEffect())*(rand() % 7)),
			D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, (rand() % 50 / 100.0f), 0.0f, 1.0f), 3, 60, EFFECTTYPE_HIBANA);
	}

}



//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawPlayer(int nIdy, int nCamera)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;				// 親のマトリックス


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer[nIdy].mtxWorld);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].rot.y, g_aPlayer[nIdy].rot.x, g_aPlayer[nIdy].rot.z);
	D3DXMatrixMultiply(&g_aPlayer[nIdy].mtxWorld, &g_aPlayer[nIdy].mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].pos.x, g_aPlayer[nIdy].pos.y, g_aPlayer[nIdy].pos.z);
	D3DXMatrixMultiply(&g_aPlayer[nIdy].mtxWorld, &g_aPlayer[nIdy].mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].mtxWorld);

	if (g_aPlayer[nIdy].bUse)
	{
		for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayerData[nIdy].nNumParts; nCntPlayerModel++)
		{

			// 過去
			g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorldOld = g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld;

			if (g_aPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant == -1)
			{// プレイヤー
				mtxParent = g_aPlayer[nIdy].mtxWorld;
			}
			else
			{// 各親
				mtxParent = g_aPlayer[nIdy].aModel[g_aPlayerData[nIdy].aModel[nCntPlayerModel].nIdxModelPareant].mtxWorld;
			}
			// ------------------------/ 親モデルの描画 /-------------------------------------------//
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.y, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.x, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.x,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.y,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxParent);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayerData[nIdy].aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();
			if (g_aPlayer[nIdy].bDraw)
			{
				for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayerData[nIdy].aModel[nCntPlayerModel].nNumMat; nCntPlayer2++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
					// テクスチャの設定
					pDevice->SetTexture(0, g_aPlayerData[nIdy].aModel[nCntPlayerModel].pTextuer[nCntPlayer2]);

					// モデル(パーツ)の描画
					g_aPlayerData[nIdy].aModel[nCntPlayerModel].pMesh->DrawSubset(nCntPlayer2);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);

			// ------------------------/ 子モデルの描画 /-------------------------------------------//
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.y, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.x, g_aPlayer[nIdy].aModel[nCntPlayerModel].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.x,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.y,
				g_aPlayer[nIdy].aModel[nCntPlayerModel].pos.z);

			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld, &mtxParent);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nIdy].aModel[nCntPlayerModel].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayerData[nIdy].aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();
			if (g_aPlayer[nIdy].bDraw)
			{
				for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayerData[nIdy].aModel[nCntPlayerModel].nNumMat; nCntPlayer2++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
					// テクスチャの設定
					pDevice->SetTexture(0, g_aPlayerData[nIdy].aModel[nCntPlayerModel].pTextuer[nCntPlayer2]);
					// モデル(パーツ)の描画
					g_aPlayerData[nIdy].aModel[nCntPlayerModel].pMesh->DrawSubset(nCntPlayer2);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
			{// 判定のマトリックス
				if (g_aPlayer[nIdy].ColAttack[nCollision].bUse)
				{// 攻撃判定
				 // 前回の位置を繁栄
					g_aPlayer[nIdy].ColAttack[nCollision].mtxWorldOld = g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld;
					// 位置を反映
					D3DXMatrixTranslation(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.x, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
					D3DXMatrixMultiply(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);
				}
				if (g_aPlayer[nIdy].ColBody[nCollision].bUse)
				{// ダメージ判定
				 // 前回の位置を繁栄
					g_aPlayer[nIdy].ColBody[nCollision].mtxWorldOld = g_aPlayer[nIdy].ColBody[nCollision].mtxWorld;
					// 位置を反映
					D3DXMatrixTranslation(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, g_aPlayer[nIdy].ColBody[nCollision].ofset.x, g_aPlayer[nIdy].ColBody[nCollision].ofset.y, g_aPlayer[nIdy].ColBody[nCollision].ofset.z);
					D3DXMatrixMultiply(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
				}
			}

		}
		PlayerMotionType type = g_aPlayer[nIdy].nMotionType;
		for (int nCntOrbit = 0; nCntOrbit < MAX_PLAYER_ORBIT; nCntOrbit++)
		{
			if (g_aPlayer[nIdy].orbit[type][nCntOrbit].nOrbitLength <= 0)
			{
				break;
			}
			// 軌跡設定
			SetOrbit(g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].orbit[type][nCntOrbit].nIdyOrbit].mtxWorld,
				g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].orbit[type][nCntOrbit].nIdyOrbit].mtxWorldOld,
				g_aPlayer[nIdy].orbit[type][nCntOrbit].nOrbitLength,
				g_aPlayer[nIdy].orbit[type][nCntOrbit].OrbitStart, g_aPlayer[nIdy].orbit[type][nCntOrbit].OrbitEnd,
				ORBITTYPE(g_aPlayer[nIdy].orbit[type][nCntOrbit].nType), g_aPlayer[nIdy].orbit[type][nCntOrbit].motion);
		}
		PlayerColInfo(nIdy);
	}
}
//==============================================================================================================================
// モデルの取得
//==============================================================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
PlayerData *GetPlayerData(void)
{
	return &g_aPlayerData[0];
}

//==============================================================================================================================
// モデルのモーション
//==============================================================================================================================
void MotionInfoPlayer(int nIdy)
{
	for (int nCnt = 0 ; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_aPlayer[nIdy].nFrameAllCounter == 1)
		{// 今のタイプと前のタイプが違ったら
			g_aPlayer[nCnt].nMotionGetCol = PLAYER_MOTION_TYPE_MAX;
		}
	}
	if (g_aPlayer[nIdy].nMotionType != g_aPlayer[nIdy].nMotionTypeOld)
	{// 今のタイプと前のタイプが違ったら
		g_aPlayer[nIdy].nKeyCounter = 0;						// キーをリセット
		g_aPlayer[nIdy].nFrameCounter = 0;					// フレームをリセット
		g_aPlayer[nIdy].bBlend = true;	// ブレンドにする
		g_aPlayer[nIdy].nFrame = BLEND_FRAME;					// ブレンドのフレーム
		g_aPlayer[nIdy].nFrameAllCounter = 0;					// 全てのフレーム
	}
	else if (g_aPlayer[nIdy].bBlend == false )
	{// 通常時のフレーム
		g_aPlayer[nIdy].nFrame = g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].nFrame;
	}
	if (g_aPlayer[nIdy].nFrameCounter == 0)
	{// 次のキーフレームのモーションを読み込む
		for (int nCntParts = 0; nCntParts < g_aPlayerData[nIdy].nNumParts; nCntParts++)
		{// Key - 現在 間の差分計算
			g_aPlayer[nIdy].aModel[nCntParts].posAddMotion =
				(g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].aPartsKey[nCntParts].pos
					- g_aPlayer[nIdy].aModel[nCntParts].pos + g_aPlayer[nIdy].aModel[nCntParts].Initpos) / float(g_aPlayer[nIdy].nFrame);

			g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion = (g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].aPartsKey[nCntParts].rot - g_aPlayer[nIdy].aModel[nCntParts].rot);

			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.x += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.y += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z > D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z < -D3DX_PI)
			{
				g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion.z += D3DX_PI * 2.0f;
			}
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion = g_aPlayer[nIdy].aModel[nCntParts].DiffrotMotion / float(g_aPlayer[nIdy].nFrame);
		}
	}
	for (int nCntParts = 0; nCntParts < g_aPlayerData[nIdy].nNumParts; nCntParts++)
	{// モーション差分加算

		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion.z += D3DX_PI * 2.0f;
		}
		g_aPlayer[nIdy].aModel[nCntParts].pos += g_aPlayer[nIdy].aModel[nCntParts].posAddMotion;
		g_aPlayer[nIdy].aModel[nCntParts].rot += g_aPlayer[nIdy].aModel[nCntParts].rotAddMotion;

		if (g_aPlayer[nIdy].aModel[nCntParts].rot.x > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.x < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.y > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.y < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.z > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nIdy].aModel[nCntParts].rot.z < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nIdy].aModel[nCntParts].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_aPlayer[nIdy].nFrameCounter++;
	g_aPlayer[nIdy].nFrameAllCounter++;
	if (g_aPlayer[nIdy].bBlend  && g_aPlayer[nIdy].nFrameCounter == BLEND_FRAME)
	{// ブレンド　ブレンドのフレーム数に達する
		g_aPlayer[nIdy].bBlend = false;
		g_aPlayer[nIdy].nFrameCounter = 0;
		g_aPlayer[nIdy].nKeyCounter = 0;// ブレンドで次のモーションの0キー目は行ってる
		if (g_aPlayer[nIdy].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL)
		{
			g_aPlayer[nIdy].nKeyCounter = 1;
		}
	}
	else if (g_aPlayer[nIdy].bBlend == false&& g_aPlayer[nIdy].nFrameCounter == g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[g_aPlayer[nIdy].nKeyCounter].nFrame)
	{// フレームカウントが上回る
		g_aPlayer[nIdy].nFrameCounter = 0;
		if (g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].bLoop == false &&
			g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].nNumKey - 1 == g_aPlayer[nIdy].nKeyCounter)
		{// ループしないかつキー数が超える
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = false;	// 着地キャンセル

			switch (g_aPlayer[nIdy].nMotionType)
			{// プレイヤーのモーションが
			case PLAYER_MOTION_TYPE_JUMP:																// ジャンプモーションだったら
				break;
			case PLAYER_MOTION_TYPE_LANDING:															// 着地モーションだったら
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_LANDING] = false;										// 着地状態じゃない状態にする
				break;
			case PLAYER_MOTION_TYPE_FLY:																// ダメージ(吹っ飛び)モーションだったら
				break;
			default:
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R] = false;										// 着地状態じゃない状態にする
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L] = false;										// 着地状態じゃない状態にする

				break;
			}
			g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_MAX;
		}
		else
		{// 次のキー数を獲得
			g_aPlayer[nIdy].nKeyCounter = (g_aPlayer[nIdy].nKeyCounter + 1) % g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].nNumKey;
		}
	}

}
//==============================================================================================================================
// プレイヤーの体の当たり判定
//==============================================================================================================================
bool CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, float nPower, float fDamageSide, float fDamageV,int nIdy)
{
	bool bHit = false;
	if (GetCamera()->nState == CAMERASTATE_NONE )
	{
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			if (g_aPlayer[nCnt].bUse)
			{
				if (nCnt != nIdy)
				{
					if (g_aPlayer[nCnt].state != PLAYERSTATE_DEATH&&
						g_aPlayer[nCnt].state != PLAYERSTATE_INVINCIBLE&&
						g_aPlayer[nCnt].state != PLAYERSTATE_DAMAGE &&
						!g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY])
					{// 死んでない
						{// ダメージ中じゃなければ
							for (int nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++)
							{// 体の当たり判定分ループ
								if (g_aPlayer[nCnt].ColBody[nCntCollision].bUse)
								{
									// 二つの当たり判定の座標の距離を計算
									float PosX = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._41 - mtxWorld.x);		// (プレイヤーの体の当たり判定の座標.x - 敵の攻撃の当たり判定の座標.x)
									float PosY = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._42 - mtxWorld.y);		// (プレイヤーの体の当たり判定の座標.y - 敵の攻撃の当たり判定の座標.y)
									float PosZ = (g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._43 - mtxWorld.z);		// (プレイヤーの体の当たり判定の座標.z - 敵の攻撃の当たり判定の座標.z)

									float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// それぞれを2乗してすべて足して距離を出す
									// 二つの当たり判定の半径の距離を計算
									float fCollisionRadius = (g_aPlayer[nCnt].ColBody[nCntCollision].fRadius + fRadius);	// プレイヤーの体の当たり判定の半径+敵の攻撃の当たり判定の半径を足す
									float fRadiusLenght = fCollisionRadius * fCollisionRadius;	// 二つの半径を足した値を2乗して、半径の距離を計算を計算

									if (fRadiusLenght > fPosLenght &&
										-fRadiusLenght < fPosLenght)
									{// 座標の距離が半径の距離よりも小さかったら
										float fAttackAngle = atan2f(pos.x - g_aPlayer[nCnt].pos.x, pos.z - g_aPlayer[nCnt].pos.z);	// 当たった角度

										if (g_aPlayer[nCnt].nMotionGetCol != g_aPlayer[nIdy].nMotionType)
										{
											g_aPlayer[nCnt].fLife -= nPower;

											if (g_aPlayer[nCnt].fLife <= 0)
											{// ライフが0以下になったら
												PlayerLifeZero(nCnt, fAttackAngle, fDamageSide, fDamageV);
											}
											else
											{// ヒット
												if (fDamageSide > 1.0f && fDamageV > 5.0f)
												{// 吹っ飛び力があるとき
													PlaySound(SOUND_LABEL_HIT001);
													SetPlayerVoice(PLAYER_MOTION_TYPE_FLY);
													g_aPlayer[nCnt].state = PLAYERSTATE_DAMAGE;
													g_aPlayer[nCnt].nCntState = 1;
													// 水平方向の吹っ飛び力を代入
													g_aPlayer[nCnt].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;
													// 垂直方向の吹っ飛び力を代入
													g_aPlayer[nCnt].fFlyPower.y = fDamageV;
													g_aPlayer[nCnt].nFlyCounter = 10;
													g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY] = true;	// 吹っ飛び中に設定

													g_aPlayer[nCnt].Destrot.y = fAttackAngle - D3DX_PI;		// キャラクターの向きを変更
													g_aPlayer[nCnt].rot.y = fAttackAngle - D3DX_PI;			// キャラクターの向きを変更
												}
												else
												{// 水平のみ


													PlaySound(SOUND_LABEL_HIT000);

													// 吹っ飛び力を代入
													g_aPlayer[nCnt].state = PLAYERSTATE_DAMAGE;
													g_aPlayer[nCnt].nCntState = 3;
													g_aPlayer[nCnt].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;
													g_aPlayer[nCnt].fFlyPower.y = fDamageV;
													g_aPlayer[nCnt].nFlyCounter = 10;
													g_aPlayer[nCnt].Destrot.y = fAttackAngle - D3DX_PI;		// キャラクターの向きを変更
													g_aPlayer[nCnt].rot.y = fAttackAngle - D3DX_PI;			// キャラクターの向きを変更

												}

											}
											// ヒットエフェクト
											HitEffect(D3DXVECTOR3(g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._41, g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._42, g_aPlayer[nCnt].ColBody[nCntCollision].mtxWorld._43));

											bHit = true;	// 当たった									// モーション情報取得
										}
										g_aPlayer[nCnt].nMotionGetCol = g_aPlayer[nIdy].nMotionType;
										break;
									}
								}
							}
						}
					}
				}

			}
		}
	}
	return bHit;
}
//==============================================================================================================================
// 判定設定(攻撃判定）
//==============================================================================================================================
void SetAttackCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius, int nCollisionStart, int nCollisionEnd,int nStopTime, PlayerMotionType nMotionType)
{
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayerData[nIdy].ColAttack[nCollision].bUse == false)
		{
			g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent = nIdxParent;
			g_aPlayerData[nIdy].ColAttack[nCollision].ofset = offset;
			g_aPlayerData[nIdy].ColAttack[nCollision].fRadius = fRadius;
			g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionStart = nCollisionStart;
			g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionEnd = nCollisionEnd;
			g_aPlayerData[nIdy].ColAttack[nCollision].nStopTime = nStopTime;
			g_aPlayerData[nIdy].ColAttack[nCollision].nMotionType = nMotionType;
			g_aPlayerData[nIdy].ColAttack[nCollision].bUse = true;
			// 位置を反映
			D3DXMatrixTranslation(&g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.x, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayerData[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayerData[nIdy].aModel[g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);
			break;
		}
	}

}
//==============================================================================================================================
// 判定設定（ダメージ判定）
//==============================================================================================================================
void SetBodyCollisionPlayer(int nIdy,int nIdxParent, D3DXVECTOR3 offset, float fRadius)
{
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayerData[nIdy].ColBody[nCollision].bUse == false)
		{
			g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent = nIdxParent;
			g_aPlayerData[nIdy].ColBody[nCollision].ofset = offset;
			g_aPlayerData[nIdy].ColBody[nCollision].fRadius = fRadius;
			g_aPlayerData[nIdy].ColBody[nCollision].bUse = true;
			// 位置を反映
			D3DXMatrixTranslation(&g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, g_aPlayerData[nIdy].ColBody[nCollision].ofset.x, g_aPlayerData[nIdy].ColBody[nCollision].ofset.y, g_aPlayerData[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayerData[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayerData[nIdy].aModel[g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
			break;
		}
	}

}
//==============================================================================================================================
// プレイヤーの操作
//==============================================================================================================================
void PlayerController(int nIdy)
{
	GamePad *pGamePad = GetgamePadStick();
	Camera *pCamera = GetCamera();
#if 1
	static int nCon = 0;

	if (nCon == nIdy)
	{
		if (GetKeyboardTrigger(DIK_J)||GetGamePadTrigger(0,BUTTON_R1))
		{
			PlaySound(SOUND_SELECTMOVE);
			g_aPlayer[nIdy].moveL.x += sinf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].moveL.z += cosf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].Destrot.y = D3DX_PI / 4 * 3 + pCamera->rot.y;	// 目的の角度
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_L] = true;
		}
		if (GetKeyboardTrigger(DIK_F)|| GetGamePadTrigger(0, BUTTON_L1))
		{
			PlaySound(SOUND_SELECTMOVE);
			g_aPlayer[nIdy].moveR.x += sinf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].moveR.z += cosf(D3DX_PI / 4  + pCamera->rot.y)*g_aPlayer[nIdy].MoveSpeed;
			g_aPlayer[nIdy].Destrot.y = -D3DX_PI / 4 * 3 + pCamera->rot.y;	// 目的の角度
			g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_KICK_R] = true;
		}
		SetSpGageDestSkillPoint(int(g_aPlayer[nIdy].moveL.z / MAX_SPEED * 100), LEG_TYPE_RIGHT );
		SetSpGageDestSkillPoint(int(g_aPlayer[nIdy].moveR.z / MAX_SPEED * 100), LEG_TYPE_LEFT);

		if (!g_aPlayer[nIdy].bJump &&
			!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] &&
			!g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_FLY] )
		{// ジャンプ&吹っ飛びしていなければ
			if (GetKeyboardTrigger(DIK_SPACE)||GetGamePadTrigger(0,BUTTON_A))
			{// ジャンプ
				PlaySound(SOUND_LABEL_JUMP);
				g_aPlayer[nIdy].move.y = g_aPlayer[nIdy].JumpSpeed;	// 移動量加算
				g_aPlayer[nIdy].bJump = true;
				g_aPlayer[nIdy].bMotionState[PLAYER_BOOL_JUMP] = true;
			}

		}
	}
#endif
}
//==============================================================================================================================
// プレイヤー同士の衝突の判定
//==============================================================================================================================
void PlayerOverLap(int nIdy)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_aPlayer[nCnt].bUse)
		{
			if (g_aPlayer[nIdy].state != PLAYERSTATE_DEATH || g_aPlayer[nCnt].state != PLAYERSTATE_DAMAGE)
			{
				if (nIdy != g_aPlayer[nCnt].nIdy)
				{
					// 敵と当たるものの距離
					float fPosX = (g_aPlayer[nIdy].pos.x - g_aPlayer[nCnt].pos.x);
					float fPosY = (g_aPlayer[nIdy].pos.y - g_aPlayer[nCnt].pos.y);
					float fPosZ = (g_aPlayer[nIdy].pos.z - g_aPlayer[nCnt].pos.z);
					float fRadiusA = fPosX*fPosX + fPosY*fPosY + fPosZ*fPosZ;
					float fRadiusB = 0;

					fRadiusB = (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nIdy].fRadiusWidth)* (g_aPlayer[nCnt].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);

					if (fRadiusB > fRadiusA&&
						-fRadiusB < fRadiusA)
					{// 人と人のあたり判定
						float fAngle2 = atan2f(g_aPlayer[nIdy].mtxWorld._41 - g_aPlayer[nCnt].pos.x, g_aPlayer[nIdy].mtxWorld._43 - g_aPlayer[nCnt].pos.z);
						g_aPlayer[nIdy].pos.x = g_aPlayer[nCnt].pos.x + sinf(fAngle2) * (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);
						g_aPlayer[nIdy].pos.z = g_aPlayer[nCnt].pos.z + cosf(fAngle2) * (g_aPlayer[nIdy].fRadiusWidth + g_aPlayer[nCnt].fRadiusWidth);
					}
				}

			}
		}
	}

}
//==============================================================================================================================
// プレイヤーのタイプ設定
//==============================================================================================================================
void SetPlayer(int nIdy, D3DXVECTOR3 pos)
{
	if (!g_aPlayer[nIdy].bUse)
	{
		g_nNumAll++;
		g_aPlayer[nIdy].pos = pos;
		g_aPlayer[nIdy].MoveSpeed = g_aPlayerData[nIdy].MoveSpeed;
		g_aPlayer[nIdy].JumpSpeed = g_aPlayerData[nIdy].JumpSpeed;
		g_aPlayer[nIdy].fRadiusWidth = g_aPlayerData[nIdy].fRadiusWidth;
		g_aPlayer[nIdy].fRadiusHeight = g_aPlayerData[nIdy].fRadiusHeight;
		g_aPlayer[nIdy].fLife = float(g_aPlayerData[nIdy].nInitLife);
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nIdy].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nIdy].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nIdy].nIdy = nIdy;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// ダメージ設定
			g_aPlayer[nIdy].ColBody[nCollision].nIdxParent = g_aPlayerData[nIdy].ColBody[nCollision].nIdxParent;
			g_aPlayer[nIdy].ColBody[nCollision].ofset = g_aPlayerData[nIdy].ColBody[nCollision].ofset;
			g_aPlayer[nIdy].ColBody[nCollision].fRadius = g_aPlayerData[nIdy].ColBody[nCollision].fRadius;
			g_aPlayer[nIdy].ColBody[nCollision].bUse = g_aPlayerData[nIdy].ColBody[nCollision].bUse;
			// 位置を反映
			D3DXMatrixTranslation(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, g_aPlayer[nIdy].ColBody[nCollision].ofset.x, g_aPlayer[nIdy].ColBody[nCollision].ofset.y, g_aPlayer[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].ColBody[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColBody[nCollision].nIdxParent].mtxWorld);
		}
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{// 攻撃設定
			g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent = g_aPlayerData[nIdy].ColAttack[nCollision].nIdxParent;
			g_aPlayer[nIdy].ColAttack[nCollision].ofset = g_aPlayerData[nIdy].ColAttack[nCollision].ofset;
			g_aPlayer[nIdy].ColAttack[nCollision].fRadius = g_aPlayerData[nIdy].ColAttack[nCollision].fRadius;
			g_aPlayer[nIdy].ColAttack[nCollision].nCollisionStart = g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionStart;
			g_aPlayer[nIdy].ColAttack[nCollision].nCollisionEnd = g_aPlayerData[nIdy].ColAttack[nCollision].nCollisionEnd;
			g_aPlayer[nIdy].ColAttack[nCollision].nStopTime = g_aPlayerData[nIdy].ColAttack[nCollision].nStopTime;
			g_aPlayer[nIdy].ColAttack[nCollision].nMotionType = g_aPlayerData[nIdy].ColAttack[nCollision].nMotionType;
			g_aPlayer[nIdy].ColAttack[nCollision].bUse = g_aPlayerData[nIdy].ColAttack[nCollision].bUse;
			// 位置を反映
			D3DXMatrixTranslation(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, g_aPlayer[nIdy].ColAttack[nCollision].ofset.x, g_aPlayer[nIdy].ColAttack[nCollision].ofset.y, g_aPlayer[nIdy].ColAttack[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].ColAttack[nCollision].mtxWorld, &g_aPlayer[nIdy].aModel[g_aPlayer[nIdy].ColAttack[nCollision].nIdxParent].mtxWorld);

		}
		for (int nIdyModel = 0; nIdyModel < g_aPlayerData[nIdy].nNumParts; nIdyModel++)
		{// モデル初期位置設定
			g_aPlayer[nIdy].aModel[nIdyModel].pos = g_aPlayerData[nIdy].aModel[nIdyModel].pos;
			g_aPlayer[nIdy].aModel[nIdyModel].Initpos = g_aPlayerData[nIdy].aModel[nIdyModel].pos;
			g_aPlayer[nIdy].aModel[nIdyModel].rot = g_aPlayerData[nIdy].aModel[nIdyModel].rot;
			g_aPlayer[nIdy].aModel[nIdyModel].rot = g_aPlayerData[nIdy].aMotion[g_aPlayer[nIdy].nMotionType].aKey[0].aPartsKey[nIdyModel].rot;
		}
		for (int nIdyMotion = 0; nIdyMotion < PLAYER_MOTION_TYPE_MAX; nIdyMotion++)
		{
			for (int nIdyOrbit = 0; nIdyOrbit < MAX_PLAYER_ORBIT; nIdyOrbit++)
			{
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nIdyOrbit = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nIdyOrbit;				// 軌跡の数
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nOrbitLength = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nOrbitLength;		// 軌跡の長さ
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitStart = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitStart;			// 視点
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitEnd = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].OrbitEnd;				// 終点
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].nType = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].nType;					// 軌跡のタイプ
				g_aPlayer[nIdy].orbit[nIdyMotion][nIdyOrbit].motion = g_aPlayerData[nIdy].orbit[nIdyMotion][nIdyOrbit].motion;	// モーション
			}
		}
		if (GetMode() == MODE_GAME)
		{
			// 影設定
			g_aPlayer[nIdy].nIdyShadow = SetShadow(g_aPlayer[nIdy].pos, g_aPlayer[nIdy].rot, g_aPlayer[nIdy].fRadiusWidth * 2.5f, g_aPlayer[nIdy].fRadiusWidth);
			// ライン判定設定
			g_aPlayer[nIdy].nIdyLine = CollisionLine(g_aPlayer[nIdy].pos, D3DXVECTOR3(-g_aPlayer[nIdy].fRadiusWidth, 0.0f, -g_aPlayer[nIdy].fRadiusWidth), D3DXVECTOR3(g_aPlayer[nIdy].fRadiusWidth, g_aPlayer[nIdy].fRadiusHeight, g_aPlayer[nIdy].fRadiusWidth), LINETYPE_PLAYER);
		}
		g_aPlayer[nIdy].bUse = true;
	}
}
void ReleasePlayer(int nIdy)
{
	if (g_aPlayer[nIdy].bUse)
	{
		g_nNumAll--;
		g_aPlayer[nIdy].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nIdy].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nIdy].nMotionGetCol = PLAYER_MOTION_TYPE_NEUTRAL;	// モーションタイプ
		g_aPlayer[nIdy].nMotionGetColOld = PLAYER_MOTION_TYPE_MAX;	// 前回のモーションタイプ
		g_aPlayer[nIdy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_aPlayer[nIdy].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転の差分
		g_aPlayer[nIdy].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の回転
		g_aPlayer[nIdy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転

		for (int nIdySound = 0; nIdySound < SOUND_LABEL_MAX; nIdySound++)
		{// 音
			g_aPlayer[nIdy].bSound[nIdySound] = false;
		}
		g_aPlayer[nIdy].state = PLAYERSTATE_NONE;
		g_aPlayer[nIdy].nCntState = 0;
		g_aPlayer[nIdy].pMeshFieldLand = NULL;
		g_aPlayer[nIdy].pModel = NULL;
		g_aPlayer[nIdy].pModelAtk = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nIdy].bMotionState[nbMotion] = false;
			g_aPlayer[nIdy].bMotionStateOld[nbMotion] = false;
		}
		g_aPlayer[nIdy].bBlend = false;
		g_aPlayer[nIdy].nDamage = 0;
		g_aPlayer[nIdy].bJump = false;							// ジャンプ
		g_aPlayer[nIdy].nFlyCounter = 0;						// 飛んでいる時間
		g_aPlayer[nIdy].fRotSpeed = 0.0f;						// 回転の減速スピード
		g_aPlayer[nIdy].nAirCounter = 0;						// 浮遊時の攻撃制御カウンター
		g_aPlayer[nIdy].nLoopCounter = 0;						// ループカウンター
		g_aPlayer[nIdy].nIdy = nIdy;							// 番号
		g_aPlayer[nIdy].nKeyCounter = 0;		// キー数
		g_aPlayer[nIdy].nFrameCounter = 0;		// フレーム数
		g_aPlayer[nIdy].nFrameAllCounter = 0;	// 最大フレーム
		g_aPlayer[nIdy].nFrame = 0;				// フレーム
		g_aPlayer[nIdy].bUse = false;
		g_aPlayer[nIdy].bDraw = true;			// 映ってるかどうか
	}
}
//==============================================================================================================================
// プレイヤーのエフェクト
//==============================================================================================================================
void PlayerMotionEffect(int nCntPlayer, int nCollision)
{
//	SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._41, g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._42, g_aPlayer[nCntPlayer].ColAttack[nCollision].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aPlayer[nCntPlayer].ColAttack[nCollision].fRadius, 10,EFFECTTYPE_SMOKE);
}
//==============================================================================================================================
// プレイヤーの判定
//==============================================================================================================================
void PlayerColInfo(int nIdy)
{
	// 攻撃判定処理
	for (int nCnt = 0; nCnt < MAX_COLLISION; nCnt++)
	{
		if (g_aPlayer[nIdy].ColAttack[nCnt].bUse == true)
		{
			if (g_aPlayer[nIdy].ColAttack[nCnt].nMotionType == g_aPlayer[nIdy].nMotionType)
			{// 決めたステートと今のステートが一緒だったら判定開始
				if (g_aPlayer[nIdy].nFrameAllCounter >= g_aPlayer[nIdy].ColAttack[nCnt].nCollisionStart &&
					g_aPlayer[nIdy].nFrameAllCounter <= g_aPlayer[nIdy].ColAttack[nCnt].nCollisionEnd)
				{
					if (CollisionModelMinMax(&D3DXVECTOR3(g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._41, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._42, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._43),
						&D3DXVECTOR3(g_aPlayer[nIdy].aModel[0].mtxWorld._41, g_aPlayer[nIdy].aModel[0].mtxWorld._42, g_aPlayer[nIdy].aModel[0].mtxWorld._43), &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						g_aPlayer[nIdy].ColAttack[nCnt].fRadius, g_aPlayer[nIdy].ColAttack[nCnt].fRadius, &g_aPlayer[nIdy].pModel) != 0)
					{
					}
					PlayerMotionEffect(nIdy, nCnt);
					// 攻撃当たり判定

					if (CollisionModelGetInfo(&D3DXVECTOR3(g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._41, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._42, g_aPlayer[nIdy].ColAttack[nCnt].mtxWorld._43),
						g_aPlayer[nIdy].ColAttack[nCnt].fRadius, g_aPlayer[nIdy].ColAttack[nCnt].fRadius, &g_aPlayer[nIdy].pModelAtkData) != 0)
					{
					}
					if (g_aPlayer[nIdy].pModelAtkData != NULL)
					{
					}
				}
			}
		}
	}

}

//=====================================================================================================================
// ライフゼロ
//=====================================================================================================================
void PlayerLifeZero(int nCnt, float fAngle, float fDamageH, float fDamageV)
{
	g_nNumAll--;
	g_aPlayer[nCnt].state = PLAYERSTATE_DEATH;
	g_aPlayer[nCnt].nCntState = 100;

	// 水平方向の吹っ飛び力を代入
	g_aPlayer[nCnt].fFlyPower.x = sinf(fAngle - D3DX_PI) * fDamageH;
	g_aPlayer[nCnt].fFlyPower.z = cosf(fAngle - D3DX_PI) * fDamageH;

	// 垂直方向の吹っ飛び力を代入
	g_aPlayer[nCnt].fFlyPower.y = fDamageV;
	g_aPlayer[nCnt].nFlyCounter = 20;
	g_aPlayer[nCnt].bMotionState[PLAYER_BOOL_FLY] = true;													// 吹っ飛び中に設定
	g_aPlayer[nCnt].Destrot.y = fAngle - D3DX_PI;							// キャラクターの向きを変更
	g_aPlayer[nCnt].rot.y = fAngle - D3DX_PI;							// キャラクターの向きを変更

}
//=========================================================================================================================
// ボイス
//===========================================================================================================================
void SetPlayerVoice(PlayerMotionType motion)
{
	if (motion == PLAYER_MOTION_TYPE_FLY)
	{
		PlaySound(SOUND_HUMAN_BUTOBI);
	}
	if (motion == PLAYER_MOTION_TYPE_MAX)
	{
		PlaySound(SOUND_HUMAN_GARD000);
	}
}
//==============================================================================================================================
//　ライフのチェック
//==============================================================================================================================
int CheckLife(void)
{
	//変数宣言
	int BigLife = 0;

	for (int nCntId = 0; nCntId < MAX_PLAYER - 1; nCntId++)
	{
		if (g_aPlayer[BigLife].fLife > g_aPlayer[nCntId + 1].fLife)
		{
			BigLife = BigLife;		//Idの保存
		}
		else
		{
			BigLife = nCntId + 1;	//Idの保存
		}
	}

	return BigLife;
}