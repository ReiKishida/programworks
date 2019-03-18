//=============================================================================
//
// ブーストゲージ処理 [boost.cpp]
// Author :Kishida Rei
//
//=============================================================================
#include "boost.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "inputx.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOOST_VERTEX ((g_Boost_h + 1) * (g_Boost_v + 1))	//ブーストゲージ頂点数
#define BOOST_INDEX ( (g_Boost_h * g_Boost_v )* 2 + (4 * (g_Boost_v - 1)) + 2 )	//ブーストゲージのインデックス数
#define BOOST_POLYGON ( (g_Boost_h * g_Boost_v) * 2 + (4 * (g_Boost_v - 1)) )//ブーストゲージのポリゴン数

#define BOOST_RADIUS (45.0f)	//半径の大きさ
#define BOOST_RADIUS2 (7.0f)	//半径の内側


#define BOOST_HEIGHT (5.0f)	//ブーストゲージの高さ

#define BOOST_H (100) //メッシュの分割数(H)
#define BOOST_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBoost(LPDIRECT3DDEVICE9 pDevice);	//頂点情報生成
void MakeIndexBoost(LPDIRECT3DDEVICE9 pDevice);		//インデックス情報生成

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBoost = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoost = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBoost = NULL; //インデックスバッファへのポインタ

D3DXVECTOR3 g_posBoost;		//位置(中心座標)
D3DXVECTOR3 g_rotBoost;		//向き
D3DXMATRIX g_mtxWorldBoost;	//ワールドマトリックス


int g_nBoost;	//ブースト量

int g_Boost_h, g_Boost_v;	//横の分割数,縦の分割数
JoyState g_JoyStateBoost;	//ゲームパッドの状態(ブーストゲージ)

//=============================================================================
// 初期化処理
//=============================================================================
void InitBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Boost_h = BOOST_H;	//横の分割数
	g_Boost_v = BOOST_V;	//縦の分割数

	int Polygon = (g_Boost_h * g_Boost_v) * 2 + (4 * (g_Boost_v - 1));	//ポリゴン数の計算
	g_nBoost = -200;//ブースト残量

	MakeVertexBoost(pDevice);	//頂点情報生成
	MakeIndexBoost(pDevice);	//インデックス情報生成

	//プレイヤーの情報の取得
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//ブーストの位置情報
	g_posBoost = pPlayer.pos;
	g_rotBoost = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	//ゲームパッドの状態
	g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NOTPUSH;



}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoost(void)
{
	//テクスチャの破棄
	if (g_pTextureBoost != NULL)
	{
		g_pTextureBoost->Release();
		g_pTextureBoost = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBoost != NULL)
	{
		g_pVtxBuffBoost->Release();
		g_pVtxBuffBoost = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffBoost != NULL)
	{
		g_pIdxBuffBoost->Release();
		g_pIdxBuffBoost = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoost(void)
{
	//ゲームパッド状態取得
	JoyState pJoyState = GetJoystate();

	//入力状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//プレイヤーの情報の取得
	PLAYER pPlayer;
	pPlayer = *GetPlayer();

	//カメラの情報の取得
	CAMERA pCamera;
	pCamera = *GetCamera();

	//ブーストゲージの位置と角度
	g_posBoost = D3DXVECTOR3(pPlayer.pos.x, pPlayer.pos.y, pPlayer.pos.z);
	g_rotBoost = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f + pPlayer.rot.y, 0.0f);

	if (GetKeyboardPress(DIK_LSHIFT) == true || (state.Gamepad.wButtons &XINPUT_GAMEPAD_LEFT_SHOULDER && pJoyState.bConnectionJoypad == true))
	{//左側のシフトキーが押されたとき、ゲームパッドの左ショルダーが押し込まれてているとき
		if (pPlayer.state == PLAYERSTATE_NORMAL)
		{//プレイヤーの状態が通常状態のとき
			g_nBoost -= 2;
		}
	}

	if (GetKeyboardTrigger(DIK_RSHIFT) == true   || (state.Gamepad.bRightTrigger == 255 && pJoyState.bConnectionJoypad == true) )
	{//右側のシフトキーが押されたとき、ゲームパッドの右ショルダーが押し込まれてているとき
		if (pPlayer.state == PLAYERSTATE_NORMAL)
		{//プレイヤーの状態が通常状態のとき
			if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				g_nBoost -= 60;
			}
			g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
	}
	if (g_nBoost > 0)
	{//ブーストゲージがゼロのときはゼロにする
		g_nBoost =  0;
	}


	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);
	if (pPlayer.state == PLAYERSTATE_NORMAL)
	{//プレイヤーの状態が通常状態のとき
		for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
		{
			for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
			{
				if (nCntBOOST_V == 0)
				{
					//頂点カラー(外側)
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
				if (nCntBOOST_V == 1)
				{
					//頂点カラー(内側)
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
	if (g_nBoost <= -100 || pPlayer.state == PLAYERSTATE_OVERHEAT)
	{//ブースト残量が少なくなってきたとき
		for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
		{
			for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
			{
				if (nCntBOOST_V == 0)
				{//垂直のカウント０のとき
					//頂点カラー
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				if (nCntBOOST_V == 1)
				{//垂直のカウント１のとき
					//頂点カラー
					pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffBoost->Unlock();

	//ブーストゲージを回復
	g_nBoost++;

	//ゲームパッドの押しっぱなしを回避
	if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateBoost.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateBoost.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoost(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;


	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBoost);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		g_rotBoost.y, g_rotBoost.x, g_rotBoost.z);

	D3DXMatrixMultiply(&g_mtxWorldBoost, &g_mtxWorldBoost, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		g_posBoost.x, g_posBoost.y, g_posBoost.z);

	D3DXMatrixMultiply(&g_mtxWorldBoost, &g_mtxWorldBoost, &mtxtrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBoost);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoost, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBoost);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, 0);


	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, BOOST_VERTEX, 0, BOOST_POLYGON + g_nBoost);



	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}


//=============================================================================
//ブーストゲージ減少
//=============================================================================
void UseBoost(int nDamage)
{
	g_nBoost += nDamage;
}

//=============================================================================
//ブーストゲージ取得
//=============================================================================

int GetBoost(void)
{
	return g_nBoost;
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBoost(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureBoost);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BOOST_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoost,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBoost->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI / g_Boost_h;
	float fDestAngle;
	for (int nCntBOOST_V = 0; nCntBOOST_V < g_Boost_v + 1; nCntBOOST_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntBOOST_H = 0; nCntBOOST_H < g_Boost_h + 1; nCntBOOST_H++)
		{
			if (nCntBOOST_V == 0)
			{//外側の生成
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.x = sinf(fDestAngle) * BOOST_RADIUS;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.y = 1.0f;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.z = cosf(fDestAngle) * BOOST_RADIUS;
				//法線の設定
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


				//頂点カラー
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].tex = D3DXVECTOR2(nCntBOOST_H * (1.0f / (g_Boost_h / 2)), nCntBOOST_V * -1.0f);

				fDestAngle += fAngle;
			}
			if (nCntBOOST_V == 1)
			{//内側の生成
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.x = sinf(fDestAngle) * (BOOST_RADIUS - BOOST_RADIUS2);
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.y = 1.0f;
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].pos.z = cosf(fDestAngle) * (BOOST_RADIUS - BOOST_RADIUS2);
				//法線の設定
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

				//頂点カラー
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標
				pVtx[nCntBOOST_V * (g_Boost_h + 1) + nCntBOOST_H].tex = D3DXVECTOR2(nCntBOOST_H * (1.0f / (g_Boost_h / 2)), nCntBOOST_V * -1.0f);

				fDestAngle += fAngle;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffBoost->Unlock();



}

//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexBoost(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * BOOST_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBoost,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffBoost->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Boost_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Boost_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Boost_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Boost_h && nCntIndex_V <= g_Boost_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Boost_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffBoost->Unlock();

}




