//=============================================================================
//
// センサー処理 [Sencer.cpp]
//　Coment: 魔法陣との当たり判定用
// Author : KishidaRei 
//
//=============================================================================
#include "sencer.h"
#include "input.h"
#include "camera.h"
#include "SencerAtk.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SENCER (50)
#define MAX_TEXTURE (2)
	
#define SENCER_VERTEX (  (g_Sencer_h + 1) * (g_Sencer_v + 1)   )	//頂点数
#define SENCER_INDEX (   (g_Sencer_h * g_Sencer_v )* 2 + (4 * (g_Sencer_v - 1)) + 2   ) //インデックス数
#define SENCER_POLYGON ( (g_Sencer_h * g_Sencer_v) * 2 + (4 * (g_Sencer_v - 1)) )	//ポリゴン数

#define SENCER_RADIUS (120.0f)
#define SENCER_HEIGHT (90.0f)

#define MESH_H (50) //メッシュの分割数(H)
#define MESH_V (1) //メッシュの分割数(V)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSencer(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSencer(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSencer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSencer = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSencer = NULL; //インデックスバッファへのポインタ

SENCER g_aSencer[MAX_SENCER];

int g_Sencer_h, g_Sencer_v;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSencer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_Sencer_h = MESH_H;
	g_Sencer_v = MESH_V;


	MakeVertexSencer(pDevice);
	MakeIndexSencer(pDevice);

	for (int nCntSencer = 0; nCntSencer < MAX_SENCER; nCntSencer++)
	{
		g_aSencer[nCntSencer].fRadius = SENCER_RADIUS;
		g_aSencer[nCntSencer].bUse = false;
	}




}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSencer(void)
{
	
		//テクスチャの破棄
		if (g_pTextureSencer != NULL)
		{
			g_pTextureSencer->Release();
			g_pTextureSencer = NULL;
		}
	
	// 頂点バッファの開放
	if (g_pVtxBuffSencer != NULL)
	{
		g_pVtxBuffSencer->Release();
		g_pVtxBuffSencer = NULL;
	}

	//インデックスバッファの解放
	if (g_pIdxBuffSencer != NULL)
	{
		g_pIdxBuffSencer->Release();
		g_pIdxBuffSencer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSencer(void)
{
	
	for (int nCntSencer = 0; nCntSencer < MAX_SENCER; nCntSencer++)
	{
		CollisionSencerAtk(&g_aSencer[nCntSencer].fRadius, &g_aSencer[nCntSencer].pos);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSencer(void)
{
#if 1
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxrot, mtxtrans;


	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング

	

	for (int nCntSencer = 0; nCntSencer < MAX_SENCER; nCntSencer++)
	{
		if (g_aSencer[nCntSencer].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSencer[nCntSencer].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxrot,
				g_aSencer[nCntSencer].rot.y, g_aSencer[nCntSencer].rot.x, g_aSencer[nCntSencer].rot.z);

			D3DXMatrixMultiply(&g_aSencer[nCntSencer].mtxWorld, &g_aSencer[nCntSencer].mtxWorld, &mtxrot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxtrans,
				g_aSencer[nCntSencer].pos.x, g_aSencer[nCntSencer].pos.y, g_aSencer[nCntSencer].pos.z);

			D3DXMatrixMultiply(&g_aSencer[nCntSencer].mtxWorld, &g_aSencer[nCntSencer].mtxWorld, &mtxtrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSencer[nCntSencer].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSencer, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSencer);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, 0);


			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SENCER_VERTEX, 0, SENCER_POLYGON);

		}
	}

#endif	
}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
void MakeVertexSencer(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gg.jpg", &g_pTextureLife);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SENCER_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSencer,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffSencer->Lock(0, 0, (void**)&pVtx, 0);
	float fAngle = D3DX_PI * 2 / g_Sencer_h;
	float fDestAngle;
	for (int nCntLife_V = 0; nCntLife_V < g_Sencer_v + 1; nCntLife_V++)
	{
		fDestAngle = 0.0f;
		for (int nCntLife_H = 0; nCntLife_H < g_Sencer_h + 1; nCntLife_H++)
		{
			
				if (fDestAngle >= D3DX_PI)
				{
					fDestAngle -= D3DX_PI * 2;
				}
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].pos.x = sinf(fDestAngle) * SENCER_RADIUS;
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].pos.y = SENCER_HEIGHT * nCntLife_V;				
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].pos.z = cosf(fDestAngle) * SENCER_RADIUS;
				//法線の設定
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラー
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].col = D3DXCOLOR(1.0f, 0.0f, 0.8f, 0.1f);
				
				//テクスチャ座標
				pVtx[nCntLife_V * (g_Sencer_h + 1) + nCntLife_H].tex = D3DXVECTOR2(nCntLife_H * (1.0f / (g_Sencer_h / 2)), nCntLife_V * -1.0f);

				fDestAngle += fAngle;
			}
			
		
	}

	//頂点バッファをアンロック
	g_pVtxBuffSencer->Unlock();


}

////=============================================================================
//// インデックス情報作成
////=============================================================================
void MakeIndexSencer(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * SENCER_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSencer,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント

						//インデックスバッファをロック
	g_pIdxBuffSencer->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < g_Sencer_v; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= g_Sencer_h; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (g_Sencer_h + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == g_Sencer_h && nCntIndex_V <= g_Sencer_v)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (g_Sencer_h + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	g_pIdxBuffSencer->Unlock();

}


//=============================================================================
// シリンダー作成
//=============================================================================
int SetSencer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, SENCERTYPE type)
{
	// 頂点情報の設定
	//VERTEX_3D *pVtx;
	int nCntSencer;
	
	for ( nCntSencer = 0; nCntSencer < MAX_SENCER; nCntSencer++)
	{
		if (g_aSencer[nCntSencer].bUse == false)
		{
			g_aSencer[nCntSencer].pos = pos;
			g_aSencer[nCntSencer].rot = rot;
			g_aSencer[nCntSencer].type = type;
			g_aSencer[nCntSencer].bUse = true;

			break;
		}
	}

	return nCntSencer;
}
//=============================================================================
// センサー判定(プレイヤーとセンサーのの判定)
//=============================================================================
bool CollisionSencer(D3DXVECTOR3 *pos)
{
	bool bCol = false;
	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for (int nCntSencer = 0; nCntSencer < MAX_SENCER; nCntSencer++)
	{
		

		fxpos = (g_aSencer[nCntSencer].pos.x - pos->x) * (g_aSencer[nCntSencer].pos.x - pos->x);
		fzpos = (g_aSencer[nCntSencer].pos.z - pos->z) * (g_aSencer[nCntSencer].pos.z - pos->z);
		fRadius = SENCER_RADIUS * SENCER_RADIUS;


		
		if (g_aSencer[nCntSencer].bUse == true)
		{
			if (fxpos + fzpos <= fRadius)
			{
				bCol = true;
			}

		}
	}
#if  0
#endif
	return bCol;
}
//=============================================================================
// センサー判定(nCntを返す)
//=============================================================================
int CollisionSencerReturnCnt(D3DXVECTOR3 * pos)
{
	int nCntSencer = 0;

	float fxpos = 0.0f;
	float fzpos = 0.0f;
	float fpos = 0.0f;
	float fRadius = 0.0f;
	for ( int Sencer = 0; Sencer < MAX_SENCER; Sencer++)
	{
		if (g_aSencer[Sencer].bUse == true)
		{

			fxpos = (g_aSencer[Sencer].pos.x - pos->x) * (g_aSencer[Sencer].pos.x - pos->x);
			fzpos = (g_aSencer[Sencer].pos.z - pos->z) * (g_aSencer[Sencer].pos.z - pos->z);
			fRadius = SENCER_RADIUS * SENCER_RADIUS;

			if (fxpos + fzpos <= fRadius)
			{
				nCntSencer = Sencer;

			}


		}
	}
	return nCntSencer;
}
//=============================================================================
// センサーを消去
//=============================================================================
void DeleteSencer(int nIdxSencer)
{
	g_aSencer[nIdxSencer].bUse = false;
}


