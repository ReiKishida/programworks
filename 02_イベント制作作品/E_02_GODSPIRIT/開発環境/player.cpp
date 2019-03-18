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
#include "effect.h"
#include "model.h"
#include "fade.h"
#include "result.h"
#include "item.h"
#include "meshField.h"
#include "meshWall.h"
#include "line.h"
#include "orbit.h"
#include "gamepad.h"
#include "enemy.h"
#include "life.h"
#include "sound.h"
#include "effectUP.h"
#include "UI.h"
#include "game.h"
#include "tutorial.h"
#include "effect3D.h"
#include "pause.h"
#include "spirit.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define DOWN_SPEED						(0.4f)	// 減速量
#define PLAYER_ROT_SPEED_NORMAL			(0.15f)	// 通常時の角度を変える量
#define PLAYER_ROT_SPEED_ATTACK			(0.02f)	// 攻撃時の角度を変える量
#define GRAVITY_SPEED					(0.6f)	// 重力加算スピード
#define INIT_JUMP_SPEED					(7)		// ジャンプ力
#define INIT_MOVE_SPEED					(0.3f)	// 移動量
#define PLAYER_MOTION_STEP_MOVE_SPEED	(5.0f)	// ステップのスピード
#define BLEND_FRAME						(3)		// ブレンド
#define ATTACKDAMAGE					(1)		// アタックのダメージ量
#define WARKZONE						(50)	// 歩きのゾーン
#define PLAYER_MOTION_MOVE_ZONE			(1.0f)	// 歩きのモーションゾーン
#define AIR_COUNT						(2)		// 空中で行動できる回数
#define PLAYER_SPIRIT_TIME				(30)	// スピリットが1減少するフレーム数
#define PLAYER_SPIRIT_RECOVERY_TIME		(10)	// スピリットが1回復するフレーム数
#define PLAYER_SPIRIT_DAMAGE			(1)		// スピリットが減少のダメージ

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Player g_aPlayer[MAX_PLAYER];
int g_nKeyCounter;			// キー数
int g_nFrameCounter;		// フレーム数
int g_nFrameAllCounter;		// 最大フレーム数
int g_nFrame;				// フレー
int g_nSpiritCounter;			// スピリット減少のカウンター
int g_nSpiritRecoveryCounter;	// スピリット回復のカウンター

//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置・向きの初期設定
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
		g_aPlayer[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Diffpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Initpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (GetMode() == MODE_GAME)
		{
			g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].Diffpos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
			g_aPlayer[nCntPlayer].Initpos = D3DXVECTOR3(0.0f, 0.0f, -0.0f);
		}
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
		g_aPlayer[nCntPlayer].nMotionTypeOld = PLAYER_MOTION_TYPE_MAX;
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Diffrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].Destrot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			g_aPlayer[nCntPlayer].bSound[nCntSound] = false;
		}
		g_aPlayer[nCntPlayer].nLife = g_aPlayer[nCntPlayer].nInitLife;
		g_aPlayer[nCntPlayer].nSpirit = g_aPlayer[nCntPlayer].nInitSpirit;
		g_aPlayer[nCntPlayer].nIdyLockEnemy = 0;
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_NONE;
		g_aPlayer[nCntPlayer].nCntState = 0;
		g_aPlayer[nCntPlayer].pMeshFieldLand = NULL;
		for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
		{
			g_aPlayer[nCntPlayer].bMotionState[nbMotion] = false;
			g_aPlayer[nCntPlayer].bMotionStateOld[nbMotion] = false;
		}
		for (int nbMotion = 0; nbMotion < PLAYER_ATTACK_TYPE_MAX; nbMotion++)
		{
			g_aPlayer[nCntPlayer].bAttackType[nbMotion];
		}
		g_aPlayer[nCntPlayer].bBlend = false;
		g_aPlayer[nCntPlayer].nDamage = 0;
		g_aPlayer[nCntPlayer].bAttention = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nFlyCounter = 0;						// 飛んでいる時間
		g_aPlayer[nCntPlayer].fRotSpeed = 0.0f;						// 回転の減速スピード
		g_aPlayer[nCntPlayer].nAirCounter = 0;							// 浮遊時の攻撃制御カウンター
		g_aPlayer[nCntPlayer].nLoopCounter = 0;						// ループカウンター
		g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAND;		// 最初は素手
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// 武器の状態
			g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
			{// Key - 現在 間の差分計算
				g_aPlayer[nCntPlayer].aModel[nCntParts][nCntWeapon].rot = g_aPlayer[nCntPlayer].aMotion[PLAYER_MOTION_TYPE_JUMP][nCntWeapon].aKey[0].aPartsKey[nCntParts].rot;
			}
		}
		g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] = true;// 最初は素手
	
		 // 影設定
		g_aPlayer[nCntPlayer].nIdyShadow = SetShadow(g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].rot, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusWidth);
		// ライン判定設定
		g_aPlayer[nCntPlayer].nIdyLine = CollisionLine(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(-g_aPlayer[nCntPlayer].fRadiusWidth, 0.0f, -g_aPlayer[nCntPlayer].fRadiusWidth), D3DXVECTOR3(g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, g_aPlayer[nCntPlayer].fRadiusWidth), LINETYPE_PLAYER);
		// ライフの設定
		SetPlayerLife(g_aPlayer[nCntPlayer].nInitLife);
		// ｓｐｉｒｉｔの設定
		SetPlayerSpirit(g_aPlayer[nCntPlayer].nInitSpirit);


	}
	g_nKeyCounter = 0;
	g_nFrameCounter = 0;
	g_nFrameAllCounter = 0;
	g_nFrame = 0;
	g_nSpiritCounter = 0;
	g_nSpiritRecoveryCounter = 0;

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// 武器の状態
			for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayer[nCntPlayer].nNumParts; nCntPlayerModel++)
			{
				// メッシュの開放
				if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh != NULL)
				{
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh->Release();
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pMesh = NULL;
				}
				// マテリアルの開放
				if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat != NULL)
				{
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat->Release();
					g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pBuffMat = NULL;
				}
				// テクスチャの開放
				for (int nCntMat = 0; nCntMat < int(g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].nNumMat); nCntMat++)
				{
					if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat] != NULL)
					{
						g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat]->Release();
						g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][nCntWeapon].pTextuer[nCntMat] = NULL;
					}
				}
			}
		}
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdatePlayer(int nCntPlayer)
{


	DIJOYSTATE2 *pGamePad = GetgamePadStick();
	MeshField *pMeshField = GetMeshField();
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel();
	UI *pUI = GetUI();
	float nDirection = 0;
	float nDirectionY = 0;
	float nDirectionZ = 0;
	nDirection = SetRandomEffect();
	nDirectionY = SetRandomEffect();
	nDirectionZ = SetRandomEffect();

	// 前回の位置 タイプ
	g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
	g_aPlayer[nCntPlayer].nMotionTypeOld = g_aPlayer[nCntPlayer].nMotionType;
	for (int nbMotion = 0; nbMotion < PLAYER_BOOL_MAX; nbMotion++)
	{
		g_aPlayer[nCntPlayer].bMotionStateOld[nbMotion] = g_aPlayer[nCntPlayer].bMotionState[nbMotion];
	}

	switch (g_aPlayer[nCntPlayer].state)
	{
	case PLAYERSTATE_NONE:
		break;
	case PLAYERSTATE_DAMAGESTART:
		g_aPlayer[nCntPlayer].state = PLAYERSTATE_DAMAGE;
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer[nCntPlayer].nCntState--;
		if (g_aPlayer[nCntPlayer].nCntState <= 0)
		{
			g_aPlayer[nCntPlayer].state = PLAYERSTATE_NONE;
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer[nCntPlayer].nCntState--;
		GetCamera()->rot.y = -D3DX_PI / 4 + GetPlayer()->rot.y;
		GetCamera()->fLength = 150;
		GetCamera()->fHeight = 270;
		GetCamera()->fLengthR = 1;
		GetCamera()->fHeightR = 1;
		if (g_aPlayer[nCntPlayer].nCntState <= 0)
		{
			if (GetGameState() != GAMESTATE_END&&GetGameState() != GAMESTATE_END2)
			{
				SetGameState(GAMESTATE_END);
				SetResultType(RESULTTYPE_GAMEOVER);
		
			}
		}
		break;
	}
	// 武器の状態
	switch (g_aPlayer[nCntPlayer].nWeaponState)
	{
	case PLAYERWEAPON_HAND:	// 手
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] == false)
		{// 武器の状態が違ったら
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAND] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
		}
		pUI[UITEX_PUNCH].col.a = 1.0f;
		pUI[UITEX_LANCE].col.a = 0.5f;
		pUI[UITEX_SORD].col.a = 0.5f;
		pUI[UITEX_HAMMER].col.a = 0.5f;
		break;
	case PLAYERWEAPON_SORD:	// むらくも
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_SORD] == false)
		{// 武器の状態が違ったら
			PlaySound(SOUND_LABEL_SE_SPIN_KICK);
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_SORD] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
			for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirection) * 10),
					D3DXVECTOR3(sinf(nDirection), cosf(nDirectionY) * float((rand() % 40 / 10.0f) + 2) + float(rand() % 200 / 100.0f), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, float(rand() % 200 / 100.0f) + 0.5f), float(rand() % 100) + 30, 10, EFFECTTYPE_FIRE);
			}
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * 10, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirection) * 10),
			D3DXVECTOR3(sinf(nDirection), cosf(nDirectionY) * float((rand() % 40 / 10.0f) + 2) + float(rand() % 200 / 100.0f), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, float(rand() % 100 / 100.0f), 0.0f, float(rand() % 100 / 100.0f) + 0.5f), float(rand() % 50) + 5, 10, EFFECTTYPE_FIRE);
		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 0.3f;
		pUI[UITEX_SORD].col.a = 1.0f;
		pUI[UITEX_HAMMER].col.a = 0.3f;
		break;
	case PLAYERWEAPON_LANCE:// ポセイドン
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_LANCE] == false)
		{// 武器の状態が違ったら
			PlaySound(SOUND_LABEL_SE_SPIN_PUNCH);
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_LANCE] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 50), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + (rand() % 500) + 100, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 50)),
					D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), float(rand() % 30) + 1, 10, EFFECTTYPE_WATER_DOWN);
			}
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
			D3DXVECTOR3(sinf(nDirection) * float(rand() % 5), cosf(nDirectionY) * float(rand() % 5), cosf(nDirectionZ)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 20), 30, EFFECTTYPE_WATER);
		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 1.0f;
		pUI[UITEX_SORD].col.a = 0.3f;
		pUI[UITEX_HAMMER].col.a = 0.3f;
		break;
	case PLAYERWEAPON_HAMMER:// にょるにる
		if (g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAMMER] == false)
		{// 武器の状態が違ったら
			PlaySound(SOUND_LABEL_SE_JUMP_PUNCH);

			for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
			{
				nDirection = SetRandomEffect();
				nDirectionY = SetRandomEffect();
				nDirectionZ = SetRandomEffect();
				SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 50), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 +  (rand() %500)+100, g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 50)),
					D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 50) + 30, 10, EFFECTTYPE_SUNDER_DOWN);
			}
			g_aPlayer[nCntPlayer].bWeaponNow[PLAYERWEAPON_HAMMER] = true;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = true;
		}
		SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41 + sinf(nDirection)* (rand() % 10), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42 + cosf(nDirection) * (rand() % 10), g_aPlayer[nCntPlayer].aModel[6][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43 + cosf(nDirectionZ) * (rand() % 10)),
			D3DXVECTOR3(sinf(nDirection) * float((rand() % 11 / 10.0f)), cosf(nDirectionY) *  float((rand() % 11 / 10.0f)), cosf(nDirectionZ) *  float((rand() % 11 / 10.0f))), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), float(rand() % 15) + 1, 10, EFFECTTYPE_SUNDER1);

		pUI[UITEX_PUNCH].col.a = 0.3f;
		pUI[UITEX_LANCE].col.a = 0.3f;
		pUI[UITEX_SORD].col.a = 0.3f;
		pUI[UITEX_HAMMER].col.a = 1.0f;
		break;
	}
	if (g_aPlayer[nCntPlayer].pos.y <= -500)
	{// 下に落ちたら死ぬ
		g_aPlayer[nCntPlayer].nLife--;
		GetPlayerLife(g_aPlayer[nCntPlayer].nLife);
		if (g_aPlayer[nCntPlayer].nLife <= 0)
		{
			if (GetGameState() != GAMESTATE_END&&GetGameState() != GAMESTATE_END2)
			{
				SetGameState(GAMESTATE_END);
				SetResultType(RESULTTYPE_GAMEOVER);
				StopSound();				// サウンド終了
				PlaySound(SOUND_LABEL_VOICE_DEATH);
				PlaySound(SOUND_LABEL_DEATH);
			}
		}
	}
	if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
	{// 生きているなら
		if (pCamera->nState == CAMERASTATE_NONE)
		{

			if (GetMode() != MODE_RESULT)
			{
				PlayerController(nCntPlayer);
			}
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == false)
			{// 吹っ飛びしていなければ
				if (GetKeyboardTrigger(DIK_K) == true || GetGamePadTrigger(BUTTON_B) == true)
				{// 攻撃
					if (g_aPlayer[nCntPlayer].bAttention == true)
					{// 注目していたら敵の方向へ攻撃
						g_aPlayer[nCntPlayer].Destrot.y = pCamera->rot.y - D3DX_PI;
						g_aPlayer[nCntPlayer].rot.y = pCamera->rot.y - D3DX_PI;
					}
					if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == true)
					{// 攻撃2
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] = true;
					}
					else if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == false&&g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == true)
					{// 攻撃1
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] = true;
					}
					else if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == false)
					{// 攻撃0
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] = true;
					}
				}
				if (GetKeyboardTrigger(DIK_I) == true || GetGamePadTrigger(BUTTON_Y) == true)
				{// 強攻撃
					if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == false)
					{// 強攻撃
						g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] = true;
					}
					if (g_aPlayer[nCntPlayer].bAttention == true)
					{// 注目していたら敵の方向へ攻撃
						g_aPlayer[nCntPlayer].Destrot.y = pCamera->rot.y - D3DX_PI;
						g_aPlayer[nCntPlayer].rot.y = pCamera->rot.y - D3DX_PI;
					}
				}
				if (GetKeyboardTrigger(DIK_J) == true || GetGamePadTrigger(BUTTON_R1) == true)
				{// ステップ
					if (g_aPlayer[nCntPlayer].nAirCounter < AIR_COUNT)
					{
						g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] = true;
					}
					PlaySound(SOUND_LABEL_SE_STEP);
				}
			}
			if (g_aPlayer[nCntPlayer].bJump == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false && g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == false)
			{// ジャンプ&吹っ飛びしていなければ
				if (GetKeyboardTrigger(DIK_SPACE) == true || GetGamePadTrigger(BUTTON_A) == true)
				{// ジャンプ
					PlaySound(SOUND_LABEL_SE_JUMP);

					g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].JumpSpeed;	// 移動量加算
					g_aPlayer[nCntPlayer].bJump = true;
					g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = true;
				}
			}
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{// 吹っ飛び力がまだあったら
				if (g_aPlayer[nCntPlayer].fFlyPower.x >= 0.1f ||
					g_aPlayer[nCntPlayer].fFlyPower.x <= -0.1f)
				{// 水平方向の吹っ飛び力があるなら
					g_aPlayer[nCntPlayer].move.x += g_aPlayer[nCntPlayer].fFlyPower.x;	// 吹っ飛び力を加算
				}
				if (g_aPlayer[nCntPlayer].fFlyPower.z >= 0.1f ||
					g_aPlayer[nCntPlayer].fFlyPower.z <= -0.1f)
				{// 水平方向の吹っ飛び力があるなら
					g_aPlayer[nCntPlayer].move.z += g_aPlayer[nCntPlayer].fFlyPower.z;	// 吹っ飛び力を加算
				}

				if (g_aPlayer[nCntPlayer].nFlyCounter > 0)
				{// 吹っ飛ぶカウンターが0より大きかったら
					g_aPlayer[nCntPlayer].fFlyPower.y -= 0.5f;
					if (g_aPlayer[nCntPlayer].fFlyPower.y > 0.0f)
					{// ジャンプ力がまだあったら
						g_aPlayer[nCntPlayer].move.y += g_aPlayer[nCntPlayer].fFlyPower.y;			// 上昇していく
					}
				}
			}
			// 向きの回転速度の減速
			if (g_aPlayer[nCntPlayer].nMotionType != PLAYER_MOTION_TYPE_GETUP &&
				g_aPlayer[nCntPlayer].nMotionType != PLAYER_MOTION_TYPE_STEP)
			{// 起き上がり時じゃなければ
				g_aPlayer[nCntPlayer].fRotSpeed = PLAYER_ROT_SPEED_NORMAL;			// 通常の回転速度を入れる
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bMotionState[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].fRotSpeed = PLAYER_ROT_SPEED_ATTACK;	// 向きの回転速度を遅くする
					}
				}
			}
		}
	}
	// 重力加算
	g_aPlayer[nCntPlayer].move.y += -GRAVITY_SPEED;
	if ( g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_STEP)
	{
		g_aPlayer[nCntPlayer].move.y = 0;
	}
	// 移動量加算
	g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;
	// 減速更新
	if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == false)
	{
		g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x)*DOWN_SPEED;
		g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z)*DOWN_SPEED;
	}
	//--------------------------/ モデルの動きをなめらかにする /---------------------------------------------------------------------------------------//
	if (g_aPlayer[nCntPlayer].Destrot.y > D3DX_PI)
	{// 差分が3.14を超えるなら
		g_aPlayer[nCntPlayer].Destrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].Destrot.y < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		g_aPlayer[nCntPlayer].Destrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nCntPlayer].Diffrot.y = g_aPlayer[nCntPlayer].Destrot.y - g_aPlayer[nCntPlayer].rot.y;// 現在の角度と目的の角度の差分
	if (g_aPlayer[nCntPlayer].Diffrot.y > D3DX_PI)
	{// 差分が3.14を超えるなら
		g_aPlayer[nCntPlayer].Diffrot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].Diffrot.y < -D3DX_PI)
	{// 差分が-3.14を超えるなら
		g_aPlayer[nCntPlayer].Diffrot.y += D3DX_PI * 2.0f;
	}
	g_aPlayer[nCntPlayer].rot.y += g_aPlayer[nCntPlayer].Diffrot.y * g_aPlayer[nCntPlayer].fRotSpeed;	// 角度の加算
	if (g_aPlayer[nCntPlayer].rot.y > D3DX_PI)
	{// 現在の角度が3.14を超えるなら
		g_aPlayer[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_aPlayer[nCntPlayer].rot.y < -D3DX_PI)
	{// 現在の角度が-3.14を超えるなら
		g_aPlayer[nCntPlayer].rot.y += D3DX_PI * 2.0f;
	}
	//----------------------------------/ 当たり判定 /---------------------------------------------------------------------------------------//
	// 衝突判定処理
	if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
	{
		CollisionEnemy(&g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].fRadiusWidth);			// キャラクターが重ならないための当たり判定
	}
	// メッシュフィールド
	if (CollisionMeshField(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move,
		g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, &g_aPlayer[nCntPlayer].pMeshFieldLand) == true)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].pos.y = g_aPlayer[nCntPlayer].pos.y + 0.1f;
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nAirCounter = 0;// 空中攻撃制御カウンター

		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = true;
				for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
				{// 着地エフェクト
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
				}
			}
		}
	
	}
	else
	{
		StopSound(SOUND_LABEL_SE_GRAUND);
		g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = false;
	}
	if (g_aPlayer[nCntPlayer].pMeshFieldLand != NULL)
	{// 床に乗っている
		if ((g_aPlayer[nCntPlayer].move.x > 1.0f || g_aPlayer[nCntPlayer].move.z > 1.0f ||
			g_aPlayer[nCntPlayer].move.x < -1.0f || g_aPlayer[nCntPlayer].move.z < -1.0f))
		{// 歩いている
			if (g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] == false)
			{
				PlaySound(SOUND_LABEL_SE_GRAUND);
				g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = true;
			}
			for (int nCntEffect = 0; nCntEffect < 1; nCntEffect++)
			{// 地面にあるく煙
				float nDirection = SetRandomEffect();
				float nDirectio2 = SetRandomEffect();
				SetEffect(g_aPlayer[nCntPlayer].pos,
					D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 / 10.0f), cosf(nDirectio2) * float(rand() % 5 / 10.0f), cosf(nDirection)*float(rand() % 10 / 10.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.5f, 0.4f, 0.4f, 0.8f), float(rand() % 10 + 20), 30, EFFECTTYPE_PLAYER);
			}
		}
		else
		{
			StopSound(SOUND_LABEL_SE_GRAUND);
			g_aPlayer[nCntPlayer].bSound[SOUND_LABEL_SE_GRAUND] = false;
		}
	}
	// アイテム獲得
	CollisionItem(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), g_aPlayer[nCntPlayer].fRadiusWidth);
	// モデル
	if (CollisionModelMinMax(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight, &g_aPlayer[nCntPlayer].pModel) == 2)
	{
		Shadow *pShadow = GetShadow();
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].pos.y = g_aPlayer[nCntPlayer].pos.y + 0.1f;
		pShadow[g_aPlayer[nCntPlayer].nIdyShadow].col.a = 1.0f;
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] = false;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].nAirCounter = 0;// 空中攻撃制御カウンター

		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
			{
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] = false;
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = true;
				for (int nCntEffect = 0; nCntEffect < 2; nCntEffect++)
				{// 着地エフェクト
					float nDirection = SetRandomEffect();
					float nDirection2 = SetRandomEffect();
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
					SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
				}
			}
		}

	}
	// メッシュウォール
	if (CollisionMeshWall(&g_aPlayer[nCntPlayer].pos, &g_aPlayer[nCntPlayer].posOld, &g_aPlayer[nCntPlayer].move, g_aPlayer[nCntPlayer].fRadiusWidth, g_aPlayer[nCntPlayer].fRadiusHeight) == true)
	{

	}
	// 攻撃判定処理
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{
		if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].bUse == true)
		{
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nMotionType == g_aPlayer[nCntPlayer].nMotionType)
			{// 決めたステートと今のステートが一緒だったら判定開始
				if (g_nFrameAllCounter >= g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nCollisionStart &&
					g_nFrameAllCounter <= g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].nCollisionEnd)
				{
					// エフェクト
					PlayerMotionEffect(nCntPlayer, nCollision,g_aPlayer[nCntPlayer].nWeaponState);
					CollisionEnemyBody(g_aPlayer[nCntPlayer].pos,
						D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
						g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nAttackPower,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].fDamageHorizontal,
						g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].fDamageVertical);					// 敵の体と攻撃の当たり判定
					if (CollisionModelMinMax(&D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
						&D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorldOld._43), &g_aPlayer[nCntPlayer].move,
						g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].fRadius, &g_aPlayer[nCntPlayer].pModel) != 0)
					{
						PlaySound(SOUND_LABEL_SE_ATTACKWOOD);
						for (int nCntEffect = 0; nCntEffect < 21; nCntEffect++)
						{// 火花
							float nDirection = SetRandomEffect();
							float nDirectio2 = SetRandomEffect();
							SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
								D3DXVECTOR3(sinf(nDirection) * 5, cosf(nDirection) * 5, cosf(nDirectio2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(1.0f, 0.3f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
							SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43),
								D3DXVECTOR3(sinf(nDirectio2) * 5, cosf(nDirection), cosf(nDirectio2) * 5), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXCOLOR(0.8f, 0.8f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
						}
					}
				}
			}
		}
	}
	if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] != g_aPlayer[nCntPlayer].bMotionStateOld[PLAYER_BOOL_JUMP])
	{// 前回と今回のジャンプモーションが違ったら着地モーション
		g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = true;

		for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
		{// 着地エフェクト
			float nDirection = SetRandomEffect();
			float nDirection2 = SetRandomEffect();
			SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection) * float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
			SetEffect(g_aPlayer[nCntPlayer].pos, D3DXVECTOR3(sinf(nDirection2) *  float(rand() % 10 + 3), cosf(rand() % 314 / 100.0f) * 0, cosf(nDirection2) *  float(rand() % 10 + 3)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, float(rand() % 100 / 100.0f)), float(rand() % 50) + 10, 30, EFFECTTYPE_WIND);
		}
	}
	//------------------------------------/ 基本モーション /-----------------------------------------------------------------/

	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == false
		&&g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == false&& g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == false)
	{// 攻撃状態じゃなければ
		if (GetPovTrigger(POV_DOWN) == true || GetKeyboardTrigger(DIK_1) == true)
		{// 素手
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAND;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// 武器の状態
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_UP) == true || GetKeyboardTrigger(DIK_2) == true)
		{// 剣
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_SORD;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// 武器の状態
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_RIGHT) == true || GetKeyboardTrigger(DIK_3) == true)
		{// 槍
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_LANCE;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// 武器の状態
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (GetPovTrigger(POV_LEFT) == true || GetKeyboardTrigger(DIK_4) == true)
		{// ハンマー
			g_aPlayer[nCntPlayer].nWeaponState = PLAYERWEAPON_HAMMER;
			for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
			{// 武器の状態
				g_aPlayer[nCntPlayer].bWeaponNow[nCntWeapon] = false;
			}
		}
		if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] == true)
		{
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_CHANGE;

		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] == true)
		{// ステップ状態なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_STEP;

			if (g_nFrameAllCounter >= 1 &&
				g_nFrameAllCounter <= 5)
			{// モーションの間の時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 10);	// プレイヤーの向いている方向に急加速
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 10);	// プレイヤーの向いている方向に急加速
			}
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] == true)
		{// 倒れてる状態なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_GETUP;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_FLY] == true)
		{// ダメージ(吹っ飛び)中なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_FLY;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] == true)
		{// ダメージ(仰け反り)中なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_DAMAGE;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_JUMP] == true)
		{// プレイヤーがジャンプ中なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_JUMP;
		}
		else if (g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] == true)
		{// 着地中なら
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_LANDING;
		}
		else if (g_aPlayer[nCntPlayer].move.x > PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.x < -PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.z >  PLAYER_MOTION_MOVE_ZONE ||
			g_aPlayer[nCntPlayer].move.z < -PLAYER_MOTION_MOVE_ZONE)
		{// プレイヤーが動いているとき
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_MOVE;
		}
		else
		{// プレイヤーが動いてないとき
			g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_NEUTRAL;
		}
	}
	//--------------------------- 攻撃モーション -----------------------------------------------------//
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_0] == true)
	{// 攻撃０モーション設定
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_0;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_1] == true)
	{// 攻撃１モーション設定
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_1;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_2] == true)
	{// 攻撃２モーション設定
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_2;
	}
	if (g_aPlayer[nCntPlayer].bAttackType[PLAYER_ATTACK_TYPE_ATTACK_BIG] == true)
	{// 強攻撃モーション設定
		g_aPlayer[nCntPlayer].nMotionType = PLAYER_MOTION_TYPE_ATTACK_BIG;
	}
	// モーション
	MotionInfoPlayer(nCntPlayer);
	switch (g_aPlayer[nCntPlayer].nMotionType)
	{// プレイヤーのモーションが
	case PLAYER_MOTION_TYPE_JUMP:						// ジャンプモーションだったら
		break;
	case PLAYER_MOTION_TYPE_LANDING:					// 着地モーションだったら
		if (g_nFrameAllCounter == 1)
		{
			PlaySound(SOUND_LABEL_SE_RANDING);
		}
		break;
	case PLAYER_MOTION_TYPE_DAMAGE:						// ダメージ(仰け反り)モーションだったら
		break;
	case PLAYER_MOTION_TYPE_FLY:						// ダメージ(吹っ飛び)モーションだったら
		break;
	case PLAYER_MOTION_TYPE_GETUP:						// 起き上がりモーションだったら
		break;
	case PLAYER_MOTION_TYPE_STEP:						// ステップモーションだったら
		break;
	case PLAYER_MOTION_TYPE_ATTACK_0:						// 攻撃モーションだったら
		if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
		{// モーションの最初のフレームの時
			g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
			g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
		}
		if (g_nFrameAllCounter == 18)
		{
			PlaySound(SOUND_LABEL_SE_PUNCH);
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_1:				// 攻撃１
		if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
		{// モーションの最初のフレームの時
			g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
			g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 2);
		}
		if (g_nFrameAllCounter == 13)
		{
			PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH);
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_2:			// 攻撃２
		if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAND)
		{
			if (g_nFrameAllCounter >= 25 && g_nFrameAllCounter <= 33)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 22)
			{//フレームが0の時
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_SORD)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 10)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//フレームが0の時
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_LANCE)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 15)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//フレームが0の時
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAMMER)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 5)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 5);
			}
			if (g_nFrameAllCounter == 1)
			{//フレームが0の時
				PlaySound(SOUND_LABEL_SE_PUNCH_PUNCH_PUNCH);
				PlaySound(SOUND_LABEL_VOICE_PUNCH_PUNCH_PUNCH);
			}
		}
		break;
	case PLAYER_MOTION_TYPE_ATTACK_BIG:			// 強攻撃
		if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAND)
		{
			if (g_nFrameAllCounter == 15)
			{
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI / 4 * 3) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI / 4 * 3) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
				SetEffect3D(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI - D3DX_PI / 4) * 150, -200.0f, cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI - D3DX_PI / 4) * 150), D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT3D_SORD);
			}
			if (g_nFrameAllCounter >= 15 && g_nFrameAllCounter <= 18)
			{
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 3);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * g_aPlayer[nCntPlayer].MoveSpeed * 3);
			}
			if (g_nFrameAllCounter == 20)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.y = 15;
			}
			g_aPlayer[nCntPlayer].bJump = true;
			if (g_nFrameAllCounter == 22)
			{
				PlaySound(SOUND_LABEL_SE_PUNCH_SPIN_JUMP);
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_SORD)
		{
			if (g_nFrameAllCounter >= 0 && g_nFrameAllCounter <= 10)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.x += (sinf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 6);
				g_aPlayer[nCntPlayer].move.z += (cosf(g_aPlayer[nCntPlayer].rot.y - D3DX_PI) * 6);
			}
			if (g_nFrameAllCounter == 1)
			{//フレームが0の時
				PlaySound(SOUND_LABEL_SE_PUNCH_SPIN_JUMP);
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_LANCE)
		{
			if (g_nFrameAllCounter >= 10 && g_nFrameAllCounter <= 40)
			{// モーションの最初のフレームの時
				g_aPlayer[nCntPlayer].move.y = 10;
			}
			if (g_nFrameAllCounter >= 35 && g_nFrameAllCounter <= 50)
			{
				g_aPlayer[nCntPlayer].move.y -= 5;
			}
			if (g_nFrameAllCounter == 35)
			{
				PlaySound(SOUND_LABEL_VOICE_JUMP_PUNCH);
				PlaySound(SOUND_LABEL_SE_JUMP_PUNCH);
			}
			if (g_nFrameAllCounter >= 44 )
			{
				SetEffectUP(g_aPlayer[nCntPlayer].pos + D3DXVECTOR3(0.0f, 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f), EFFECTUPTYPE_MAHOUJIN, 500);
			}
		}
		else if (g_aPlayer[nCntPlayer].nWeaponState == PLAYERWEAPON_HAMMER)
		{
			if (g_nFrameAllCounter == 20)
			{
				SetEffect3D(D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[17][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, g_aPlayer[nCntPlayer].rot.y, -D3DX_PI / 2), EFFECT3D_ARM);
			}
		}
		break;
	}
	//-------------------------------------/ 影 線 軌跡　コンボ　を移動する /---------------------------------------------------------------------------------------//
	SetPositionShadow(g_aPlayer[nCntPlayer].nIdyShadow, D3DXVECTOR3(g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].aModel[0][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld._43), g_aPlayer[nCntPlayer].Initpos);
	SetPositionLine(g_aPlayer[nCntPlayer].nIdyLine, g_aPlayer[nCntPlayer].pos, g_aPlayer[nCntPlayer].rot);
	SetPosOrbit(g_aPlayer[nCntPlayer].nMotionType,g_aPlayer[nCntPlayer].nWeaponState);
	PlayerSpirit(nCntPlayer);
}



//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawPlayer(int nCntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;				// 親のマトリックス
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
	D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
	D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);
	for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
	{// 判定のマトリックス
		for (int nCntWeapon = 0; nCntWeapon < PLAYERWEAPON_MAX; nCntWeapon++)
		{// 武器の状態
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].bUse == true)
			{// 攻撃判定
				// 前回の位置を繁栄
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorldOld = g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld;
				// 位置を反映
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.x, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.y, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, &g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nCntWeapon].nIdxParent][nCntWeapon].mtxWorld);
			}
		}
		if (g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse == true)
		{// ダメージ判定
			// 前回の位置を繁栄
			g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorldOld = g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld;
			// 位置を反映
			D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.x, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.y, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
		}
	}

		for (int nCntPlayerModel = 0; nCntPlayerModel < g_aPlayer[nCntPlayer].nNumParts; nCntPlayerModel++)
		{
			if (g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nIdxModelPareant == -1)
			{// プレイヤー
				mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
			}
			else
			{// 各親
				mtxParent = g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nIdxModelPareant][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld;
			}
			// ------------------------/ 親モデルの描画 /-------------------------------------------//
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxParent);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pBuffMat->GetBufferPointer();

			for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nNumMat; nCntPlayer2++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pTextuer[nCntPlayer2]);

				// モデル(パーツ)の描画
				g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pMesh->DrawSubset(nCntPlayer2);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			// ------------------------/ 子モデルの描画 /-------------------------------------------//
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.x, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.y, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxTrans);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld, &mtxParent);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pBuffMat->GetBufferPointer();
			for (int nCntPlayer2 = 0; nCntPlayer2 < (int)g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].nNumMat; nCntPlayer2++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntPlayer2].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pTextuer[nCntPlayer2]);
				// モデル(パーツ)の描画
				g_aPlayer[nCntPlayer].aModel[nCntPlayerModel][g_aPlayer[nCntPlayer].nWeaponState].pMesh->DrawSubset(nCntPlayer2);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
}
//==============================================================================================================================
// モデルの取得
//==============================================================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
//==============================================================================================================================
// モデルのモーション
//==============================================================================================================================
void MotionInfoPlayer(int nCntPlayer)
{
	if (g_aPlayer[nCntPlayer].nMotionType != g_aPlayer[nCntPlayer].nMotionTypeOld)
	{// 今のタイプと前のタイプが違ったら
		g_nKeyCounter = 0;						// キーをリセット
		g_nFrameCounter = 0;					// フレームをリセット
		g_aPlayer[nCntPlayer].bBlend = true;	// ブレンドにする
		g_nFrame = BLEND_FRAME;					// ブレンドのフレーム
		g_nFrameAllCounter = 0;					// 全てのフレーム
	}
	else if (g_aPlayer[nCntPlayer].bBlend == false )
	{// 通常時のフレーム
		g_nFrame = g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].nFrame;
	}
	if (g_nFrameCounter == 0)
	{// 次のキーフレームのモーションを読み込む
		for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
		{// Key - 現在 間の差分計算
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].posAddMotion =
				(g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].aPartsKey[nCntParts].pos
					- g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].pos + g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].Initpos) / float(g_nFrame);

			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion = (g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].aPartsKey[nCntParts].rot - g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot);

			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.x += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.y += D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z > D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z < -D3DX_PI)
			{
				g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion.z += D3DX_PI * 2.0f;
			}
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion = g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].DiffrotMotion / float(g_nFrame);
		}
	}
	for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumParts; nCntParts++)
	{// モーション差分加算	

		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z > D3DX_PI)
		{// 差分が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z < -D3DX_PI)
		{// 差分が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion.z += D3DX_PI * 2.0f;
		}
		g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].pos += g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].posAddMotion;
		g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot += g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rotAddMotion;

		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.x += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.y += D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z > D3DX_PI)
		{// 現在の角度が3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z < -D3DX_PI)
		{// 現在の角度が-3.14を超えるなら
			g_aPlayer[nCntPlayer].aModel[nCntParts][g_aPlayer[nCntPlayer].nWeaponState].rot.z += D3DX_PI * 2.0f;
		}
	}
	g_nFrameCounter++;
	g_nFrameAllCounter++;
	if (g_aPlayer[nCntPlayer].bBlend == true && g_nFrameCounter == BLEND_FRAME)
	{// ブレンド　ブレンドのフレーム数に達する
		g_aPlayer[nCntPlayer].bBlend = false;
		g_nFrameCounter = 0;
		g_nKeyCounter = 0;// ブレンドで次のモーションの0キー目は行ってる
		if (g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL)
		{
			g_nKeyCounter = 1;
		}
	}
	else if (g_aPlayer[nCntPlayer].bBlend == false&&g_nFrameCounter == g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].aKey[g_nKeyCounter].nFrame)
	{// フレームカウントが上回る
		g_nFrameCounter = 0;
		if (g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].bLoop == false &&
			g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nNumKey - 1 == g_nKeyCounter)
		{// ループしないかつキー数が超える	
		//	g_nFrameAllCounter = 0;
			g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = false;	// 着地キャンセル
			
			switch (g_aPlayer[nCntPlayer].nMotionType)
			{// プレイヤーのモーションが
	
			case PLAYER_MOTION_TYPE_JUMP:																// ジャンプモーションだったら
				break;
			case PLAYER_MOTION_TYPE_LANDING:															// 着地モーションだったら
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_LANDING] = false;										// 着地状態じゃない状態にする
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_DAMAGE:																// ダメージ(仰け反り)モーションだったら
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_DAMAGE] = false;										// 着地状態じゃない状態にする
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_FLY:																// ダメージ(吹っ飛び)モーションだったら
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_GETUP:																// 起き上がりモーションだったら
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_GETUP] = false;						// 起き上がりの状態を解除
				break;
			case PLAYER_MOTION_TYPE_STEP:																// ステップモーションだったら
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_STEP] = false;						// ステップの状態を解除
				g_aPlayer[nCntPlayer].nAirCounter++;// 空中攻撃制御カウンター
				break;
			case PLAYER_MOTION_TYPE_CHANGE:																// ステップモーションだったら
				g_aPlayer[nCntPlayer].bMotionState[PLAYER_BOOL_CHANGE] = false;						// ステップの状態を解除
				break;

			case PLAYER_MOTION_TYPE_ATTACK_0:																// 攻撃モーションだったら
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_1:																// 攻撃モーションだったら
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_2:																// 攻撃モーションだったら
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			case PLAYER_MOTION_TYPE_ATTACK_BIG:																// 攻撃モーションだったら
				for (int nCntAttack = 0; nCntAttack < PLAYER_ATTACK_TYPE_MAX; nCntAttack++)
				{// 攻撃の種類分ループ
					if (g_aPlayer[nCntPlayer].bAttackType[nCntAttack] == true)
					{// 攻撃中だったら
						g_aPlayer[nCntPlayer].bAttackType[nCntAttack] = false;								// 攻撃状態を解除
					}
				}
				break;
			}
		}
		else
		{// 次のキー数を獲得
			g_nKeyCounter = (g_nKeyCounter + 1) % g_aPlayer[nCntPlayer].aMotion[g_aPlayer[nCntPlayer].nMotionType][g_aPlayer[nCntPlayer].nWeaponState].nNumKey;
		}
	}

}
//==============================================================================================================================
// プレイヤーの体の当たり判定
//==============================================================================================================================
void CollisionPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 mtxWorld, float fRadius, int nPower, float fDamageSide, float fDamageVertical)
{
	if (g_aPlayer[0].state != PLAYERSTATE_DEATH)
	{
		if (g_aPlayer[0].bMotionState[PLAYER_BOOL_DAMAGE] == false &&
			g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] == false &&
			g_aPlayer[0].bMotionState[PLAYER_BOOL_GETUP] == false)
		{// ダメージ中じゃなければ
			for (int nCntCollision = 0; nCntCollision < MAX_COLLISION; nCntCollision++)
			{// 体の当たり判定分ループ
			 // 二つの当たり判定の座標の距離を計算
				float PosX = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._41 - mtxWorld.x);		// (プレイヤーの体の当たり判定の座標.x - 敵の攻撃の当たり判定の座標.x)
				float PosY = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._42 - mtxWorld.y);		// (プレイヤーの体の当たり判定の座標.y - 敵の攻撃の当たり判定の座標.y)
				float PosZ = (g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._43 - mtxWorld.z);		// (プレイヤーの体の当たり判定の座標.z - 敵の攻撃の当たり判定の座標.z)

				float fPosLenght = (PosX * PosX) + (PosY * PosY) + (PosZ * PosZ);				// それぞれを2乗してすべて足して距離を出す

																								// 二つの当たり判定の半径の距離を計算
				float fCollisionRadius = (g_aPlayer[0].CollisionBody[nCntCollision].fRadius + fRadius);		// プレイヤーの体の当たり判定の半径+敵の攻撃の当たり判定の半径を足す

				float fRadiusLenght = fCollisionRadius * fCollisionRadius;								// 二つの半径を足した値を2乗して、半径の距離を計算を計算

				if (fRadiusLenght > fPosLenght &&
					-fRadiusLenght < fPosLenght)
				{// 座標の距離が半径の距離よりも小さかったら
					float fAttackAngle = atan2f(pos.x - g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._41,		// 当たった角度を計算
						pos.z - g_aPlayer[0].CollisionBody[nCntCollision].mtxWorld._43);
					if (GetMode() == MODE_GAME)
					{
						g_aPlayer[0].nLife -= nPower;															// ライフを減らす
					}
					GetPlayerLife(g_aPlayer[0].nLife);														// ライフを減らす
					PlaySound(SOUND_LABEL_VOICE_DAMAGE);

					if (g_aPlayer[0].nLife <= 0)
					{// ライフが0以下になったら
						StopSound();				// サウンド終了
						PlaySound(SOUND_LABEL_VOICE_DEATH);
						PlaySound(SOUND_LABEL_DEATH);

						g_aPlayer[0].state = PLAYERSTATE_DEATH;
						g_aPlayer[0].nCntState = 100;

						// 水平方向の吹っ飛び力を代入
						g_aPlayer[0].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
						g_aPlayer[0].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

						// 垂直方向の吹っ飛び力を代入
						g_aPlayer[0].fFlyPower.y = fDamageVertical;
						g_aPlayer[0].nFlyCounter = 10;
						g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// 吹っ飛び中に設定
						g_aPlayer[0].Destrot.y = fAttackAngle - D3DX_PI;							// キャラクターの向きを変更
						g_aPlayer[0].rot.y = fAttackAngle - D3DX_PI;							// キャラクターの向きを変更

					}
					else
					{
						if (fDamageSide > 0.0f || fDamageVertical > 0.0f)
						{// 吹っ飛び力があるとき

						 // 水平方向の吹っ飛び力を代入
							g_aPlayer[0].fFlyPower.x = sinf(fAttackAngle - D3DX_PI) * fDamageSide;
							g_aPlayer[0].fFlyPower.z = cosf(fAttackAngle - D3DX_PI) * fDamageSide;

							// 垂直方向の吹っ飛び力を代入
							g_aPlayer[0].fFlyPower.y = fDamageVertical;
							g_aPlayer[0].nFlyCounter = 10;
							g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// 吹っ飛び中に設定
							g_aPlayer[0].Destrot.y = fAttackAngle - D3DX_PI;							// キャラクターの向きを変更
							g_aPlayer[0].rot.y = fAttackAngle - D3DX_PI;							// キャラクターの向きを変更
						}
						else
						{// 吹っ飛び力がないとき			
							g_aPlayer[0].bMotionState[PLAYER_BOOL_DAMAGE] = true;												// ダメージ中に設定
							// 効果音再生
							PlaySound(SOUND_LABEL_SE_HIT_1);		// ダメージ音を流す
						}

					}
					for (int nCntEffect = 0; nCntEffect < 30; nCntEffect++)
					{// 火花
						float nDirection = SetRandomEffect();
						float nDirectio2 = SetRandomEffect();
						SetEffect(D3DXVECTOR3(g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._41, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._42, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._43),
							D3DXVECTOR3(sinf(nDirection) * 5, cosf(nDirection) * 5, cosf(nDirectio2)), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(1.0f, 0.3f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
						SetEffect(D3DXVECTOR3(g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._41, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._42, g_aPlayer[0].aModel[9][g_aPlayer[0].nWeaponState].mtxWorld._43),
							D3DXVECTOR3(sinf(nDirectio2) * 5, cosf(nDirection), cosf(nDirectio2) * 5), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.8f, 0.8f, 0.0f, 0.8f), 20, 30, EFFECTTYPE_EXPLOSION);
					}
					break;
				}

			}
		}
	}
}
//==============================================================================================================================
// 判定設定(攻撃判定）
//==============================================================================================================================
void SetAttackCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius, int nCollisionStart, int nCollisionEnd, PlayerMotionType nMotionType,PLAYERWEAPON nWeaponState)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{
			if (g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].bUse == false)
			{
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nIdxParent = nIdxParent;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset = offset;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].fRadius = fRadius;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nCollisionStart = nCollisionStart;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nCollisionEnd = nCollisionEnd;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nMotionType = nMotionType;
				g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].bUse = true;
				// 位置を反映
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.x, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.y, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
				break;
			}
		}
	}
}
//==============================================================================================================================
// 判定設定（ダメージ判定）
//==============================================================================================================================
void SetBodyCollisionPlayer(int nIdxParent, D3DXVECTOR3 offset, float fRadius)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for (int nCollision = 0; nCollision < MAX_COLLISION; nCollision++)
		{
			if (g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse == false)
			{
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent = nIdxParent;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset = offset;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].fRadius = fRadius;
				g_aPlayer[nCntPlayer].CollisionBody[nCollision].bUse = true;
				// 位置を反映
				D3DXMatrixTranslation(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.x, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.y, g_aPlayer[nCntPlayer].CollisionBody[nCollision].ofset.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].CollisionBody[nCollision].mtxWorld, &g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].CollisionBody[nCollision].nIdxParent][g_aPlayer[nCntPlayer].nWeaponState].mtxWorld);
				break;
			}
		}
	}
}
//==============================================================================================================================
// プレイヤーの操作
//==============================================================================================================================
void PlayerController(int nCntPlayer)
{
	DIJOYSTATE2 *pGamePad = GetgamePadStick();
	Camera *pCamera = GetCamera();
	g_aPlayer[nCntPlayer].CtrlMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pGamePad->lX <= -1 || pGamePad->lX >= 1 || pGamePad->lY <= -1 || pGamePad->lY >= 1)
	{// ゲームパッドの処理
		if (pGamePad->lX >= -WARKZONE && pGamePad->lX <= WARKZONE && pGamePad->lY >= -WARKZONE && pGamePad->lY <= WARKZONE)
		{
			float fGamePagStickAngle = atan2f(float(-pGamePad->lX), float(pGamePad->lY));
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed / 2.0f;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed / 2.0f;
			g_aPlayer[nCntPlayer].Destrot.y = fGamePagStickAngle + pCamera->rot.y;	// 目的の角度
		}
	}
	if (pGamePad->lX < -WARKZONE || pGamePad->lX > WARKZONE || pGamePad->lY < -WARKZONE || pGamePad->lY> WARKZONE)
	{
		float fGamePagStickAngle = atan2f(float(-pGamePad->lX), float(pGamePad->lY));

		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(fGamePagStickAngle + D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = fGamePagStickAngle + pCamera->rot.y;	// 目的の角度
	}
	if (GetKeyboardPress(DIK_W) == true)
	{// 前移動
		if (GetKeyboardPress(DIK_D) == true)
		{// 右斜め前移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 4 * 3 + pCamera->rot.y;	// 目的の角度
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{// 左斜め前移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 4 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 4 * 3 + pCamera->rot.y;	// 目的の角度
		}
		else
		{// 前移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(0.0f + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(0.0f + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI + pCamera->rot.y;		// 目的の角度
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{// 後ろ移動
		if (GetKeyboardPress(DIK_D) == true)
		{// 右斜め後ろ移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 4 + pCamera->rot.y;	// 目的の角度
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{// 左斜め後ろ移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 4 * 3 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 4 + pCamera->rot.y;	// 目的の角度
		}
		else
		{// 後ろ移動
			g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
			g_aPlayer[nCntPlayer].Destrot.y = 0.0f + pCamera->rot.y;		// 目的の角度
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{// 右移動
		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = -D3DX_PI / 2 + pCamera->rot.y;	// 目的の角度
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{// 左移動
		g_aPlayer[nCntPlayer].CtrlMove.x = sinf(-D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].CtrlMove.z = cosf(-D3DX_PI / 2 + pCamera->rot.y)*g_aPlayer[nCntPlayer].MoveSpeed;
		g_aPlayer[nCntPlayer].Destrot.y = D3DX_PI / 2 + pCamera->rot.y;	// 目的の角度
	}
	if (g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_NEUTRAL ||
		g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_MOVE ||
		g_aPlayer[nCntPlayer].nMotionType == PLAYER_MOTION_TYPE_JUMP )
	{// 特定のモーションだげ移動量加算
		g_aPlayer[nCntPlayer].move += g_aPlayer[nCntPlayer].CtrlMove;
	}

}
//==============================================================================================================================
// プレイヤーのスピリットの処理
//==============================================================================================================================
void PlayerSpirit(int nCntPlayer)
{
	if (g_aPlayer[nCntPlayer].nWeaponState != PLAYERWEAPON_HAND)
	{
		if (g_aPlayer[nCntPlayer].state != PLAYERSTATE_DEATH)
		{
			// スピリット回復カウンターをリセット
			g_nSpiritRecoveryCounter = 0;
			// スピリット減少カウンターを進める
			g_nSpiritCounter++;
			if (g_nSpiritCounter % PLAYER_SPIRIT_TIME == 0)
			{
				if (g_aPlayer[nCntPlayer].nSpirit > 0)
				{ // スピリットが残っていたらスピリットを減らす
					g_aPlayer[nCntPlayer].nSpirit--;
					GetPlayerSpirit(g_aPlayer[nCntPlayer].nSpirit);		// スピリットを反映
				}
				else
				{// 残っていなかったら
					g_aPlayer[0].nLife -= PLAYER_SPIRIT_DAMAGE;		// ライフを減らす
					GetPlayerLife(g_aPlayer[0].nLife);				// ライフを減らす
					PlaySound(SOUND_LABEL_VOICE_DAMAGE);
					if (g_aPlayer[0].nLife <= 0)
					{// ライフが0以下になったら
						StopSound();				// サウンド終了
						PlaySound(SOUND_LABEL_VOICE_DEATH);
						PlaySound(SOUND_LABEL_DEATH);

						g_aPlayer[0].state = PLAYERSTATE_DEATH;
						g_aPlayer[0].nCntState = 100;

						g_aPlayer[0].bMotionState[PLAYER_BOOL_FLY] = true;													// 吹っ飛び中に設定

					}
				}
			}
		}
	}
	else
	{
		// スピリット減少カウンターをリセット
		g_nSpiritCounter = 0;
		// スピリット回復カウンターを進める
		g_nSpiritRecoveryCounter++;
		if (g_nSpiritRecoveryCounter % PLAYER_SPIRIT_RECOVERY_TIME == 0)
		{
			if (g_aPlayer[nCntPlayer].nSpirit < g_aPlayer[nCntPlayer].nInitSpirit)
			{ // スピリットが残っていたらスピリットを減らす
				g_aPlayer[nCntPlayer].nSpirit++;
				GetPlayerSpirit(g_aPlayer[nCntPlayer].nSpirit);		// スピリットを反映
			}
		}
	}
}

//==============================================================================================================================
// プレイヤーのスピリット回復処理
//==============================================================================================================================
void PlayerSpiritAdd(int nSpirit)
{
	g_aPlayer[0].nSpirit += nSpirit;
	if (g_aPlayer[0].nSpirit > g_aPlayer[0].nInitSpirit)
	{ // 初期値を超えていた場合初期値に戻す
		g_aPlayer[0].nSpirit = g_aPlayer[0].nInitSpirit;
	}
	GetPlayerSpirit(g_aPlayer[0].nSpirit);		// スピリットを反映
}
//==============================================================================================================================
// プレイヤーのエフェクト
//==============================================================================================================================
void PlayerMotionEffect(int nCntPlayer, int nCollision,PLAYERWEAPON nWeaponState)
{
	
	//SetEffect(D3DXVECTOR3(g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._41, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._42, g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].mtxWorld._43), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aPlayer[nCntPlayer].CollisionAttack[nCollision][nWeaponState].fRadius, 10, EFFECTTYPE_EXPLOSION);
	
}
