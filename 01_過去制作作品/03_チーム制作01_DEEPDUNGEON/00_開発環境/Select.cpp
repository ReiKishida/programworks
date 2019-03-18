//=============================================================================
//
// ステージ移動セレクト処理 [Pause.cpp]
// Author : TEAM_Fascinator
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "inputx.h"
#include "Stagefade.h"
#include "Select.h"
#include "Select_NO.h"
#include "Select_YES.h"
#include "SelectBG.h"
#include "player.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
STAGESERECTSTATE	g_SelectMode = STAGESERECTSATE_NONE;
int					g_nSelectManu;
JoyState			g_JoyStateSelect;		// ジョイパッドの状態

//=============================================================================
// ステージ移動セレクト初期化処理[InitPolygon]
//=============================================================================
void InitSelect(void)
{
	g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//セレクト画面背景の初期化処理
	InitSelectBG();

	//セレクト画面Noの初期化処理
	InitSelectNo();

	//セレクト画面Yesの初期化処理
	InitSelectYes();
}

//=============================================================================
//ステージ移動セレクト終了処理 [UninitPolygon]
//=============================================================================
void UninitSelect(void)
{
	//セレクト画面背景の終了処理
	UninitSelectBG();

	//セレクト画面Noの終了処理
	UninitSelectNo();

	//セレクト画面Yesの終了処理
	UninitSelectYes();
}

//=============================================================================
// ステージ移動セレクト更新処理[UpdataPolygon]
//=============================================================================
void UpdateSelect(void)
{
	FADE pFade = GetFade();					//フェード状態の取得
	PLAYER *pPlayer = GetPlayer();			//プレイヤー情報の取得
	STAGEFADE pStageFade = GetStageFade();		//フェード状況を取得
	STAGEMODE pStageMode = GetStageMode();	//ステージモードの取得
	JoyState pJoystate = GetJoystate();		//ジョイパッドの接続情報確認

	//ジョイパッドの入力情報取得
	XINPUT_STATE state;
	XInputGetState(0, &state);

	if (GetKeyboardTrigger(DIK_A) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && pJoystate.bConnectionJoypad == true))
	{//Aキーカーソル操作 or PRESS GAMEPAD RIGHTPAD
		if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{//ジョイパッドの連打防止
			PlaySound(SOUND_LABEL_SE_SELECT);

			g_nSelectManu = (g_nSelectManu + 1) % 2;
		}
		g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_PUSH; //ジョイパッドを押下状態にする
	}

	if (GetKeyboardTrigger(DIK_D) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && pJoystate.bConnectionJoypad == true))
	{//Dキーカーソル操作 or PRESS GAMEPAD LEFTPAD
		if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{//ジョイパッドの連打防止
			PlaySound(SOUND_LABEL_SE_SELECT);

			g_nSelectManu = (g_nSelectManu + 1) % 2;
		}
		g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_PUSH; //ジョイパッドを押下状態にする
	}

	if (g_nSelectManu == 0)
	{//YESにカーソルがあっている
		SetSelectYes(YESSTATE_SELECT);//Yesが選ばれている状態
		SetSelectNo(NOSTATE_NOSELECT);//Noは選ばれてない状態
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
		{//先に進む or PRESS GAMEPAD BKEY
			if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{//ジョイパッドの連打防止
				if (pStageFade != STAGEFADE_OUT)
				{//Enter連打による点滅防止
					PlaySound(SOUND_LABEL_SE_DECISION);

					g_SelectMode = STAGESERECTSATE_YES;
				}
			}
			g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_PUSH;  //ジョイパッドを押下状態にする
		}
	}

	if (g_nSelectManu == 1)
	{//NOにカーソルがあっている
		SetSelectYes(YESSTATE_NOSELECT);//Yesは選ばれていない
		SetSelectNo(NOSTATE_SELECT);	//Noが選ばれている
		if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && pJoystate.bConnectionJoypad == true))
		{//進まない or PRESS GAMEPAD BKEY
			if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{//ジョイパッドの連打防止
				if (pStageFade != STAGEFADE_OUT)
				{//Enter連打による点滅防止
					PlaySound(SOUND_LABEL_SE_DECISION);

					g_SelectMode = STAGESERECTSATE_NO;
				}
			}
			g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_PUSH; //ジョイパッドを押下状態にする
		}
	}

	switch (g_SelectMode)
	{
	case STAGESERECTSATE_YES:
		if (pStageMode == STAGEMODE_STAGE1)
		{//ステージ1から2へ移動
			SetStageFade(STAGEMODE_STAGE2);
			g_SelectMode = STAGESERECTSATE_NONE;
			SetSelect(false);
			pPlayer->pos = D3DXVECTOR3(350.0f, 0.0f, -350.0f);	//プレイヤーの位置を移動させる		
			pPlayer->rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 00.0f);	//プレイヤーの位置を移動させる		
			break;
		}
		else if (pStageMode == STAGEMODE_STAGE2)
		{//ステージ2から3へ移動
			SetStageFade(STAGEMODE_STAGE3);
			g_SelectMode = STAGESERECTSATE_NONE;
			SetSelect(false);
			pPlayer->pos = D3DXVECTOR3(350.0f, 0.0f, -350.0f);	//プレイヤーの位置を移動させる		
			pPlayer->rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 00.0f);	//プレイヤーの位置を移動させる		
			break;
		}
		else if (pStageMode == STAGEMODE_STAGE3)
		{//ステージ3から4に移動
			SetGameState(GAMESTATE_CLEAR);
			g_SelectMode = STAGESERECTSATE_NONE;
			SetSelect(false);
			pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//プレイヤーの位置を移動させる
			break;
		}
	case STAGESERECTSATE_NO:
		g_SelectMode = STAGESERECTSATE_NONE;
		SetSelect(false);
		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//プレイヤーの移動をやめる
		pPlayer->pos = pPlayer->posOld;				//プレイヤーの位置を移動させる
		break;
	}

	//セレクト画面の背景の更新処理
	UpdateSelectBG();

	//セレクト画面Noの更新処理
	UpdateSelectNo();

	//セレクト画面Yesの更新処理
	UpdateSelectYes();

	//ジョイパッドの状態更新
	if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateSelect.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateSelect.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// ステージ移動セレクト描写処理[DrawPolygon]
//=============================================================================
void DrawSelect(void)
{
	//セレクト画面の背景の描画処理
	DrawSelectBG();
	
	//セレクト画面Noの描画処理
	DrawSelectNo();

	//セレクト画面Yesの描画処理
	DrawSelectYes();
}