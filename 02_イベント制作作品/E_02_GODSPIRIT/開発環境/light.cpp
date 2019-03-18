//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 佐藤亮太
//
//=============================================================================
#include "light.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT			(10)		// ライトの最大数
#define LIGHT_MOVE_SPEED	(0.05f)		// ライトの移動速度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Light g_Light[MAX_LIGHT];				// ライトの情報

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();					// デバイスの取得
	D3DXVECTOR3 vecDir;											// ライトの角度を入れる変数

// -----一つ目-----------------------
	// ライトをクリアする
	ZeroMemory(&g_Light[0], sizeof(D3DLIGHT9));

	// ライトの種類を設定
	g_Light[0].Light.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_Light[0].Light.Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.5f);		// ライトの色を設定
	// ライトの周囲色を設定
	g_Light[0].Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
	// ライトの鏡面色を設定
	g_Light[0].Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.72f, -0.8f, 0.54f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	g_Light[0].Light.Direction = vecDir;								// 正規化された方向を代入する

	// ライトを設定する
	pDevice->SetLight(0, &g_Light[0].Light);

	// ライトを有効にする
	pDevice->LightEnable(0, TRUE);

	// 使用しているかどうかを設定する
	g_Light[0].bLight = TRUE;											// オン
	g_Light[0].bUse = TRUE;												// 使用している状態にする


// ----二つ目------------------------
	// ライトをクリアする
	ZeroMemory(&g_Light[1], sizeof(D3DLIGHT9));

	// ライトの種類を設定
	g_Light[1].Light.Type = D3DLIGHT_DIRECTIONAL;
	// ライトの色を設定
	g_Light[1].Light.Diffuse  = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.5f);		// ライトの拡散光を設定
	g_Light[1].Light.Ambient  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ライトの周囲光を設定
	g_Light[1].Light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);		// ライトの反射光を設定

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.88f, 0.88f, -0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	g_Light[1].Light.Direction = vecDir;								// 正規化された方向を代入する

	// ライトを設定する
	pDevice->SetLight(1, &g_Light[1].Light);

	// ライトを有効にする
	pDevice->LightEnable(1, TRUE);

	// 使用しているかどうかを設定する
	g_Light[1].bLight = TRUE;											// オン
	g_Light[1].bUse = TRUE;											// 使用している状態にする


// ----三つ目------------------------
	// ライトをクリアする
	ZeroMemory(&g_Light[2], sizeof(D3DLIGHT9));

	// ライトの種類を設定
	g_Light[2].Light.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_Light[2].Light.Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.5f);		// ライトの色を設定

	// ライトの範囲を設定
//	g_Light[2].Range = 70.75f;

	// ライトの位置を設定
	g_Light[2].Light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.89f, -0.1f, 0.94f);
	D3DXVec3Normalize(&vecDir, &vecDir);								// 正規化する
	g_Light[2].Light.Direction = vecDir;								// 正規化された方向を代入する

	// ライトを設定する
	pDevice->SetLight(2, &g_Light[2].Light);

	// ライトを有効にする
	pDevice->LightEnable(2, TRUE);

	// 使用しているかどうかを設定する
	g_Light[2].bLight = TRUE;											// オン
	g_Light[2].bUse = TRUE;												// 使用している状態にする
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

//=============================================================================
// ポイントライトの設定
//=============================================================================
int SetPointLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse,float Attenuation,float Range)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntLight = 0;							// ライトのカウント変数

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの最大数分ループ
		if (g_Light[nCntLight].bUse == false)
		{// ライトが使われていない状態なら
			// ライトをクリアする
			ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

			// ライトの種類を設定
			g_Light[nCntLight].Light.Type = D3DLIGHT_POINT;

			// ライトの位置の設定
			g_Light[nCntLight].Light.Position = pos;									// ライトの位置を設定

			// ライトの色を設定
			g_Light[nCntLight].Light.Diffuse  = Diffuse;								// ライトの拡散光を設定
			g_Light[nCntLight].Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
			g_Light[nCntLight].Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定

			// 減衰の設定
			g_Light[nCntLight].Light.Attenuation0 = Attenuation;						// 線形減衰係数(基本的にここで変わる)
			g_Light[nCntLight].Light.Attenuation1 = 0.0f;								// 2次減衰係数(基本的に0.0fで大丈夫)
			g_Light[nCntLight].Light.Attenuation2 = 0.0f;								// 指数減衰係数(基本的に0.0fで大丈夫)

			// 光源の範囲の設定
			g_Light[nCntLight].Light.Range = Range;										// 範囲

			// ライトを設定する
			pDevice->SetLight(nCntLight, &g_Light[nCntLight].Light);

			// ライトを有効にする
			pDevice->LightEnable(nCntLight, TRUE);

			g_Light[nCntLight].bLight = true;											// ライトをオンにする
			g_Light[nCntLight].bUse	  = true;											// ライトを使用状態にする

			break;
		}
	}
	return nCntLight;																	// 作った番号を送る
}

//=============================================================================
// スポットライトの設定
//=============================================================================
int SetSpotLight(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR Diffuse)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;							// ライトの角度を入れる変数
	int nCntLight = 0;							// ライトのカウント変数

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの最大数分ループ
		if (g_Light[nCntLight].bUse == false)
		{// ライトが使われていない状態なら
		 // ライトをクリアする
			ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

			// ライトの種類を設定
			g_Light[nCntLight].Light.Type = D3DLIGHT_SPOT;

			// ライトの位置の設定
			g_Light[nCntLight].Light.Position = pos;									// ライトの位置を設定

			// ライトの色を設定
			g_Light[nCntLight].Light.Diffuse  = Diffuse;								// ライトの拡散光を設定
			g_Light[nCntLight].Light.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの周囲色を設定
			g_Light[nCntLight].Light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ライトの鏡面色を設定

			// 減衰の設定
			g_Light[nCntLight].Light.Attenuation0 = 0.1f;								// 線形減衰係数(基本的に0.1fぐらいで大丈夫)
			g_Light[nCntLight].Light.Attenuation1 = 0.0f;								// 2次減衰係数(基本的に0.0fで大丈夫)
			g_Light[nCntLight].Light.Attenuation2 = 0.0f;								// 指数減衰係数(基本的に0.0fで大丈夫)

			// 光源の範囲の設定
			g_Light[nCntLight].Light.Range = 220.0f;										// 範囲

			// スポットライトのフォールオフの設定
			g_Light[nCntLight].Light.Falloff = 1.0f;

			// スポットライトの内部コーンの角度の設定
			g_Light[nCntLight].Light.Theta = 0.5f;

			// スポットライトの外部コーンの角度の設定
			g_Light[nCntLight].Light.Phi = D3DX_PI / 2;


			// ライトの方向の設定
			vecDir = rot;														// ライトの角度を代入
			D3DXVec3Normalize(&vecDir, &vecDir);								// ライトの角度を正規化する
			g_Light[nCntLight].Light.Direction = vecDir;								// 正規化された方向で角度を設定する

			// ライトを設定する
			pDevice->SetLight(nCntLight, &g_Light[nCntLight].Light);

			// ライトを有効にする
			pDevice->LightEnable(nCntLight, TRUE);

			g_Light[nCntLight].bLight = true;											// ライトをオンにする
			g_Light[nCntLight].bUse = true;												// ライトを使用状態にする

			break;
		}
	}
	return nCntLight;																	// 作った番号を送る
}

//=============================================================================
// ライトの位置更新
//=============================================================================
void SetPositionLight(int nIdxLight, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	if (nIdxLight != -1)
	{// -1なら処理しない
		LPDIRECT3DDEVICE9 pDevice = GetDevice();					// デバイスの取得
		D3DXVECTOR3 vecDir;											// ライトの角度を入れる変数

		// ライトの位置を設定
		g_Light[nIdxLight].Light.Position = pos;					// ライトの位置を設定

		// ライトの方向の設定
		vecDir = rot;												// ライトの角度を入れる
		D3DXVec3Normalize(&vecDir, &vecDir);						// 正規化する
		g_Light[nIdxLight].Light.Direction = vecDir;				// 正規化された方向を代入する

		// ライトを設定する
		pDevice->SetLight(nIdxLight, &g_Light[nIdxLight].Light);

	}
}

//=============================================================================
// ライトのオンオフ切り替え
//=============================================================================
void SwitchingLight(int nIdxLight)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ライトのオンオフを切り替え
	g_Light[nIdxLight].bLight = g_Light[nIdxLight].bLight ? false : true;

	// ライトを有効にする
	pDevice->LightEnable(nIdxLight, g_Light[nIdxLight].bLight);

}