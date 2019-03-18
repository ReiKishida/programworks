//=============================================================================
//
// カメラ処理 [titlecamera.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "titlecamera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MDD2 (1.0f)
#define ROT_CAMERASPEED (0.025f)
#define ROT_SPEED (0.2f)

#define CAMERA_ANGLE (12.0f)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLECAMERA g_TitleCamera;


//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitTitleCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	MODE *pMode;
	pMode = GetMode();


		g_TitleCamera.posV = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
		g_TitleCamera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TitleCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

		g_TitleCamera.fLength = 0.0f;
		g_TitleCamera.fAngle = 0.0f;

		g_TitleCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);


}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitTitleCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateTitleCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();




	MODE *pMode;
	pMode = GetMode();



	OverTitleView();


}
//=============================================================================
// カメラの設定処理
//=============================================================================
void SetTitleCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_TitleCamera.mtxProjection);

	// プロジェクションマトリックスを作成
	/*D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection,
	D3DXToRadian(45.0f),
	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	10.0f,
	1000.0f);*/
	D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		1200.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_TitleCamera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_TitleCamera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_TitleCamera.mtxView,//ビューマトリックス
		&g_TitleCamera.posV,//視点
		&g_TitleCamera.posR,//注視点
		&g_TitleCamera.vecU);//上方向ベクトル

						// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_TitleCamera.mtxView);
}

//=============================================================================
// カメラの調整
//=============================================================================
void OverTitleView()
{
	if (g_TitleCamera.rot.y > D3DX_PI)
	{

		g_TitleCamera.rot.y -= D3DX_PI * 2;
	}

	if (g_TitleCamera.rot.y < -D3DX_PI)
	{

		g_TitleCamera.rot.y += D3DX_PI * 2;
	}

	if (g_TitleCamera.rot.x > D3DX_PI / 4)
	{

		g_TitleCamera.rot.x = D3DX_PI / 4;
	}

	if (g_TitleCamera.rot.x < -D3DX_PI / 4)
	{

		g_TitleCamera.rot.x = -D3DX_PI / 4;
	}




	if (g_TitleCamera.fAngle > D3DX_PI)
	{
		g_TitleCamera.fAngle -= D3DX_PI * 2;
	}

	if (g_TitleCamera.fAngle < -D3DX_PI)
	{
		g_TitleCamera.fAngle += D3DX_PI * 2;
	}
}



//=============================================================================
// カメラの取得
//=============================================================================
TITLECAMERA *GetCamera(void)
{
	return &g_TitleCamera;
}



