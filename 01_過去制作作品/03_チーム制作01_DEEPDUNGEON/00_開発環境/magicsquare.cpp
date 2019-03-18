//=============================================================================
//
// 弾発射処理 [MagicSquare.cpp]
// Author : 
//
//=============================================================================
#include "magicsquare.h"
#include "input.h"
#include "player.h"
#include "brazen.h"
#include "sencer.h"
#include "life.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAGICSQUARE_TEXTURENAME	"data/TEXTURE/magicsquare.jpg"	// ファイル名
#define MAX_MAGICSQUARE			(10)							// 魔法陣の最大数
#define MAGICAQUARE_WIDTH		(120.0f)							// 魔法陣の幅
#define MAGICSQUARE_DEPTH		(120.0f)							// 魔法陣の奥行


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMagicSquare(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMagicSquare = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagicSquare = NULL;	//頂点バッファへのポインタ

MAGICSQUARE g_aMagicSquare[MAX_MAGICSQUARE];

float fDestCol;
float fCol;
//=============================================================================
// 初期化処理
//=============================================================================
void InitMagicSquare(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
	{
		g_aMagicSquare[nCntMagicSquare].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagicSquare[nCntMagicSquare].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);
		g_aMagicSquare[nCntMagicSquare].bUse = false;
	}

	MakeVertexMagicSquare(pDevice);






}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagicSquare(void)
{
	//テクスチャの破棄
	if (g_pTextureMagicSquare != NULL)
	{
		g_pTextureMagicSquare->Release();
		g_pTextureMagicSquare = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMagicSquare != NULL)
	{
		g_pVtxBuffMagicSquare->Release();
		g_pVtxBuffMagicSquare = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagicSquare(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;


	//頂点バッファをロック
	g_pVtxBuffMagicSquare->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
	{
		g_aMagicSquare[nCntMagicSquare].rot.y += 0.015f;

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[1].pos = D3DXVECTOR3(g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, -g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[3].pos = D3DXVECTOR3(g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, -g_aMagicSquare[nCntMagicSquare].fRadius);


		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffMagicSquare->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagicSquare(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス


	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
	{
		if (g_aMagicSquare[nCntMagicSquare].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagicSquare[nCntMagicSquare].rot.y, g_aMagicSquare[nCntMagicSquare].rot.x, g_aMagicSquare[nCntMagicSquare].rot.z);
			D3DXMatrixMultiply(&g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare, &g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare, &mtxRot);


			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aMagicSquare[nCntMagicSquare].pos.x, g_aMagicSquare[nCntMagicSquare].pos.y, g_aMagicSquare[nCntMagicSquare].pos.z);

			D3DXMatrixMultiply(&g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare, &g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare, &mtxTrans);


			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMagicSquare[nCntMagicSquare].mtxWorldMagicSquare);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMagicSquare, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			//pDevice->SetFVF(D3DXGetFVFVertexSize);
			pDevice->SetTexture(0, g_pTextureMagicSquare);


			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMagicSquare * 4, 2);
		}
	}


	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMagicSquare(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magicsquare.jpg", &g_pTextureMagicSquare);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAGICSQUARE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMagicSquare,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMagicSquare->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[1].pos = D3DXVECTOR3(g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[2].pos = D3DXVECTOR3(-g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, -g_aMagicSquare[nCntMagicSquare].fRadius);
		pVtx[3].pos = D3DXVECTOR3(g_aMagicSquare[nCntMagicSquare].fRadius, 0.0f, -g_aMagicSquare[nCntMagicSquare].fRadius);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffMagicSquare->Unlock();
}

//=============================================================================
// 魔法陣の設置
//=============================================================================

int SetMagicSquare(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	int nCntMagicSquare;
	//頂点バッファをロック
	g_pVtxBuffMagicSquare->Lock(0, 0, (void**)&pVtx, 0);
	for ( nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
	{
		if (g_aMagicSquare[nCntMagicSquare].bUse == false)
		{

			g_aMagicSquare[nCntMagicSquare].pos = pos;
			g_aMagicSquare[nCntMagicSquare].rot = rot;
			g_aMagicSquare[nCntMagicSquare].fRadius = fRadius;
			g_aMagicSquare[nCntMagicSquare].col = D3DXCOLOR(1.0f, 0.0f, 0.9f, 1.0f);
			g_aMagicSquare[nCntMagicSquare].nIdxSencer = SetSencer(pos, rot, SENCERTYPE_COLLISION);
			g_aMagicSquare[nCntMagicSquare].bUse = true;

		

			break;

		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffMagicSquare->Unlock();
	return nCntMagicSquare;
}
//=============================================================================
// 魔法陣の消去
//=============================================================================
void DeleteMagicSquare(int nIdxMagicSquare)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	

	

		fDestCol = 0.0f;
		fCol = fDestCol - g_aMagicSquare[nIdxMagicSquare].col.a;
		if (g_aMagicSquare[nIdxMagicSquare].bUse == true)
		{

			g_aMagicSquare[nIdxMagicSquare].col.a -=  0.013f;
			HitDamage(+2);


			if (g_aMagicSquare[nIdxMagicSquare].col.a <= 0.0f)
			{
				g_aMagicSquare[nIdxMagicSquare].col.a = 0.0f;
				g_aMagicSquare[nIdxMagicSquare].bUse = false;
				DeleteSencer(g_aMagicSquare[nIdxMagicSquare].nIdxSencer);
				DecreaseBrazen(-25);
			}
		}



		//頂点バッファをロック
		g_pVtxBuffMagicSquare->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntMagicSquare = 0; nCntMagicSquare < MAX_MAGICSQUARE; nCntMagicSquare++)
		{
			g_aMagicSquare[nCntMagicSquare].rot.y += 0.015f;

			//頂点座標
			pVtx[0].col = g_aMagicSquare[nCntMagicSquare].col;
			pVtx[1].col = g_aMagicSquare[nCntMagicSquare].col;
			pVtx[2].col = g_aMagicSquare[nCntMagicSquare].col;
			pVtx[3].col = g_aMagicSquare[nCntMagicSquare].col;


			pVtx += 4;
		}
		//頂点バッファをアンロック
		g_pVtxBuffMagicSquare->Unlock();
	
}
//=============================================================================
// 魔法陣の消去
//=============================================================================
void DeleteOnlyMagicSquare(int nCntMagicsquare)
{
	if (g_aMagicSquare[nCntMagicsquare].bUse == true)
	{
		g_aMagicSquare[nCntMagicsquare].bUse = false;
		DeleteSencer(g_aMagicSquare[nCntMagicsquare].nIdxSencer);
	}
}


MAGICSQUARE *GetMagicSquare(void)
{
	return &g_aMagicSquare[0];
}
