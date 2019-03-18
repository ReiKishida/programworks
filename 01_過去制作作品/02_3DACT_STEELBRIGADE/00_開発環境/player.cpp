//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "model.h"
#include "game.h"
#include "fade.h"
#include "enemy.h"
#include "sound.h"
#include "scoreitem.h"
#include "score.h"
#include "boost.h"
#include "inputx.h"
#include "text.h"
#include "motion.h"
#include "smoke.h"
#include "modelwall.h"
#include "gate.h"
#include "pedestal.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define JUMP_POWER (10.00f)	//ジャンプ力
#define JUMP_POWER (10.00f)	//ジャンプ力

#define GRAVITY (0.5f)		//重力
#define GRAVITY_MAX (50.0f)	//重力の最大量

#define FALLEN (0.3f)		//重力
#define FALLEN_MAX (50.0f)	//重力の最大量

#define MOVE_SPEED (01.0f)
#define DASHSPEED  (50.0f)
#define ROT_SPEED (0.05f)
#define ROT_SPEED2 (0.2f)

#define STAGE_LIMIT_MAX_X (1450.0f)
#define STAGE_LIMIT_MIN_X (-40.0f)
#define STAGE_LIMIT_MAX_Z (30.0f)
#define STAGE_LIMIT_MIN_Z (-3840.0f)

#define EFFECT_MAX (50)

#define PLAYER_PARTS_0	"data/MODEL/gear/000_body.x"		//体
#define PLAYER_PARTS_1	"data/MODEL/gear/001_jiku.x"		//軸１
#define PLAYER_PARTS_2	"data/MODEL/gear/002_jiku2.x"		//軸２
#define PLAYER_PARTS_3	"data/MODEL/gear/003_thighR.x"		//右二の腕
#define PLAYER_PARTS_4	"data/MODEL/gear/004_legR.x"		//右二の腕
#define PLAYER_PARTS_5	"data/MODEL/gear/005_footR.x"		//右腕
#define PLAYER_PARTS_6	"data/MODEL/gear/006_thighL.x"		//右腕
#define PLAYER_PARTS_7	"data/MODEL/gear/007_legL.x"		//右腕
#define PLAYER_PARTS_8	"data/MODEL/gear/008_footL.x"		//右腕
#define PLAYER_PARTS_9	"data/MODEL/gear/009_weapon.x"		//右腕
#define PLAYER_PARTS_10 "data/MODEL/gear/010_lader.x"		//右腕

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

PLAYER g_Player;
PLAYERSTATUS g_Playerstatus = PLAYERSTATUS_NEUTRAL;
PLAYERSTATUS g_PlayerstatusNow = PLAYERSTATUS_NEUTRAL;

//STAGE g_aLimit[4];

bool bJump;

float fJump;	//ジャンプ力
float fGravity;	//重力

int g_nCntSound = 0;


bool bLand; //着地判定
JoyState g_JoyStatePlayer;
//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	//スモーク初期化
	InitSmoke();

	//テキスト情報取得
	Text *pText;
	pText = GetText();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int nCntIdx = 0; nCntIdx < pText->MAX_TEXTPARTS; nCntIdx++)
	{//各種値の代入
		//読み込むモデル名
		g_Player.aModel[nCntIdx].FileName[0] = pText->aModelTex[nCntIdx].FileName[0];

		//親子関係
		g_Player.aModel[nCntIdx].nIdxModelParent = pText->aModelTex[nCntIdx].Index;

		//位置
		g_Player.aModel[nCntIdx].pos = D3DXVECTOR3(pText->aModelTex[nCntIdx].ModelPosX, pText->aModelTex[nCntIdx].ModelPosY, pText->aModelTex[nCntIdx].ModelPosZ);

		//回転
		g_Player.aModel[nCntIdx].rot = D3DXVECTOR3(pText->aModelTex[nCntIdx].ModelRotX,
												   pText->aModelTex[nCntIdx].ModelRotY,
												   pText->aModelTex[nCntIdx].ModelRotZ);

		//モデルの設定
		D3DXLoadMeshFromX(&pText->aModelTex[nCntIdx].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			 NULL,
			 &g_Player.aModel[nCntIdx].pBuffMat,
			 NULL,
			 &g_Player.aModel[nCntIdx].nNumMat,
			 &g_Player.aModel[nCntIdx].pMesh);
	}


	//ゲームパッドの状態
	g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//ジャンプの状態
	g_Player.bUseJump = false;

	//プレイヤーの状態
	g_Player.state = PLAYERSTATE_OVERHEAT;
	g_Player.nCoolDown = 0;

	//着地しているか
	bLand = false;

	//プレイヤーが使用されているか
	g_Player.bUse = true;

	//プレイヤーのライフ
	g_Player.nLife = 120;

	fJump = JUMP_POWER;
	fGravity = GRAVITY;

	//Load_XFile();

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマット
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	//頂点数を取得
	nNumVtx = g_Player.aModel[0].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[0].pMesh->GetFVF());

	//頂点バッファをロック
	g_Player.aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標

		if (Vtx.x < g_Player.aModel[0].VtxMinModel.x)
		{
			g_Player.aModel[0].VtxMinModel.x = Vtx.x + -10.0f;
		}

		if (Vtx.y < g_Player.aModel[0].VtxMinModel.y)
		{
			g_Player.aModel[0].VtxMinModel.y = Vtx.y + -20.0f;
		}

		if (Vtx.z < g_Player.aModel[0].VtxMinModel.z)
		{
			g_Player.aModel[0].VtxMinModel.z = Vtx.x + -10.0f;
		}


		if (Vtx.x > g_Player.aModel[0].VtxMaxModel.x)
		{
			g_Player.aModel[0].VtxMaxModel.x = Vtx.x + 10.0f;
		}

		if (Vtx.y > g_Player.aModel[0].VtxMaxModel.y)
		{
			g_Player.aModel[0].VtxMaxModel.y = Vtx.y + 20.0f;
		}

		if (Vtx.z > g_Player.aModel[0].VtxMaxModel.z)
		{
			g_Player.aModel[0].VtxMaxModel.z = Vtx.z + 10.0f;
		}
		pVtxBuff += sizeFVF;
	}

	g_Player.VtxMinPlayer.x = g_Player.aModel[0].VtxMinModel.x;
	g_Player.VtxMaxPlayer.x = g_Player.aModel[0].VtxMaxModel.x;

	g_Player.VtxMinPlayer.y = g_Player.aModel[0].VtxMinModel.y;
	g_Player.VtxMaxPlayer.y = g_Player.aModel[0].VtxMaxModel.y;

	g_Player.VtxMinPlayer.z = g_Player.aModel[0].VtxMinModel.z;
	g_Player.VtxMaxPlayer.z = g_Player.aModel[0].VtxMaxModel.z;


	CAMERA *pCamera;
	pCamera = GetCamera();

	// 位置・向きの初期設定
	g_Player.pos = D3DXVECTOR3(180.0f, 0.0f, -3000.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//Offset_Model();


	//g_Player.aModel[2].fDestAngle = 0.0f;		//目的の角度
	g_Player.fAngle = 0.0f;			//差分


	//影の設定
	g_Player.nIndexShadow = SetShadow(g_Player.pos, g_Player.rot, 28.0f, 28.0f);




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//スモーク終了
	UninitSmoke();

	// メッシュの開放
	if (g_Player.aModel[0].pMesh != NULL)
	{
		g_Player.aModel[0].pMesh->Release();
		g_Player.aModel[0].pMesh = NULL;
	}

	// マテリアルの開放
	if (g_Player.aModel[0].pBuffMat != NULL)
	{
		g_Player.aModel[0].pBuffMat->Release();
		g_Player.aModel[0].pBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	Motion *pMotion;
	pMotion = GetMotion();

	Text *pText;
	pText = GetText();
	//スモーク
	UpdateSmoke();

	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_OVERHEAT:


		g_Player.nCoolDown--;
		if (g_Player.nCoolDown <= 0)
		{
			g_Player.nCoolDown = 0;
			g_Player.state = PLAYERSTATE_NORMAL;
		}

		break;
	}



	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//カメラ取得
	CAMERA *pCamera;
	pCamera = GetCamera();

	D3DXVECTOR3 EffectPos;

	//EffectPos = D3DXVECTOR3(g_Player.aModel[0].pos.x + pCamera->rot.y, g_Player.aModel[0].pos.y - 2.0f + pCamera->rot.y, g_Player.aModel[0].pos.z + 0.0f + pCamera->rot.y);



	g_Player.posold = g_Player.pos;



	MODE *pMode;
	pMode = GetMode();




	if (*pMode == MODE_GAME)
	{
		if (g_Player.bUse == true)
		{
			if (g_Player.nLife < 50)
			{
				SetSmoke(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 27.0f, g_Player.pos.z), 60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				if (rand() % 20 == 0)
				{
					SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 27.0f, g_Player.pos.z), 30, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
				}
			}

			if (g_PlayerstatusNow == PLAYERSTATUS_NEUTRAL)
			{
				if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true) ||
					GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true) ||
					GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)||
					GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
				{
					g_nCntSound++;
					if (g_nCntSound % 1 == 0)
					{
						if (g_Player.bUseJump == false)
						{

							PlaySound(SOUND_LABEL_SE_LAND);
						}
					}
					g_Playerstatus = PLAYERSTATUS_MOVE;
				}
			}


			if (GetKeyboardPress(DIK_Q) == true || (state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				g_Player.rot.y += -0.02f;

			}
			else if (GetKeyboardPress(DIK_E) == true || (state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				g_Player.rot.y += 0.02f;
			}

			if (GetKeyboardPress(DIK_T) == true)
			{
				g_Player.aModel[9].rot.x -= 0.02f;
			}
			else if (GetKeyboardPress(DIK_G) == true)
			{
				g_Player.aModel[9].rot.x += 0.02f;
			}

			if (GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
				{//右上

					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true) )
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{
							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);

								g_Player.move.x -= sinf((D3DX_PI *  0.25f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z -= cosf((D3DX_PI *  0.25f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}
					g_Player.move.x -= sinf(D3DX_PI *  0.25f + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z -= cosf(D3DX_PI *  0.25f + pCamera->rot.y) * MOVE_SPEED;

					g_Player.aModel[2].fDestAngle = (D3DX_PI * 0.25f);
				}
				else if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
				{
					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE|| (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{
							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);

								g_Player.move.x -= sinf((D3DX_PI *  0.75f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z -= cosf((D3DX_PI *  0.75f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}

					g_Player.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * MOVE_SPEED;

					g_Player.aModel[2].fDestAngle = (D3DX_PI * -0.25f);
				}

				else
				{
					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{
							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);

								g_Player.move.x -= sinf((D3DX_PI *  0.5f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z -= cosf((D3DX_PI *  0.5f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}
					g_Player.move.x -= sinf((D3DX_PI *  0.5f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z -= cosf((D3DX_PI *  0.5f) + pCamera->rot.y) * MOVE_SPEED;

					g_Player.aModel[2].fDestAngle = (D3DX_PI * 0.5f) /*+ pCamera->rot.y*/;

				}
			}

			else if (GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
				{
					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{
							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);

								g_Player.move.x += sinf((D3DX_PI *  0.75f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z += cosf((D3DX_PI *  0.75f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}
					g_Player.move.x += sinf((D3DX_PI *  0.75f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z += cosf((D3DX_PI *  0.75f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.aModel[2].fDestAngle = (D3DX_PI * -0.25f);

				}
				else if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
				{
					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{

							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);
								g_Player.move.x += sinf((D3DX_PI *  0.25f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z += cosf((D3DX_PI *  0.25f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}
					g_Player.move.x += sinf((D3DX_PI *  0.25f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z += cosf((D3DX_PI *  0.25f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.aModel[2].fDestAngle = (D3DX_PI * 0.25f);

				}
				else
				{
					if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE|| (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
					{
						if (g_Player.state == PLAYERSTATE_NORMAL)
						{
							if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
							{
								PlaySound(SOUND_LABEL_SE_QUICK);

								g_Player.move.x -= sinf((-D3DX_PI *  0.5f) + pCamera->rot.y) * DASHSPEED;
								g_Player.move.z -= cosf((-D3DX_PI *  0.5f) + pCamera->rot.y) * DASHSPEED;
								for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
								{
									SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
								}
							}
							g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
						}
					}
					g_Player.move.x -= sinf((-D3DX_PI *  0.5f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.move.z -= cosf((-D3DX_PI *  0.5f) + pCamera->rot.y) * MOVE_SPEED;
					g_Player.aModel[2].fDestAngle = (D3DX_PI * -0.5f);


				}
			}

			else if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
				{
					if (g_Player.state == PLAYERSTATE_NORMAL)
					{
						if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
						{
							PlaySound(SOUND_LABEL_SE_QUICK);

							g_Player.move.x += sinf(pCamera->rot.y) * DASHSPEED;
							g_Player.move.z += cosf(pCamera->rot.y) * DASHSPEED;
							for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
							{
								SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
						}
						g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
					}
				}
				g_Player.move.x += sinf(pCamera->rot.y) * MOVE_SPEED;
				g_Player.move.z += cosf(pCamera->rot.y) * MOVE_SPEED;
				g_Player.aModel[2].fDestAngle = D3DX_PI * 0.0f;

			}

			else if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true) )
			{
				if (GetKeyboardTrigger(DIK_RSHIFT) == TRUE  || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true))
				{
					if (g_Player.state == PLAYERSTATE_NORMAL)
					{
						if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
						{
							g_Player.move.x -= sinf(pCamera->rot.y) * DASHSPEED;
							g_Player.move.z -= cosf(pCamera->rot.y) * DASHSPEED;
							PlaySound(SOUND_LABEL_SE_QUICK);

							for (int nCnt = 0; nCnt < EFFECT_MAX; nCnt++)
							{
								SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, rand() % 1 + 0.45f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
							}
						}
						g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
					}
				}
				g_Player.move.x -= sinf(pCamera->rot.y) * MOVE_SPEED;
				g_Player.move.z -= cosf(pCamera->rot.y) * MOVE_SPEED;

				g_Player.aModel[2].fDestAngle = D3DX_PI * 0.0f;

			}




			if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_RIGHT_SHOULDER && pJoyState.bConnectionJoypad == true))
			{ //弾の発射
				if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{
					CAMERA *pCamera;
					pCamera = GetCamera();

					SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 25.0f, g_Player.pos.z), D3DXVECTOR3(-20.0f, pCamera->fPosR * -1, -20.0f), g_Player.rot, 70, 5.0f, BULLETTYPE_PLAYER);

					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
			}

			if (CollisionModel(&g_Player.pos, &g_Player.posold, &g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer) == true)
			{//接触判定
				if (GetKeyboardTrigger(DIK_SPACE) == true || (state.Gamepad.bLeftTrigger == 255 && pJoyState.bConnectionJoypad == true))
				{//ジャンプ

					if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
					{
						if (g_Player.bUseJump == false)
						{
							PlaySound(SOUND_LABEL_SE_LAND);
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetParticle(g_Player.pos, 60, D3DXCOLOR(0.4f, 1.0f, 0.0f, 0.0f), PARTICLETYPE_SHOWER, 0);
							}
							g_Player.aModel[1].fDestAngle = (D3DX_PI * 0.25f);
							g_Player.bUseJump = true;
							if (g_Player.bUseJump == true)
							{
								g_Player.move.y += fJump;
							}
						}
					}
					g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
				}
			}
			if (GetKeyboardTrigger(DIK_SPACE) == true || (state.Gamepad.bLeftTrigger == 255 && pJoyState.bConnectionJoypad == true))
			{//ジャンプ

				if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
				{
					if (g_Player.bUseJump == false)
					{
						PlaySound(SOUND_LABEL_SE_LAND);
						g_Player.aModel[1].fDestAngle = (D3DX_PI * 0.25f);
						g_Player.bUseJump = true;
						if (g_Player.bUseJump == true)
						{
							g_Player.move.y += fJump;
						}
					}
				}
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
			}
			if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_PUSH)
			{
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NONE;
			}
			else if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NONE)
			{
				g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
			}
		}//プレイヤーのbUse


	 //=========//
	}//ゲーム中//
	 //========//

	//SetMotion(1);

	g_Player.move.y -= fGravity;
	if (g_Player.move.y > GRAVITY_MAX)
	{
		g_Player.move.y = GRAVITY_MAX;
	}

	int pBoost;
	pBoost = GetBoost();

	if (pBoost >= 0)
	{
		pBoost = 0;


	}

	if (pBoost >= -100)
	{
		g_Player.state = PLAYERSTATE_NORMAL;
	}
	if (pBoost <= -200)
	{
		pBoost = -200;
		g_Player.state = PLAYERSTATE_OVERHEAT;
		g_Player.nCoolDown = 200;
	}


		if (GetKeyboardPress(DIK_LSHIFT) == true && g_Player.state == PLAYERSTATE_NORMAL || (state.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_SHOULDER && pJoyState.bConnectionJoypad == true))
		{

			g_Player.move.x += (0.0f - g_Player.move.x) * 0.12f;
			g_Player.move.z += (0.0f - g_Player.move.z) * 0.12f;
		}
		else
		{
			g_Player.move.x += (0.0f - g_Player.move.x) * 0.25f;
			g_Player.move.z += (0.0f - g_Player.move.z) * 0.25f;

		}


	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;
	g_Player.pos.z += g_Player.move.z;

	if (g_Player.pos.y <= 0.0)
	{

		g_Player.pos.y = 0.0f;
		g_Player.move.y = 0.0f;
		g_Player.bUseJump = false;

	}
	else
	{
		g_Player.bUseJump = true;

	}
	if (g_Player.pos.y <= 0.0 && g_Player.posold.y >= 1.0f)
	{
		PlaySound(SOUND_LABEL_SE_LAND);
		for (int nCnt = 0; nCnt < 80; nCnt++)
		{
			SetSmoke(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z), 60, D3DXCOLOR(0.2f, 0.4f, 0.9f, 0.5f));
		}
	}
#if 1


	//===========================================================
	// 軸2の回転
	//===========================================================
	g_Player.aModel[2].fAngle = g_Player.aModel[2].fDestAngle - g_Player.aModel[2].rot.y;
	if (g_Player.aModel[2].fAngle > D3DX_PI)
	{
		g_Player.aModel[2].fAngle -= D3DX_PI * 2;
	}

	if (g_Player.fAngle < -D3DX_PI)
	{
		g_Player.aModel[2].fAngle += D3DX_PI * 2;
	}
	g_Player.aModel[2].rot.y += g_Player.aModel[2].fAngle * ROT_SPEED;

#endif



	SetPositionShadow(g_Player.nIndexShadow, g_Player.pos);//影の位置

	///////////////////////////////////////
	//			ステージ端				///
	///////////////////////////////////////
	if (g_Player.pos.x > STAGE_LIMIT_MAX_X)
	{
		g_Player.pos.x = STAGE_LIMIT_MAX_X;
	}
	if (g_Player.pos.x < STAGE_LIMIT_MIN_X)
	{
		g_Player.pos.x = STAGE_LIMIT_MIN_X;
	}
	if (g_Player.pos.z > STAGE_LIMIT_MAX_Z)
	{
		g_Player.pos.z = STAGE_LIMIT_MAX_Z;
	}

	if (g_Player.pos.z < STAGE_LIMIT_MIN_Z)
	{
		g_Player.pos.z = STAGE_LIMIT_MIN_Z;
	}

#if 0
	if (g_Player.aModel[9].rot.x > D3DX_PI /8)
	{
		//g_Player.aModel[9].rot.x = D3DX_PI /8;

	}

	if (g_Player.aModel[9].rot.x < -D3DX_PI /13)
	{
		//g_Player.aModel[9].rot.x = -D3DX_PI /13;
	}
#endif

	SCOREITMEM pScore;
	pScore = *GetScoreItem();

	GATE *pGate;
	pGate = GetGate();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////接触判定関係//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (CollisionModel(&g_Player.pos, &g_Player.posold,&g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer) == true)
	{//接触判定
		GAMESTATE *pGame;
		pGame = GetGameState();
		g_Player.bUseJump = false;
	}
	else
	{
	}

	if (CollisionScoreItem(&g_Player.pos, &g_Player.posold, &g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer) == true)
	{
		PlaySound(SOUND_LABEL_SE_DECIDE);

		AddScore(15000);
	}

	if (CollisionEnemy(&g_Player.pos, &g_Player.posold, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer) == true)
	{//接触判定
		GAMESTATE *pGame;
		pGame = GetGameState();
		g_Player.bUseJump = false;
	}

	CollisionModelWall(&g_Player.pos, &g_Player.posold, &g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer);
	CollisionGate(&g_Player.pos, &g_Player.posold, &g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer);
	CollisionPedestal(&g_Player.pos, &g_Player.posold, &g_Player.move, &g_Player.VtxMaxPlayer, &g_Player.VtxMinPlayer);




	switch (g_Playerstatus)
	{
	case PLAYERSTATUS_NEUTRAL:
		if (pMotion->KeyFlame == pText->KEYFLAME[pMotion->nNumPlayerMotion] - 1)
		{
			if (g_PlayerstatusNow = PLAYERSTATUS_NEUTRAL)
			{


				SetMotion(0);
			}
			else
			{
				pMotion->KeyFlame = 0;
				SetMotion(0);
				g_PlayerstatusNow = PLAYERSTATUS_NEUTRAL;
				g_Playerstatus = PLAYERSTATUS_NEUTRAL;

			}
		}
		break;

	case PLAYERSTATUS_MOVE:
		g_PlayerstatusNow = PLAYERSTATUS_MOVE;
		g_Playerstatus = PLAYERSTATUS_NEUTRAL;
		SetMotion(1);
		break;
	}

	//g_Player.aModel[4].rot.x -= 0.1f;
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxParent2;

	//========================//
	//プレイヤーのインデックス//
	//========================//
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxtrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);


	for (int nCntModelParts = 0; nCntModelParts < MAX_PARTS; nCntModelParts++)
	{
		if (g_Player.bUse == true)
		{

			if (g_Player.aModel[nCntModelParts].nIdxModelParent == -1)
			{//プレイヤーのマトリックスに
				mtxParent = g_Player.mtxWorld;
			}
			else if (g_Player.aModel[nCntModelParts].nIdxModelParent == 0)
			{//体のマトリックスに
				mtxParent = g_Player.aModel[0].mtxWorld;
			}
			else if (g_Player.aModel[nCntModelParts].nIdxModelParent == 1)
			{//軸１に
				mtxParent = g_Player.aModel[1].mtxWorld;
			}
			else if (g_Player.aModel[nCntModelParts].nIdxModelParent == 2)
			{//軸２に
				mtxParent = g_Player.aModel[2].mtxWorld;
			}
			else
			{//その他
				mtxParent = g_Player.aModel[nCntModelParts-1].mtxWorld;
			}
			//=======================================================================//
			//						    親モデルのインデックス						//
			//=====================================================================//
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Player.aModel[nCntModelParts].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_Player.aModel[nCntModelParts].rot.y, g_Player.aModel[nCntModelParts].rot.x, g_Player.aModel[nCntModelParts].rot.z);
			D3DXMatrixMultiply(&g_Player.aModel[nCntModelParts].mtxWorld, &g_Player.aModel[nCntModelParts].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_Player.aModel[nCntModelParts].pos.x, g_Player.aModel[nCntModelParts].pos.y, g_Player.aModel[nCntModelParts].pos.z);
			D3DXMatrixMultiply(&g_Player.aModel[nCntModelParts].mtxWorld, &g_Player.aModel[nCntModelParts].mtxWorld, &mtxtrans);

			//親のマトリックス反映
			D3DXMatrixMultiply(&g_Player.aModel[nCntModelParts].mtxWorld, &g_Player.aModel[nCntModelParts].mtxWorld, &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntModelParts].mtxWorld);


			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModelParts].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntModelParts].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//pDevice->SetTexture(0, g_Player.aModel[0].pTexture);
				pDevice->SetTexture(0, 0);
				// モデル(パーツ)の描画
				g_Player.aModel[nCntModelParts].pMesh->DrawSubset(nCntMat);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
	//=======================================親モデル設定終了============================================================//

}

//=============================================================================
// 被ダメージ処理
//=============================================================================
void HitPlayer(int nDamage)
{

	if (g_Player.bUse == true)
	{
		g_Player.nLife += nDamage;
		for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
		{
			SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);
		}

		if (g_Player.nLife <= 0)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			g_Player.bUse = false;
			DeleteShadow(g_Player.nIndexShadow);
			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), PARTICLETYPE_EXPLOSION, 0);
				SetParticle(g_Player.pos, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PARTICLETYPE_SHOWER, 0);

			}
			SetGameState(GAMESTATE_FAILED);
		}

	}
}




PLAYER *GetPlayer(void)
{
	return &g_Player;
}

PLAYERSTATUS *GetPlayerStatus(void)
{
	return &g_Player.status;
}


