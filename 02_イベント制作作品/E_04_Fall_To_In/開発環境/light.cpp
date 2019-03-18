//=============================================================================
//
// ライト処理 [light.cpp]
// Author : RyotaKinoshita
//
//=============================================================================
#include "light.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "DebugProc.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{

}
//=============================================================================
// デスストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得
	D3DXVECTOR3 vecDir;											// ライトの角度を入れる変数

// -----一つ目-----------------------
	// ライトをクリアする
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));

	// ライトの種類を設定
	m_aLight[0].m_Light.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_aLight[0].m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの色を設定
	// ライトの周囲色を設定
	m_aLight[0].m_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
	// ライトの鏡面色を設定
	m_aLight[0].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定


	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.8f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	m_aLight[0].m_Light.Direction = vecDir;								// 正規化された方向を代入する

	// ライトを設定する
	pDevice->SetLight(0, &m_aLight[0].m_Light);
	// ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	// 使用しているかどうかを設定する
	m_aLight[0].m_bLight = TRUE;											// オン
	m_aLight[0].m_bUse = TRUE;												// 使用している状態にする

// ----二つ目------------------------
	// ライトをクリアする
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));

	// ライトの種類を設定
	m_aLight[1].m_Light.Type = D3DLIGHT_DIRECTIONAL;
	// ライトの色を設定
	m_aLight[1].m_Light.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ライトの拡散光を設定
	m_aLight[1].m_Light.Ambient  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ライトの周囲光を設定
	m_aLight[1].m_Light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ライトの反射光を設定
	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.88f, 0.88f, -0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	m_aLight[1].m_Light.Direction = vecDir;								// 正規化された方向を代入する
	// ライトを設定する
	pDevice->SetLight(1, &m_aLight[1].m_Light);
	// ライトを有効にする
	pDevice->LightEnable(1, TRUE);
	// 使用しているかどうかを設定する
	m_aLight[1].m_bLight = TRUE;											// オン
	m_aLight[1].m_bUse = TRUE;											// 使用している状態にする
// ----三つ目------------------------
	// ライトをクリアする
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	// ライトの種類を設定
	m_aLight[2].m_Light.Type = D3DLIGHT_DIRECTIONAL;
	// ライトの拡散光を設定
	m_aLight[2].m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの色を設定
	// ライトの範囲を設定
//	m_aLight[2].Range = 70.75f;
	// ライトの位置を設定
	m_aLight[2].m_Light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.89f, -0.1f, 0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	m_aLight[2].m_Light.Direction = vecDir;								// 正規化された方向を代入する
	// ライトを設定する
	pDevice->SetLight(2, &m_aLight[2].m_Light);
	// ライトを有効にする
	pDevice->LightEnable(2, TRUE);
	// 使用しているかどうかを設定する
	m_aLight[2].m_bLight = TRUE;											// オン
	m_aLight[2].m_bUse = TRUE;												// 使用している状態にする
	return S_OK;
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
	CDebugProc::print("ライトの方向：(%.1f,%.1f,%.1f)\n", m_aLight[0].m_Light.Direction.x, m_aLight[0].m_Light.Direction.y, m_aLight[0].m_Light.Direction.z);
}

//=============================================================================
// ポイントライトの設定
//=============================================================================
int CLight::SetPoint(D3DXVECTOR3 pos, D3DXCOLOR Diffuse,float Attenuation,float Range)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得
	int nCntLight = 0;							// ライトのカウント変数

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの最大数分ループ
		if (m_aLight[nCntLight].m_bUse == false)
		{// ライトが使われていない状態なら
			// ライトをクリアする
			ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

			// ライトの種類を設定
			m_aLight[nCntLight].m_Light.Type = D3DLIGHT_POINT;

			// ライトの位置の設定
			m_aLight[nCntLight].m_Light.Position = pos;									// ライトの位置を設定

			// ライトの色を設定
			m_aLight[nCntLight].m_Light.Diffuse  = Diffuse;								// ライトの拡散光を設定
			m_aLight[nCntLight].m_Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
			m_aLight[nCntLight].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定

			// 減衰の設定
			m_aLight[nCntLight].m_Light.Attenuation0 = Attenuation;						// 線形減衰係数(基本的にここで変わる)
			m_aLight[nCntLight].m_Light.Attenuation1 = 0.0f;								// 2次減衰係数(基本的に0.0fで大丈夫)
			m_aLight[nCntLight].m_Light.Attenuation2 = 0.0f;								// 指数減衰係数(基本的に0.0fで大丈夫)

			// 光源の範囲の設定
			m_aLight[nCntLight].m_Light.Range = Range;										// 範囲

			// ライトを設定する
			pDevice->SetLight(nCntLight, &m_aLight[nCntLight].m_Light);

			// ライトを有効にする
			pDevice->LightEnable(nCntLight, TRUE);

			m_aLight[nCntLight].m_bLight = true;											// ライトをオンにする
			m_aLight[nCntLight].m_bUse	  = true;											// ライトを使用状態にする

			break;
		}
	}
	return nCntLight;																	// 作った番号を送る
}

//=============================================================================
// スポットライトの設定
//=============================================================================
int CLight::SetSpot(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得
	D3DXVECTOR3 vecDir;							// ライトの角度を入れる変数
	int nCntLight = 0;							// ライトのカウント変数

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの最大数分ループ
		if (m_aLight[nCntLight].m_bUse == false)
		{// ライトが使われていない状態なら
		 // ライトをクリアする
			ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

			// ライトの種類を設定
			m_aLight[nCntLight].m_Light.Type = D3DLIGHT_SPOT;

			// ライトの位置の設定
			m_aLight[nCntLight].m_Light.Position = pos;									// ライトの位置を設定

			// ライトの色を設定
			m_aLight[nCntLight].m_Light.Diffuse  = Diffuse;								// ライトの拡散光を設定
			m_aLight[nCntLight].m_Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
			m_aLight[nCntLight].m_Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定

			// 減衰の設定
			m_aLight[nCntLight].m_Light.Attenuation0 = 0.1f;								// 線形減衰係数(基本的に0.1fぐらいで大丈夫)
			m_aLight[nCntLight].m_Light.Attenuation1 = 0.0f;								// 2次減衰係数(基本的に0.0fで大丈夫)
			m_aLight[nCntLight].m_Light.Attenuation2 = 0.0f;								// 指数減衰係数(基本的に0.0fで大丈夫)

			// 光源の範囲の設定
			m_aLight[nCntLight].m_Light.Range = 220.0f;										// 範囲

			// スポットライトのフォールオフの設定
			m_aLight[nCntLight].m_Light.Falloff = 1.0f;

			// スポットライトの内部コーンの角度の設定
			m_aLight[nCntLight].m_Light.Theta = 0.5f;

			// スポットライトの外部コーンの角度の設定
			m_aLight[nCntLight].m_Light.Phi = D3DX_PI / 2;


			// ライトの方向の設定
			vecDir = rot;														// ライトの角度を代入
			D3DXVec3Normalize(&vecDir, &vecDir);								// ライトの角度を正規化する
			m_aLight[nCntLight].m_Light.Direction = vecDir;								// 正規化された方向で角度を設定する

			// ライトを設定する
			pDevice->SetLight(nCntLight, &m_aLight[nCntLight].m_Light);

			// ライトを有効にする
			pDevice->LightEnable(nCntLight, TRUE);

			m_aLight[nCntLight].m_bLight = true;											// ライトをオンにする
			m_aLight[nCntLight].m_bUse = true;												// ライトを使用状態にする

			break;
		}
	}
	return nCntLight;																	// 作った番号を送る
}

//=============================================================================
// ライトの位置更新
//=============================================================================
void CLight::SetPosition(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	if (nIdxLight != -1)
	{// -1なら処理しない
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得
		D3DXVECTOR3 vecDir;											// ライトの角度を入れる変数

		// ライトの位置を設定
		m_aLight[nIdxLight].m_Light.Position = pos;					// ライトの位置を設定

		// ライトの方向の設定
		vecDir = rot;												// ライトの角度を入れる
		D3DXVec3Normalize(&vecDir, &vecDir);						// 正規化する
		m_aLight[nIdxLight].m_Light.Direction = vecDir;				// 正規化された方向を代入する

		// ライトを設定する
		pDevice->SetLight(nIdxLight, &m_aLight[nIdxLight].m_Light);

	}
}

//=============================================================================
// ライトのオンオフ切り替え
//=============================================================================
void CLight::Switching(int nIdxLight)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();					// デバイスの取得

	// ライトのオンオフを切り替え
	m_aLight[nIdxLight].m_bLight = m_aLight[nIdxLight].m_bLight ? false : true;

	// ライトを有効にする
	pDevice->LightEnable(nIdxLight, m_aLight[nIdxLight].m_bLight);

}