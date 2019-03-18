////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 判定用ラインの処理 [libe.cpp]
// Author : Ryota Kinoshita
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "line.h"

//******************************************************************************************************************************
// マクロ定義
//******************************************************************************************************************************
#define MAX_LINE	(40000)
//******************************************************************************************************************************
// 構造体定義
//******************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 中心座標
	D3DXVECTOR3 rot;			// 向き
	LINETYPE nType;				// 種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
} LineInfo;
typedef struct
{
	char *pFileName;
}LineTexture;
//******************************************************************************************************************************
// プロトタイプ宣言
//******************************************************************************************************************************
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice);

//******************************************************************************************************************************
// グローバル変数
//******************************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_apTextureLine[LINETYPE_MAX] = {};	// テクスチャへのポインタ
Line g_aLine[MAX_LINE];
//BulletInfo g_aBulletInfo[] =
//{
//	{D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER, 30.0f, 30.0f},
//};
//LineTexture g_aLineTexInfo[LINETYPE_MAX] =
//{
//	{ "data/TEXTURE/bullet000.png" },
//};
int g_nIdyLine;
//==============================================================================================================================
// 初期化処理
//==============================================================================================================================
void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//for (int nCntLineTex = 0; nCntLineTex < LINETYPE_MAX; nCntLineTex++)
	//{// テクスチャの設定
	//	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
	//		g_aLineTexInfo[nCntLineTex].pFileName,		// ファイルの名前
	//		&g_apTextureLine[nCntLineTex]);				// テクスチャへのポインタ
	//}
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		g_aLine[nCntLine].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_aLine[nCntLine].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
		g_aLine[nCntLine].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最初の位置
		g_aLine[nCntLine].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最後の位置
		g_aLine[nCntLine].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);	// 色
		g_aLine[nCntLine].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 速さ
		g_aLine[nCntLine].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_aLine[nCntLine].nType = LINETYPE_MAX;						// 種類
		g_aLine[nCntLine].bUse = false;								// 使用されているかどうか
	}
	g_nIdyLine = 0;
	MakeVertexLine(pDevice);


}

//==============================================================================================================================
// 終了処理
//==============================================================================================================================
void UninitLine(void)
{
	// テクスチャの破棄
	for (int nCntLine = 0; nCntLine < LINETYPE_MAX; nCntLine++)
	{
		if (g_apTextureLine[nCntLine] != NULL)
		{
			g_apTextureLine[nCntLine]->Release();
			g_apTextureLine[nCntLine] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//==============================================================================================================================
// 更新処理
//==============================================================================================================================
void UpdateLine(void)
{

}

//==============================================================================================================================
// 描画処理
//==============================================================================================================================
void DrawLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビルボード用マトリックス

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		if (g_aLine[nCntLine].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLine[nCntLine].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLine[nCntLine].rot.y, g_aLine[nCntLine].rot.x, g_aLine[nCntLine].rot.z);
			D3DXMatrixMultiply(&g_aLine[nCntLine].mtxWorld, &g_aLine[nCntLine].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLine[nCntLine].pos.x, g_aLine[nCntLine].pos.y, g_aLine[nCntLine].pos.z);
			D3DXMatrixMultiply(&g_aLine[nCntLine].mtxWorld, &g_aLine[nCntLine].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLine[nCntLine].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, 0);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_LINELIST, nCntLine * 2, 1);
		}
	}
	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==============================================================================================================================
// 頂点情報の作成
//==============================================================================================================================
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL);

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++, pVtx += 2)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[0].col = g_aLine[nCntLine].col;
		pVtx[1].col = g_aLine[nCntLine].col;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();

}
//==============================================================================================================================
// ラインの設定
//==============================================================================================================================
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, D3DXCOLOR col, LINETYPE nType)
{
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++, pVtx += 2)
	{
		if (g_aLine[nCntLine].bUse == false)
		{
			g_aLine[nCntLine].pos = pos;
			g_aLine[nCntLine].posOld = pos;
			g_aLine[nCntLine].posStart = posStart;
			g_aLine[nCntLine].posEnd = posEnd;
			g_aLine[nCntLine].col = col;
			g_aLine[nCntLine].nType = nType;
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aLine[nCntLine].posStart.x, g_aLine[nCntLine].posStart.y, g_aLine[nCntLine].posStart.z);
			pVtx[1].pos = D3DXVECTOR3(g_aLine[nCntLine].posEnd.x, g_aLine[nCntLine].posEnd.y, g_aLine[nCntLine].posEnd.z);
			// 頂点カラーの設定
			pVtx[0].col = g_aLine[nCntLine].col;
			pVtx[1].col = g_aLine[nCntLine].col;

			g_aLine[nCntLine].bUse = true;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();
}
//==============================================================================================================================
// ラインの情報取得
//==============================================================================================================================
int CollisionLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, LINETYPE nType)
{

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nType);
	g_nIdyLine++;
	if (!g_aLine[(g_nIdyLine-1) * 12 + 0].bUse)
	{
		return 0;
	}
	return g_nIdyLine - 1;
}
void SetPositionLine(int nIdyLine, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_aLine[nIdyLine * 12 + 0].pos = pos;
	g_aLine[nIdyLine * 12 + 1].pos = pos;
	g_aLine[nIdyLine * 12 + 2].pos = pos;
	g_aLine[nIdyLine * 12 + 3].pos = pos;
	g_aLine[nIdyLine * 12 + 4].pos = pos;
	g_aLine[nIdyLine * 12 + 5].pos = pos;
	g_aLine[nIdyLine * 12 + 6].pos = pos;
	g_aLine[nIdyLine * 12 + 7].pos = pos;
	g_aLine[nIdyLine * 12 + 8].pos = pos;
	g_aLine[nIdyLine * 12 + 9].pos = pos;
	g_aLine[nIdyLine * 12 + 10].pos = pos;
	g_aLine[nIdyLine * 12 + 11].pos = pos;

	g_aLine[nIdyLine * 12 + 0].rot = rot;
	g_aLine[nIdyLine * 12 + 1].rot = rot;
	g_aLine[nIdyLine * 12 + 2].rot = rot;
	g_aLine[nIdyLine * 12 + 3].rot = rot;
	g_aLine[nIdyLine * 12 + 4].rot = rot;
	g_aLine[nIdyLine * 12 + 5].rot = rot;
	g_aLine[nIdyLine * 12 + 6].rot = rot;
	g_aLine[nIdyLine * 12 + 7].rot = rot;
	g_aLine[nIdyLine * 12 + 8].rot = rot;
	g_aLine[nIdyLine * 12 + 9].rot = rot;
	g_aLine[nIdyLine * 12 + 10].rot = rot;
	g_aLine[nIdyLine * 12 + 11].rot = rot;
}
void DeleteLine(LINETYPE nType)
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		if (g_aLine[nCntLine].nType == nType)
		{
			if (g_aLine[nCntLine].bUse == true)
			{
				g_aLine[nCntLine].bUse = false;
				if (nCntLine % 12 == 0)
				{
					g_nIdyLine--;
				}
			}
		}
	}


}