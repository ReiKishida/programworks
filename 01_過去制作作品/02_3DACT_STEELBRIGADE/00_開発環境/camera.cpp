//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "inputx.h"
#include "reticle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ROT_SPEED (0.25f)//カメラの旋回速度(上下)
#define ROT_SPEED_Y (0.3f)	//カメラの旋回速度(左右)



//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA g_Camera;	//カメラの情報
JoyState g_JoyStateCamera;//ゲームパッドの状況(カメラ)
float fTrueZoom;//目標のズーム値
float fZoom;//今のズーム値
float fDestZoom;//ズーム値の差分

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	//プレイヤーの取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//モードの取得
	MODE *pMode;
	pMode = GetMode();

	//プレイヤーと視点の距離
	g_Camera.fLength = pPlayer->pos.z - g_Camera.posV.z;
	g_Camera.fRLength = 10.0f;
	g_Camera.fAngle = 0.0f;

	//角度
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//角度の差分(視点)
	g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	//角度の差分(注視点)
	g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	g_Camera.posV = D3DXVECTOR3(pPlayer->pos.x,
		pPlayer->pos.y/* + 50.0f*/,
		pPlayer->pos.z/* - 100.0f*/);

	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	g_Camera.fPosR = -5.0f;

	fZoom = 0.0f;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	JoyState pJoyState = GetJoystate();

	XINPUT_STATE state;
	XInputGetState(0, &state);

	//プレイヤーの情報取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();


	//===========================回り込み===================================//
	g_Camera.fDestAngle.y = D3DX_PI + pPlayer->rot.y - g_Camera.rot.y;
	if (g_Camera.fDestAngle.y > D3DX_PI)
	{
		g_Camera.fDestAngle.y -= D3DX_PI * 2;
	}
	if (g_Camera.fDestAngle.y < -D3DX_PI)
	{
		g_Camera.fDestAngle.y += D3DX_PI * 2;
	}
	g_Camera.rot.y += g_Camera.fDestAngle.y * ROT_SPEED;

	RETICLE pReticle;
	pReticle = GetReticle();
	if (GetKeyboardTrigger(DIK_UP) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && pJoyState.bConnectionJoypad == true))
	{//十字キーの上が押されたとき
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			fDestZoom = -30.0f;
			pReticle.fHeight = 600.0f;
			pReticle.fWidth = 600.0f;


		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	if (GetKeyboardTrigger(DIK_DOWN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && pJoyState.bConnectionJoypad == true))
	{//十字キーの下が押されたとき
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			fDestZoom = 0;
			pReticle.fHeight = 150.0f;
			pReticle.fWidth = 150.0f;

		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	fTrueZoom = fDestZoom - fZoom;
	fZoom += fTrueZoom *0.05f;
 	if (GetKeyboardPress(DIK_T) == true || (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Ｔキーが押されたとき、右スティックを上に倒した時
		g_Camera.fPosR -= 2.0f;
	}

	else if (GetKeyboardPress(DIK_G) == true || (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
	{//Gキーが押されたとき、右スティックを下に倒した時
		g_Camera.fPosR += 2.0f;
	}

	//============================================================================//


	//===========================ちょっと先追従（前）===================================//
	g_Camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * -2.0f;
	g_Camera.posRDest.y = pPlayer->pos.y + 50.0f;
	g_Camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * -2.0f;


	g_Camera.posVDest.x = pPlayer->pos.x - sinf(g_Camera.rot.y) * -170.0f;
	g_Camera.posVDest.y = pPlayer->pos.y + 70.0f + g_Camera.fPosR;
	g_Camera.posVDest.z = pPlayer->pos.z - cosf(g_Camera.rot.y) * -170.0f;

	g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * ROT_SPEED;
	g_Camera.posR.y += (g_Camera.posRDest.y - g_Camera.posR.y) * ROT_SPEED_Y;
	g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * ROT_SPEED;

	g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * ROT_SPEED;
	g_Camera.posV.y += (g_Camera.posVDest.y - g_Camera.posV.y) * ROT_SPEED_Y;
	g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * ROT_SPEED;
	//===========================================================================//



	OverView();//半周超えたら

	if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			g_Camera.fPosR = -6.0f;
		}
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_PUSH;

	}

	if (GetKeyboardPress(DIK_H) == true)
	{//Hキーが押されたとき
		g_Camera.fPosR = 0;

	}

	if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateCamera.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateCamera.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}

}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスを作成
	/*D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
	D3DXToRadian(45.0f),
	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	10.0f,
	1000.0f);*/
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		D3DXToRadian(45.0f + fZoom),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView,//ビューマトリックス
		&g_Camera.posV,//視点
		&g_Camera.posR,//注視点
		&g_Camera.vecU);//上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=============================================================================
// カメラの調整
//=============================================================================
void OverView()
{

	if (g_Camera.fPosR > 70)
	{//上に向ききったとき

		g_Camera.fPosR = 70;
	}

	if (g_Camera.fPosR < -70)
	{//下に向ききったとき

		g_Camera.fPosR = -70;
	}


	//半周こえたとき//
	if (g_Camera.fAngle > D3DX_PI)
	{
		g_Camera.fAngle -= D3DX_PI * 2;
	}

	if (g_Camera.fAngle < -D3DX_PI)
	{
		g_Camera.fAngle += D3DX_PI * 2;
	}
}



//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_Camera;
}



