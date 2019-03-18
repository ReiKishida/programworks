//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "scene.h"
#include "boss.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_SPEED (1.0f)
#define CAMERA_HEIGHT (150.0f)
#define CAMERA_MOVE (3.5f)
#define ROT_CAMERASPEED (0.01f)
#define ROT_SPEED (0.1f)

#define CAMERA_ANGLE (12.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CCamera::m_pManager = NULL;
bool CCamera::m_bFree = false;
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	m_fLength = 0.0f;
	m_fHeight_V = 0.0f;
	m_fHeight_R = 0.0f;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);
	m_pManager = NULL;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 関数名：カメラの初期化処理
// 関数の概要：カメラの位置や、距離を設定する。
//=============================================================================
HRESULT CCamera::Init(void)
{
	//m_type = type;

	//カメラの位置情報の初期化
	m_posV = D3DXVECTOR3(450.0f, 250.0f, -1200.0f);
	m_posR = D3DXVECTOR3(450.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	m_fLength = 700.0f;		// 注視点からの距離
	m_fHeight_V = 270.0f;		// 高さ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);	// 角度情報
	m_bFree = false;

	return S_OK;
}

//=============================================================================
// 関数名：カメラの終了
// 関数の概要：
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 関数名：カメラの更新
// 関数の概要：追従カメラの設定
//=============================================================================
void CCamera::Update(void)
{

#if 1

																//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	CPlayer *pPlayer = NULL;
	CBoss *pBoss = NULL;

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		pBoss = CGame::GetBoss();
		break;

	case CManager::MODE_TUTORIAL:
		pPlayer = CTutorial::GetPlayer();
		break;
	}



	if (mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
	{
		D3DXVECTOR3 playerpos = pPlayer->GetPos();
		D3DXVECTOR3 playerrot = pPlayer->GetRot();
		D3DXVECTOR3 playermove = pPlayer->GetMove();


		if (CScene::GetBossStop() == true)
		{//ボス以外が止まっているとき
		 //目的の角度設定
			if (pBoss != NULL)
			{
				D3DXVECTOR3 bosspos = pBoss->GetPos();
				D3DXVECTOR3 bossrot = pBoss->GetRot();
				D3DXVECTOR3 bossmove = pBoss->GetMove();

				if (bosspos.z < -400.0f)
				{
					if (bossrot.y > -1.6 && bossrot.y < 1.6)
					{
						m_fDestAngle = 2.0f;
					}
					else if (bossrot.y < -1.6 && bossrot.y > 1.6)
					{
						m_fDestAngle = -2.0f;
					}
				}

				//目的の角度まで
				m_fAngle = m_fDestAngle - m_rot.y;
				//角度調整
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//角度情報
				m_rot.y += m_fAngle * 0.035f;
				//目的の距離設定
				m_fdestLength = 80.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;

				//目的の高さ設定(V)
				m_fdestHeight_V = 70.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;

				//目的の高さ設定(R)
				m_fdestHeight_R = 80.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//目的の視点設定(V)
				m_posVDest.x = bosspos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = bosspos.z + cosf(m_rot.y) * -m_fLength;

				//目的の視点設定(R)
				m_posRDest.x = bosspos.x - sinf(bossrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = bosspos.z - cosf(bossrot.y) * 1.0f;

				//現在の視点の設定(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.1f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.1f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.1f;

				//現在の視点の設定(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;
			}
		}
		if (CScene::GetStop() == true && CScene::GetBossStop() == false)
		{//プレイヤー以外が止まっているとき


		 //目的の角度設定
			if (playerpos.z < -400.0f)
			{
				if (playerrot.y > -1.6 && playerrot.y < 1.6)
				{
					m_fDestAngle = 0.9f;
				}
				else if (playerrot.y < -1.6 && playerrot.y > 1.6)
				{
					m_fDestAngle = -0.9f;
				}
			}
			else
			{
				if (playerrot.y > 0)
				{
					m_fDestAngle = 0.5f;
				}
				else if (playerrot.y < 0)
				{
					m_fDestAngle = -0.5f;
				}
				if (playerrot.y == 0)
				{
					m_fDestAngle = 0.0f;
				}
			}
			//目的の角度まで
			m_fAngle = m_fDestAngle - m_rot.y;
			//角度調整
			if (m_fAngle > D3DX_PI)
			{
				m_fAngle -= D3DX_PI * 2;
			}

			if (m_fAngle < -D3DX_PI)
			{
				m_fAngle += D3DX_PI * 2;
			}
			//角度情報
			m_rot.y += m_fAngle * 0.035f;
			//目的の距離設定
			m_fdestLength = 80.0f;
			m_fLength += (m_fdestLength - m_fLength) * 0.1f;

			//目的の高さ設定(V)
			m_fdestHeight_V = 50.0f;
			m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;

			//目的の高さ設定(R)
			m_fdestHeight_R = 60.0f;
			m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

			//目的の視点設定(V)
			m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
			m_posVDest.y = m_fHeight_V;
			m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;

			//目的の視点設定(R)
			m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
			m_posRDest.y = m_fHeight_R;
			m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

			//現在の視点の設定(V)
			m_posV.x += (m_posVDest.x - m_posV.x) * 0.1f;
			m_posV.y += (m_posVDest.y - m_posV.y) * 0.1f;
			m_posV.z += (m_posVDest.z - m_posV.z) * 0.1f;

			//現在の視点の設定(R)
			m_posR.x += (m_posRDest.x - m_posR.x) * 0.1f;
			m_posR.y += (m_posRDest.y - m_posR.y) * 0.1f;
			m_posR.z += (m_posRDest.z - m_posR.z) * 0.1f;
		}
		else if(CScene::GetStop() == false && CScene::GetBossStop() == false)
		{//通常時
			if (playerpos.z > -35.0f)
			{//電車の中
			 //目的の角度設定
				m_fDestAngle = 0.0f;
				//目的の角度まで
				m_fAngle = m_fDestAngle - m_rot.y;
				//角度調整
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//角度情報
				m_rot.y += m_fAngle * 0.035f;
				//目的の距離設定
				m_fdestLength = 280.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;
				//目的の高さ設定(V)
				m_fdestHeight_V = 180.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
				//目的の高さ設定(R)
				m_fdestHeight_R = 100.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//目的の視点設定(V)
				m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = -m_fLength;
				//目的の視点設定(R)
				m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = 0.0f;

				//現在の視点の設定(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;

				//現在の視点の設定(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
			else if (playerpos.z < -400.0f)
			{//改札後の通路
			 //目的の角度設定
				m_fDestAngle = D3DX_PI * 0.5f;
				//目的の角度まで
				m_fAngle = m_fDestAngle - m_rot.y;
				//角度調整
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//角度情報
				m_rot.y += m_fAngle * 0.035f;
				//目的の距離設定
				m_fdestLength = 300.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.2f;
				//目的の高さ設定(V)
				m_fdestHeight_V = 220.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.2f;
				//目的の高さ設定(R)
				m_fdestHeight_R = 80.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.2f;


				//目的の視点設定(V)
				m_posVDest.x = 3850.0f + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;
				//目的の視点設定(R)
				m_posRDest.x = 3850.0f - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

				//現在の視点の設定(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
				//現在の視点の設定(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
			else
			{//それ以外のところ
			 //目的の角度設定
				m_fDestAngle = 0.0f;
				//目的の角度まで
				m_fAngle = m_fDestAngle - m_rot.y;
				//角度調整
				if (m_fAngle > D3DX_PI)
				{
					m_fAngle -= D3DX_PI * 2;
				}

				if (m_fAngle < -D3DX_PI)
				{
					m_fAngle += D3DX_PI * 2;
				}
				//角度情報
				m_rot.y += m_fAngle * 0.035f;
				//目的の距離設定
				m_fdestLength = 580.0f;
				m_fLength += (m_fdestLength - m_fLength) * 0.1f;
				//目的の高さ設定(V)
				m_fdestHeight_V = 200.0f;
				m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
				//目的の高さ設定(R)
				m_fdestHeight_R = -10.0f;
				m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

				//目的の視点設定(V)
				m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
				m_posVDest.y = m_fHeight_V;
				m_posVDest.z = cosf(m_rot.y) * -m_fLength;
				//目的の視点設定(R)
				m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
				m_posRDest.y = m_fHeight_R;
				m_posRDest.z = cosf(playerrot.y) * 1.0f;

				//現在の視点の設定(V)
				m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
				m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
				m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
				//現在の視点の設定(R)
				m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
				m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
				m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;
			}
		}
	}//ゲームモードのとき

	if (mode == CManager::MODE_RANKING || mode == CManager::MODE_TITLE)
	{

		//角度調整
		if (m_fAngle > D3DX_PI)
		{
			m_fAngle -= D3DX_PI * 2;
		}

		if (m_fAngle < -D3DX_PI)
		{
			m_fAngle += D3DX_PI * 2;
		}
		//角度情報
		m_rot.y += 0.0f;

		//目的の距離設定
		m_fdestLength = 663.0f;
		m_fLength += (m_fdestLength - m_fLength) * 0.1f;
		//目的の高さ設定(V)
		m_fdestHeight_V = 240.0f;
		m_fHeight_V += (m_fdestHeight_V - m_fHeight_V) * 0.1f;
		//目的の高さ設定(R)
		m_fdestHeight_R = 0.0f;
		m_fHeight_R += (m_fdestHeight_R - m_fHeight_R) * 0.1f;

		//目的の視点設定(V)
		m_posVDest.x = 0.0f;
		m_posVDest.y = m_fHeight_V;
		m_posVDest.z = -m_fLength;
		//目的の視点設定(R)
		m_posRDest.x = 0.0f;
		m_posRDest.y = m_fHeight_R;
		m_posRDest.z = 0.0f;

		//現在の視点の設定(V)
		m_posV.x += (m_posVDest.x - m_posV.x) * 0.15f;
		m_posV.y += (m_posVDest.y - m_posV.y) * 0.15f;
		m_posV.z += (m_posVDest.z - m_posV.z) * 0.15f;
		//現在の視点の設定(R)
		m_posR.x += (m_posRDest.x - m_posR.x) * 0.15f;
		m_posR.y += (m_posRDest.y - m_posR.y) * 0.15f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.15f;



	}

#ifdef _DEBUG

	////カメラの位置情報
	//CDebugProc::Print("PosV(X,Y,Z):%.1f, %.1f, %.1f\n", m_posV.x, m_posV.y, m_posV.z);
	//CDebugProc::Print("Rot(X,Y,Z):%.1f, %.2f, %.1f\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("PosR(X,Y,Z):%.1f, %.1f, %.1f\n", m_posR.x, m_posR.y, m_posR.z);
#endif


	OverView();
#endif

}

//=============================================================================
// 関数名：カメラの設定処理
// 関数の概要：カメラを設置する
//=============================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		900.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,//ビューマトリックス
		&m_posV,//視点
		&m_posR,//注視点
		&m_vecU);//上方向ベクトル

				 // ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// 関数名：カメラの調整
// 関数の概要：カメラの角度が一周過ぎたときの調整
//=============================================================================
void CCamera::OverView()
{
	if (m_rot.y > D3DX_PI)
	{

		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{

		m_rot.y += D3DX_PI * 2;
	}


	if (m_rot.z > D3DX_PI)
	{

		m_rot.z -= D3DX_PI * 2;
	}

	if (m_rot.z < -D3DX_PI)
	{

		m_rot.z += D3DX_PI * 2;
	}

	if (m_rot.x > D3DX_PI)
	{

		m_rot.x -= D3DX_PI * 2;
	}

	if (m_rot.x < -D3DX_PI)
	{

		m_rot.x += D3DX_PI * 2;
	}


}

//=============================================================================
// 関数名：カメラのマトリックスの取得
// 関数の概要：カメラのマトリックスを返す
//=============================================================================
D3DXMATRIX CCamera::GetmtxView(void)
{
	return m_mtxView;
}

D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

void CCamera::SetFree(void)
{
	m_bFree = m_bFree ? false : true;

}

