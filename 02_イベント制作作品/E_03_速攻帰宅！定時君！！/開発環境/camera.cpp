////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// カメラ処理 [camera.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "camera.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "gamepad.h"
#include "game.h"
#include "sound.h"
#include "meshWall.h"
#include "model.h"
#include "light.h"
//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_DISTANCE	(200.0f)		// 距離
#define MAX_HEIGHT		(150)			// 高さの距離
#define MAX_SPEED		(10.0f)			// カメラのスピード
#define MAX_ROLL		(0.03f)			// 回転量
#define MAX_DISTANCE_R	(10)			// プレイヤーと注視点の距離
#define MAX_HEIGHT_R	(70)			// 注視点の高さ
#define MAX_CAMERA		(5)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3	posV;			// 視点
	D3DXVECTOR3	posR;			// 注視点
	D3DXVECTOR3	rot;			// 回転量
	float fLength;				// V長さ
	float fHeight;				// V高さ
	float fLengthR;				// R長さ
	float fHeightR;				// R高さ
	int posX;					// ビューポート
	int posY;					// ビューポート
	int Width;					// ビューポート
	int Hight;					// ビューポート
	float MinZ;					// ビューポート
	float MaxZ;					// ビューポート
	float Viewrot;				// 画角
}CameraInfo;

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
Camera g_camera[MAX_CAMERA];// カメラの情報
CameraInfo g_cameraInfo[] =
{
	{ D3DXVECTOR3(0.0f, MAX_HEIGHT, -MAX_DISTANCE) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),MAX_DISTANCE,MAX_HEIGHT,MAX_DISTANCE_R,MAX_HEIGHT_R,
	0,0,SCREEN_WIDTH ,SCREEN_HEIGHT,0.0f,1.0f,45.0f },

	{ D3DXVECTOR3(0.0f, 50, -50) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),90,0,20,10,
	0,0,400,290,0.0f,1.0f,45.0f },
	{ D3DXVECTOR3(0.0f, 50, -50) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),90,0,20,10,
	900,0,400,290,0.0f,1.0f,45.0f },
	{ D3DXVECTOR3(0.0f, 50, -50) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),90,0,20,10,
	0,350,400,300,0.0f,1.0f,45.0f },
	{ D3DXVECTOR3(0.0f, 50, -50) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.8f, 0.0f, 0.0f),90,0,20,10,
	900,350,400,300,0.0f,1.0f,45.0f },

};
bool g_bAutoCamera;
int nDamageCameraCounter;
bool bDamageCamera;
float g_fAngle;
bool bPauseCamera;
int g_nTitleCameraCnt;
int g_nRankingEnemyCnt;
int g_nBattleStart;
int g_nFlontIdy;
//==============================================================================================================================
// カメラの初期化処理
//==============================================================================================================================
void InitCamera(void)
{
	for (int nCamera = 0; nCamera < MAX_CAMERA; nCamera++)
	{
		g_camera[nCamera].posV = g_cameraInfo[nCamera].posV;
		g_camera[nCamera].posVOld = g_cameraInfo[nCamera].posV;
		g_camera[nCamera].posR = g_cameraInfo[nCamera].posR;
		g_camera[nCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera[nCamera].rot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].Diffrot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].Destrot = g_cameraInfo[nCamera].rot;
		g_camera[nCamera].posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].nState = CAMERASTATE_NONE;
		g_camera[nCamera].nStateCounter = 0;
		g_camera[nCamera].fLength = g_cameraInfo[nCamera].fLength;
		g_camera[nCamera].fHeight = g_cameraInfo[nCamera].fHeight;
		g_camera[nCamera].fLengthR = g_cameraInfo[nCamera].fLengthR;
		g_camera[nCamera].fHeightR = g_cameraInfo[nCamera].fHeightR;
		g_camera[nCamera].viewpoart.X = g_cameraInfo[nCamera].posX;
		g_camera[nCamera].viewpoart.Y = g_cameraInfo[nCamera].posY;
		g_camera[nCamera].viewpoart.Width = g_cameraInfo[nCamera].Width;
		g_camera[nCamera].viewpoart.Height = g_cameraInfo[nCamera].Hight;
		g_camera[nCamera].viewpoart.MinZ = g_cameraInfo[nCamera].MinZ;
		g_camera[nCamera].viewpoart.MaxZ = g_cameraInfo[nCamera].MaxZ;
		g_camera[nCamera].fLengthAdd = 0;
		g_camera[nCamera].fHeightWall = 0;
		g_camera[nCamera].PauseOldposV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera[nCamera].PauseOldposR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	bPauseCamera = false;
	g_bAutoCamera = false;
	g_fAngle = 0;
	g_nTitleCameraCnt = 0;
	g_nRankingEnemyCnt = 0;
	g_nBattleStart = 0;		// バトルはじめのカウンター
	g_nFlontIdy = 0;		// フロントポリゴンのID
}

//==============================================================================================================================
// カメラの終了処理
//==============================================================================================================================
void UninitCamera(void)
{
}

//==============================================================================================================================
// カメラの更新処理
//==============================================================================================================================
void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	MODE Mode;
	Mode = GetMode();
	Player *pPlayer;
	pPlayer = GetPlayer();
	GamePad *pGamePad = GetgamePadStick();
	for (int nCamera = 0; nCamera < MAX_CAMERA; nCamera++)
	{
		if (nCamera == 0)
		{
			g_camera[nCamera].posVOld = g_camera[nCamera].posV;
			if (Mode == MODE_GAME || Mode == MODE_RESULT)
			{

				// カメラの移動
				if (GetPauseView() == true)
				{
					for (int nIdy = 0; nIdy < MAX_GAMEPAD; nIdy++)
					{// ゲームパッド
						if (pGamePad[nIdy].pDevGamePad != NULL)
						{
							if (pGamePad[nIdy].aGamePad.lX < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lX > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lY> GAMEPAD_DEADZONE)
							{
								float fGamePagStickAngle = atan2f(float(-pGamePad[nIdy].aGamePad.lX), float(pGamePad[nIdy].aGamePad.lY));
								g_camera[nCamera].posV.x += sinf(fGamePagStickAngle - D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posV.z += cosf(fGamePagStickAngle - D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
								g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
								g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
							}
							if (pGamePad[nIdy].aGamePad.lZ < -GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lZ > GAMEPAD_DEADZONE)
							{// アナログスティックの方向[角度]に移動
							 // 角度を計算する
								g_camera[nCamera].rot.y += float(pGamePad[nIdy].aGamePad.lZ / (MAX_ZONE*20.0f));
								g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
								g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
							}
							if (pGamePad[nIdy].aGamePad.lRz > GAMEPAD_DEADZONE || pGamePad[nIdy].aGamePad.lRz < -GAMEPAD_DEADZONE)
							{// アナログスティックの方向[角度]に移動
								 // 角度を計算する
								g_camera[nCamera].rot.x += float(pGamePad[nIdy].aGamePad.lRz / (MAX_ZONE*50.0f));
								g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
							}
						}
					}
					if (GetKeyboardPress(DIK_Z) == true)
					{// 視点左旋回
						g_camera[nCamera].rot.y += MAX_ROLL;
						g_camera[nCamera].Destrot.y += MAX_ROLL;
						g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
						g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
					}
					if (GetKeyboardPress(DIK_C) == true)
					{// 視点右旋回
						g_camera[nCamera].rot.y += -MAX_ROLL;
						g_camera[nCamera].Destrot.y += -MAX_ROLL;
						g_camera[nCamera].posV.x = g_camera[nCamera].posR.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
						g_camera[nCamera].posV.z = g_camera[nCamera].posR.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * g_camera[nCamera].fLength;
					}
					if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
					{// 左移動
						g_camera[nCamera].posV.x += sinf(-D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posV.z += cosf(-D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
					}
					if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
					{// 右移動
						g_camera[nCamera].posV.x += sinf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posV.z += cosf(D3DX_PI / 2 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
					}
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{// 前移動
						g_camera[nCamera].posV.x += sinf(0 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posV.z += cosf(0 + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
					}
					if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{// 後ろ移動
						g_camera[nCamera].posV.x += sinf(D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posV.z += cosf(D3DX_PI + g_camera[nCamera].rot.y)*MAX_SPEED;
						g_camera[nCamera].posR.x = g_camera[nCamera].posV.x + sinf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
						g_camera[nCamera].posR.z = g_camera[nCamera].posV.z + cosf(g_camera[nCamera].rot.y) * g_camera[nCamera].fLength;
					}
					for (int nIdy = 0; nIdy < MAX_GAMEPAD; nIdy++)
					{// ゲームパッド
						if (GetGamePadPress(nIdy, ELECOM_Y) )
						{// 上昇
							g_camera[nCamera].posV.y += MAX_SPEED;
							g_camera[nCamera].posR.y += MAX_SPEED;
						}
						if (GetGamePadPress(nIdy, ELECOM_X) )
						{// 下降
							g_camera[nCamera].posV.y -= MAX_SPEED;
							g_camera[nCamera].posR.y -= MAX_SPEED;
						}
					}
					if ( GetKeyboardPress(DIK_R) == true)
					{
						g_camera[nCamera].posV.y += MAX_SPEED;
						g_camera[nCamera].posR.y += MAX_SPEED;
					}
					if (GetKeyboardPress(DIK_F) == true)
					{
						g_camera[nCamera].posV.y -= MAX_SPEED;
						g_camera[nCamera].posR.y -= MAX_SPEED;
					}
					if (GetKeyboardPress(DIK_T) == true)
					{// Iを押した
					 // 角度を計算する
						g_camera[nCamera].rot.z += -0.02f;
						g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.z + g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
					}
					if (GetKeyboardPress(DIK_G) == true)
					{// Kを押した
					 // 角度を計算する
						g_camera[nCamera].rot.z += 0.02f;
						g_camera[nCamera].posV.y = g_camera[nCamera].posR.y + cosf(g_camera[nCamera].rot.z + g_camera[nCamera].rot.x) * g_camera[nCamera].fHeight;
					}

					bPauseCamera = true;
				}
				else
				{
					if (bPauseCamera == false)
					{// ポーズしてない
						g_camera[nCamera].PauseOldposV = g_camera[nCamera].posV;
						g_camera[nCamera].PauseOldposR = g_camera[nCamera].posR;
						g_camera[nCamera].PauseOldrot = g_camera[nCamera].rot;
					}
					if (bPauseCamera == true)
					{// ポーズ開始
						g_camera[nCamera].posVDest = g_camera[nCamera].PauseOldposV;
						g_camera[nCamera].posRDest = g_camera[nCamera].PauseOldposR;
						g_camera[nCamera].Destrot = g_camera[nCamera].PauseOldrot;
						bPauseCamera = false;
					}
					// -----------------------/ x の差分 /-----------------------------------------//
					if (g_camera[nCamera].Destrot.x >= D3DX_PI / 2.5f)
					{// 限界地
						g_camera[nCamera].Destrot.x = D3DX_PI / 2.5f;
					}
					if (g_camera[nCamera].Destrot.x <= 0)
					{// 限界地
						g_camera[nCamera].Destrot.x = 0;
					}
					if (g_camera[nCamera].Destrot.x > D3DX_PI)
					{// 差分が3.14を超えるなら
						g_camera[nCamera].Destrot.x -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].Destrot.x < -D3DX_PI)
					{// 差分が-3.14を超えるなら
						g_camera[nCamera].Destrot.x += D3DX_PI * 2.0f;
					}
					g_camera[nCamera].Diffrot.x = g_camera[nCamera].Destrot.x - g_camera[nCamera].rot.x;
					if (g_camera[nCamera].Diffrot.x > D3DX_PI)
					{// 差分が3.14を超えるなら
						g_camera[nCamera].Diffrot.x -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].Diffrot.x < -D3DX_PI)
					{// 差分が-3.14を超えるなら
						g_camera[nCamera].Diffrot.x += D3DX_PI * 2.0f;
					}
					g_camera[nCamera].rot.x += g_camera[nCamera].Diffrot.x*0.15f;
					if (g_camera[nCamera].rot.x > D3DX_PI)
					{// 現在の角度が3.14を超えるなら
						g_camera[nCamera].rot.x -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].rot.x < -D3DX_PI)
					{// 現在の角度が-3.14を超えるなら
						g_camera[nCamera].rot.x += D3DX_PI * 2.0f;
					}
					// -----------------------/ y の差分 /-----------------------------------------//
					if (g_camera[nCamera].Destrot.y > D3DX_PI)
					{// 差分が3.14を超えるなら
						g_camera[nCamera].Destrot.y -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].Destrot.y < -D3DX_PI)
					{// 差分が-3.14を超えるなら
						g_camera[nCamera].Destrot.y += D3DX_PI * 2.0f;
					}
					g_camera[nCamera].Diffrot.y = g_camera[nCamera].Destrot.y - g_camera[nCamera].rot.y;
					if (g_camera[nCamera].Diffrot.y > D3DX_PI)
					{// 差分が3.14を超えるなら
						g_camera[nCamera].Diffrot.y -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].Diffrot.y < -D3DX_PI)
					{// 差分が-3.14を超えるなら
						g_camera[nCamera].Diffrot.y += D3DX_PI * 2.0f;
					}
					g_camera[nCamera].rot.y += g_camera[nCamera].Diffrot.y*0.15f;
					if (g_camera[nCamera].rot.y > D3DX_PI)
					{// 現在の角度が3.14を超えるなら
						g_camera[nCamera].rot.y -= D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].rot.y < -D3DX_PI)
					{// 現在の角度が-3.14を超えるなら
						g_camera[nCamera].rot.y += D3DX_PI * 2.0f;
					}
					if (g_camera[nCamera].nState == CAMERASTATE_NONE)
					{// 通常時

						// 視点移動
						g_camera[nCamera].posV.x = pPlayer->pos.x + sinf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);
						g_camera[nCamera].posV.y = pPlayer->pos.y + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight + g_camera[nCamera].fHeightWall);			// 角度の加算
						g_camera[nCamera].posV.z = pPlayer->pos.z + cosf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);		// 角度の加算																																										// プレイヤーの少し前の注視点追尾
						g_camera[nCamera].posRDest.x = pPlayer->pos.x  + sinf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
						g_camera[nCamera].posRDest.y = pPlayer->pos.y + cosf(pPlayer->rot.x) * g_camera[nCamera].fHeightR;
						g_camera[nCamera].posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
						g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// 現在の角度と目的の角度の差分
						g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x * 0.3f;				// 角度の加算
						g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y * 0.8f;				// 角度の加算
						g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z * 0.3f;				// 角度の加算

					}
				}
			}

		}

		if (nCamera >= 1)
		{
			if (pPlayer[nCamera - 1].bUse)
			{
				g_camera[nCamera].Destrot.y = pPlayer[nCamera - 1].rot.y + D3DX_PI / 4;
				if (g_camera[nCamera].Destrot.y > D3DX_PI)
				{// 差分が3.14を超えるなら
					g_camera[nCamera].Destrot.y -= D3DX_PI * 2.0f;
				}
				if (g_camera[nCamera].Destrot.y < -D3DX_PI)
				{// 差分が-3.14を超えるなら
					g_camera[nCamera].Destrot.y += D3DX_PI * 2.0f;
				}
				g_camera[nCamera].Diffrot.y = g_camera[nCamera].Destrot.y - g_camera[nCamera].rot.y;
				if (g_camera[nCamera].Diffrot.y > D3DX_PI)
				{// 差分が3.14を超えるなら
					g_camera[nCamera].Diffrot.y -= D3DX_PI * 2.0f;
				}
				if (g_camera[nCamera].Diffrot.y < -D3DX_PI)
				{// 差分が-3.14を超えるなら
					g_camera[nCamera].Diffrot.y += D3DX_PI * 2.0f;
				}
				g_camera[nCamera].rot.y += g_camera[nCamera].Diffrot.y;
				if (g_camera[nCamera].rot.y > D3DX_PI)
				{// 現在の角度が3.14を超えるなら
					g_camera[nCamera].rot.y -= D3DX_PI * 2.0f;
				}
				if (g_camera[nCamera].rot.y < -D3DX_PI)
				{// 現在の角度が-3.14を超えるなら
					g_camera[nCamera].rot.y += D3DX_PI * 2.0f;
				}
				g_camera[nCamera].posV.x = pPlayer[nCamera - 1].aModel[3].mtxWorld._41;
				g_camera[nCamera].posV.y = pPlayer[nCamera - 1].aModel[3].mtxWorld._42 + cosf(g_camera[nCamera].rot.x) * (g_camera[nCamera].fHeight + g_camera[nCamera].fHeightWall);			// 角度の加算
				g_camera[nCamera].posV.z = pPlayer[nCamera - 1].aModel[3].mtxWorld._43 + cosf(g_camera[nCamera].rot.y - D3DX_PI) * (g_camera[nCamera].fLength + g_camera[nCamera].fLengthAdd);		// 角度の加算																																										// プレイヤーの少し前の注視点追尾
				g_camera[nCamera].posR.x = pPlayer[nCamera - 1].aModel[3].mtxWorld._41 + sinf(pPlayer[nCamera - 1].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
				g_camera[nCamera].posR.y = pPlayer[nCamera - 1].aModel[3].mtxWorld._42 + g_camera[nCamera].fHeightR;
				g_camera[nCamera].posR.z = pPlayer[nCamera - 1].aModel[3].mtxWorld._43 + cosf(pPlayer[nCamera - 1].rot.y - D3DX_PI) * g_camera[nCamera].fLengthR;
				//g_camera[nCamera].posRDiff = g_camera[nCamera].posRDest - g_camera[nCamera].posR;// 現在の角度と目的の角度の差分
				//g_camera[nCamera].posR.x += g_camera[nCamera].posRDiff.x ;				// 角度の加算
				//g_camera[nCamera].posR.y += g_camera[nCamera].posRDiff.y ;				// 角度の加算
				//g_camera[nCamera].posR.z += g_camera[nCamera].posRDiff.z ;				// 角度の加算

			}
		}

	}
}

//==============================================================================================================================
// カメラの設定処理
//==============================================================================================================================
void SetCamera(int nIdxCamera)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	// ビューポート
	pDevice->SetViewport(&g_camera[nIdxCamera].viewpoart);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdxCamera].mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera[nIdxCamera].mtxProjection, D3DXToRadian(45.0f),
								(float)g_camera[nIdxCamera].viewpoart.Width / (float)g_camera[nIdxCamera].viewpoart.Height,
								10.0f,			// 最小
								30000.0f);		// 最大

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdxCamera].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera[nIdxCamera].mtxView);
	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera[nIdxCamera].mtxView,
		&g_camera[nIdxCamera].posV, &g_camera[nIdxCamera].posR, &g_camera[nIdxCamera].vecU);
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdxCamera].mtxView);


}

//==============================================================================================================================
// カメラの取得
//==============================================================================================================================
Camera *GetCamera(void)
{
	return &g_camera[0];
}