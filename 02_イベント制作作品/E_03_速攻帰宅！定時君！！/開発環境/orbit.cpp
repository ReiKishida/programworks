////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 軌跡の処理 [orbit.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "orbit.h"
#include "player.h"
#include "gamepad.h"
#include "input.h"
#include "game.h"
#include "player.h"

//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	char *pFileName;
}OrbitTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureOrbit[ORBITTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff= NULL;	// 頂点バッファへのポインタ

Orbit g_aOrbit[MAX_ORBIT];
OrbitTexture g_aOrbitTexInfo[ORBITTYPE_MAX] =
{
	{ "data/TEXTURE/orbit000.jpg" },
	{ "data/TEXTURE/orbit001.jpg" },
	{ "data/TEXTURE/orbit002.jpg" },
	{ "data/TEXTURE/orbit003.jpg" },
};
int g_nCntOrbit;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntOrbitTex = 0; nCntOrbitTex < ORBITTYPE_MAX; nCntOrbitTex++)
	{// テクスチャの設定
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			g_aOrbitTexInfo[nCntOrbitTex].pFileName,	// ファイルの名前
			&g_apTextureOrbit[nCntOrbitTex]);			// テクスチャへのポインタ
	}

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aOrbit[nCntOrbit].nType = ORBITTYPE_0;					// 種類
		g_aOrbit[nCntOrbit].nMotionType = PLAYER_MOTION_TYPE_MAX;
		g_aOrbit[nCntOrbit].Start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 始点
		g_aOrbit[nCntOrbit].End = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 長さ
		g_aOrbit[nCntOrbit].InitStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 始点
		g_aOrbit[nCntOrbit].InitEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 長さ
		g_aOrbit[nCntOrbit].nLength = MAX_ORBIT_LENGTH;				// 長さ
		g_aOrbit[nCntOrbit].nCntTrueTime = 0;						// 消えるまでの時間
		g_aOrbit[nCntOrbit].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_aOrbit[nCntOrbit].bUse = false;							// 使用されているかどうか
	}
	MakeVertexOrbit(pDevice);

}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitOrbit(void)
{
	// テクスチャの破棄
	for (int nCntOrbit = 0; nCntOrbit < ORBITTYPE_MAX; nCntOrbit++)
	{
		if (g_apTextureOrbit[nCntOrbit] != NULL)
		{
			g_apTextureOrbit[nCntOrbit]->Release();
			g_apTextureOrbit[nCntOrbit] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}

}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++,pVtx+=4)
	{
		if (g_aOrbit[nCntOrbit].bUse)
		{
			g_aOrbit[nCntOrbit].col.a -= 0.03f;
			// 頂点カラーの設定
			pVtx[0].col = g_aOrbit[nCntOrbit].col;
			pVtx[1].col = g_aOrbit[nCntOrbit].col;
			pVtx[2].col = g_aOrbit[nCntOrbit].col;
			pVtx[3].col = g_aOrbit[nCntOrbit].col;
			if (g_aOrbit[nCntOrbit].col.a <= 0)
			{
				g_aOrbit[nCntOrbit].bUse = false;
			}
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuff->Unlock();

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxPlayerWorld;
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);					// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ライト影響受けない
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		mtxPlayerWorld = g_aOrbit[nCntOrbit].playerWorld;

		if (g_aOrbit[nCntOrbit].bUse == true)
		{
			// 前回の位置を獲得
			g_aOrbit[nCntOrbit].mtxStartOld = g_aOrbit[nCntOrbit].mtxStart;
			g_aOrbit[nCntOrbit].mtxEndOld = g_aOrbit[nCntOrbit].mtxEnd;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxStart);
			D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxStart, g_aOrbit[nCntOrbit].Start.x, g_aOrbit[nCntOrbit].Start.y, g_aOrbit[nCntOrbit].Start.z);	// スタート
			D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxStart, &g_aOrbit[nCntOrbit].mtxStart, &mtxPlayerWorld);	// [始点*プレイヤー]のマトリックス計算
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxStart);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxEnd);
			D3DXMatrixTranslation(&g_aOrbit[nCntOrbit].mtxEnd, g_aOrbit[nCntOrbit].End.x, g_aOrbit[nCntOrbit].End.y, g_aOrbit[nCntOrbit].End.z);	// エンド
			D3DXMatrixMultiply(&g_aOrbit[nCntOrbit].mtxEnd, &g_aOrbit[nCntOrbit].mtxEnd, &mtxPlayerWorld);		// [終点*プレイヤー]のマトリックス計算
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxEnd);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCntOrbit].mtxWorld);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCntOrbit].mtxWorld);


			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_3D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureOrbit[g_aOrbit[nCntOrbit].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOrbit * 4, 2);


		}
	}
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);					// Zバッファへの書き込み
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライト影響受けない
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

		// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ORBIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuff->Unlock();

}
//==============================================================================================================================
// 軌跡の設定
//==============================================================================================================================
void SetOrbit(D3DXMATRIX Idxmtx, D3DXMATRIX IdxmtxOld, int nLength, D3DXVECTOR3 Start, D3DXVECTOR3 End, ORBITTYPE nType, PlayerMotionType nMotionType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < MAX_ORBIT; nCnt++,pVtx+=4)
	{
		if (g_aOrbit[nCnt].bUse == false)
		{
			g_aOrbit[nCnt].playerWorld = Idxmtx;
			g_aOrbit[nCnt].playerWorldOld = IdxmtxOld;
			g_aOrbit[nCnt].Start = Start;
			g_aOrbit[nCnt].End = End;
			g_aOrbit[nCnt].InitStart = Start;
			g_aOrbit[nCnt].InitEnd = End;
			g_aOrbit[nCnt].nType = nType;
			g_aOrbit[nCnt].nMotionType = nMotionType;
			g_aOrbit[nCnt].nLength = nLength;
			g_aOrbit[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			if (g_aOrbit[nCnt].nLength == 0)
			{
				break;
			}
			D3DXMATRIX mtxPlayerWorld,mtxPlayerWorldOld;

			mtxPlayerWorld = g_aOrbit[nCnt].playerWorld;
			mtxPlayerWorldOld = g_aOrbit[nCnt].playerWorldOld;

			// 現在地
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCnt].mtxStart);
			D3DXMatrixTranslation(&g_aOrbit[nCnt].mtxStart, g_aOrbit[nCnt].Start.x, g_aOrbit[nCnt].Start.y, g_aOrbit[nCnt].Start.z);	// スタート
			D3DXMatrixMultiply(&g_aOrbit[nCnt].mtxStart, &g_aOrbit[nCnt].mtxStart, &mtxPlayerWorld);	// [始点*プレイヤー]のマトリックス計算																											// ワールドマトリックスの設定
		//	pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCnt].mtxStart);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCnt].mtxEnd);
			D3DXMatrixTranslation(&g_aOrbit[nCnt].mtxEnd, g_aOrbit[nCnt].End.x, g_aOrbit[nCnt].End.y, g_aOrbit[nCnt].End.z);	// エンド
			D3DXMatrixMultiply(&g_aOrbit[nCnt].mtxEnd, &g_aOrbit[nCnt].mtxEnd, &mtxPlayerWorld);		// [終点*プレイヤー]のマトリックス計算																												// ワールドマトリックスの設定
	//		pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCnt].mtxEnd);

			// 過去
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCnt].mtxStartOld);
			D3DXMatrixTranslation(&g_aOrbit[nCnt].mtxStartOld, g_aOrbit[nCnt].Start.x, g_aOrbit[nCnt].Start.y, g_aOrbit[nCnt].Start.z);	// スタート
			D3DXMatrixMultiply(&g_aOrbit[nCnt].mtxStartOld, &g_aOrbit[nCnt].mtxStartOld, &mtxPlayerWorldOld);	// [始点*プレイヤー]のマトリックス計算																											// ワールドマトリックスの設定
		//	pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCnt].mtxStartOld);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aOrbit[nCnt].mtxEndOld);
			D3DXMatrixTranslation(&g_aOrbit[nCnt].mtxEndOld, g_aOrbit[nCnt].End.x, g_aOrbit[nCnt].End.y, g_aOrbit[nCnt].End.z);	// エンド
			D3DXMatrixMultiply(&g_aOrbit[nCnt].mtxEndOld, &g_aOrbit[nCnt].mtxEndOld, &mtxPlayerWorldOld);		// [終点*プレイヤー]のマトリックス計算																												// ワールドマトリックスの設定
		//	pDevice->SetTransform(D3DTS_WORLD, &g_aOrbit[nCnt].mtxEndOld);

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aOrbit[nCnt].mtxStart._41, g_aOrbit[nCnt].mtxStart._42, g_aOrbit[nCnt].mtxStart._43);
			pVtx[1].pos = D3DXVECTOR3(g_aOrbit[nCnt].mtxEnd._41, g_aOrbit[nCnt].mtxEnd._42, g_aOrbit[nCnt].mtxEnd._43);
			pVtx[2].pos = D3DXVECTOR3(g_aOrbit[nCnt].mtxStartOld._41, g_aOrbit[nCnt].mtxStartOld._42, g_aOrbit[nCnt].mtxStartOld._43);
			pVtx[3].pos = D3DXVECTOR3(g_aOrbit[nCnt].mtxEndOld._41, g_aOrbit[nCnt].mtxEndOld._42, g_aOrbit[nCnt].mtxEndOld._43);

			// テクスチャの設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


			g_aOrbit[nCnt].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuff->Unlock();

}
